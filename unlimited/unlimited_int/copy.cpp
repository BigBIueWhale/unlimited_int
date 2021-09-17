#include "unlimited_int.hpp"
#include <algorithm>
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::copy_to(unlimited_int& num_to_paste_into) const
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"copy_to\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("inconsistency found in the beginning of \"void unlimited_int::copy_to(unlimited_int& num_to_paste_into) const\"");
#endif
	num_to_paste_into.auto_destroy = true;
	size_t this_num_of_used_ints = this->num_of_used_ints;
	num_to_paste_into.increase_until_num_of_ints(this_num_of_used_ints);
	num_to_paste_into.num_of_used_ints = this_num_of_used_ints;
	num_to_paste_into._is_negative = this->_is_negative;
	if (this->num_of_used_ints == (size_t)0)
	{
		num_to_paste_into.set_to_zero();
		return;
	}
	size_t num_of_used_arrays_in_paste = (size_t)1;
	custom_linked_list_node<int_array>* it_this = this->intarrays->first();
	custom_linked_list_node<int_array>* it_paste = num_to_paste_into.intarrays->first();
	int_array* current_int_array_this = it_this->value;
	int_array* current_int_array_paste = it_paste->value;
	size_t index_this = (size_t)0, index_paste = (size_t)0;
	size_t num_int = (size_t)0, previous_num_int = (size_t)0;
	size_t current_int_array_this_num_of_used_ints = current_int_array_this->num_of_used_ints;
	size_t current_int_array_paste_intarr_len = current_int_array_paste->intarr_len;
	few_bits* current_int_array_paste_intarr = current_int_array_paste->intarr;
	few_bits* current_int_array_this_intarr = current_int_array_this->intarr;
	size_t stop_at = this_num_of_used_ints;
	if (current_int_array_this_num_of_used_ints < stop_at)
		stop_at = current_int_array_this_num_of_used_ints;
	if (current_int_array_paste_intarr_len < stop_at)
		stop_at = current_int_array_paste_intarr_len;
	current_int_array_paste->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int >= stop_at)
		{
			const size_t difference = num_int - previous_num_int;
			index_this += difference;
			index_paste += difference;
			previous_num_int = num_int;
			if (num_int >= this_num_of_used_ints)
				break;
			if (index_this >= current_int_array_this_num_of_used_ints)
			{
				it_this = it_this->next;
				current_int_array_this = it_this->value;
				current_int_array_this_num_of_used_ints = current_int_array_this->num_of_used_ints;
				current_int_array_this_intarr = current_int_array_this->intarr;
				index_this = (size_t)0;
			}
			if (index_paste >= current_int_array_paste_intarr_len)
			{
				it_paste = it_paste->next;
				current_int_array_paste = it_paste->value;
				num_of_used_arrays_in_paste++;
				current_int_array_paste->set_num_of_used_ints_to_maximum();
				current_int_array_paste_intarr_len = current_int_array_paste->intarr_len;
				current_int_array_paste_intarr = current_int_array_paste->intarr;
				index_paste = (size_t)0;
			}
			const size_t paste_intarr_len_left_to_go_through = current_int_array_paste_intarr_len - index_paste;
			const size_t this_intarr_len_left_to_go_through = current_int_array_this_num_of_used_ints - index_this;
			if (paste_intarr_len_left_to_go_through < this_intarr_len_left_to_go_through)
				stop_at += paste_intarr_len_left_to_go_through;
			else
				stop_at += this_intarr_len_left_to_go_through;
			if (this_num_of_used_ints < stop_at)
				stop_at = this_num_of_used_ints;
			continue;
		}
		*current_int_array_paste_intarr = *current_int_array_this_intarr;
		++current_int_array_paste_intarr;
		++current_int_array_this_intarr;
		++num_int;
	}
	num_to_paste_into.num_of_intarrays_used = num_of_used_arrays_in_paste;
	current_int_array_paste->num_of_used_ints = index_paste;
	num_to_paste_into.flush_unused();
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"copy_to\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_paste_into.find_inconsistencies())
		throw std::logic_error("Inconsistency was found in the end of \"void unlimited_int::copy_to(unlimited_int& num_to_paste_into) const\"");
#endif
}
void unlimited_int::copy_most_significant_to(unlimited_int& num_to_paste_into, const size_t num_of_ints_to_copy) const
{
	size_t num_of_ints_to_copy_cpy = num_of_ints_to_copy;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"copy_most_significant_to\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("Inconsistency was found in start of function \"void unlimited_int::copy_most_significant_to(unlimited_int& num_to_paste_into, const size_t num_of_ints_to_copy) const\"");
#endif
	num_to_paste_into._is_negative = false;
	if (this->num_of_intarrays_used == (size_t)0)
	{
		num_to_paste_into.set_to_zero();
		return;
	}
	size_t this_num_of_used_ints = this->num_of_used_ints;
	if (num_of_ints_to_copy_cpy > this_num_of_used_ints)
		num_of_ints_to_copy_cpy = this_num_of_used_ints;
	num_to_paste_into.increase_until_num_of_ints(num_of_ints_to_copy_cpy);
	num_to_paste_into.num_of_used_ints = num_of_ints_to_copy_cpy;
	int_array_list::list_location ll = num_to_paste_into.intarrays->find_num_of_int_from_insignificant(num_of_ints_to_copy_cpy);
	custom_linked_list_node<int_array>* it_paste = ll.node;
	size_t index_paste = ll.index;
	num_to_paste_into.num_of_intarrays_used = ll.num_array;
	num_to_paste_into.num_of_used_ints = num_of_ints_to_copy_cpy;
	int_array* current_int_array_paste = it_paste->value;
	current_int_array_paste->num_of_used_ints = index_paste + (size_t)1;
	custom_linked_list_node<int_array>* it_this = this->get_most_significant_used_int_array();
	int_array* current_int_array_this = it_this->value;
	size_t index_this = current_int_array_this->num_of_used_ints - (size_t)1;
	size_t num_int = (size_t)0, stop_at, previous_num_int = (size_t)0;
	size_t num_of_ints_left_for_this = index_this + (size_t)1;
	size_t num_of_ints_left_for_paste = index_paste + (size_t)1;
	few_bits* current_intarr_this = current_int_array_this->intarr + index_this;
	few_bits* current_intarr_paste = current_int_array_paste->intarr + index_paste;
	if (num_of_ints_left_for_this < num_of_ints_left_for_paste)
		stop_at = num_of_ints_left_for_this;
	else
		stop_at = num_of_ints_left_for_paste;
	if (num_of_ints_to_copy_cpy < stop_at)
		stop_at = num_of_ints_to_copy_cpy;
	while (true)
	{
		if (num_int >= stop_at)
		{
			const size_t difference = num_int - previous_num_int;
			const bool reached_end_paste = difference > index_paste;
			index_paste -= difference;
			const bool reached_end_this = difference > index_this;
			index_this -= difference;
			previous_num_int = num_int;
			if (num_int >= num_of_ints_to_copy_cpy)
				break;
			if (reached_end_this)
			{
				it_this = it_this->previous;
				current_int_array_this = it_this->value;
				index_this = current_int_array_this->num_of_used_ints - (size_t)1;
				current_intarr_this = current_int_array_this->intarr + index_this;
			}
			if (reached_end_paste)
			{
				it_paste = it_paste->previous;
				current_int_array_paste = it_paste->value;
				index_paste = current_int_array_paste->intarr_len - (size_t)1;
				current_intarr_paste = current_int_array_paste->intarr + index_paste;
				current_int_array_paste->set_num_of_used_ints_to_maximum();
			}
			num_of_ints_left_for_this = index_this + (size_t)1;
			num_of_ints_left_for_paste = index_paste + (size_t)1;
			if (num_of_ints_left_for_this < num_of_ints_left_for_paste)
				stop_at += num_of_ints_left_for_this;
			else
				stop_at += num_of_ints_left_for_paste;
			if (num_of_ints_to_copy_cpy < stop_at)
				stop_at = num_of_ints_to_copy_cpy;
			continue;
		}
		*current_intarr_paste = *current_intarr_this;
		--current_intarr_paste;
		--current_intarr_this;
		++num_int;
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"copy_most_significant_to\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_paste_into.find_inconsistencies())
		throw std::logic_error("Inconsistency was found in the end of function \"void unlimited_int::copy_most_significant_to(unlimited_int& num_to_paste_into, const size_t num_of_ints_to_copy) const\"");
#endif
}
void unlimited_int::swap(unlimited_int& num_to_swap_with)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"swap\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_swap_with.find_inconsistencies())
		throw std::logic_error("Inconsistency was found in start of function \"void unlimited_int::swap(unlimited_int& num_to_swap_with)\"");
#endif
	std::swap(this->_is_negative, num_to_swap_with._is_negative);
	std::swap(this->auto_destroy, num_to_swap_with.auto_destroy);
	std::swap(this->num_of_intarrays_used, num_to_swap_with.num_of_intarrays_used);
	std::swap(this->num_of_used_ints, num_to_swap_with.num_of_used_ints);
	std::swap(this->intarrays, num_to_swap_with.intarrays);
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"swap\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_swap_with.find_inconsistencies())
		throw std::logic_error("Inconsistency was found in end of function \"void unlimited_int::swap(unlimited_int& num_to_swap_with)\"");
#endif
}
