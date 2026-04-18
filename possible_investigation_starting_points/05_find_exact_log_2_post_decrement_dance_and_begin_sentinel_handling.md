# `find_exact_log_2` (unlimited_int overload) — the post-decrement dance, the `got_to_beginning` flag, and the begin-sentinel edge

## Context

`unlimited_int::find_exact_log_2(bool* is_power_2) const` in `bitwise_log2.cpp` determines whether `*this` is exactly a power of 2 and, if so, returns its base-2 logarithm. It's a tight function: a handful of local variables, a short prologue that reads the most-significant int and applies the `x & (x - 1) == 0` power-of-2 test, and a backward walk through the less-significant ints checking that they're all zero. If any less-significant int has a nonzero bit, the number is *not* a pure power of 2 and the function returns with `is_power_2 = false`.

The walk is structured as a backward iteration — starting from the second-most-significant int (inside the most-significant int_array if there are enough ints there, or stepping back one node in the linked list otherwise) — and it uses a `got_to_beginning` flag plus a post-decrement idiom `if (index_in_int_array-- == 0)` to coordinate "I need to move to the previous int_array" with "I'm about to underflow index_in_int_array." The combination is awkward, and the prologue does a slightly-different version of the same dance before entering the main loop, so you effectively have the begin-sentinel / end-of-int_array logic duplicated in two places with subtly different conventions. That's a classic setup for an off-by-one.

## The pattern, annotated

The prologue (lines 35–46):

```cpp
bool got_to_beginning = false;
if (index_in_int_array-- == (size_t)0)
{
    current_int_array_Node = current_int_array_Node->previous;
    if (current_int_array_Node != this->intarrays->begin())
    {
        current_int_array = *current_int_array_Node->value;
        index_in_int_array = current_int_array.num_of_used_ints;
        if (index_in_int_array-- == (size_t)0)
            got_to_beginning = true;
    }
}
```

Read carefully:

- The outer `if` fires when we're currently at index 0 of the most-significant int_array *and* there are fewer ints in this int_array than in the whole number (i.e., the most-significant int_array's `num_of_used_ints == 1`, so the top int was at index 0 and we've exhausted this node). The post-decrement has already underflowed `index_in_int_array` to `MAX_size_t`.
- Step back one node. Check whether we hit the begin-sentinel. If we did, `current_int_array_Node` now *is* the begin-sentinel, but we don't set `got_to_beginning = true` in that path — we simply exit the outer `if` and proceed to the next `if (current_int_array_Node != this->intarrays->begin())` guard on line 47, which will skip the while-loop. That's correct behavior for the case of "the whole number is just one int, already processed" — but it means after this block `index_in_int_array` holds its underflowed `MAX_size_t` value, and if the next `if` on line 47 *did* run (because we didn't hit begin-sentinel), we'd be reading `current_int_array.num_of_used_ints` of the new node via *a different path* (the inner `if (current_int_array_Node != this->intarrays->begin())` on line 39).
- Inside that inner `if` (we stepped back to a non-begin node), we read the new node's `num_of_used_ints` into `index_in_int_array`, *then* post-decrement. If the new node has `num_of_used_ints == 0`, `index_in_int_array--` underflows and sets `got_to_beginning = true` — which causes the main loop to step back yet again on its first iteration.

**The question**: can a non-begin-sentinel int_array node have `num_of_used_ints == 0`? By `find_inconsistencies`'s invariant, no — every int_array node *within `num_of_intarrays_used`* must have `num_of_used_ints >= 1`. But this walk is not bounded by `num_of_intarrays_used`; it walks all the way back to the begin-sentinel by chasing `->previous`. Nodes *beyond* `num_of_intarrays_used` (i.e., unused spare nodes that the list_of_int_arrays has allocated but the unlimited_int doesn't reference) could have `num_of_used_ints == 0` or be in any state.

**But** — and this is where the layout matters — spare nodes are at the *end* of the list (most-significant side), not at the beginning. A backward walk from the most-significant *used* node steps toward less-significant used nodes, not toward spare ones. It should never see a spare node on `->previous`. So the `num_of_used_ints == 0` case in the prologue should be unreachable.

Unless the layout assumption is wrong somewhere. If any caller ever flushes only *some* of the most-significant spares (leaving a gap), or if the piggy-bank withdraw logic ever places a withdrawn node somewhere other than the most-significant end, the walk could encounter a zero-used-ints node going backward. **Verify the layout invariant** — specifically verify that `flush_significant_to_piggy_bank` and `increase_by_one_array_to_insignificant` and their friends never leave the list in a state where spare nodes are between used nodes.

## The main loop (lines 47–68)

```cpp
if (current_int_array_Node != this->intarrays->begin())
{
    while (true)
    {
        if (got_to_beginning)
        {
            got_to_beginning = false;
            current_int_array_Node = current_int_array_Node->previous;
            if (current_int_array_Node == this->intarrays->begin())
                break;
            current_int_array = *current_int_array_Node->value;
            index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
        }
        if (current_int_array.intarr[index_in_int_array] != (few_bits)0)
        {
            *is_power_2 = false;
            return MAX_size_t_NUM;
        }
        if (index_in_int_array-- == (size_t)0)
            got_to_beginning = true;
    }
}
```

The loop has two decoupled state transitions per iteration:

1. **At the top of each iteration**, if `got_to_beginning` is set, advance `current_int_array_Node` one step backward, clear the flag, and set `index_in_int_array` to the new node's last used index.
2. **At the bottom**, test `current_int_array.intarr[index_in_int_array]`, then post-decrement `index_in_int_array`, setting `got_to_beginning = true` if it was already 0.

The subtle part is that when we enter the loop the very first time after the prologue, `index_in_int_array` holds the post-decremented value from the prologue — one less than the position we haven't yet tested. The first loop iteration then tests `intarr[index_in_int_array]` at this already-decremented position. Is that the correct position? Let's count:

- Enter function with `current_int_array = most_significant_int_array`, `index_in_int_array = num_of_used_ints - 1` (positional index of the most-significant int within that int_array).
- Read `most_significant_int = intarr[index_in_int_array]`. Test power-of-2. OK.
- Post-decrement `index_in_int_array`. Now it's `num_of_used_ints - 2` (or `MAX_size_t` if `num_of_used_ints == 1`).
- Either we enter the prologue `if` block (because `num_of_used_ints == 1`) and potentially step back a node, or we don't (because `num_of_used_ints >= 2`).
- If we don't step back: enter the main loop with `index_in_int_array = num_of_used_ints - 2`. First iteration tests `intarr[num_of_used_ints - 2]` — the second-most-significant int. Correct.
- If we do step back one node and the new node has `num_of_used_ints = N`: `index_in_int_array` is set to `N` then immediately post-decremented to `N - 1`. First main-loop iteration tests `intarr[N - 1]` — the most-significant int of the less-significant node. Correct.
- If the new node has `num_of_used_ints = 0` (spare node case, should be unreachable): `index_in_int_array = 0 - 1 = MAX_size_t`, and `got_to_beginning = true`. First main-loop iteration steps back another node. If that next step lands on begin-sentinel, we break. If it lands on a used node with nonzero data, the read of `intarr[index_in_int_array]` uses `MAX_size_t` as the index — out of bounds.

If the "new node has `num_of_used_ints == 0`" case is really unreachable, then the code is correct. **But there's no assertion or guard** — the code just assumes the layout invariant. Any refactor of the piggy-bank or list-growth logic that breaks the invariant would cause this function to read wildly out-of-bounds and either crash or return a garbage answer.

## The worse edge case: the begin-sentinel test on line 39

Look at line 39 again:

```cpp
current_int_array_Node = current_int_array_Node->previous;
if (current_int_array_Node != this->intarrays->begin())
{
```

If we just stepped back from the only used int_array and landed on the begin-sentinel, `current_int_array_Node == begin()`. We skip the inner block. `current_int_array` is still set to the most-significant int_array's value (from earlier, line 22), which is stale. `index_in_int_array` is at its post-decremented `MAX_size_t` value.

Then execution falls through to line 47: `if (current_int_array_Node != this->intarrays->begin())`. We just set it to the begin-sentinel, so this `if` is false. Skip the while-loop.

Line 69: `const size_t num_of_used_bits_in_number = (this->num_of_used_ints - (size_t)1) * NUM_OF_BITS_few_bits + num_of_used_bits_in_most_significant_int;`

Then `*is_power_2 = true;` and return `num_of_used_bits_in_number - 1`. This path is the "the whole number has exactly one used int, and that int is a power of 2, so the number is a power of 2" case. Correct behavior.

But notice: after the prologue, whether the main loop runs depends solely on whether `current_int_array_Node != begin()`. The main loop's termination condition is the same test (line 55–56). If the main loop runs, it must handle the `got_to_beginning` path correctly on the step that lands on begin-sentinel.

In the `got_to_beginning` path (line 51–59):

```cpp
if (got_to_beginning)
{
    got_to_beginning = false;
    current_int_array_Node = current_int_array_Node->previous;
    if (current_int_array_Node == this->intarrays->begin())
        break;
    current_int_array = *current_int_array_Node->value;
    index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
}
```

We step back. If we hit begin-sentinel, break — correct, we're done walking. But — **there's no test here for whether we already *were* at begin-sentinel.** If the main loop entered with `got_to_beginning = true` *and* `current_int_array_Node == begin()` already (which the prologue could plausibly leave us in if the inner `if (current_int_array_Node != this->intarrays->begin())` on line 39 was *not* taken and `got_to_beginning` was also set somewhere along the way) — wait, inspection shows that prologue's `got_to_beginning = true` is only reachable inside the inner `if`, so if we're at begin-sentinel after the outer stepback, `got_to_beginning` is still false and the main loop isn't entered (the outer `if (current_int_array_Node != begin())` on line 47 blocks it). OK.

But the analog worry is: in the main-loop body, if `got_to_beginning` is set, we step `->previous` from `current_int_array_Node`. What is `begin()->previous`? In a sentinel-node doubly-linked list, `begin()->previous` is typically `nullptr` or `end()` depending on the implementation. **Check `custom_linked_list`'s sentinel handling.** If `begin()->previous == nullptr`, and the main-loop ever invokes `got_to_beginning` while already at begin-sentinel, we nullptr-deref the next loop iteration when the top-of-loop `if (got_to_beginning)` block does `current_int_array_Node->previous`.

Can we get there? The outer `if (current_int_array_Node != begin())` on line 47 guards the main loop from starting at begin-sentinel. Inside the main loop, the only way to land on begin-sentinel is through the stepback on line 54, and that's immediately guarded by a `break`. So to nullptr-deref, we'd need a code path that sets `got_to_beginning = true` *while already at begin-sentinel*. Inspection doesn't find one today. But any refactor of the begin-sentinel handling (e.g., making the sentinel circular instead of terminated) would expose a bug here.

## Investigation plan

1. Audit the layout invariant: does any code path in `list_of_int_arrays`, `int_array_piggy_bank`, or any callers of `increase_by_one_array*` / `flush_significant_to_piggy_bank` / `flush_to_piggy_bank` ever leave the list with an unused (zero `num_of_used_ints`) node *before* the most-significant used node? If so, `find_exact_log_2` can walk into it.
2. Build a unit test for `find_exact_log_2` that deliberately constructs an `unlimited_int` whose `intarrays` has a zero-`num_of_used_ints` node between two used nodes (via white-box manipulation). Verify the function returns the mathematically-correct answer — or fails the `find_inconsistencies` check up front. Either outcome tells you something.
3. Re-read the begin-sentinel behavior in `custom_linked_list`. Confirm `begin()->previous` is either (a) a self-loop back to `end()`, (b) nullptr, or (c) a second sentinel. The function's correctness depends on the answer. If `begin()->previous` is nullptr, every `->previous` step in the function is a potential nullptr-deref guarded only by implicit invariants.
4. Consider whether the same dance appears in `sha256.cpp` and `sha512.cpp`'s backward walks over `intarrays`. They use a similar pattern (get-most-significant, walk back, check-for-begin). If `find_exact_log_2` has a latent bug, the hash functions probably have the same one — and for SHA the consequence is a wrong hash, which the fingerprint cache uses as its collision-disambiguation key.
5. The `x & (x - 1) == 0` power-of-2 test on line 27 is correct for all `few_bits` values including `MAX_few_bits_NUM`. But the *derivation* of `num_of_zeros_preceding_most_significant_int` (line 33) still calls the O(NUM_OF_BITS_few_bits) helper `num_of_zero_bits_preceding_number`. The commit message for a1b220d touts replacing *both* helper calls with the single bit-twiddle, but half the replacement is still a helper call. If the intent was to eliminate both, there's an optimization opportunity; if the intent was to keep one, the comment in the source is slightly misleading.
