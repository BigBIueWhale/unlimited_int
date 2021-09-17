#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
int unlimited_int::num_of_zero_bits_preceding_number(const few_bits original_num)
{
	if (original_num == (few_bits)0U)
		return UNLIMITED_INT_NUM_OF_BITS_few_bits;
	few_bits original_num_cpy = original_num;
	int amount_to_shift;
	for (amount_to_shift = 1; amount_to_shift < UNLIMITED_INT_NUM_OF_BITS_few_bits; ++amount_to_shift)
	{
		original_num_cpy <<= amount_to_shift;
		original_num_cpy >>= amount_to_shift;
		if (original_num_cpy != original_num)
			break;
	}
	return amount_to_shift - 1;
}
int unlimited_int::num_of_zero_bits_succeeding_number(const few_bits original_num)
{
	if (original_num == (few_bits)0U)
		return UNLIMITED_INT_NUM_OF_BITS_few_bits;
	few_bits original_num_cpy = original_num;
	int amount_to_shift;
	for (amount_to_shift = 1; amount_to_shift < UNLIMITED_INT_NUM_OF_BITS_few_bits; ++amount_to_shift)
	{
		original_num_cpy >>= amount_to_shift;
		original_num_cpy <<= amount_to_shift;
		if (original_num_cpy != original_num)
			break;
	}
	return amount_to_shift - 1;
}
int unlimited_int::find_exact_log_2(const few_bits num, bool* const is_power_2)
{
	const int preceding = unlimited_int::num_of_zero_bits_preceding_number(num);
	const int succeeding = unlimited_int::num_of_zero_bits_succeeding_number(num);
	const bool is_power_of_2_local = preceding + succeeding + 1 == UNLIMITED_INT_NUM_OF_BITS_few_bits;
	*is_power_2 = is_power_of_2_local;
	if (is_power_of_2_local)
		return succeeding;
	return 0;
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
		return (this->num_of_used_ints * (size_t)UNLIMITED_INT_NUM_OF_BITS_few_bits - (size_t)num_of_0_bits_preceding_num);
	}
}
