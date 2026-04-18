# `subtract` — the fall-through path when `num_to_subtract == answer` and `*this` is a zero-valued unlimited_int with `intarrays == nullptr`

## Context

`unlimited_int::subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const` is a `protected` member function in `subtraction.cpp` that implements `*answer = *this - *num_to_subtract` with full aliasing support. The function is the backbone of `operator-`, `operator-=`, `operator%`, `operator%=`, and every internal cancellation step in `divide_by`, `divide_by_repeated_addition`, `divide_using_reciprocal`, and the Karatsuba middle-term reconstruction `z1 = z1 - z2 - z0`. Its aliasing contract is: *any* of the three pointers `this`, `num_to_subtract`, `answer` may refer to the same object, and the function is expected to produce the correct result in every combination. The early-return dispatch at the top of the function enumerates the aliasing cases — but one combination isn't handled correctly, and it's only unreachable by accident.

## The fall-through pattern in subtraction.cpp:20–33

```cpp
if (this->num_of_intarrays_used == (size_t)0)
{
    if (num_to_subtract != answer)
    {
        if (num_to_subtract->num_of_used_ints == (size_t)0)
            answer->set_to_zero();
        else
        {
            num_to_subtract->copy_to(*answer);
            answer->_is_negative = !answer->_is_negative;
        }
        return;
    }
}
```

Observe the *intentional* missing `else` after the outer `if`: when `this` is empty *and* `num_to_subtract == answer`, the function does not return. It falls through to the main path. The authorial intent is presumably that the main path's "swap and set_answer_to_negative" logic will handle the empty-minus-nonzero-in-place case correctly. Whether that's *actually* what happens depends on the state of the empty `this`.

## The null-pointer expression that's hiding in plain sight

An empty `unlimited_int` can be in either of two observably-different internal states:

- **State A:** `intarrays == nullptr`, `num_of_intarrays_used == 0`, `num_of_used_ints == 0`, `auto_destroy == true`. This is the state set up by the default constructor (via `forget_memory()`), by `flush()`, and by move-from-source. No dynamically-allocated storage exists for this object.
- **State B:** `intarrays != nullptr` (points at an owned `list_of_int_arrays` that may have zero or more spare nodes), `num_of_intarrays_used == 0`, `num_of_used_ints == 0`. This is the state reached by `set_to_zero()` on an object that previously had data, and by `cutoff_leading_zeros` reducing a number to the zero value without flushing its capacity.

Both states pass `is_zero()` and both pass `find_inconsistencies()`. Every code path except the fall-through can tell them apart trivially (e.g., by testing `intarrays != nullptr`). The subtract fall-through does not.

Tracing the main path when `this` is in State A and `num_to_subtract == answer` holds some large non-negative value:

```cpp
unlimited_int this_copy(*this, false);                     // intarrays = nullptr
unlimited_int num_to_subtract_copy(*num_to_subtract, false); // intarrays = answer's intarrays
this_copy._is_negative        = false;
num_to_subtract_copy._is_negative = false;
// num_of_negatives == 0 (State A requires _is_negative == false by find_inconsistencies)
char compare_result = this_copy.compare_to_ignore_sign(num_to_subtract_copy);
// returns 'S': empty < non-empty
this_copy.swap(num_to_subtract_copy);
// after swap: this_copy.intarrays = answer's intarrays (non-null, large)
//             num_to_subtract_copy.intarrays = nullptr
set_answer_to_negative = true;

if (this_copy.fits_in_many_bits() && num_to_subtract_copy.fits_in_many_bits())
    // only taken for small answers; for any answer > 2 ints, falls through
    ...

size_t num_of_used_ints_this = this_copy.num_of_used_ints;  // = answer's length
answer->increase_until_num_of_ints(num_of_used_ints_this);
custom_linked_list_node<int_array>* it_this     = this_copy.intarrays->first();
custom_linked_list_node<int_array>* it_subtract = num_to_subtract_copy.intarrays->first();
//                                                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// num_to_subtract_copy.intarrays is nullptr. ->first() on nullptr is a null-pointer deref.
```

On Linux x86_64 this is a segfault. On targets with lenient null semantics it's worse — silent garbage that then gets read into `subtract_current_int_array` as `(nullptr)->value`.

## Why it doesn't fire today

`subtract` is `protected`. The public entry points that route into it are: the six `operator-` / `operator-=` / `operator%` / `operator%=` overloads in `operators.cpp`, and the internal calls from `add` (negative-operand redirection), `divide_by` (remainder accumulation), `divide_by_repeated_addition`, and `divide_using_reciprocal` (remainder construction).

For each caller, trace whether the three-way alias pattern `{ *this empty, num_to_subtract == answer, num_to_subtract->num_of_used_ints > 2 }` is achievable:

- `operator-`, `operator%`: construct a fresh `unlimited_int answer` locally before calling subtract. `&answer` can never equal `&other` (an externally-passed reference).
- `operator-=`, `operator%=`: call `this->subtract(&other, this)`, so `this == answer`, but `num_to_subtract == &other ≠ this == answer` in every overload that exists today.
- `add`'s negative-operand redirection: passes local `this_copy` and `num_to_add_copy` as the arguments; `answer` is the outer `answer`. The `num_to_subtract == answer` alias isn't constructible there because the sub-call's `num_to_subtract` is `&this_copy` or `&num_to_add_copy`, neither of which equals the outer `answer`.
- `divide_by`, `divide_by_repeated_addition`, `divide_using_reciprocal`: each calls subtract with `answer = &partial_this` and `num_to_subtract` = a just-constructed product. The product is a fresh local. Not aliased.

So in the current call graph, the bug is latent: reachable only by new protected-function usage or by a future public method that wants in-place `x = x - y` where `y` is the caller's variable. **But** `operator-=`'s signature is `operator-=(const unlimited_int& other)`, and the compiler cannot prevent a future `this->subtract(&other, &const_cast_off(other))` refactor or a new `void subtract_from(const unlimited_int& from_whom, unlimited_int* into)` helper that mirrors the `a - b` into `b` pattern. One such caller is a commit away.

## The deeper issue — two co-occurring symptoms of the same missing early-return

Independent of the null deref, even in State B (`intarrays` non-null but `size() == 0`), the fall-through still goes wrong in a subtler way. `intarrays->first()` on an empty `custom_linked_list` returns the end-sentinel node (not nullptr), and accessing `sentinel->value` yields whatever the sentinel's `value` member happens to hold — which in this codebase is default-initialized to some unspecified state in `custom_linked_list_node`'s default constructor. The downstream `subtract_current_int_array_num_of_used_ints = subtract_current_int_array->num_of_used_ints` reads uninitialized memory. The result is either a silent use of random trailing data as if it were part of the subtrahend, or a crash, depending on the sentinel's layout.

Both symptoms share the same root cause: the fall-through assumes the main path will route back to a correct answer via compare-and-swap, but the compare-and-swap assumes both operands have real storage.

## Investigation plan

1. Attempt the minimal reproduction as a pure unit test: `unlimited_int x; unlimited_int y("12345678901234567890"); x.subtract(&y, &y);`. You'll need to make `subtract` temporarily `public` or friend a test shim to call it. The direct call should crash; the observable behavior pins down which of State A vs State B you're in.
2. Grep the tree for every call site to `subtract` and confirm none of them can be massaged, through a small plausible refactor, into the triggering alias. Pay attention to `operator%=` and to any future `remainder` out-parameter on a method that operates in-place (e.g., an `in_place_modulo` or `divmod_in_place`). The trap is structural: any function whose contract is "compute `x - y` and store in `y`" hits it.
3. Fix by adding the symmetrical branch at the top of `subtract`: when `num_to_subtract == answer` *and* `this->num_of_intarrays_used == 0`, the correct answer is `-*num_to_subtract`, which can be accomplished in place with a single `answer->_is_negative = !answer->_is_negative`. No copy, no allocation. Also add the symmetrical branch for `this == answer` and `num_to_subtract->num_of_intarrays_used == 0` (already partially handled but worth double-checking).
4. Cross-audit `add` for the mirror pattern: is there an `add` aliasing case — specifically `this == answer && num_to_add->num_of_intarrays_used == 0` — that also falls through due to a missing branch? The symmetric structure of the early-returns suggests yes.
