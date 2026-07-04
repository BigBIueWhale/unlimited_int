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
	}
	const size_t amount_needed_to_shift = shift_by - amount_to_shift_least_significant_arr;
	//Build the list to prepend before touching "this", so that if the allocation inside
	//increase_until_num_of_ints throws, "this" is left completely unchanged. Everything after the
	//list is built (the least significant array's in-place shift, the prepend and the counter updates)
	//allocates nothing and can't throw, so the whole shift is all-or-nothing.
	list_of_int_arrays list_to_prepend;
	if (amount_needed_to_shift > (size_t)0)
	{
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
			int_array_in_list_to_prepend->value->destroy();
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
	}
	//All allocations have succeeded; the remaining mutations of "this" can't throw.
	if (amount_to_shift_least_significant_arr > (size_t)0)
		least_significant_int_array->shift_left(amount_to_shift_least_significant_arr);
	if (amount_needed_to_shift > (size_t)0)
	{
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
	const size_t macro_shift = num_of_bits_to_shift_by / (size_t)NUM_OF_BITS_few_bits; //macro shift (as opposed to micro)
	const int micro_shift = num_of_bits_to_shift_by % (size_t)NUM_OF_BITS_few_bits;
	if (micro_shift == 0)
		this->shift_left(macro_shift); //a pure whole-limb shift is already all-or-nothing on its own
	else
	{
		const int amount_to_shift_remainder = NUM_OF_BITS_few_bits - micro_shift;
		few_bits mask_of_shift_builder = 0;
		for (int bit_counter = NUM_OF_BITS_few_bits - 1; bit_counter >= amount_to_shift_remainder; --bit_counter)
			mask_of_shift_builder += (few_bits)1 << bit_counter;
		const few_bits mask_of_shift = mask_of_shift_builder;
		this->flush_unused();
		//Whether the sub-limb shift carries out of the most significant int depends only on that int's top
		//"micro_shift" bits, which the whole-limb macro shift below leaves untouched (it only inserts zeroed least
		//significant ints). So if there is a carry, reserve a spare int_array from the piggy bank off to the side
		//now, before "this" is mutated at all. Kept in its own list (never linked into "this") the spare neither
		//trips shift_left's entry consistency check nor gets reclaimed by flush_unused, and if reserving it has to
		//allocate and throws, "this" is untouched. From the macro shift onward nothing else allocates, so the whole
		//bit shift is all-or-nothing. The macro shift can fill the most significant int_array (when that is also the
		//least significant one), so whether the carry actually needs the spare is only settled after it runs; a
		//spare left unused is handed straight back to the bank below.
		int_array *const most_significant_int_array = this->intarrays->last()->value;
		const few_bits most_significant_int = most_significant_int_array->intarr[most_significant_int_array->num_of_used_ints - (size_t)1];
		const bool carry_out_of_most_significant_int = ((most_significant_int & mask_of_shift) >> amount_to_shift_remainder) != (few_bits)0;
		list_of_int_arrays reserved_carry_int_array_list;
		if (carry_out_of_most_significant_int)
			reserved_carry_int_array_list.increase_by_one_array_from_piggy_bank();
		this->shift_left(macro_shift);
		few_bits remainder = (few_bits)0;
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
		//remainder != 0 here is exactly carry_out_of_most_significant_int, so the spare was reserved above. When the
		//most significant int_array is full the spare becomes the carry's home (appended as the new most significant
		//node); otherwise the carry uses the free slot already there. Either way nothing is allocated here, so this
		//tail can't throw.
		if (remainder != (few_bits)0)
		{
			int_array* last_int_array = this->intarrays->last()->value;
			if (last_int_array->is_full())
			{
				this->intarrays->append(reserved_carry_int_array_list);
				++this->num_of_intarrays_used;
				last_int_array = this->intarrays->last()->value;
				last_int_array->num_of_used_ints = (size_t)0;
			}
			last_int_array->intarr[last_int_array->num_of_used_ints] = remainder;
			++last_int_array->num_of_used_ints;
			++this->num_of_used_ints;
		}
		//If the carry fit in the existing most significant int_array the reserved spare went unused; hand it back to
		//the piggy bank so it is reused rather than leaked (when the append above consumed it the list is empty and
		//this is a no-op).
		if (reserved_carry_int_array_list.size() > (size_t)0)
			reserved_carry_int_array_list.flush_to_piggy_bank();
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_left_by_bits()\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The error was found in end of function \"void unlimited_int::shift_left_by_bits(const many_bits num_of_bits_to_shift_by)\"");
#endif
}