# `split_at_and_use_original` — transient inconsistent state between node detachment and counter reconciliation

## Context

`split_at_and_use_original` (in `split_at.cpp`, approx. lines 170–342) is the memory-reuse sibling of `split_at`. Where `split_at` produces two owning deep-copies of the most- and least-significant portions of `*this`, `split_at_and_use_original` *moves* the least-significant sublist of `*this`'s `intarrays` into `*low`, copies the straddling int_array's upper ints into `*high`'s fresh backing storage, moves the remaining most-significant sublist into `*high`, and finally flushes the now-empty `this`. It is the essential optimization that makes `multiply_karatsuba_destroy_this`, `multiply_karatsuba_destroy_this_and_num_to_mult`, and `power2_destroy_this` asymptotically better than their non-destroying counterparts — without it, every level of the Karatsuba recursion would deep-copy the operand, turning the O(n^log2(3)) multiplication into something with a bigger constant and worse memory traffic.

The function also has to survive the four possible aliasing arrangements between `this`, `low`, and `high`, the `index == 0` and `index >= num_of_used_ints` corner cases, and — critically — **allocator failure at every allocation point**. The author has clearly thought about exception safety (the comment on line ~237 explicitly says "If either allocation throws, this list is completely untouched"), but the invariant is only asserted for one of the allocation points, and there are at least three others that can throw.

## The inconsistency window

The function's sequence of mutations, in order:

1. Early-return paths for empty/degenerate inputs and for `low == this` or `high == this` aliasing. These either return immediately or swap state into a local variable and re-dispatch, so they don't leave `this` in an inconsistent state on throw.
2. `high->flush()` and `low->flush()`. Normal unlimited_int lifecycle; `this` is untouched.
3. `low->intarrays = new list_of_int_arrays;` — allocation #1.
4. `high->intarrays = new list_of_int_arrays;` — allocation #2.
5. **Loop to find the straddling int_array.** Walks `this->intarrays` forward. Pure reads, no mutations.
6. `std::unique_ptr<int_array_list> least_significant_part_of_this = this->intarrays->sublist_int_array_list(...);` — allocation #3 inside `sublist_int_array_list` (the `std::make_unique<int_array_list>()` at `int_array_list.cpp:26`). If this `make_unique` throws, `sublist_int_array_list` returns without having called `sublist`, so `this->intarrays` is untouched and we cleanly unwind.
7. **Inside `sublist_int_array_list`**, after `make_unique` succeeds, `this->custom_linked_list::sublist(...)` is called. This function (in `custom_linked_list.hpp`) is documented as "pure pointer patching (cannot throw)" — it detaches a node range by adjusting `previous`/`next` pointers, without allocating. If that documentation is accurate, detachment is `noexcept`. **Verify this.** If `sublist` can throw for any reason (including a debug-mode assert or any future refactor that adds allocation), the detachment is interleaved with the throwing expression and `this` ends up with some nodes missing from its linked list while `this->num_of_used_ints` and `this->num_of_intarrays_used` still reflect the pre-detachment counts.
8. `this->num_of_ints -= sum_ints;` — member of `list_of_int_arrays`, not of `unlimited_int`. Internal bookkeeping of the list object.
9. `result->custom_linked_list<int_array>::append(*substr_base);` — pointer patching, noexcept per the comment.
10. `low->intarrays->prepend(*least_significant_part_of_this);` — calls `int_array_list::prepend`, which calls `custom_linked_list::prepend`, which (per the pattern) is pointer patching and noexcept.
11. `low->num_of_intarrays_used = num_of_taken_int_arrays_from_this;`
12. `low->num_of_used_ints = index;`
13. `if (num_of_ints_left_in_shared_int_array > 0) high->increase_until_num_of_ints(num_of_ints_left_in_shared_int_array);` — **allocation #4, and this one can definitely throw.** `increase_until_num_of_ints` calls `bank_storage.withdraw` and then `increase_by_one_array` in a loop, each of which does `new int_array` and `int_array::resize` (which does `new few_bits[]`). Either can throw `std::bad_alloc`.

**At the moment of a throw from step 13**, the state is:

- `this->intarrays` is missing the nodes that went to `low`.
- `this->num_of_intarrays_used` still says the pre-split count.
- `this->num_of_used_ints` still says the pre-split count.
- `low` has a valid, consistent state (its counters match its nodes).
- `high` has fresh empty intarrays but `high->num_of_used_ints` is still whatever `flush()` left it at (which is zero — `flush` calls `set_to_zero`).
- `this->intarrays->num_of_ints` has been decremented to match the detachment, so the list's own counter is consistent with its node count, but `this->num_of_used_ints` is inconsistent with `this->intarrays->num_of_ints`.

When the stack unwinds, `this`'s destructor (if any) runs. Per `unlimited_int.hpp:628–635`, the destructor only calls `flush()` if `auto_destroy` is true; flush iterates nodes directly and returns them to the piggy bank without consulting `num_of_used_ints` or `num_of_intarrays_used`. **So no memory leaks**. The nodes `this` still owns get cleaned up, and the nodes now in `low` get cleaned up by `low`'s own destructor.

But the inconsistency has an observable consequence: **any debug-mode `find_inconsistencies` call on `this` *between* the throw and the destructor will fire a `logic_error`**. This includes debug-mode inconsistency checks in callers' catch blocks, in telemetry, in logging, in the `find_inconsistencies` calls at the top of every subsequent operation on `this` (if the caller catches, recovers, and keeps using `this`). And more dangerously, any *release-mode* caller that catches the exception and inspects `this`'s counters — e.g., to decide whether to retry with a smaller allocation, or to surface diagnostic information — will read stale values and can make wrong decisions.

## The same pattern also hides in step 14+

Continuing past step 13, the code modifies `current_int_array_this->num_of_used_ints -= num_of_ints_left_in_shared_int_array;` and then copies the upper bits of the straddling int_array into `high`. Later, `this->intarrays->sublist_int_array_list(...)` is called a *second* time (around line 326) to move the remaining most-significant nodes into `high`, and `high->intarrays->append(...)` finishes the transfer. That second sublist's `std::make_unique<int_array_list>()` is **allocation #5**, and if it throws, we're now deep in an even more inconsistent state: the shared straddling int_array has had its `num_of_used_ints` decremented, `this`'s list has lost its least-significant sublist, `low` is consistent, `high` has partial contents (the bits copied from the straddling int_array but not the transferred tail nodes), and none of `this`'s unlimited_int-level counters reflect any of that.

Again, the per-list-and-per-node cleanup still works because destructors walk nodes directly. But:

- `high->num_of_used_ints` has *never* been set during this call (it's set only in the final `high->num_of_used_ints = size_to_make_high;` on line 330, after both transfers are complete). So if allocation #5 throws, `high` contains data (the straddling int_array's upper bits) but `high->num_of_used_ints == 0`. That's a `find_inconsistencies` violation (the int_array has used ints but the unlimited_int says zero), though a latent one — `high` will be destructed and the violation won't propagate *unless* a caller catches and inspects.
- The straddling int_array's permanently-decremented `num_of_used_ints` is a leak of a sort: data that used to be accessible through `this->intarrays` is now in an int_array whose `num_of_used_ints` says it isn't there. The bytes are still there (until `this` is flushed) but they're not reachable through the normal iteration pattern. No *memory* leak, but an *information* leak in the sense that state didn't round-trip.

## The subtler flavor: throwing in the copy-the-shared-int_array loop (lines 282–310)

After `high->increase_until_num_of_ints` succeeds (step 13), there's a copy loop that walks `high->intarrays` and writes the upper bits of the straddling int_array into it. This loop calls `set_num_of_used_ints_to_maximum()` and writes into `intarr` positions. No allocations in the loop body itself — but the loop depends on `high->intarrays`'s nodes being correctly set up by `increase_until_num_of_ints`. If `increase_until_num_of_ints` silently left `high->intarrays` short (e.g., if the piggy bank returned fewer nodes than requested and the `while (this->num_of_ints < ...) increase_by_one_array()` safety loop hit some bound), the copy loop could read or write past the end of `high->intarrays`. Verify the postcondition of `increase_until_num_of_ints` under all reasonable failure modes, not just OOM.

## Investigation plan

1. Instrument the allocator with a failure-injection hook (e.g., overload `operator new` to fail the Nth call). Run `main.cpp`'s fuzz harness with N stepping through every integer in `[1, 10000]`. For each N, after the exception propagates back out, verify `find_inconsistencies()` returns false on every non-destroyed `unlimited_int`. Any `logic_error` from `find_inconsistencies` localizes which allocation point leaves which object inconsistent.
2. Audit `sublist` in `custom_linked_list.hpp` and confirm `noexcept` is actually specified on the function signature. If it isn't, either add `noexcept` (and audit the body to confirm it holds) or treat it as potentially-throwing in the exception-safety analysis of `split_at_and_use_original`.
3. Restructure `split_at_and_use_original` to establish a strong exception-safety guarantee: *either* the function completes fully and all four objects are in their documented post-state, *or* it throws and all four objects are in their pre-call state. The pattern is: do every allocation up front (allocation #1, #2, #4, #5) before touching any linked-list pointers; perform all detachments and copies as `noexcept` operations; finalize counters last. The current ordering does the first couple of allocations up front but interleaves the remainder with mutations.
4. Consider whether *any* caller of `split_at_and_use_original` actually depends on `this` being in a documented inconsistent-but-still-usable state after a throw. Today, the callers (`multiply_karatsuba_destroy_this`, `multiply_karatsuba_destroy_this_and_num_to_mult`, `power2_destroy_this`) let the exception propagate and rely on destructors for cleanup — so none of them inspect `this` after a throw. That means the invariant violations are currently latent. But they're exactly the kind of latent bug that bites six months later when someone adds a retry loop or a diagnostic log.
