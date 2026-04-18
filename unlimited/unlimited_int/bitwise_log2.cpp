#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
//finds the log2 only if the result is a whole number. That's why it's technically a bitwise operation (sort of)
size_t unlimited_int::find_exact_log_2(bool *const is_power_2) const
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"size_t unlimited_int::find_exact_log_2(bool* is_power_2) const\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function: \"size_t unlimited_int::find_exact_log_2(bool* is_power_2) const\"");
#endif
	if (this->num_of_used_ints == (size_t)0)
	{
		*is_power_2 = false;
		return MAX_size_t_NUM;
	}
	custom_linked_list_node<int_array> *const most_significant_int_array_Node = this->get_most_significant_used_int_array();
	const int_array& most_significant_int_array = *most_significant_int_array_Node->value;
	const few_bits most_significant_int = most_significant_int_array.intarr[most_significant_int_array.num_of_used_ints - (size_t)1];
	//Check whether most_significant_int has exactly one bit set. The expression "x & (x - 1)" clears the lowest set bit of x, so the result is zero exactly when x had at most one bit set. Combined with the most_significant_int != 0 guard this is the standard one-line power-of-2 test. It returns the same answer as "num_of_zero_bits_preceding_number + num_of_zero_bits_succeeding_number + 1 == NUM_OF_BITS_few_bits" but does not call either helper, which avoids those helpers' O(NUM_OF_BITS_few_bits) internal loop in the common case where most_significant_int is not a power of 2.
	if (most_significant_int == (few_bits)0 || ((most_significant_int & (most_significant_int - (few_bits)1)) != (few_bits)0)) //there's more than one 1 bit in the number, or the number is zero
	{
		*is_power_2 = false;
		return MAX_size_t_NUM;
	}
	//Reaching here means most_significant_int has exactly one bit set, so the whole number is a power of 2 if and only if every less-significant int is zero. num_of_zero_bits_preceding_number is used below only to figure out which bit inside most_significant_int is the one that is set, so the bit position can be added to the count of bits in the less-significant ints when producing the return value.
	const int num_of_zeros_preceding_most_significant_int = unlimited_int::num_of_zero_bits_preceding_number(most_significant_int);
	const size_t num_of_used_bits_in_most_significant_int = (size_t)NUM_OF_BITS_few_bits - (size_t)num_of_zeros_preceding_most_significant_int;
	//Iterate forward (the natural list direction) from the least-significant int_array to the int_array before the most-significant one, checking that every int in every such int_array is zero. This avoids ->previous, avoids a begin-sentinel termination check, and naturally absorbs any zero-num_of_used_ints int_array that might appear on the path (its inner loop body just runs zero times).
	for (custom_linked_list_node<int_array>* it = this->intarrays->first(); it != most_significant_int_array_Node; it = it->next)
	{
		const int_array& current_int_array = *it->value;
		for (size_t index_in_int_array = (size_t)0; index_in_int_array < current_int_array.num_of_used_ints; ++index_in_int_array)
		{
			if (current_int_array.intarr[index_in_int_array] != (few_bits)0)
			{
				*is_power_2 = false;
				return MAX_size_t_NUM;
			}
		}
	}
	//Check the less-significant ints within the most-significant int_array itself (every int except the most-significant one, which was already tested above).
	for (size_t index_in_int_array = (size_t)0; index_in_int_array + (size_t)1 < most_significant_int_array.num_of_used_ints; ++index_in_int_array)
	{
		if (most_significant_int_array.intarr[index_in_int_array] != (few_bits)0)
		{
			*is_power_2 = false;
			return MAX_size_t_NUM;
		}
	}
	const size_t num_of_used_bits_in_number = (this->num_of_used_ints - (size_t)1) * (size_t)NUM_OF_BITS_few_bits + num_of_used_bits_in_most_significant_int;
	*is_power_2 = true;
	return num_of_used_bits_in_number - (size_t)1;
}
