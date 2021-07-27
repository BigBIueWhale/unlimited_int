#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::multiply_basecase(const unlimited_int* num_to_mult, unlimited_int* answer) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_basecase(unlimited_int* num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_mult->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"void unlimited_int::multiply_basecase(const unlimited_int* num_to_mult, unlimited_int* answer) const\"";
#endif
	if ((num_to_mult->num_of_intarrays_used == 0) || (this->num_of_intarrays_used == 0))
	{
		answer->set_to_zero();
		return;
	}
	const unlimited_int* this_cpy = this;
	const unlimited_int* num_to_mult_cpy = num_to_mult;
	bool this_is_answer = false;
	bool num_to_mult_is_answer = false;
	if (this == answer)
	{
		this_cpy = this->copy();
		this_is_answer = true;
	}
	if (answer == num_to_mult)
	{
		num_to_mult_cpy = num_to_mult->copy();
		num_to_mult_is_answer = true;
	}
	if (this_cpy->is_negative != num_to_mult_cpy->is_negative) { answer->is_negative = true; }
	const unlimited_int* smaller_num, * bigger_num;
	char value_of_comparison = this_cpy->estimate_compare_to_ignore_sign(*num_to_mult_cpy);
	if (value_of_comparison == 'L')
	{
		bigger_num = this_cpy;
		smaller_num = num_to_mult_cpy;
	}
	else
	{
		bigger_num = num_to_mult_cpy;
		smaller_num = this_cpy;
	}
	many_bits smaller_num_of_used_ints = smaller_num->num_of_used_ints;
	many_bits bigger_used_ints = bigger_num->num_of_used_ints;
	many_bits num_of_ints_needed_for_answer = bigger_used_ints + smaller_num_of_used_ints;
	answer->intarrays.increase_until_num_of_ints(num_of_ints_needed_for_answer);
	answer->fill_0_until_num_of_ints_and_set_used_ints_to_maximum(num_of_ints_needed_for_answer);
	Node* it_smaller = smaller_num->intarrays.intarrays.first;
	int_array* current_int_array_for_smaller = it_smaller->value;
	many_bits current_int_array_len_for_smaller = current_int_array_for_smaller->num_of_used_ints;
	few_bits* current_actual_int_array_in_smaller = current_int_array_for_smaller->intarr;
	Node* it_start_answer = answer->intarrays.intarrays.first;
	int_array* current_int_array_for_start_answer = it_start_answer->value;
	many_bits current_int_array_len_for_start_answer = current_int_array_for_start_answer->intarr_len;
	few_bits* current_actual_int_array_in_start_answer = current_int_array_for_start_answer->intarr;
	many_bits num_of_intarrays_used_start_answer = 1;
	many_bits num_int_outer_loop = 0, previous_num_int_outer_loop = 0;
	many_bits next_stop_outer_loop;
	if (current_int_array_len_for_start_answer < current_int_array_len_for_smaller)
	{
		next_stop_outer_loop = current_int_array_len_for_start_answer;
	}
	else { next_stop_outer_loop = current_int_array_len_for_smaller; }
	if (smaller_num_of_used_ints < next_stop_outer_loop) { next_stop_outer_loop = smaller_num_of_used_ints; }
	many_bits index_smaller = 0, index_start_answer = 0;
	many_bits num_of_used_ints_in_most_significant_int_array = 0;
	int_array* current_intarray_for_answer = current_int_array_for_start_answer;
	bool last_digit_is_zero = true;
	while (true)
	{
		if (num_int_outer_loop == next_stop_outer_loop)
		{
			previous_num_int_outer_loop = num_int_outer_loop - previous_num_int_outer_loop;
			index_smaller += previous_num_int_outer_loop;
			previous_num_int_outer_loop = num_int_outer_loop;
			if (num_int_outer_loop == smaller_num_of_used_ints) { break; }
			if (index_smaller == current_int_array_len_for_smaller)
			{
				index_smaller = 0;
				it_smaller = it_smaller->next;
				current_int_array_for_smaller = it_smaller->value;
				current_int_array_len_for_smaller = current_int_array_for_smaller->num_of_used_ints;
				current_actual_int_array_in_smaller = current_int_array_for_smaller->intarr;
			}
			if (index_start_answer == current_int_array_len_for_start_answer)
			{
				index_start_answer = 0;
				it_start_answer = it_start_answer->next;
				current_int_array_for_start_answer = it_start_answer->value;
				current_int_array_len_for_start_answer = current_int_array_for_start_answer->intarr_len;
				current_actual_int_array_in_start_answer = current_int_array_for_start_answer->intarr;
				num_of_intarrays_used_start_answer++;
			}
			many_bits remaining_length_of_current_intarr_smaller = current_int_array_len_for_smaller - index_smaller;
			many_bits remaining_length_of_current_intarr_start_answer = current_int_array_len_for_start_answer - index_start_answer;
			if (remaining_length_of_current_intarr_smaller < remaining_length_of_current_intarr_start_answer)
			{
				next_stop_outer_loop += remaining_length_of_current_intarr_smaller;
			}
			else { next_stop_outer_loop += remaining_length_of_current_intarr_start_answer; }
			if (smaller_num_of_used_ints < next_stop_outer_loop) { next_stop_outer_loop = smaller_num_of_used_ints; }
			continue;
		}
		many_bits num_to_mult_many = *current_actual_int_array_in_smaller;
		if (num_to_mult_many != 0)
		{
			Node* it_bigger = bigger_num->intarrays.intarrays.first;
			Node* it_answer = it_start_answer;
			int_array* current_intarray_for_bigger = it_bigger->value;
			current_intarray_for_answer = current_int_array_for_start_answer;
			many_bits num_int = 0, previous_num_int = 0;
			many_bits index_bigger = 0, index_answer = index_start_answer;
			many_bits carry = 0;
			many_bits current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
			many_bits current_intarray_for_bigger_intarr_len = current_intarray_for_bigger->num_of_used_ints;
			few_bits* current_intarray_for_bigger_intarr = current_intarray_for_bigger->intarr;
			few_bits* current_intarray_for_answer_intarr = current_actual_int_array_in_start_answer;
			many_bits num_of_intarrays_used_for_answer = num_of_intarrays_used_start_answer;
			many_bits remaining_length_of_answer_intarr = current_intarray_for_answer_intarr_len - index_answer;
			many_bits stop_at;
			if (remaining_length_of_answer_intarr < current_intarray_for_bigger_intarr_len)
			{
				stop_at = remaining_length_of_answer_intarr;
			}
			else { stop_at = current_intarray_for_bigger_intarr_len; }
			if (bigger_used_ints < stop_at) { stop_at = bigger_used_ints; }
			while (true)
			{
				if (num_int == stop_at)
				{
					previous_num_int = num_int - previous_num_int;
					index_bigger += previous_num_int;
					index_answer += previous_num_int;
					previous_num_int = num_int;
					if (num_int == bigger_used_ints) { break; }
					if (index_bigger == current_intarray_for_bigger_intarr_len)
					{
						it_bigger = it_bigger->next;
						current_intarray_for_bigger = it_bigger->value;
						current_intarray_for_bigger_intarr_len = current_intarray_for_bigger->num_of_used_ints;
						current_intarray_for_bigger_intarr = current_intarray_for_bigger->intarr;
						index_bigger = 0;
					}
					if (index_answer == current_intarray_for_answer_intarr_len)
					{
						++num_of_intarrays_used_for_answer;
						it_answer = it_answer->next;
						current_intarray_for_answer = it_answer->value;
						current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
						current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
						index_answer = 0;
					}
					many_bits remaining_length_of_bigger_intarr = current_intarray_for_bigger_intarr_len - index_bigger;
					remaining_length_of_answer_intarr = current_intarray_for_answer_intarr_len - index_answer;
					if (remaining_length_of_bigger_intarr < remaining_length_of_answer_intarr)
					{
						stop_at += remaining_length_of_bigger_intarr;
					}
					else { stop_at += remaining_length_of_answer_intarr; }
					if (bigger_used_ints < stop_at) { stop_at = bigger_used_ints; }
					continue;
				}
				carry += ((many_bits)(*current_intarray_for_bigger_intarr)) * num_to_mult_many + ((many_bits)(*current_intarray_for_answer_intarr));
				*current_intarray_for_answer_intarr = (few_bits)carry;
				carry >>= NUM_OF_BITS_few_bits;
				++current_intarray_for_bigger_intarr;
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
				last_digit_is_zero = true;
				if (index_answer == 0) { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer - 1; }
				else { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer; }
				num_of_used_ints_in_most_significant_int_array = index_answer;
			}
			else
			{
				last_digit_is_zero = false;
				current_intarray_for_answer->intarr[index_answer] = (few_bits)carry;
				answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
				num_of_used_ints_in_most_significant_int_array = index_answer + 1;
			}
		}
		++num_int_outer_loop;
		++current_actual_int_array_in_smaller;
		++current_actual_int_array_in_start_answer;
		++index_start_answer;
	}
	if (last_digit_is_zero) { answer->num_of_used_ints = num_of_ints_needed_for_answer - 1; }
	else { answer->num_of_used_ints = num_of_ints_needed_for_answer; }
	current_intarray_for_answer->num_of_used_ints = num_of_used_ints_in_most_significant_int_array;
	if (this_is_answer)
		delete this_cpy;
	if (num_to_mult_is_answer)
		delete num_to_mult_cpy;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_basecase(unlimited_int* num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw "\nThe inconsistencies were found at the end of multiply_basecase with answer.";
#endif
}
