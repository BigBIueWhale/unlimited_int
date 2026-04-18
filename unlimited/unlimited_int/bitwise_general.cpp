#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
int unlimited_int::num_of_zero_bits_preceding_number(const few_bits original_num)
{
	if (original_num == (few_bits)0U)
		return NUM_OF_BITS_few_bits;
	few_bits original_num_cpy = original_num;
	int amount_to_shift;
	for (amount_to_shift = 1; amount_to_shift < NUM_OF_BITS_few_bits; ++amount_to_shift)
	{
		original_num_cpy <<= amount_to_shift;
		original_num_cpy >>= amount_to_shift;
		if (original_num_cpy != original_num)
			break;
	}
	return amount_to_shift - 1;
}
int unlimited_int::find_exact_log_2(const few_bits num, bool* const is_power_2)
{
	//Fast power-of-2 test: num has exactly one bit set if and only if num is non-zero and "num & (num - 1)" is zero. See the longer comment in bitwise_log2.cpp for the reasoning. This rejection runs in a handful of native instructions and avoids calling the O(NUM_OF_BITS_few_bits) helpers in the common case where num is not a power of 2.
	if (num == (few_bits)0 || ((num & (num - (few_bits)1)) != (few_bits)0))
	{
		*is_power_2 = false;
		return 0;
	}
	//num has exactly one bit set, so it is a power of 2. The position of that bit inside num is what we need to return as the log2. That position is NUM_OF_BITS_few_bits - 1 - the count of zero bits to the left of the set bit.
	const int preceding = unlimited_int::num_of_zero_bits_preceding_number(num);
	*is_power_2 = true;
	return NUM_OF_BITS_few_bits - 1 - preceding;
}
size_t unlimited_int::get_length_in_bits() const
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"size_t unlimited_int::get_length_in_bits() const\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function: \"size_t unlimited_int::get_length_in_bits() const\"");
#endif
	if (this->num_of_used_ints == (size_t)0)
		return (size_t)0;
	else
	{
		const custom_linked_list_node<int_array> *const most_significant_used_int_array_Node = this->get_most_significant_used_int_array();
		const int_array most_significant_used_int_array = *most_significant_used_int_array_Node->value;
		const size_t index_of_most_significant = most_significant_used_int_array.num_of_used_ints - (size_t)1;
		const few_bits most_significant_value = most_significant_used_int_array.intarr[index_of_most_significant];
		const int num_of_0_bits_preceding_num = unlimited_int::num_of_zero_bits_preceding_number(most_significant_value);
		return (this->num_of_used_ints * (size_t)NUM_OF_BITS_few_bits - (size_t)num_of_0_bits_preceding_num);
	}
}
