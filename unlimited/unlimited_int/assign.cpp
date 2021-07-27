#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::assign(uint32_t* arr, many_bits len)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory();  this->auto_destroy = true; }
	if (len == 0) { return; }
#if NUM_OF_BITS_few_bits == 16
	const many_bits num_of_ints = len * 2;
#elif NUM_OF_BITS_few_bits == 32
	const many_bits num_of_ints = len;
#endif
	this->intarrays.increase_until_num_of_ints(num_of_ints);
	Node* current_int_array_Node = this->intarrays.intarrays.first;
	int_array current_int_array = *current_int_array_Node->value;
	current_int_array_Node->value->set_num_of_used_ints_to_maximum();
	many_bits index_in_current_int_array = 0;
	many_bits int_array_counter = 0;
#if NUM_OF_BITS_few_bits == 16
	bool using_significant_part = false;
	uint32_t previous_num = 0U;
#endif
	for (many_bits_signed counter_ints = len - 1; counter_ints >= 0; )
	{
#if NUM_OF_BITS_few_bits == 16
		if (using_significant_part)
		{
			current_int_array.intarr[index_in_current_int_array] = (few_bits)(previous_num >> 16);
			using_significant_part = false;
			--counter_ints;
		}
		else
		{
			previous_num = arr[counter_ints];
			current_int_array.intarr[index_in_current_int_array] = (few_bits)(previous_num & MASK_LOW_BITS);
			using_significant_part = true;
		}
#else
		current_int_array.intarr[index_in_current_int_array] = arr[counter_ints];
		--counter_ints;
#endif
		++index_in_current_int_array;
		if (index_in_current_int_array >= current_int_array.intarr_len)
		{
			if (counter_ints >= 0)
			{
				++int_array_counter;
				current_int_array_Node->value->set_num_of_used_ints_to_maximum();
				index_in_current_int_array = 0;
				current_int_array_Node = current_int_array_Node->next;
				current_int_array = *current_int_array_Node->value;
			}
		}
	}
	current_int_array_Node->value->num_of_used_ints = index_in_current_int_array;
	this->num_of_used_ints = num_of_ints;
	this->num_of_intarrays_used = int_array_counter + 1;
	this->cutoff_leading_zeros(current_int_array_Node);
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::assign(uint32_t* arr, many_bits len)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function: \"void unlimited_int::assign(uint32_t* arr, many_bits len)\"";
#endif
}
void unlimited_int::assign(const few_bits value_to_assign)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory();  this->auto_destroy = true; }
	if (value_to_assign == 0) { return; }
	else
	{
		this->intarrays.increase_until_num_of_ints(1);
		this->num_of_intarrays_used = 1;
		this->is_negative = false;
		this->num_of_used_ints = 1;
		this->intarrays.intarrays.first->value->assign(value_to_assign);
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"assign(few_ints value_to_assign)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::assign(const few_bits value_to_assign)\"";
#endif
}
void unlimited_int::assign(const many_bits value_to_assign)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory();  this->auto_destroy = true; }
	if (value_to_assign == 0) { return; }
	else
	{
		this->is_negative = false;
		few_bits remainder = (few_bits)value_to_assign; //value_to_assign & MASK_LOW_BITS
		few_bits carry = value_to_assign >> NUM_OF_BITS_few_bits;
		if (carry != 0)
		{
			this->num_of_used_ints = 2;
			this->intarrays.increase_until_num_of_ints(2);
			Node* it = this->intarrays.intarrays.first;
			int_array* int_array_to_change = it->value;
			int_array_to_change->assign(remainder);
			if (int_array_to_change->intarr_len == 1)
			{
				this->num_of_intarrays_used = 2;
				it = it->next;
				it->value->assign(carry);
			}
			else
			{
				this->num_of_intarrays_used = 1;
				int_array_to_change->intarr[1] = carry;
				int_array_to_change->num_of_used_ints = 2;
			}
		}
		else
		{
			this->num_of_used_ints = 1;
			this->num_of_intarrays_used = 1;
			this->intarrays.increase_until_num_of_ints(1);
			this->intarrays.intarrays.first->value->assign(remainder);
		}
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"assign(many_bits value_to_assign)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::assign(const many_bits value_to_assign)\"";
#endif
}
void unlimited_int::assign(uint64_t* arr, many_bits len)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory();  this->auto_destroy = true; }
	if (len == 0) { return; }
#if NUM_OF_BITS_few_bits == 16
	const many_bits num_of_ints = len * 4;
#elif NUM_OF_BITS_few_bits == 32
	const many_bits num_of_ints = len * 2;
#endif
	this->intarrays.increase_until_num_of_ints(num_of_ints);
	Node* current_int_array_Node = this->intarrays.intarrays.first;
	int_array current_int_array = *current_int_array_Node->value;
	current_int_array_Node->value->set_num_of_used_ints_to_maximum();
	many_bits index_in_current_int_array = 0;
	many_bits int_array_counter = 0;
#if NUM_OF_BITS_few_bits == 32
	bool using_significant_part = false;
	uint64_t previous_num = 0U;
#elif NUM_OF_BITS_few_bits == 16
	uint64_t previous_num = 0;
	int num_of_bits_used_from_previous_num = 0;
#endif
	for (many_bits_signed counter_ints = len - 1; counter_ints >= 0; )
	{
#if NUM_OF_BITS_few_bits == 16
		if (num_of_bits_used_from_previous_num == 0)
			previous_num = arr[counter_ints];
		current_int_array.intarr[index_in_current_int_array] = (few_bits)((previous_num >> num_of_bits_used_from_previous_num) & (uint64_t)MASK_LOW_BITS);
		num_of_bits_used_from_previous_num += 16;
		if (num_of_bits_used_from_previous_num == 64)
		{
			--counter_ints;
			num_of_bits_used_from_previous_num = 0;
		}
#elif NUM_OF_BITS_few_bits == 32
		if (using_significant_part)
		{
			current_int_array.intarr[index_in_current_int_array] = (few_bits)(previous_num >> 32);
			using_significant_part = false;
			--counter_ints;
		}
		else
		{
			previous_num = arr[counter_ints];
			current_int_array.intarr[index_in_current_int_array] = (few_bits)(previous_num & (uint64_t)MASK_LOW_BITS);
			using_significant_part = true;
		}
#endif
		++index_in_current_int_array;
		if (index_in_current_int_array >= current_int_array.intarr_len)
		{
			if (counter_ints >= 0)
			{
				++int_array_counter;
				current_int_array_Node->value->set_num_of_used_ints_to_maximum();
				index_in_current_int_array = 0;
				current_int_array_Node = current_int_array_Node->next;
				current_int_array = *current_int_array_Node->value;
			}
		}
	}
	current_int_array_Node->value->num_of_used_ints = index_in_current_int_array;
	this->num_of_used_ints = num_of_ints;
	this->num_of_intarrays_used = int_array_counter + 1;
	this->cutoff_leading_zeros(current_int_array_Node);
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::assign(uint32_t* arr, many_bits len)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function: \"void unlimited_int::assign(uint32_t* arr, many_bits len)\"";
#endif
}
