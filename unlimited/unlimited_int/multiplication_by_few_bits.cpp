#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::multiply(const few_bits num_to_mult, unlimited_int* answer) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::multiply(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
	if ((this->num_of_intarrays_used == 0) || (num_to_mult == 0))
	{
		answer->set_to_zero();
		return;
	}
	many_bits max_num_of_ints_needed_for_answer = this->num_of_used_ints + 1;
	answer->intarrays.increase_until_num_of_ints(max_num_of_ints_needed_for_answer);
	const bool this_uses_only_one_array = this->num_of_intarrays_used == 1;
	const bool answer_needs_only_one_array = max_num_of_ints_needed_for_answer <= answer->intarrays.intarrays.first->value->intarr_len;
	if (this_uses_only_one_array || answer_needs_only_one_array)
	{
		if (this_uses_only_one_array && answer_needs_only_one_array)
			this->multiply_both_only_one_array(num_to_mult, answer);
		else if (this_uses_only_one_array)
			this->multiply_this_only_one_array(num_to_mult, answer);
		else
			this->multiply_answer_only_one_array(num_to_mult, answer);
		answer->is_negative = this->is_negative;
		return;
	}
	answer->is_negative = this->is_negative;
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	Node* it_this = this->intarrays.intarrays.first;
	Node* it_answer = answer->intarrays.intarrays.first;
	int_array* current_intarray_for_this = it_this->value;
	int_array* current_intarray_for_answer = it_answer->value;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits index_this = 0, index_answer = 0;
	many_bits num_of_intarrays_used_for_answer = 1;
	many_bits carry = 0;
	many_bits current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
	many_bits current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
	few_bits* current_intarray_for_this_intarr = current_intarray_for_this->intarr;
	few_bits* current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
	many_bits this_used_ints = this->num_of_used_ints;
	many_bits stop_at = this_used_ints;
	if (current_intarray_for_answer_intarr_len < stop_at)
	{
		stop_at = current_intarray_for_answer_intarr_len;
	}
	if (current_intarray_for_this_intarr_len < stop_at)
	{
		stop_at = current_intarray_for_this_intarr_len;
	}
	current_intarray_for_answer->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this += previous_num_int;
			index_answer += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_used_ints) { break; }
			if (index_this == current_intarray_for_this_intarr_len)
			{
				it_this = it_this->next;
				current_intarray_for_this = it_this->value;
				current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
				current_intarray_for_this_intarr = current_intarray_for_this->intarr;
				index_this = 0;
			}
			if (index_answer == current_intarray_for_answer_intarr_len)
			{
				++num_of_intarrays_used_for_answer;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = 0;
			}
			many_bits remaining_length_of_this_intarr = current_intarray_for_this_intarr_len - index_this;
			many_bits remaining_length_of_answer_intarr = current_intarray_for_answer_intarr_len - index_answer;
			if (remaining_length_of_this_intarr < remaining_length_of_answer_intarr)
			{
				stop_at += remaining_length_of_this_intarr;
			}
			else { stop_at += remaining_length_of_answer_intarr; }
			if (this_used_ints < stop_at) { stop_at = this_used_ints; }
			continue;
		}
		carry += ((many_bits)(*current_intarray_for_this_intarr)) * num_to_mult_many;
		*current_intarray_for_answer_intarr = (few_bits)carry;
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarray_for_this_intarr;
		++current_intarray_for_answer_intarr;
		++num_int;
	}
	if (index_answer == current_intarray_for_answer_intarr_len)
	{
		++num_of_intarrays_used_for_answer;
		it_answer = it_answer->next;
		index_answer = 0;
		current_intarray_for_answer = it_answer->value;
	}
	if (carry == 0)
	{
		answer->num_of_used_ints = this_used_ints;
		current_intarray_for_answer->num_of_used_ints = index_answer;
		if (index_answer == 0) { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer - 1; }
		else { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer; }
	}
	else
	{
		answer->num_of_used_ints = max_num_of_ints_needed_for_answer;
		current_intarray_for_answer->intarr[index_answer] = (few_bits)carry;
		current_intarray_for_answer->num_of_used_ints = index_answer + 1;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::multiply(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
}
void unlimited_int::multiply_both_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_both_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::multiply_both_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	int_array* this_int_array = this->intarrays.intarrays.first->value;
	few_bits* this_intarr = this_int_array->intarr;
	int_array* answer_int_array = answer->intarrays.intarrays.first->value;
	few_bits* answer_intarr = answer_int_array->intarr;
	few_bits* stop_ptr = &this_intarr[this_int_array->num_of_used_ints - 1];
	many_bits carry = 0;
	while (this_intarr != stop_ptr)
	{
		carry += ((many_bits)(*this_intarr)) * num_to_mult_many;
		*answer_intarr = (few_bits)carry;
		carry >>= NUM_OF_BITS_few_bits;
		++this_intarr;
		++answer_intarr;
	}
	carry += ((many_bits)(*this_intarr)) * num_to_mult_many;
	*answer_intarr = (few_bits)carry;
	carry >>= NUM_OF_BITS_few_bits;
	++this_intarr;
	++answer_intarr;
	many_bits unum_of_used_ints_answer;
	if (carry == 0) { unum_of_used_ints_answer = this->num_of_used_ints; }
	else
	{
		unum_of_used_ints_answer = this->num_of_used_ints + 1;
		*answer_intarr = (few_bits)carry;
	}
	answer_int_array->num_of_used_ints = unum_of_used_ints_answer;
	answer->num_of_used_ints = unum_of_used_ints_answer;
	answer->num_of_intarrays_used = 1;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_both_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::multiply_both_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
}
void unlimited_int::multiply_this_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_this_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::multiply_this_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	Node* it_answer = answer->intarrays.intarrays.first;
	int_array* current_intarray_for_answer = it_answer->value;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits index_answer = 0;
	many_bits num_of_intarrays_used_for_answer = 1;
	many_bits carry = 0;
	many_bits current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
	few_bits* current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
	many_bits this_used_ints = this->num_of_used_ints;
	few_bits* current_intarray_for_this_intarr = this->intarrays.intarrays.first->value->intarr;
	many_bits stop_at = this_used_ints;
	if (current_intarray_for_answer_intarr_len < stop_at)
	{
		stop_at = current_intarray_for_answer_intarr_len;
	}
	current_intarray_for_answer->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_answer += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_used_ints) { break; }
			if (index_answer == current_intarray_for_answer_intarr_len)
			{
				num_of_intarrays_used_for_answer++;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = 0;
			}
			stop_at += current_intarray_for_answer_intarr_len - index_answer;
			if (this_used_ints < stop_at)
			{
				stop_at = this_used_ints;
			}
			continue;
		}
		carry += ((many_bits)(*current_intarray_for_this_intarr)) * num_to_mult_many;
		*current_intarray_for_answer_intarr = (few_bits)carry;
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarray_for_this_intarr;
		++current_intarray_for_answer_intarr;
		++num_int;
	}
	if (index_answer == current_intarray_for_answer_intarr_len)
	{
		++num_of_intarrays_used_for_answer;
		it_answer = it_answer->next;
		index_answer = 0;
		current_intarray_for_answer = it_answer->value;
	}
	if (carry == 0)
	{
		answer->num_of_used_ints = this_used_ints;
		current_intarray_for_answer->num_of_used_ints = index_answer;
		if (index_answer == 0)
		{
			answer->num_of_intarrays_used = num_of_intarrays_used_for_answer - 1;
		}
		else { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer; }
	}
	else
	{
		answer->num_of_used_ints = this_used_ints + 1;
		current_intarray_for_answer->intarr[index_answer] = (few_bits)carry;
		current_intarray_for_answer->num_of_used_ints = index_answer + 1;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_this_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::multiply_this_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
}
void unlimited_int::multiply_answer_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_answer_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::multiply_answer_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	Node* it_this = this->intarrays.intarrays.first;
	int_array* current_intarray_for_this = it_this->value;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits index_this = 0;
	many_bits carry = 0;
	many_bits current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
	few_bits* current_intarray_for_this_intarr = current_intarray_for_this->intarr;
	many_bits this_used_ints = this->num_of_used_ints;
	int_array* answer_first_int_array = answer->intarrays.intarrays.first->value;
	few_bits* first_intarray_for_answer_intarr = answer_first_int_array->intarr;
	many_bits stop_at = this_used_ints;
	if (current_intarray_for_this_intarr_len < stop_at) { stop_at = current_intarray_for_this_intarr_len; }
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_used_ints) { break; }
			if (index_this == current_intarray_for_this_intarr_len)
			{
				it_this = it_this->next;
				current_intarray_for_this = it_this->value;
				current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
				current_intarray_for_this_intarr = current_intarray_for_this->intarr;
				index_this = 0;
			}
			stop_at += current_intarray_for_this_intarr_len - index_this;
			if (this_used_ints < stop_at) { stop_at = this_used_ints; }
			continue;
		}
		carry += ((many_bits)(*current_intarray_for_this_intarr)) * num_to_mult_many;
		*first_intarray_for_answer_intarr = (few_bits)carry;
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarray_for_this_intarr;
		++first_intarray_for_answer_intarr;
		++num_int;
	}
	if (carry == 0)
	{
		answer->num_of_used_ints = this_used_ints;
		answer_first_int_array->num_of_used_ints = this_used_ints;
	}
	else
	{
		answer->num_of_used_ints = this_used_ints + 1;
		*first_intarray_for_answer_intarr = (few_bits)carry;
		answer_first_int_array->num_of_used_ints = this_used_ints + 1;
	}
	answer->num_of_intarrays_used = 1;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_answer_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"void unlimited_int::multiply_answer_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const\"";
#endif
}
