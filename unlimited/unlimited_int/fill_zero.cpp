#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::fill_0_until_num_of_ints(const size_t fill_0_until)
{
	if (fill_0_until == (size_t)0)
		return;
	else if (this->intarrays == nullptr)
		throw std::logic_error("Error in function fill_0_until_num_of_ints, can\'t fill a list of arrays that doesn\'t exist with zeros");
	else if (fill_0_until > this->intarrays->num_of_ints)
		throw std::logic_error("Error in function fill_0_until_num_of_ints, trying to fill with zeros more ints than exist in the list.");
	//if (!this->auto_destroy) { this->forget_memory(); } this line of code might be important
	custom_linked_list_node<int_array>* it = this->intarrays->first();
	size_t num_of_ints_zeroed_until_now = (size_t)0;
	int_array* current_int_array;
	while (true)
	{
		current_int_array = it->value;
		size_t sum = num_of_ints_zeroed_until_now + current_int_array->intarr_len;
		if (sum >= fill_0_until)
		{
			current_int_array->fillzero_until(fill_0_until - num_of_ints_zeroed_until_now);
			break;
		}
		else
		{
			num_of_ints_zeroed_until_now = sum;
			current_int_array->fillzero();
		}
		it = it->next;
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"fill_0_until_num_of_ints(many_bits fill_0_until)\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("inconsistency found in the end of \"void unlimited_int::fill_0_until_num_of_ints(const many_bits fill_0_until)\"");
#endif
}
void unlimited_int::fill_0_until_num_of_ints_and_set_used_ints_to_maximum(const size_t fill_0_until)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until)\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("inconsistency found in the beginning of \"void unlimited_int::fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until)\"");
#endif
	if (fill_0_until == (size_t)0)
		return;
	else if (this->intarrays == nullptr)
		throw std::logic_error("Error in function fill_0_until_num_of_ints, can\'t fill a list of arrays that doesn\'t exist with zeros");
	else if (fill_0_until > this->intarrays->num_of_ints)
		throw std::logic_error("Error in function fill_0_until_num_of_ints, trying to fill with zeros more ints than exist in the list.");
	custom_linked_list_node<int_array>* it = this->intarrays->first();
	size_t num_of_ints_zeroed_until_now = (size_t)0;
	while (true)
	{
		int_array *const current_int_array = it->value;
		current_int_array->set_num_of_used_ints_to_maximum();
		size_t sum = num_of_ints_zeroed_until_now + current_int_array->intarr_len;
		if (sum >= fill_0_until)
		{
			current_int_array->fillzero_until(fill_0_until - num_of_ints_zeroed_until_now);
			break;
		}
		else
		{
			num_of_ints_zeroed_until_now = sum;
			current_int_array->fillzero();
		}
		it = it->next;
	}
}
