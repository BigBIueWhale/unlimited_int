#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::assign(const uint32_t *const arr, const size_t len)
{
	if (this->auto_destroy)
		this->flush();
	else
	{
		this->forget_memory();
		this->auto_destroy = true;
	}
	if (len == (size_t)0)
		return;
	static_assert(NUM_OF_BITS_few_bits == 32, "Assertion error. Wrong assumption that UNLIMITED_INT_NUM_OF_BITS_few_bits is 32 bits.");
	const size_t num_of_ints = len;
	this->increase_until_num_of_ints(num_of_ints);
	custom_linked_list_node<int_array>* current_int_array_Node = this->intarrays->first();
	int_array current_int_array = *current_int_array_Node->value;
	current_int_array_Node->value->set_num_of_used_ints_to_maximum();
	size_t index_in_current_int_array = (size_t)0;
	size_t int_array_counter = (size_t)0;
	size_t counter_ints = len - (size_t)1;
	bool stop_now = false;
	while (true)
	{
		current_int_array.intarr[index_in_current_int_array++] = (few_bits)arr[counter_ints];
		if (counter_ints-- == (size_t)0)
			stop_now = true;
		if (stop_now)
			break;
		if (index_in_current_int_array >= current_int_array.intarr_len)
		{
			++int_array_counter;
			current_int_array_Node->value->set_num_of_used_ints_to_maximum();
			index_in_current_int_array = (size_t)0;
			current_int_array_Node = current_int_array_Node->next;
			current_int_array = *current_int_array_Node->value;
		}
	}
	current_int_array_Node->value->num_of_used_ints = index_in_current_int_array;
	this->num_of_used_ints = num_of_ints;
	this->num_of_intarrays_used = int_array_counter + (size_t)1;
	this->cutoff_leading_zeros(current_int_array_Node);
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::assign(uint32_t* arr, size_t len)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"void unlimited_int::assign(uint32_t* arr, size_t len)\"");
#endif
}
void unlimited_int::assign(const uint64_t *const arr, const size_t len)
{
	if (this->auto_destroy)
		this->flush();
	else
	{
		this->forget_memory();
		this->auto_destroy = true;
	}
	if (len == (size_t)0)
		return;
	static_assert(NUM_OF_BITS_few_bits == 32, "Assertion error. Wrong assumption that UNLIMITED_INT_NUM_OF_BITS_few_bits is 32 bits.");
	const size_t num_of_ints = len * (size_t)2;
	this->increase_until_num_of_ints(num_of_ints);
	custom_linked_list_node<int_array>* current_int_array_Node = this->intarrays->first();
	int_array current_int_array = *current_int_array_Node->value;
	current_int_array_Node->value->set_num_of_used_ints_to_maximum();
	size_t index_in_current_int_array = (size_t)0;
	size_t int_array_counter = (size_t)0;
	bool using_significant_part = false;
	uint64_t previous_num = 0U;
	size_t counter_ints = len - (size_t)1;
	bool stop_now = false;
	while (true)
	{
		if (using_significant_part)
		{
			current_int_array.intarr[index_in_current_int_array++] = (few_bits)(previous_num >> 32);
			using_significant_part = false;
			if (counter_ints-- == (size_t)0)
				stop_now = true;
		}
		else
		{
			previous_num = arr[counter_ints];
			current_int_array.intarr[index_in_current_int_array++] = (few_bits)(previous_num & (uint64_t)MASK_LOW_BITS);
			using_significant_part = true;
		}
		if (stop_now)
			break;
		if (index_in_current_int_array >= current_int_array.intarr_len)
		{
			++int_array_counter;
			current_int_array_Node->value->set_num_of_used_ints_to_maximum();
			index_in_current_int_array = (size_t)0;
			current_int_array_Node = current_int_array_Node->next;
			current_int_array = *current_int_array_Node->value;
		}
	}
	current_int_array_Node->value->num_of_used_ints = index_in_current_int_array;
	this->num_of_used_ints = num_of_ints;
	this->num_of_intarrays_used = int_array_counter + (size_t)1;
	this->cutoff_leading_zeros(current_int_array_Node);
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::assign(uint32_t* arr, many_bits len)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"void unlimited_int::assign(uint32_t* arr, many_bits len)\"");
#endif
}
void unlimited_int::assign(const few_bits value_to_assign)
{
	if (this->auto_destroy)
		this->flush();
	else
	{
		this->forget_memory();
		this->auto_destroy = true;
	}
	if (value_to_assign == (few_bits)0)
		return;
	else
	{
		this->increase_until_num_of_ints((size_t)1);
		this->num_of_intarrays_used = (size_t)1;
		this->_is_negative = false;
		this->num_of_used_ints = (size_t)1;
		this->intarrays->first()->value->assign(value_to_assign);
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"assign(few_ints value_to_assign)\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::assign(const few_bits value_to_assign)\"");
#endif
}
void unlimited_int::assign(const many_bits value_to_assign)
{
	if (this->auto_destroy)
		this->flush();
	else
	{
		this->forget_memory();
		this->auto_destroy = true;
	}
	if (value_to_assign == (many_bits)0)
		return;
	else
	{
		this->_is_negative = false;
		const few_bits remainder = (few_bits)value_to_assign;
		const few_bits carry = (few_bits)(value_to_assign >> NUM_OF_BITS_few_bits);
		if (carry != (few_bits)0)
		{
			this->num_of_used_ints = (size_t)2;
			this->increase_until_num_of_ints((size_t)2);
			custom_linked_list_node<int_array>* it = this->intarrays->first();
			int_array* int_array_to_change = it->value;
			int_array_to_change->assign(remainder);
			if (int_array_to_change->intarr_len == (size_t)1)
			{
				this->num_of_intarrays_used = (size_t)2;
				it = it->next;
				it->value->assign(carry);
			}
			else
			{
				this->num_of_intarrays_used = (size_t)1;
				int_array_to_change->intarr[(size_t)1] = carry;
				int_array_to_change->num_of_used_ints = (size_t)2;
			}
		}
		else
		{
			this->num_of_used_ints = (size_t)1;
			this->num_of_intarrays_used = (size_t)1;
			this->increase_until_num_of_ints((size_t)1);
			this->intarrays->first()->value->assign(remainder);
		}
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"assign(many_bits value_to_assign)\":";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::assign(const many_bits value_to_assign)\"");
#endif
}
void unlimited_int::assign(const few_bits_signed value_to_assign)
{
	few_bits few_bits_equivalent;
	if (value_to_assign < (few_bits_signed)0)
		//It's required to do it this way, in case value_to_assign is the most negative number that few_bits_signed can be.
		//In that case the positive version wouldn't fit in few_bits_signed, so that's why we do the add 1 and only then use unary operator -
		few_bits_equivalent = static_cast<few_bits>(-(value_to_assign + (few_bits_signed)1)) + (few_bits)1;
	else
		few_bits_equivalent = static_cast<few_bits>(value_to_assign);
	this->assign(few_bits_equivalent);
	this->_is_negative = value_to_assign < (few_bits_signed)0;
}
void unlimited_int::assign(const many_bits_signed value_to_assign)
{
	many_bits many_bits_equivalent;
	if (value_to_assign < (many_bits_signed)0)
		//It's required to do it this way, in case value_to_assign is the most negative number that many_bits_signed can be.
		//In that case the positive version wouldn't fit in many_bits_signed, so that's why we do the add 1 and only then use unary operator -
		many_bits_equivalent = static_cast<many_bits>(-(value_to_assign + (many_bits_signed)1)) + (many_bits)1;
	else
		many_bits_equivalent = static_cast<many_bits>(value_to_assign);
	this->assign(many_bits_equivalent);
	this->_is_negative = value_to_assign < (many_bits_signed)0;
}
