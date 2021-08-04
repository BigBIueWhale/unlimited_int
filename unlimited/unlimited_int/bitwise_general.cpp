#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
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
	return ((many_bits)amount_to_shift - (many_bits)1);
}
many_bits unlimited_int::get_length_in_bits() const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"many_bits unlimited_int::get_length_in_bits() const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function: \"many_bits unlimited_int::get_length_in_bits() const\"";
#endif
	if (this->num_of_used_ints == 0)
		return (many_bits)0;
	else
	{
		const Node* most_significant_used_int_array_Node = this->get_most_significant_used_int_array();
		const int_array most_significant_used_int_array = *most_significant_used_int_array_Node->value;
		const many_bits index_of_most_significant = most_significant_used_int_array.num_of_used_ints - 1;
		const few_bits most_significant_value = most_significant_used_int_array.intarr[index_of_most_significant];
		const many_bits num_of_0_bits_preceding_num = unlimited_int::num_of_zero_bits_preceding_number(most_significant_value);
		return (this->num_of_used_ints * (many_bits)NUM_OF_BITS_few_bits - num_of_0_bits_preceding_num);
	}
}