#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//finds the log2 only if the result is a whole number. That's why it's technically a bitwise operation (sort of)
size_t unlimited_int::find_exact_log_2(bool *const is_power_2) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"size_t unlimited_int::find_exact_log_2(bool* is_power_2) const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function: \"size_t unlimited_int::find_exact_log_2(bool* is_power_2) const\"");
#endif
	if (this->num_of_used_ints == (size_t)0)
	{
		*is_power_2 = false;
		return MAX_size_t_NUM;
	}
	custom_linked_list_node<int_array>* current_int_array_Node = this->get_most_significant_used_int_array();
	int_array current_int_array = *current_int_array_Node->value;
	size_t index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
	few_bits current_int = current_int_array.intarr[index_in_int_array];
	const few_bits most_significant_int = current_int;
	const int num_of_zeros_preceding_most_significant_int = unlimited_int::num_of_zero_bits_preceding_number(most_significant_int);
	const size_t num_of_used_bits_in_most_significant_int = (size_t)NUM_OF_BITS_few_bits - (size_t)num_of_zeros_preceding_most_significant_int;
	if ((few_bits)(most_significant_int << (num_of_zeros_preceding_most_significant_int + 1)) > (few_bits)0) //there's more than one 1 bit in the number
	{
		*is_power_2 = false;
		return MAX_size_t_NUM;
	}
	bool got_to_beginning = false;
	if (index_in_int_array-- == (size_t)0)
	{
		current_int_array_Node = current_int_array_Node->previous;
		if (current_int_array_Node != this->intarrays->begin())
		{
			current_int_array = *current_int_array_Node->value;
			index_in_int_array = current_int_array.num_of_used_ints;
			if (index_in_int_array-- == (size_t)0)
				got_to_beginning = true;
		}
	}
	if (current_int_array_Node != this->intarrays->begin())
	{
		while (true)
		{
			if (got_to_beginning)
			{
				got_to_beginning = false;
				current_int_array_Node = current_int_array_Node->previous;
				if (current_int_array_Node == this->intarrays->begin())
					break;
				current_int_array = *current_int_array_Node->value;
				index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
			}
			if (current_int_array.intarr[index_in_int_array] != (few_bits)0)
			{
				*is_power_2 = false;
				return MAX_size_t_NUM;
			}
			if (index_in_int_array-- == (size_t)0)
				got_to_beginning = true;
		}
	}
	const size_t num_of_used_bits_in_number = (this->num_of_used_ints - (size_t)1) * (size_t)NUM_OF_BITS_few_bits + num_of_used_bits_in_most_significant_int;
	*is_power_2 = true;
	return num_of_used_bits_in_number - (size_t)1;
}
