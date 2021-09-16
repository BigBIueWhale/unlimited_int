#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
unlimited_int unlimited_int::operator&(const unlimited_int& right) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in beginning of function \"unlimited_int* unlimited_int::operator&(const unlimited_int& right) const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || right.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in beginning of function: \"unlimited_int* unlimited_int::operator&(const unlimited_int& right) const\"");
#endif
	if (this->is_negative() || right.is_negative())
		throw std::invalid_argument("Can\'t do bitwise operation on negative number");
	if (this->is_zero() || right.is_zero())
		return unlimited_int(); //returns 0
	const unlimited_int* shorter_num = this;
	const unlimited_int* longer_num = &right;
	if (this->num_of_used_ints > right.num_of_used_ints)
	{
		const unlimited_int* temp_ui_ptr = shorter_num;
		shorter_num = longer_num;
		longer_num = temp_ui_ptr;
	}
	unlimited_int result;
	const size_t len_of_result = shorter_num->num_of_used_ints;
	if (len_of_result == (size_t)0)
		return result;
	result.increase_until_num_of_ints(len_of_result);
	custom_linked_list_node<int_array>* current_int_array_Node_shorter_num = shorter_num->intarrays->first();
	custom_linked_list_node<int_array>* current_int_array_Node_longer_num = longer_num->intarrays->first();
	custom_linked_list_node<int_array>* current_int_array_Node_result = result.intarrays->first();
	size_t num_of_intarrays_in_result = (size_t)1;
	int_array current_int_array_shorter_num = *current_int_array_Node_shorter_num->value;
	int_array current_int_array_longer_num = *current_int_array_Node_longer_num->value;
	int_array current_int_array_result = *current_int_array_Node_result->value;
	current_int_array_Node_result->value->set_num_of_used_ints_to_maximum();
	size_t index_shorter_num = (size_t)0, index_longer_num = (size_t)0, index_result = (size_t)0;

	size_t stop_for_result = current_int_array_result.intarr_len;
	size_t stop_for_shorter_num = current_int_array_shorter_num.num_of_used_ints;
	size_t stop_for_longer_num = current_int_array_longer_num.num_of_used_ints;

	size_t stop_at = stop_for_result;
	if (stop_for_shorter_num < stop_at)
		stop_at = stop_for_shorter_num;
	if (stop_for_longer_num < stop_at)
		stop_at = stop_for_longer_num;
	if (len_of_result < stop_at)
		stop_at = len_of_result;

	size_t int_num_counter = (size_t)0;
	while (true)
	{
		if (int_num_counter >= stop_at)
		{
			if (int_num_counter >= len_of_result)
				break;
			if (index_result >= current_int_array_result.intarr_len)
			{
				index_result = (size_t)0;
				current_int_array_Node_result = current_int_array_Node_result->next;
				current_int_array_result = *current_int_array_Node_result->value;
				++num_of_intarrays_in_result;
				current_int_array_Node_result->value->num_of_used_ints = current_int_array_result.intarr_len;
			}
			if (index_shorter_num >= current_int_array_shorter_num.num_of_used_ints)
			{
				index_shorter_num = (size_t)0;
				current_int_array_Node_shorter_num = current_int_array_Node_shorter_num->next;
				current_int_array_shorter_num = *current_int_array_Node_shorter_num->value;
			}
			if (index_longer_num >= current_int_array_longer_num.num_of_used_ints)
			{
				index_longer_num = (size_t)0;
				current_int_array_Node_longer_num = current_int_array_Node_longer_num->next;
				current_int_array_longer_num = *current_int_array_Node_longer_num->value;
			}
			const size_t stop_for_result = int_num_counter + current_int_array_result.intarr_len - index_result;
			const size_t stop_for_shorter_num = int_num_counter + current_int_array_shorter_num.num_of_used_ints - index_shorter_num;
			const size_t stop_for_longer_num = int_num_counter + current_int_array_longer_num.num_of_used_ints - index_longer_num;
			stop_at = stop_for_result;
			if (stop_for_shorter_num < stop_at)
				stop_at = stop_for_shorter_num;
			if (stop_for_longer_num < stop_at)
				stop_at = stop_for_longer_num;
			if (len_of_result < stop_at)
				stop_at = len_of_result;
			continue;
		}
		current_int_array_result.intarr[index_result] = current_int_array_shorter_num.intarr[index_shorter_num] & current_int_array_longer_num.intarr[index_longer_num];
		++index_result;
		++index_longer_num;
		++index_shorter_num;
		++int_num_counter;
	}
	current_int_array_Node_result->value->num_of_used_ints = index_result;
	result.num_of_used_ints = len_of_result;
	result.num_of_intarrays_used = num_of_intarrays_in_result;
	result.cutoff_leading_zeros(current_int_array_Node_result);
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int* unlimited_int::operator&(const unlimited_int& right) const\"";
#endif
#if DEBUG_MODE > 0
	if (result.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int* unlimited_int::operator&(const unlimited_int& right) const\"");
#endif
	return result;
}
