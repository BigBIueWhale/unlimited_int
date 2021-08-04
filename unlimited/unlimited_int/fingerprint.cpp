#include "unlimited_int.hpp"
using namespace unlimited;

//quickest possible type of hash function. Non cryptographic, ignores sign.
size_t unlimited_int::fingerprint() const
{
	if (this->num_of_used_ints == 0)
	{
#if IS_64_BIT_SYSTEM
		return 6507803568836410511; //arbitrary number
#else
		return 568218086; //arbitrary number
#endif
	}
	constexpr int half_sizeof_size_t = (sizeof(size_t) * 8) / 2;
	const many_bits length_in_bits = this->get_length_in_bits();
	if (this->num_of_used_ints == 1)
	{
		const few_bits only_num = (few_bits)this->get_least_significant();
#if IS_64_BIT_SYSTEM
		return 6507803568836410511 ^ ((size_t)only_num ^ (size_t)length_in_bits);
#else
		return 568218086 ^ ((size_t)only_num ^ (size_t)length_in_bits);
#endif
	}
#if IS_64_BIT_SYSTEM
	size_t result_hash = 9180187563408628080;
#else
	size_t result_hash = 412785383;
#endif
	Node* it = this->intarrays.intarrays.first;
	int_array current_int_array = *it->value;
	many_bits index_in_array = 0;
	bool xor_with_significant = true; //alternating
	while (true)
	{
		if (index_in_array >= current_int_array.num_of_used_ints)
		{
			it = it->next;
			if (it == nullptr)
				break;
			current_int_array = *it->value;
			index_in_array = 0;
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
	result_hash ^= (size_t)length_in_bits;
	return result_hash;
}