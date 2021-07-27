#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//finds the log2 only if the result is a whole number. That's why it's technically a bitwise operation (sort of)
many_bits_signed unlimited_int::find_exact_log_2() const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"many_bits_signed unlimited_int::find_exact_log_2() const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function: \"many_bits_signed unlimited_int::find_exact_log_2() const\"";
#endif
	if (this->num_of_used_ints == 0)
		return (many_bits_signed)(-1);
	Node* current_int_array_Node = this->get_most_significant_used_int_array();
	int_array current_int_array = *current_int_array_Node->value;
	many_bits_signed index_in_int_array = current_int_array.num_of_used_ints - 1;
	few_bits current_int = current_int_array.intarr[index_in_int_array];
	const few_bits most_significant_int = current_int;
	const int num_of_zeros_preceding_most_significant_int = unlimited_int::num_of_zero_bits_preceding_number(most_significant_int);
	const many_bits num_of_used_bits_in_most_significant_int = (many_bits)NUM_OF_BITS_few_bits - (many_bits)num_of_zeros_preceding_most_significant_int;
	if ((few_bits)(most_significant_int << (num_of_zeros_preceding_most_significant_int + 1)) > (few_bits)0U) //there's more than one 1 bit in the number
		return (many_bits_signed)(-1);
	--index_in_int_array;
	if (index_in_int_array < 0)
	{
		current_int_array_Node = current_int_array_Node->previous;
		if (current_int_array_Node != nullptr)
		{
			current_int_array = *current_int_array_Node->value;
			index_in_int_array = current_int_array.num_of_used_ints - 1;
		}
	}
	if (current_int_array_Node != nullptr)
	{
		while (true)
		{
			if (index_in_int_array < 0)
			{
				current_int_array_Node = current_int_array_Node->previous;
				if (current_int_array_Node == nullptr)
					break;
				current_int_array = *current_int_array_Node->value;
				index_in_int_array = current_int_array.num_of_used_ints - 1;
			}
			if (current_int_array.intarr[index_in_int_array] != (few_bits)0)
				return (many_bits_signed)(-1);
			--index_in_int_array;
		}
	}
	many_bits_signed num_of_used_bits_in_number = (this->num_of_used_ints - (many_bits)1) * (many_bits)NUM_OF_BITS_few_bits + num_of_used_bits_in_most_significant_int;
	return num_of_used_bits_in_number - 1;
}
