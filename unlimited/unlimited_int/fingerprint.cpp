#include "unlimited_int.hpp"
using namespace unlimited;

//quickest possible type of hash function. Non cryptographic, ignores sign.
//The random numbers in this file were literally just random numbers generated using python's random.randint().
//There's nothing special about them, don't scratch your head about it.
size_t unlimited_int::fingerprint() const
{
	if (this->num_of_used_ints == (size_t)0)
	{
#if IS_64_BIT_SYSTEM
		return (size_t)6507803568836410511; //arbitrary number
#else
		return (size_t)568218086; //arbitrary number
#endif
	}
	constexpr int half_sizeof_size_t = (sizeof(size_t) * (size_t)8) / (size_t)2;
	const size_t length_in_bits = this->get_length_in_bits();
	if (this->num_of_used_ints == (size_t)1)
	{
		const few_bits only_num = (few_bits)this->get_least_significant();
#if IS_64_BIT_SYSTEM
		return (size_t)6507803568836410511 ^ ((size_t)only_num ^ length_in_bits);
#else
		return (size_t)568218086 ^ ((size_t)only_num ^ length_in_bits);
#endif
	}
#if IS_64_BIT_SYSTEM
	size_t result_hash = (size_t)9180187563408628080;
#else
	size_t result_hash = (size_t)412785383;
#endif
	custom_linked_list_node<int_array>* it = this->intarrays->first();
	int_array current_int_array = *it->value;
	size_t index_in_array = (size_t)0;
	bool xor_with_significant = true; //alternating
	const custom_linked_list_node<int_array>* const this_intarrays_end = this->intarrays->end();
	while (true)
	{
		if (index_in_array >= current_int_array.num_of_used_ints)
		{
			it = it->next;
			if (it == this_intarrays_end)
				break;
			current_int_array = *it->value;
			index_in_array = (size_t)0;
			continue;
		}
		const few_bits current_num = current_int_array.intarr[index_in_array];
		if (xor_with_significant)
		{
			xor_with_significant = false;
			result_hash ^= (size_t)current_num << half_sizeof_size_t;
		}
		else
		{
			xor_with_significant = true;
			result_hash ^= (size_t)current_num;
		}
		++index_in_array;
	}
	result_hash ^= length_in_bits;
	return result_hash;
}