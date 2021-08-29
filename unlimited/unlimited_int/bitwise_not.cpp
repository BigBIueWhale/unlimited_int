#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
std::shared_ptr<unlimited_int> unlimited_int::operator~() const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in beginning of function \"unlimited_int* unlimited_int::operator~() const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in beginning of function: \"unlimited_int* unlimited_int::operator~() const\"");
#endif
	unlimited_int* result = new unlimited_int;
	const size_t len_of_result = this->num_of_used_ints;
	if (len_of_result == (size_t)0)
		return std::shared_ptr<unlimited_int>(result);
	result->increase_until_num_of_ints(len_of_result);
	const size_t len_of_this = this->num_of_used_ints;
	custom_linked_list_node<int_array>* current_int_array_Node_this = this->intarrays->first();
	custom_linked_list_node<int_array>* current_int_array_Node_result = result->intarrays->first();
	size_t num_of_intarrays_in_result = (size_t)1;
	int_array current_int_array_this = *current_int_array_Node_this->value;
	int_array current_int_array_result = *current_int_array_Node_result->value;
	current_int_array_Node_result->value->num_of_used_ints = current_int_array_result.intarr_len;
	size_t index_this = (size_t)0, index_result = (size_t)0;

	const size_t stop_for_result = current_int_array_result.intarr_len;
	const size_t stop_for_this = current_int_array_this.num_of_used_ints;

	size_t stop_at = stop_for_result;
	if (stop_for_this < stop_at)
		stop_at = stop_for_this;
	if (len_of_result < stop_at)
		stop_at = len_of_result;

	size_t int_num_counter = (size_t)0;
	while (true)
	{
		if (int_num_counter >= stop_at)
		{
			if (int_num_counter >= len_of_this)
				break;
			if (index_result >= current_int_array_result.intarr_len)
			{
				index_result = (size_t)0;
				current_int_array_Node_result = current_int_array_Node_result->next;
				current_int_array_result = *current_int_array_Node_result->value;
				++num_of_intarrays_in_result;
				current_int_array_Node_result->value->num_of_used_ints = current_int_array_result.intarr_len;
			}
			if (index_this >= current_int_array_this.num_of_used_ints)
			{
				index_this = (size_t)0;
				current_int_array_Node_this = current_int_array_Node_this->next;
				current_int_array_this = *current_int_array_Node_this->value;
			}
			const size_t stop_for_result = int_num_counter + current_int_array_result.intarr_len - index_result;
			const size_t stop_for_this = int_num_counter + current_int_array_this.num_of_used_ints - index_this;
			stop_at = stop_for_result;
			if (stop_for_this < stop_at)
				stop_at = stop_for_this;
			if (len_of_this < stop_at)
				stop_at = len_of_this;
			continue;
		}
		current_int_array_result.intarr[index_result] = ~current_int_array_this.intarr[index_this];
		++index_result;
		++index_this;
		++int_num_counter;
	}
	current_int_array_Node_result->value->num_of_used_ints = index_result;
	--index_result;
	--index_this;
	const few_bits most_significant_int_in_this = current_int_array_this.intarr[index_this];
	const int num_preceding_zeros = unlimited_int::num_of_zero_bits_preceding_number(most_significant_int_in_this);
	const few_bits inverted_most_significant_int_in_this = (few_bits)(~(most_significant_int_in_this << num_preceding_zeros)) >> num_preceding_zeros;
	current_int_array_result.intarr[index_result] = inverted_most_significant_int_in_this;
	result->num_of_used_ints = len_of_result;
	result->num_of_intarrays_used = num_of_intarrays_in_result;
	result->cutoff_leading_zeros(current_int_array_Node_result);
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int* unlimited_int::operator~() const\"";
#endif
#if DEBUG_MODE > 0
	if (result->find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in end of function: \"unlimited_int* unlimited_int::operator~() const\"");
#endif
	return std::shared_ptr<unlimited_int>(result);
}
void unlimited_int::invert_bits()
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in beginning of function \"void unlimited_int::invert_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in beginning of function: \"void unlimited_int::invert_bits()\"");
#endif
	const size_t len_of_this = this->num_of_used_ints;
	custom_linked_list_node<int_array>* current_int_array_Node_this = this->intarrays->first();
	int_array current_int_array_this = *current_int_array_Node_this->value;
	size_t index_this = (size_t)0;

	const size_t stop_for_this = current_int_array_this.num_of_used_ints;

	size_t stop_at = stop_for_this;
	if (len_of_this < stop_at)
		stop_at = len_of_this;
	size_t int_num_counter = (size_t)0;
	while (true)
	{
		if (int_num_counter >= stop_at)
		{
			if (int_num_counter >= len_of_this)
				break;
			if (index_this >= current_int_array_this.num_of_used_ints)
			{
				index_this = (size_t)0;
				current_int_array_Node_this = current_int_array_Node_this->next;
				current_int_array_this = *current_int_array_Node_this->value;
			}
			const many_bits stop_for_this = int_num_counter + current_int_array_this.num_of_used_ints - index_this;
			stop_at = stop_for_this;
			if (len_of_this < stop_at)
				stop_at = len_of_this;
			continue;
		}
		current_int_array_this.intarr[index_this] = ~current_int_array_this.intarr[index_this];
		++index_this;
		++int_num_counter;
	}
	--index_this;
	const few_bits most_significant_int_in_this = current_int_array_this.intarr[index_this];
	const int num_preceding_zeros = unlimited_int::num_of_zero_bits_preceding_number(most_significant_int_in_this);
	const few_bits inverted_most_significant_int_in_this = (few_bits)(~(most_significant_int_in_this << num_preceding_zeros)) >> num_preceding_zeros;
	current_int_array_this.intarr[index_this] = inverted_most_significant_int_in_this;
	this->cutoff_leading_zeros(current_int_array_Node_this);
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::invert_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in end of function: \"void unlimited_int::invert_bits()\"");
#endif
}
