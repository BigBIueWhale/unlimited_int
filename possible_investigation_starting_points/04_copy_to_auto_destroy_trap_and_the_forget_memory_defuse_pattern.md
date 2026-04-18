# `copy_to` — the unconditional `auto_destroy = true` assignment and the latent trap it sets for non-owning destinations

## Context

The non-owning copy constructor `unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)` is the library's core hack for working around `const`-correctness: given a const reference `x`, it produces a second `unlimited_int` object that shares `x`'s `intarrays` pointer, `num_of_used_ints`, and so on, but whose destructor is neutered (it doesn't `flush()` if `auto_destroy` is false). This lets code like subtract, add, divide, compare-ignore-sign, and operator* all take a `const` input, produce a "mutable view" to strip its sign or otherwise reinterpret it, and never violate the constness contract with the caller.

The whole pattern works *because* two invariants are maintained by callers of the non-owning constructor:

1. The non-owning view is **only ever read from** during its lifetime; its backing storage (which is shared) is never mutated through it.
2. The non-owning view **never gets its `intarrays` replaced** during its lifetime. If it did, the replacement would either leak the old (shared!) intarrays or — worse — cause a double-free later when the original owner also flushes.

Both invariants are maintained by convention, not by the type system. `copy_to` is the function most at risk of violating invariant #2 silently.

## The line in question

`copy.cpp:16`:

```cpp
void unlimited_int::copy_to(unlimited_int& num_to_paste_into) const
{
    ...
    num_to_paste_into.auto_destroy = true;
    size_t this_num_of_used_ints = this->num_of_used_ints;
    num_to_paste_into.increase_until_num_of_ints(this_num_of_used_ints);
    ...
```

Two mutations to `num_to_paste_into` before any `this` vs `&num_to_paste_into` aliasing check:

- `num_to_paste_into.auto_destroy = true;` — forcibly takes ownership.
- `num_to_paste_into.increase_until_num_of_ints(this_num_of_used_ints);` — if `num_to_paste_into.intarrays` is shared (because `num_to_paste_into` was a non-owning view), this call **mutates the shared list**, adding int_arrays to it, which are then "owned" by `num_to_paste_into` from its perspective but also still visible through the original owner's `intarrays` pointer.

The interaction is adversarial. At the end of `copy_to`, `num_to_paste_into.auto_destroy == true`, so when `num_to_paste_into`'s lifetime ends, its destructor will `flush()` — which deletes the shared `intarrays`. The original owner then has a dangling `intarrays` pointer. First subsequent use is a use-after-free.

## The forget_memory() defuse

The author is aware of this trap. The evidence is in `Newton_Raphson.cpp:64–70`:

```cpp
unlimited_int reciprocal_shifted(*reciprocal.reciprocal, false);
//Only make an independent copy of reciprocal if it needs to be shifted
if (amount_to_reduce_reciprocal > 0)
{
    reciprocal_shifted.forget_memory();
    reciprocal_shifted = *reciprocal.reciprocal;
    reciprocal_shifted.shift_right(amount_to_reduce_reciprocal);
}
```

The pattern is: before reassigning into `reciprocal_shifted` (which would invoke `copy_to` via `operator=`), explicitly `forget_memory()` first. `forget_memory()` sets `intarrays = nullptr` and `auto_destroy = true`. Now the subsequent assignment sees `num_to_paste_into.intarrays == nullptr`, so `increase_until_num_of_ints` will have to allocate a fresh list — no mutation of any shared data. And `num_to_paste_into.auto_destroy = true` is a no-op since it was already true.

**This is the single defuse pattern that makes non-owning views safe to reassign.** Any place in the codebase that reassigns into a non-owning view without calling `forget_memory()` first is a latent bug.

## Audit checklist

Every non-owning view construction in the tree — grep for `unlimited_int\s+\w+\s*\([^,]*,\s*false\s*\)` — needs to be checked for the following *reassignment* patterns:

- `view = expression;` (invokes copy-assignment, which invokes `copy_to`)
- `some_func(&source, &view)` where `some_func` writes to its output parameter and the output parameter path goes through `copy_to` (including `add`'s early-return `num_to_add->copy_to(*answer)`, `subtract`'s early-return `num_to_subtract->copy_to(*answer)`, `split_at`'s degenerate-case `this->copy_to(*low)` and `this->copy_to(*high)`)
- `view.copy_to(something)` — safe, `view` is on the source side
- `something.copy_to(view)` — **unsafe unless `forget_memory()` preceded it**
- `std::move(view)` into something — move-assign into a receiver invokes move-from-view, which in `operator=(unlimited_int&&)` first flushes the receiver and then takes the view's intarrays pointer. If the receiver is owning, the receiver's pre-move intarrays is flushed (fine), then it steals the shared intarrays (BAD: now the receiver thinks it owns shared storage). `source.forget_memory()` at the end of move-assign makes the view safe, but the receiver is corrupt.
- `view.swap(something)` — swaps `intarrays`, `auto_destroy`, `_is_negative`, `num_of_intarrays_used`, `num_of_used_ints`. If `something` was owning (auto_destroy true), after swap the view has auto_destroy true AND its own new intarrays, which is fine for `view`'s destructor. But `something` now has auto_destroy false and shares what was the view's intarrays (now *something else*'s backing storage). In the `subtract` / `add` / operator-paths, `swap` is used on **two non-owning views of different sources**, so both still have auto_destroy false post-swap. Audit each `.swap(` call on a non-owning view and verify the symmetry.

## Specific places worth auditing

- `subtraction.cpp:52`: `this_copy.swap(num_to_subtract_copy);` — both are non-owning views, both have auto_destroy false. Symmetric, safe *unless* `swap`'s `std::swap(auto_destroy)` runs between two objects with the same auto_destroy value (which it does here). OK.
- `subtraction.cpp:92`: `this_copy.swap(num_to_subtract_copy);` — same situation. OK.
- `addition.cpp:41–47`: the scoped-block `{ unlimited_int this_abs(*this, false); ...; shifted = this_abs << (size_t)1; }` — `this_abs` is read-only; `shifted` is a separate local owning object. The assignment to `shifted` invokes operator= with an rvalue (prvalue from operator<<), which invokes move-assignment. `shifted` was default-constructed (empty), and the rvalue is owning. Move-assign flushes `shifted` (no-op, was empty), takes rvalue's intarrays. `this_abs` is untouched and dies at end of block without flushing. Safe. But the *ordering comment* on line 44 is important — *"it's safe to assign to *answer (which may alias *this, invalidating any non-owning view's intarrays)"* — which tells you the author already thought about this trap and made sure `this_abs` dies before `*answer` assignment. Good. But it means this is the *known* trap and the author is defending against it here. Are all other sites defended?
- `split_at.cpp:19, 25`: `this->copy_to(*low)` and `this->copy_to(*high)` in the degenerate-index branches. `low` and `high` are passed by caller. All current callers pass `&owning_local_unlimited_int`, so safe. But if a future caller passed a non-owning view as `low` or `high`, `copy_to` would corrupt. (Cf. `split_at_and_use_original` has an explicit `if (low == this)` guard; `split_at` does not — the analog trap is slightly different because `split_at` is `const` and can't be called with `low == this` from a well-typed caller, but the non-owning-view-as-`low` trap is available.)
- `operators.cpp:40`: `unlimited_int other_positive(other, false);` — non-owning view. Used read-only through the function. Safe.
- `division.cpp:96, 224`: `unlimited_int num_to_divide_by_cpy(num_to_divide_by, false);` — inside `#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0` block, used only for read. Safe.
- `Newton_Raphson.cpp:82, 105, 158`: all three are used read-only. Safe.

## The deeper structural question

`copy_to` setting `auto_destroy = true` is an anti-pattern for a function that is semantically "write into the destination, don't take ownership of the destination." The *caller* owns the destination. `copy_to` shouldn't be fiddling with the destination's ownership bit.

The only reason `copy_to` sets `auto_destroy = true` is to paper over one specific workflow: when the caller does `unlimited_int x; something.copy_to(x);` and `x` was default-constructed (auto_destroy = true) — in which case the assignment is a no-op. And when the caller does `unlimited_int x; ... ; x = y;` where the `operator=` dispatches to `copy_to`, the same no-op. The line doesn't do anything useful; it only exists to avoid a theoretical case where some code path flipped `auto_destroy` to false on the destination before calling copy_to. **Removing that line** would break nothing in the current codebase *and* would make `copy_to` into a non-owning mutator that could safely be called on a non-owning view — which is a much stronger contract.

Deleting the line and seeing what breaks is a cheap experiment that would tell you either (a) nothing breaks and the line has been superstitious defense all along, or (b) something breaks, and the breaking site is the location of a pre-existing bug of the "reassign a non-owning view without forget_memory()" shape.

## Investigation plan

1. Delete `copy.cpp:16` (`num_to_paste_into.auto_destroy = true;`) on a scratch branch and run `main.cpp`'s 80,000-iteration harness. Anything that now crashes, leaks, or fails a comparison is a pre-existing site that was being papered over.
2. Grep for every assignment-into-view pattern and verify `forget_memory()` precedes each. The `Newton_Raphson` site is the template; check whether analogous sites exist elsewhere (in particular: anywhere that reassigns into a variable that was declared with the `(other, false)` constructor earlier in the function).
3. Audit whether `copy_most_significant_to` has the symmetric bug for the *opposite* reason: it *doesn't* set `auto_destroy = true`. That means if the caller passes a default-constructed destination (auto_destroy true) everything's fine, but if the caller passes a *previously-owning* destination whose auto_destroy was subsequently set to false (e.g., via `copy_to`'s old behavior interacting with a now-flushed source), the post-condition of `copy_most_significant_to` leaves the destination in an auto_destroy=false state when it should be auto_destroy=true. That's a memory leak on destruction.
4. Consider introducing a type-level distinction. An owning `unlimited_int` and a non-owning `unlimited_int_view` as separate types would let the compiler catch every instance of this trap at compile time. The current hack is productive — it avoids allocation in the hot path — but it's been the root cause of multiple recent bugs (see the 8c585ea commit message's 22-item fix list) and is probably the single largest source of remaining latent bugs in the library.
