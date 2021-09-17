//in use in Karatsuba algorithm
#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::split_at(const size_t index, unlimited_int* high, unlimited_int* low) const
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"split_at(size_t index, unlimited_int* low, unlimited_int* high)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"void unlimited_int::split_at(const size_t index, unlimited_int* high, unlimited_int* low) const\"");
#endif
	const size_t this_num_of_used_ints = this->num_of_used_ints;
	if (index >= this_num_of_used_ints)
	{
		this->copy_to(*low);
		high->set_to_zero();
		return;
	}
	else if (index == (size_t)0)
	{
		this->copy_to(*high);
		low->set_to_zero();
		return;
	}
	high->_is_negative = false;
	low->_is_negative = false;
	const size_t length_of_high = this_num_of_used_ints - index;
	const size_t length_of_low = index;
	low->increase_until_num_of_ints(length_of_low);
	low->num_of_used_ints = length_of_low;
	high->increase_until_num_of_ints(length_of_high);
	high->num_of_used_ints = length_of_high;
	custom_linked_list_node<int_array>* it_this = this->intarrays->first();
	size_t num_int = (size_t)0, previous_num_int = (size_t)0;
	size_t this_index = (size_t)0;
	int_array* current_this_int_array = it_this->value;
	size_t current_this_int_array_length = current_this_int_array->num_of_used_ints;
	custom_linked_list_node<int_array>* it_low = low->intarrays->first();
	size_t low_index = (size_t)0;
	int_array* current_low_int_array = it_low->value;
	current_low_int_array->set_num_of_used_ints_to_maximum();
	size_t current_low_int_array_length = current_low_int_array->intarr_len;
	size_t num_of_used_int_arrays_low = (size_t)1;
	few_bits* current_intarr_this = current_this_int_array->intarr;
	few_bits* current_intarr_low = current_low_int_array->intarr;
	size_t stop_at;
	if (current_this_int_array_length < current_low_int_array_length)
		stop_at = current_this_int_array_length;
	else
		stop_at = current_low_int_array_length;
	if (length_of_low < stop_at)
		stop_at = length_of_low;
	while (true)
	{
		if (num_int >= stop_at)
		{
			const size_t difference = num_int - previous_num_int;
			this_index += difference;
			low_index += difference;
			previous_num_int = num_int;
			if (num_int >= length_of_low)
				break;
			if (this_index >= current_this_int_array_length)
			{
				this_index = (size_t)0;
				it_this = it_this->next;
				current_this_int_array = it_this->value;
				current_this_int_array_length = current_this_int_array->num_of_used_ints;
				current_intarr_this = current_this_int_array->intarr;
			}
			if (low_index >= current_low_int_array_length)
			{
				++num_of_used_int_arrays_low;
				low_index = (size_t)0;
				it_low = it_low->next;
				current_low_int_array = it_low->value;
				current_low_int_array->set_num_of_used_ints_to_maximum();
				current_low_int_array_length = current_low_int_array->intarr_len;
				current_intarr_low = current_low_int_array->intarr;
			}
			const size_t num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
			const size_t num_of_ints_left_for_current_low = current_low_int_array_length - low_index;
			if (num_of_ints_left_for_current_low < num_of_ints_left_for_current_this)
				stop_at += num_of_ints_left_for_current_low;
			else
				stop_at += num_of_ints_left_for_current_this;
			if (length_of_low < stop_at)
				stop_at = length_of_low;
			continue;
		}
		*current_intarr_low = *current_intarr_this;
		++current_intarr_low;
		++current_intarr_this;
		++num_int;
	}
	current_low_int_array->num_of_used_ints = low_index;
	low->num_of_intarrays_used = num_of_used_int_arrays_low;
	low->cutoff_leading_zeros(it_low);
	custom_linked_list_node<int_array>* it_high = high->intarrays->first();
	size_t high_index = (size_t)0;
	int_array* current_high_int_array = it_high->value;
	current_high_int_array->set_num_of_used_ints_to_maximum();
	size_t current_high_int_array_length = current_high_int_array->intarr_len;
	size_t num_of_used_int_arrays_high = (size_t)1;
	few_bits* current_intarr_high = current_high_int_array->intarr;
	const size_t num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
	if (num_of_ints_left_for_current_this < current_high_int_array_length)
		stop_at += num_of_ints_left_for_current_this;
	else
		stop_at += current_high_int_array_length;
	if (this_num_of_used_ints < stop_at)
		stop_at = this_num_of_used_ints;
	while (true)
	{
		if (num_int >= stop_at)
		{
			const size_t difference = num_int - previous_num_int;
			this_index += difference;
			high_index += difference;
			previous_num_int = num_int;
			if (num_int >= this_num_of_used_ints)
				break;
			if (this_index >= current_this_int_array_length)
			{
				this_index = (size_t)0;
				it_this = it_this->next;
				current_this_int_array = it_this->value;
				current_this_int_array_length = current_this_int_array->num_of_used_ints;
				current_intarr_this = current_this_int_array->intarr;
			}
			if (high_index >= current_high_int_array_length)
			{
				++num_of_used_int_arrays_high;
				high_index = (size_t)0;
				it_high = it_high->next;
				current_high_int_array = it_high->value;
				current_high_int_array->set_num_of_used_ints_to_maximum();
				current_high_int_array_length = current_high_int_array->intarr_len;
				current_intarr_high = current_high_int_array->intarr;
			}
			const size_t num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
			const size_t num_of_ints_left_for_current_high = current_high_int_array_length - high_index;
			if (num_of_ints_left_for_current_high < num_of_ints_left_for_current_this)
				stop_at += num_of_ints_left_for_current_high;
			else
				stop_at += num_of_ints_left_for_current_this;
			if (this_num_of_used_ints < stop_at)
				stop_at = this_num_of_used_ints;
			continue;
		}
		*current_intarr_high = *current_intarr_this;
		++current_intarr_high;
		++current_intarr_this;
		++num_int;
	}
	high->num_of_intarrays_used = num_of_used_int_arrays_high;
	current_high_int_array->num_of_used_ints = high_index;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"split_at(size_t index, unlimited_int* low, unlimited_int* high)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (high->find_inconsistencies() || low->find_inconsistencies() || this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::split_at(const size_t index, unlimited_int* high, unlimited_int* low) const\"");
#endif
}
void unlimited_int::split_at_and_use_original(const size_t index, unlimited_int* high, unlimited_int* low)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"split_at_and_use_original(size_t index, unlimited_int* high, unlimited_int* low)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"void unlimited_int::split_at_and_use_original(const size_t index, unlimited_int* high, unlimited_int* low)\"");
#endif
	size_t size_of_this = this->num_of_used_ints;
	if (size_of_this == (size_t)0)
	{
		low->set_to_zero();
		high->set_to_zero();
		this->flush();
		return;
	}
	else if (index == (size_t)0)
	{
		this->swap(*high);
		this->flush();
		low->set_to_zero();
		return;
	}
	else if (index >= size_of_this)
	{
		this->swap(*low);
		this->flush();
		high->set_to_zero();
		return;
	}
	high->flush();
	low->flush();
	const size_t this_num_of_used_int_arrays = this->num_of_intarrays_used;
	custom_linked_list_node<int_array>* it_this = this->intarrays->first();
	size_t num_of_ints_filled_lower = (size_t)0;
	size_t num_of_taken_int_arrays_from_this = (size_t)0;
	size_t size_to_make_high = size_of_this - index;
	size_t sum_used = (size_t)0;
	while (true)
	{
		num_of_ints_filled_lower += it_this->value->intarr_len;
		sum_used += it_this->value->num_of_used_ints;
		++num_of_taken_int_arrays_from_this;
		if (sum_used >= index)
			break;
		it_this = it_this->next;
	}
	int_array* current_int_array_this = it_this->value;
	std::shared_ptr<int_array_list> least_significant_part_of_this = this->intarrays->sublist_int_array_list(this->intarrays->first(), it_this, num_of_taken_int_arrays_from_this, num_of_ints_filled_lower);
	if (low->intarrays == nullptr)
		low->intarrays = new list_of_int_arrays;
	low->intarrays->prepend(*least_significant_part_of_this);
	least_significant_part_of_this.reset();
	low->num_of_intarrays_used = num_of_taken_int_arrays_from_this;
	low->num_of_used_ints = index;
	const size_t num_of_ints_left_in_shared_int_array = sum_used - index;
	current_int_array_this->num_of_used_ints -= num_of_ints_left_in_shared_int_array;
	if (num_of_ints_left_in_shared_int_array > (size_t)0)
	{
		size_t stop_at, num_int = (size_t)0, previous_num_int = (size_t)0;
		high->increase_until_num_of_ints(num_of_ints_left_in_shared_int_array);
		few_bits* intarr_this = current_int_array_this->intarr + current_int_array_this->num_of_used_ints;
		size_t index_high = (size_t)0;
		custom_linked_list_node<int_array>* it_high = high->intarrays->first();
		int_array* current_high_int_array = it_high->value;
		size_t current_num_of_ints_in_high_int_array = current_high_int_array->intarr_len;
		current_high_int_array->set_num_of_used_ints_to_maximum();
		few_bits* high_intarr = current_high_int_array->intarr;
		if (current_num_of_ints_in_high_int_array < num_of_ints_left_in_shared_int_array)
			stop_at = current_num_of_ints_in_high_int_array;
		else
			stop_at = num_of_ints_left_in_shared_int_array;
		while (true)
		{
			if (num_int >= stop_at)
			{
				const size_t difference = num_int - previous_num_int;
				index_high += difference;
				previous_num_int = num_int;
				if (num_int >= num_of_ints_left_in_shared_int_array)
					break;
				if (index_high >= current_num_of_ints_in_high_int_array)
				{
					index_high = (size_t)0;
					it_high = it_high->next;
					current_high_int_array = it_high->value;
					current_high_int_array->set_num_of_used_ints_to_maximum();
					current_num_of_ints_in_high_int_array = current_high_int_array->intarr_len;
					high_intarr = current_high_int_array->intarr;
				}
				const size_t current_num_of_ints_left_in_high_int_array = current_num_of_ints_in_high_int_array - index_high;
				stop_at += current_num_of_ints_left_in_high_int_array;
				if (num_of_ints_left_in_shared_int_array < stop_at)
					stop_at = num_of_ints_left_in_shared_int_array;
				continue;
			}
			*high_intarr = *intarr_this;
			++num_int;
			++high_intarr;
			++intarr_this;
		}
		current_high_int_array->num_of_used_ints = index_high;
	}
	const size_t this_num_of_used_int_arrays_left = this_num_of_used_int_arrays - num_of_taken_int_arrays_from_this;
	if (this_num_of_used_int_arrays_left > (size_t)0)
	{
		size_t num_of_transfered_int_arrays_to_high = (size_t)0, num_of_transfered_ints_to_high = (size_t)0;
		it_this = this->intarrays->first();
		while (true)
		{
			num_of_transfered_ints_to_high += it_this->value->intarr_len;
			++num_of_transfered_int_arrays_to_high;
			if (num_of_transfered_int_arrays_to_high >= this_num_of_used_int_arrays_left)
				break;
			it_this = it_this->next;
		}
		std::unique_ptr<int_array_list> most_significant_part_of_this = this->intarrays->sublist_int_array_list(this->intarrays->first(), it_this, num_of_transfered_int_arrays_to_high, num_of_transfered_ints_to_high);
		if (high->intarrays == nullptr)
			high->intarrays = new list_of_int_arrays;
		high->intarrays->append(*most_significant_part_of_this);
	}
	high->num_of_intarrays_used = high->intarrays->size();
	high->num_of_used_ints = size_to_make_high;
	low->cutoff_leading_zeros(low->intarrays->last());
	this->flush();
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"split_at_and_use_original(size_t index, unlimited_int* high, unlimited_int* low)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (high->find_inconsistencies() || low->find_inconsistencies())
		throw std::logic_error("The inconsistencies were found at the end of split_at_and_use_original.");
#endif
}
