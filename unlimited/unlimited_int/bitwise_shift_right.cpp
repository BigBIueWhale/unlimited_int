#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::shift_right(const many_bits shift_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"void unlimited_int::shift_right(const many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe error was found in start of function \"void unlimited_int::shift_right(const many_bits shift_by)\"";
#endif
	if (shift_by == 0) { return; }
	if (shift_by >= this->num_of_used_ints) { this->set_to_zero(); return; }
	Node* it_this = this->intarrays.intarrays.first;
	many_bits num_of_ints_went_through = 0, num_of_intarrays_went_through = 0;
	while (true)
	{
		many_bits sum = num_of_ints_went_through + it_this->value->num_of_used_ints;
		if (sum > shift_by) { break; }
		num_of_ints_went_through = sum;
		++num_of_intarrays_went_through;
		if (sum == shift_by) { break; }
		else { it_this = it_this->next; }
	}
	this->intarrays.flush_insignificant_to_piggy_bank(num_of_intarrays_went_through);
	this->intarrays.intarrays.first->value->shift_right(shift_by - num_of_ints_went_through);
	this->num_of_used_ints -= shift_by;
	this->num_of_intarrays_used -= num_of_intarrays_went_through;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::shift_right(const many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::shift_right(const many_bits shift_by)\"";
	}
#endif
}
//first uses shift_right [by number of few_bits] and then uses "microshift" which is a shift by bits of less than NUM_OF_BITS_few_bits
void unlimited_int::shift_right_by_bits(const many_bits num_of_bits_to_shift_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_right_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe error was found in start of function \"void unlimited_int::shift_right_by_bits(const many_bits num_of_bits_to_shift_by)\"";
#endif
	if (this->num_of_used_ints == 0 || num_of_bits_to_shift_by == 0) { return; }
	this->shift_right(num_of_bits_to_shift_by / (many_bits)NUM_OF_BITS_few_bits); //macro shift
	if (this->num_of_used_ints == 0) { return; }
	const int micro_shift = num_of_bits_to_shift_by % NUM_OF_BITS_few_bits;
	if (micro_shift != 0)
	{
		const int amount_to_shift_remainder = NUM_OF_BITS_few_bits - micro_shift;
		few_bits remainder = 0;
		this->flush_unused();
		Node* it_this = this->intarrays.intarrays.last;
		while (it_this != nullptr)
		{
			int_array* current_int_array = it_this->value;
			few_bits* it_ptr = &current_int_array->intarr[current_int_array->num_of_used_ints - 1];
			const few_bits* stop_at = current_int_array->intarr;
			few_bits current_value;
			while (it_ptr != stop_at)
			{
				current_value = *it_ptr;
				*it_ptr >>= micro_shift;
				*it_ptr += remainder;
				remainder = current_value << amount_to_shift_remainder;
				--it_ptr;
			}
			current_value = *it_ptr;
			*it_ptr >>= micro_shift;
			*it_ptr += remainder;
			remainder = current_value << amount_to_shift_remainder;
			it_this = it_this->previous;
		}
		this->cutoff_leading_zeros(this->intarrays.intarrays.last);
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_right_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe error was found in end of function \"void unlimited_int::shift_right_by_bits(const many_bits num_of_bits_to_shift_by)\"";
#endif
}