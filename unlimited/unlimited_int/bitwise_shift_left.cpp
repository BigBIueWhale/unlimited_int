#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//basically multiply by (0x10000000000000000 ^ shift_by) (if you're in 64 bits mode)
//Works by simply attaching more zeroed-out int arrays to the lest significant side of the number.
void unlimited_int::shift_left(const many_bits shift_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_left(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::shift_left(const many_bits shift_by)\"";
#endif
	if ((this->num_of_intarrays_used == 0) || (shift_by == 0)) { return; }
	list_of_int_arrays list_to_prepend;
	list_to_prepend.increase_until_num_of_ints(shift_by);
	list_to_prepend.fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(shift_by);
	this->num_of_intarrays_used += list_to_prepend.intarrays.length;
	this->intarrays.prepend(list_to_prepend);
	this->num_of_used_ints += shift_by;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_left(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::shift_left(const many_bits shift_by)\"";
#endif
}
void unlimited_int::shift_left_by_bits(const many_bits num_of_bits_to_shift_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_left_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe error was found in start of function \"void unlimited_int::shift_left_by_bits(const many_bits num_of_bits_to_shift_by)\"";
#endif
	if (this->num_of_used_ints == 0 || num_of_bits_to_shift_by == 0) { return; }
	this->shift_left(num_of_bits_to_shift_by / (many_bits)NUM_OF_BITS_few_bits); //macro shift
	if (this->num_of_used_ints == 0) { return; }
	const int micro_shift = num_of_bits_to_shift_by % NUM_OF_BITS_few_bits;
	if (micro_shift != 0)
	{
		const int amount_to_shift_remainder = NUM_OF_BITS_few_bits - micro_shift;
		few_bits mask_of_shift_builder = 0;
		for (int bit_counter = NUM_OF_BITS_few_bits - 1; bit_counter >= amount_to_shift_remainder; --bit_counter)
		{
			mask_of_shift_builder += 1 << bit_counter;
		}
		const few_bits mask_of_shift = mask_of_shift_builder;
		few_bits remainder = 0;
		this->flush_unused();
		Node* it_this = this->intarrays.intarrays.first;
		while (it_this != nullptr)
		{
			int_array* current_int_array = it_this->value;
			few_bits* it_ptr = current_int_array->intarr;
			const few_bits* stop_at = &current_int_array->intarr[current_int_array->num_of_used_ints - 1];
			few_bits current_value;
			while (it_ptr != stop_at)
			{
				current_value = *it_ptr;
				*it_ptr <<= micro_shift;
				*it_ptr += remainder;
				remainder = (current_value & mask_of_shift) >> amount_to_shift_remainder;
				++it_ptr;
			}
			current_value = *it_ptr;
			*it_ptr <<= micro_shift;
			*it_ptr += remainder;
			remainder = (current_value & mask_of_shift) >> amount_to_shift_remainder;
			it_this = it_this->next;
		}
		if (remainder != 0)
		{
			int_array* last_int_array = this->intarrays.intarrays.last->value;
			if (last_int_array->is_full())
			{
				this->intarrays.increase_by_one_array_from_piggy_bank();
				++this->num_of_intarrays_used;
				last_int_array = this->intarrays.intarrays.last->value;
				last_int_array->num_of_used_ints = 0;
			}
			++this->num_of_used_ints;
			last_int_array->intarr[last_int_array->num_of_used_ints] = remainder;
			++last_int_array->num_of_used_ints;
		}
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_left_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe error was found in end of function \"void unlimited_int::shift_left_by_bits(const many_bits num_of_bits_to_shift_by)\"";
#endif
}