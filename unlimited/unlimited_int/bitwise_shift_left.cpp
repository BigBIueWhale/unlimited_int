#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
//basically multiply by (0x10000000000000000 ^ shift_by) (if you're in 64 bits mode)
//Works by simply attaching more zeroed-out int arrays to the lest significant side of the number.
void unlimited_int::shift_left(const size_t shift_by)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_left(const size_t shift_by)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"void unlimited_int::shift_left(const size_t shift_by)\"");
#endif
	if (this->num_of_intarrays_used == (size_t)0 || shift_by == (size_t)0)
		return;
	int_array *const least_significant_int_array = this->intarrays->first()->value;
	const size_t room_available_to_shift_left = least_significant_int_array->intarr_len - least_significant_int_array->num_of_used_ints;
	size_t amount_to_shift_least_significant_arr = (size_t)0;
	if (room_available_to_shift_left > (size_t)0)
	{
		amount_to_shift_least_significant_arr = room_available_to_shift_left;
		if (shift_by < amount_to_shift_least_significant_arr)
			amount_to_shift_least_significant_arr = shift_by;
		least_significant_int_array->shift_left(amount_to_shift_least_significant_arr);
	}
	const size_t amount_needed_to_shift = shift_by - amount_to_shift_least_significant_arr;
	if (amount_needed_to_shift > (size_t)0)
	{
		list_of_int_arrays list_to_prepend;
		list_to_prepend.increase_until_num_of_ints(amount_needed_to_shift);
		const custom_linked_list_node<int_array>* const list_to_prepend_end = list_to_prepend.end();
	//The list to prepend will be completely full except for the least significant int_array which might not be completely full. That's purposeful.
		custom_linked_list_node<int_array>* int_array_in_list_to_prepend = list_to_prepend.first();
		size_t num_of_extra_ints_in_list = list_to_prepend.num_of_ints - amount_needed_to_shift;
		size_t current_intarr_len = int_array_in_list_to_prepend->value->intarr_len;
		while (num_of_extra_ints_in_list >= current_intarr_len)
		{
			list_to_prepend.num_of_ints -= current_intarr_len;
			num_of_extra_ints_in_list -= current_intarr_len;
			int_array_in_list_to_prepend = list_to_prepend.erase(int_array_in_list_to_prepend);
			current_intarr_len = int_array_in_list_to_prepend->value->intarr_len;
		}
		const size_t amount_used_in_least_significant_int_array = current_intarr_len - num_of_extra_ints_in_list;
		int_array_in_list_to_prepend->value->num_of_used_ints = amount_used_in_least_significant_int_array;
		int_array_in_list_to_prepend->value->fillzero_until(amount_used_in_least_significant_int_array);
		for (int_array_in_list_to_prepend = int_array_in_list_to_prepend->next; int_array_in_list_to_prepend != list_to_prepend_end; int_array_in_list_to_prepend = int_array_in_list_to_prepend->next)
		{
			int_array_in_list_to_prepend->value->set_num_of_used_ints_to_maximum();
			int_array_in_list_to_prepend->value->fillzero();
		}
		this->num_of_intarrays_used += list_to_prepend.size();
		this->intarrays->prepend(list_to_prepend);
	}
	this->num_of_used_ints += shift_by;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_left(const size_t shift_by)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::shift_left(const size_t shift_by)\"");
#endif
}
void unlimited_int::shift_left_by_bits(const size_t num_of_bits_to_shift_by)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_left_by_bits()\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The error was found in start of function \"void unlimited_int::shift_left_by_bits(const many_bits num_of_bits_to_shift_by)\"");
#endif
	if (this->is_negative())
		throw std::invalid_argument("Can\'t do bitwise operation on negative number");
	if (this->num_of_used_ints == (size_t)0 || num_of_bits_to_shift_by == (size_t)0)
		return;
	this->shift_left(num_of_bits_to_shift_by / (size_t)NUM_OF_BITS_few_bits); //macro shift (as opposed to micro)
	const int micro_shift = num_of_bits_to_shift_by % (size_t)NUM_OF_BITS_few_bits;
	if (micro_shift != 0)
	{
		const int amount_to_shift_remainder = NUM_OF_BITS_few_bits - micro_shift;
		few_bits mask_of_shift_builder = 0;
		for (int bit_counter = NUM_OF_BITS_few_bits - 1; bit_counter >= amount_to_shift_remainder; --bit_counter)
			mask_of_shift_builder += 1 << bit_counter;
		const few_bits mask_of_shift = mask_of_shift_builder;
		few_bits remainder = (size_t)0;
		this->flush_unused();
		custom_linked_list_node<int_array>* it_this = this->intarrays->first();
		while (it_this != this->intarrays->end())
		{
			int_array* current_int_array = it_this->value;
			few_bits* it_ptr = current_int_array->intarr;
			const few_bits *const stop_at = current_int_array->intarr + current_int_array->num_of_used_ints;
			while (it_ptr != stop_at)
			{
				const few_bits current_value = *it_ptr;
				*it_ptr <<= micro_shift;
				*it_ptr += remainder;
				remainder = (current_value & mask_of_shift) >> amount_to_shift_remainder;
				++it_ptr;
			}
			it_this = it_this->next;
		}
		if (remainder != 0)
		{
			int_array* last_int_array = this->intarrays->last()->value;
			if (last_int_array->is_full())
			{
				this->intarrays->increase_by_one_array_from_piggy_bank();
				++this->num_of_intarrays_used;
				last_int_array = this->intarrays->last()->value;
				last_int_array->num_of_used_ints = (size_t)0;
			}
			++this->num_of_used_ints;
			last_int_array->intarr[last_int_array->num_of_used_ints] = remainder;
			++last_int_array->num_of_used_ints;
		}
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_left_by_bits()\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The error was found in end of function \"void unlimited_int::shift_left_by_bits(const many_bits num_of_bits_to_shift_by)\"");
#endif
}