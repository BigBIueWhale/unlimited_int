//in use in Karatsuba algorithm
#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::split_at(const many_bits index, unlimited_int* high, unlimited_int* low) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"split_at(many_bits index, unlimited_int* low, unlimited_int* high)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::split_at(const many_bits index, unlimited_int* high, unlimited_int* low) const\"";
#endif
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if (index >= this_num_of_used_ints)
	{
		this->copy_to(*low);
		high->set_to_zero();
		return;
	}
	else if (index == 0)
	{
		this->copy_to(*high);
		low->set_to_zero();
		return;
	}
	high->is_negative = false;
	low->is_negative = false;
	many_bits length_of_high = this_num_of_used_ints - index;
	many_bits length_of_low = index;
	low->intarrays.increase_until_num_of_ints(length_of_low);
	low->num_of_used_ints = length_of_low;
	high->intarrays.increase_until_num_of_ints(length_of_high);
	high->num_of_used_ints = length_of_high;
	Node* it_this = this->intarrays.intarrays.first;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits this_index = 0;
	int_array* current_this_int_array = it_this->value;
	many_bits current_this_int_array_length = current_this_int_array->num_of_used_ints;
	Node* it_low = low->intarrays.intarrays.first;
	many_bits low_index = 0;
	int_array* current_low_int_array = it_low->value;
	current_low_int_array->set_num_of_used_ints_to_maximum();
	many_bits current_low_int_array_length = current_low_int_array->intarr_len;
	many_bits num_of_used_int_arrays_low = 1;
	few_bits* current_intarr_this = current_this_int_array->intarr;
	few_bits* current_intarr_low = current_low_int_array->intarr;
	many_bits stop_at;
	if (current_this_int_array_length < current_low_int_array_length)
	{
		stop_at = current_this_int_array_length;
	}
	else { stop_at = current_low_int_array_length; }
	if (length_of_low < stop_at) { stop_at = length_of_low; }
	while (true)
	{
		if (num_int >= stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			this_index += previous_num_int;
			low_index += previous_num_int;
			previous_num_int = num_int;
			if (num_int >= length_of_low) { break; }
			if (this_index >= current_this_int_array_length)
			{
				this_index = 0;
				it_this = it_this->next;
				current_this_int_array = it_this->value;
				current_this_int_array_length = current_this_int_array->num_of_used_ints;
				current_intarr_this = current_this_int_array->intarr;
			}
			if (low_index >= current_low_int_array_length)
			{
				++num_of_used_int_arrays_low;
				low_index = 0;
				it_low = it_low->next;
				current_low_int_array = it_low->value;
				current_low_int_array->set_num_of_used_ints_to_maximum();
				current_low_int_array_length = current_low_int_array->intarr_len;
				current_intarr_low = current_low_int_array->intarr;
			}
			many_bits num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
			many_bits num_of_ints_left_for_current_low = current_low_int_array_length - low_index;
			if (num_of_ints_left_for_current_low < num_of_ints_left_for_current_this)
			{
				stop_at += num_of_ints_left_for_current_low;
			}
			else { stop_at += num_of_ints_left_for_current_this; }
			if (length_of_low < stop_at) { stop_at = length_of_low; }
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
	Node* it_high = high->intarrays.intarrays.first;
	many_bits high_index = 0;
	int_array* current_high_int_array = it_high->value;
	current_high_int_array->set_num_of_used_ints_to_maximum();
	many_bits current_high_int_array_length = current_high_int_array->intarr_len;
	many_bits num_of_used_int_arrays_high = 1;
	few_bits* current_intarr_high = current_high_int_array->intarr;
	many_bits num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
	if (num_of_ints_left_for_current_this < current_high_int_array_length)
	{
		stop_at += num_of_ints_left_for_current_this;
	}
	else { stop_at += current_high_int_array_length; }
	if (this_num_of_used_ints < stop_at) { stop_at = this_num_of_used_ints; }
	while (true)
	{
		if (num_int >= stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			this_index += previous_num_int;
			high_index += previous_num_int;
			previous_num_int = num_int;
			if (num_int >= this_num_of_used_ints) { break; }
			if (this_index >= current_this_int_array_length)
			{
				this_index = 0;
				it_this = it_this->next;
				current_this_int_array = it_this->value;
				current_this_int_array_length = current_this_int_array->num_of_used_ints;
				current_intarr_this = current_this_int_array->intarr;
			}
			if (high_index >= current_high_int_array_length)
			{
				++num_of_used_int_arrays_high;
				high_index = 0;
				it_high = it_high->next;
				current_high_int_array = it_high->value;
				current_high_int_array->set_num_of_used_ints_to_maximum();
				current_high_int_array_length = current_high_int_array->intarr_len;
				current_intarr_high = current_high_int_array->intarr;
			}
			num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
			many_bits num_of_ints_left_for_current_high = current_high_int_array_length - high_index;
			if (num_of_ints_left_for_current_high < num_of_ints_left_for_current_this)
			{
				stop_at += num_of_ints_left_for_current_high;
			}
			else { stop_at += num_of_ints_left_for_current_this; }
			if (this_num_of_used_ints < stop_at) { stop_at = this_num_of_used_ints; }
			continue;
		}
		*current_intarr_high = *current_intarr_this;
		++current_intarr_high;
		++current_intarr_this;
		++num_int;
	}
	high->num_of_intarrays_used = num_of_used_int_arrays_high;
	current_high_int_array->num_of_used_ints = high_index;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"split_at(many_bits index, unlimited_int* low, unlimited_int* high)\"";
#endif
#if DEBUG_MODE > 0
	if (high->find_inconsistencies() || low->find_inconsistencies() || this->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::split_at(const many_bits index, unlimited_int* high, unlimited_int* low) const\"";
#endif
}
void unlimited_int::split_at_and_use_original(const many_bits index, unlimited_int* high, unlimited_int* low)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"split_at_and_use_original(many_bits index, unlimited_int* high, unlimited_int* low)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::split_at_and_use_original(const many_bits index, unlimited_int* high, unlimited_int* low)\"";
#endif
	many_bits size_of_this = this->num_of_used_ints;
	if (size_of_this == 0)
	{
		low->set_to_zero();
		high->set_to_zero();
		this->flush();
		return;
	}
	else if (index == 0)
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
	many_bits this_num_of_used_int_arrays = this->num_of_intarrays_used;
	Node* it_low = low->intarrays.intarrays.first;
	Node* it_this = this->intarrays.intarrays.first;
	many_bits num_of_ints_filled_lower = 0;
	many_bits num_of_taken_int_arrays_from_this = 0;
	many_bits size_to_make_high = size_of_this - index;
	many_bits sum_used = 0;
	while (true)
	{
		num_of_ints_filled_lower += it_this->value->intarr_len;
		sum_used += it_this->value->num_of_used_ints;
		++num_of_taken_int_arrays_from_this;
		if (sum_used >= index) { break; }
		it_this = it_this->next;
	}
	int_array* current_int_array_this = it_this->value;
	if (low->intarrays.intarrays.length == 0)
	{
		low->intarrays.intarrays.first = this->intarrays.intarrays.first;
		this->intarrays.intarrays.first = it_this->next;
		if (this->intarrays.intarrays.first == nullptr) { this->intarrays.intarrays.last = nullptr; }
		else { this->intarrays.intarrays.first->previous = nullptr; }
		low->intarrays.intarrays.last = it_this;
		low->intarrays.intarrays.last->next = nullptr;
	}
	else
	{
		Node* temp_node = it_this->next;
		it_this->next = low->intarrays.intarrays.first;
		low->intarrays.intarrays.first->previous = it_this;
		low->intarrays.intarrays.first = this->intarrays.intarrays.first;
		this->intarrays.intarrays.first = temp_node;
		if (this->intarrays.intarrays.first == nullptr) { this->intarrays.intarrays.last = nullptr; }
		else { this->intarrays.intarrays.first->previous = nullptr; }
	}
	this->intarrays.num_of_ints -= num_of_ints_filled_lower;
	low->intarrays.num_of_ints += num_of_ints_filled_lower;
	this->intarrays.intarrays.length -= num_of_taken_int_arrays_from_this;
	low->intarrays.intarrays.length += num_of_taken_int_arrays_from_this;
	low->num_of_intarrays_used = num_of_taken_int_arrays_from_this;
	low->num_of_used_ints = index;
	Node* it_high;
	many_bits num_of_ints_left_in_shared_int_array = sum_used - index;
	current_int_array_this->num_of_used_ints -= num_of_ints_left_in_shared_int_array;
	list_of_int_arrays high_prepend;
	if (num_of_ints_left_in_shared_int_array > 0)
	{
		many_bits stop_at, num_int = 0, previous_num_int = 0;
		high_prepend.increase_until_num_of_ints(num_of_ints_left_in_shared_int_array);
		few_bits* intarr_this = &current_int_array_this->intarr[current_int_array_this->num_of_used_ints];
		many_bits index_high = 0;
		it_high = high_prepend.intarrays.first;
		int_array* current_high_int_array = it_high->value;
		many_bits current_num_of_ints_in_high_int_array = current_high_int_array->intarr_len;
		current_high_int_array->set_num_of_used_ints_to_maximum();
		few_bits* high_intarr = current_high_int_array->intarr;
		if (current_num_of_ints_in_high_int_array < num_of_ints_left_in_shared_int_array)
		{
			stop_at = current_num_of_ints_in_high_int_array;
		}
		else { stop_at = num_of_ints_left_in_shared_int_array; }
		while (true)
		{
			if (num_int == stop_at)
			{
				previous_num_int = num_int - previous_num_int;
				index_high += previous_num_int;
				previous_num_int = num_int;
				if (num_int == num_of_ints_left_in_shared_int_array) { break; }
				if (index_high >= current_num_of_ints_in_high_int_array)
				{
					index_high = 0;
					it_high = it_high->next;
					current_high_int_array = it_high->value;
					current_high_int_array->set_num_of_used_ints_to_maximum();
					current_num_of_ints_in_high_int_array = current_high_int_array->intarr_len;
					high_intarr = current_high_int_array->intarr;
				}
				many_bits current_num_of_ints_left_in_high_int_array = current_num_of_ints_in_high_int_array - index_high;
				stop_at += current_num_of_ints_left_in_high_int_array;
				if (num_of_ints_left_in_shared_int_array < stop_at)
				{
					stop_at = num_of_ints_left_in_shared_int_array;
				}
				continue;
			}
			*high_intarr = *intarr_this;
			++num_int;
			++high_intarr;
			++intarr_this;
		}
		current_high_int_array->num_of_used_ints = index_high;
	}
	many_bits this_num_of_used_int_arrays_left = this_num_of_used_int_arrays - num_of_taken_int_arrays_from_this;
	if (this_num_of_used_int_arrays_left != 0)
	{
		many_bits num_of_transfered_int_arrays_to_high = 0, num_of_transfered_ints_to_high = 0;
		it_this = this->intarrays.intarrays.first;
		while (true)
		{
			num_of_transfered_ints_to_high += it_this->value->intarr_len;
			++num_of_transfered_int_arrays_to_high;
			if (num_of_transfered_int_arrays_to_high >= this_num_of_used_int_arrays_left) { break; }
			it_this = it_this->next;
		}
		if (high_prepend.intarrays.last == nullptr)
		{
			high_prepend.intarrays.first = this->intarrays.intarrays.first;
		}
		else
		{
			high_prepend.intarrays.last->next = this->intarrays.intarrays.first;
			this->intarrays.intarrays.first->previous = high_prepend.intarrays.last;
		}
		high_prepend.intarrays.last = it_this;
		this->intarrays.intarrays.first = it_this->next;
		high_prepend.intarrays.last->next = nullptr;
		if (this->intarrays.intarrays.first == nullptr) { this->intarrays.intarrays.last = nullptr; }
		else { this->intarrays.intarrays.first->previous = nullptr; }
		this->intarrays.intarrays.length -= num_of_transfered_int_arrays_to_high;
		high_prepend.intarrays.length += num_of_transfered_int_arrays_to_high;
		this->intarrays.num_of_ints -= num_of_transfered_ints_to_high;
		high_prepend.num_of_ints += num_of_transfered_ints_to_high;
	}
	high->num_of_intarrays_used = high_prepend.intarrays.length;
	high->intarrays.prepend(high_prepend);
	high->num_of_used_ints = size_to_make_high;
	if (it_low == nullptr) { it_low = low->intarrays.intarrays.last; }
	else { it_low = it_low->previous; }
	low->cutoff_leading_zeros(it_low);
	this->flush();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"split_at_and_use_original(many_bits index, unlimited_int* high, unlimited_int* low)\"";
#endif
#if DEBUG_MODE > 0
	if (high->find_inconsistencies() || low->find_inconsistencies())
		throw "\nThe inconsistencies were found at the end of split_at_and_use_original.";
#endif
}
