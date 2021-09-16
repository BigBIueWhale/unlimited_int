#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const
{
#if DEBUG_MODE == 2
	std::cout << "\nStart of subtract:";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_subtract->find_inconsistencies())
		throw std::logic_error("The error was found in start of function \"void unlimited_int::subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const\"");
#endif
	if (num_to_subtract == this)
	{
		answer->set_to_zero();
		return;
	}
	if (this->num_of_intarrays_used == (size_t)0)
	{
		if (num_to_subtract != answer)
		{
			if (num_to_subtract->num_of_used_ints == (size_t)0)
				answer->set_to_zero();
			else
			{
				num_to_subtract->copy_to(*answer);
				answer->_is_negative = !answer->_is_negative;
			}
			return;
		}
	}
	if (num_to_subtract->num_of_intarrays_used == (size_t)0)
	{
		if (this != answer)
			this->copy_to(*answer);
		return;
	}
	const bool num_to_subtract_is_negative = num_to_subtract->_is_negative;
	const bool this_is_negative = this->_is_negative;
	unlimited_int this_copy(*this, false);
	unlimited_int num_to_subtract_copy(*num_to_subtract, false);
	this_copy._is_negative = false;
	num_to_subtract_copy._is_negative = false;
	int num_of_negatives = 0;
	if (this_is_negative)
		++num_of_negatives;
	if (num_to_subtract_is_negative)
		++num_of_negatives;
	if (num_of_negatives == 2)
		this_copy.swap(num_to_subtract_copy);
	else if (num_of_negatives == 1)
	{
		if (this_is_negative)
		{
			this_copy.add(&num_to_subtract_copy, answer);
			answer->_is_negative = true;
		}
		else
			this_copy.add(&num_to_subtract_copy, answer);
#if DEBUG_MODE == 2
		std::cout << "\nEnd of subtract:";
#endif
#if DEBUG_MODE > 0
		if (answer->find_inconsistencies())
			throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const\"");
#endif
		return;
	}
	char compare_result = this_copy.compare_to_ignore_sign(num_to_subtract_copy);
	if (compare_result == 'E')
	{
		answer->set_to_zero();
#if DEBUG_MODE == 2
		std::cout << "\nEnd of subtract:";
#endif
#if DEBUG_MODE > 0
		if (answer->find_inconsistencies())
		{
			if (answer->find_inconsistencies())
				throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const\"");
		}
#endif
		return;
	}
	bool set_answer_to_negative;
	if (compare_result == 'L')
		set_answer_to_negative = false;
	else //if compare_result == 'S'
	{
		this_copy.swap(num_to_subtract_copy);
		set_answer_to_negative = true;
	}
	if (this_copy.fits_in_many_bits() && num_to_subtract_copy.fits_in_many_bits())
	{
		*answer = this_copy.to_many_bits() - num_to_subtract_copy.to_many_bits();
		answer->_is_negative = set_answer_to_negative;
		return;
	}
	answer->_is_negative = set_answer_to_negative;
	size_t num_of_used_ints_this = this_copy.num_of_used_ints;
	answer->increase_until_num_of_ints(num_of_used_ints_this);
	custom_linked_list_node<int_array>* it_this = this_copy.intarrays->first();
	custom_linked_list_node<int_array>* it_subtract = num_to_subtract_copy.intarrays->first();
	custom_linked_list_node<int_array>* it_answer = answer->intarrays->first();
	int_array* this_current_int_array = it_this->value;
	int_array* subtract_current_int_array = it_subtract->value;
	int_array* answer_current_int_array = it_answer->value;
	size_t num_of_int_arrays_used_in_answer = (size_t)1;
	size_t index_this = (size_t)0, index_subtract = (size_t)0, index_answer = (size_t)0;
	size_t this_current_int_array_num_of_used_ints = this_current_int_array->num_of_used_ints;
	size_t subtract_current_int_array_num_of_used_ints = subtract_current_int_array->num_of_used_ints;
	size_t answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
	few_bits* this_current_int_array_intarr = this_current_int_array->intarr;
	few_bits* subtract_current_int_array_intarr = subtract_current_int_array->intarr;
	few_bits* answer_current_int_array_intarr = answer_current_int_array->intarr;
	size_t int_num = (size_t)0, previous_int_num = (size_t)0;
	size_t num_of_used_ints_subtract = num_to_subtract_copy.num_of_used_ints;
	size_t next_stop = num_of_used_ints_subtract;
	if (subtract_current_int_array_num_of_used_ints < next_stop)
		next_stop = subtract_current_int_array_num_of_used_ints;
	if (this_current_int_array_num_of_used_ints < next_stop)
		next_stop = this_current_int_array_num_of_used_ints;
	if (answer_current_int_array_intarr_len < next_stop)
		next_stop = answer_current_int_array_intarr_len;
	bool is_carry = false;
	answer_current_int_array->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (int_num >= next_stop)
		{
			const size_t difference = int_num - previous_int_num;
			index_answer += difference;
			index_subtract += difference;
			index_this += difference;
			previous_int_num = int_num;
			if (int_num >= num_of_used_ints_subtract)
				break;
			if (index_this >= this_current_int_array_num_of_used_ints)
			{
				it_this = it_this->next;
				index_this = (size_t)0;
				this_current_int_array = it_this->value;
				this_current_int_array_num_of_used_ints = this_current_int_array->num_of_used_ints;
				this_current_int_array_intarr = this_current_int_array->intarr;
			}
			if (index_subtract >= subtract_current_int_array_num_of_used_ints)
			{
				it_subtract = it_subtract->next;
				index_subtract = (size_t)0;
				subtract_current_int_array = it_subtract->value;
				subtract_current_int_array_num_of_used_ints = subtract_current_int_array->num_of_used_ints;
				subtract_current_int_array_intarr = subtract_current_int_array->intarr;
			}
			if (index_answer >= answer_current_int_array_intarr_len)
			{
				++num_of_int_arrays_used_in_answer;
				it_answer = it_answer->next;
				index_answer = (size_t)0;
				answer_current_int_array = it_answer->value;
				answer_current_int_array->set_num_of_used_ints_to_maximum();
				answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
				answer_current_int_array_intarr = answer_current_int_array->intarr;
			}
			const size_t remaining_intarr_len_subtract = subtract_current_int_array_num_of_used_ints - index_subtract;
			const size_t remaining_intarr_len_this = this_current_int_array_num_of_used_ints - index_this;
			const size_t remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
			const bool subtract_smaller_than_this = remaining_intarr_len_subtract < remaining_intarr_len_this;
			const bool answer_smaller_than_this = remaining_intarr_len_answer < remaining_intarr_len_this;
			const bool answer_smaller_than_subtract = remaining_intarr_len_answer < remaining_intarr_len_subtract;
			if (subtract_smaller_than_this && !answer_smaller_than_subtract)
				next_stop += remaining_intarr_len_subtract;
			else if (answer_smaller_than_this && answer_smaller_than_subtract)
				next_stop += remaining_intarr_len_answer;
			else
				next_stop += remaining_intarr_len_this;
			continue;
		}
		many_bits_signed current_value_in_this;
		const many_bits_signed current_value_in_subtract = *subtract_current_int_array_intarr;
		if (is_carry)
			current_value_in_this = static_cast<many_bits_signed>(*this_current_int_array_intarr) - static_cast<many_bits_signed>(1);
		else
			current_value_in_this = static_cast<many_bits_signed>(*this_current_int_array_intarr);
		if (current_value_in_this < current_value_in_subtract)
		{
			*answer_current_int_array_intarr = static_cast<few_bits>(current_value_in_this + (many_bits_signed)MAX_few_bits_NUM_PLUS_ONE - current_value_in_subtract);
			is_carry = true;
		}
		else
		{
			*answer_current_int_array_intarr = static_cast<few_bits>(current_value_in_this - current_value_in_subtract);
			is_carry = false;
		}
		++answer_current_int_array_intarr;
		++this_current_int_array_intarr;
		++subtract_current_int_array_intarr;
		++int_num;
	}
	if (int_num < num_of_used_ints_this)
	{
		if (index_this >= this_current_int_array_num_of_used_ints)
		{
			it_this = it_this->next;
			index_this = (size_t)0;
			this_current_int_array = it_this->value;
			this_current_int_array_num_of_used_ints = this_current_int_array->num_of_used_ints;
			this_current_int_array_intarr = this_current_int_array->intarr;
		}
		if (index_answer >= answer_current_int_array_intarr_len)
		{
			++num_of_int_arrays_used_in_answer;
			it_answer = it_answer->next;
			index_answer = (size_t)0;
			answer_current_int_array = it_answer->value;
			answer_current_int_array->set_num_of_used_ints_to_maximum();
			answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
			answer_current_int_array_intarr = answer_current_int_array->intarr;
		}
		const size_t remaining_intarr_len_this = this_current_int_array_num_of_used_ints - index_this;
		const size_t remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
		if (remaining_intarr_len_this < remaining_intarr_len_answer)
			next_stop = remaining_intarr_len_this + int_num;
		else
			next_stop = remaining_intarr_len_answer + int_num;
		if (is_carry)
		{
			while (true)
			{
				if (int_num >= next_stop)
				{
					if (int_num >= num_of_used_ints_this)
						break;
					const size_t difference = int_num - previous_int_num;
					index_answer += difference;
					index_this += difference;
					previous_int_num = int_num;
					if (index_this >= this_current_int_array_num_of_used_ints)
					{
						it_this = it_this->next;
						index_this = (size_t)0;
						this_current_int_array = it_this->value;
						this_current_int_array_num_of_used_ints = this_current_int_array->num_of_used_ints;
						this_current_int_array_intarr = this_current_int_array->intarr;
					}
					if (index_answer >= answer_current_int_array_intarr_len)
					{
						++num_of_int_arrays_used_in_answer;
						it_answer = it_answer->next;
						index_answer = (size_t)0;
						answer_current_int_array = it_answer->value;
						answer_current_int_array->set_num_of_used_ints_to_maximum();
						answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
						answer_current_int_array_intarr = answer_current_int_array->intarr;
					}
					const size_t remaining_intarr_len_this = this_current_int_array_num_of_used_ints - index_this;
					const size_t remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
					if (remaining_intarr_len_answer < remaining_intarr_len_this)
						next_stop += remaining_intarr_len_answer;
					else
						next_stop += remaining_intarr_len_this;
					continue;
				}
				if (!is_carry)
					break;
				few_bits current_value_in_this;
				current_value_in_this = *this_current_int_array_intarr;
				if (current_value_in_this == (few_bits)0)
				{
					*answer_current_int_array_intarr = (few_bits)MAX_few_bits_NUM;
					is_carry = true;
				}
				else
				{
					*answer_current_int_array_intarr = current_value_in_this - (few_bits)1;
					is_carry = false;
				}
				++this_current_int_array_intarr;
				++answer_current_int_array_intarr;
				++int_num;
			}
			const size_t difference = int_num - previous_int_num;
			index_answer += difference;
			index_this += difference;
			previous_int_num = int_num;
		}
		if (int_num < num_of_used_ints_this)
		{
			if (index_this >= this_current_int_array_num_of_used_ints)
			{
				it_this = it_this->next;
				index_this = (size_t)0;
				this_current_int_array = it_this->value;
				this_current_int_array_num_of_used_ints = this_current_int_array->num_of_used_ints;
				this_current_int_array_intarr = this_current_int_array->intarr;
			}
			if (index_answer >= answer_current_int_array_intarr_len)
			{
				++num_of_int_arrays_used_in_answer;
				it_answer = it_answer->next;
				index_answer = (size_t)0;
				answer_current_int_array = it_answer->value;
				answer_current_int_array->set_num_of_used_ints_to_maximum();
				answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
				answer_current_int_array_intarr = answer_current_int_array->intarr;
			}
			const size_t remaining_intarr_len_this = this_current_int_array_num_of_used_ints - index_this;
			const size_t remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
			if (remaining_intarr_len_this < remaining_intarr_len_answer)
				next_stop = remaining_intarr_len_this + int_num;
			else
				next_stop = remaining_intarr_len_answer + int_num;
			while (true)
			{
				if (int_num >= next_stop)
				{
					const size_t difference = int_num - previous_int_num;
					index_answer += difference;
					index_this += difference;
					previous_int_num = int_num;
					if (int_num >= num_of_used_ints_this)
						break;
					if (index_this >= this_current_int_array_num_of_used_ints)
					{
						it_this = it_this->next;
						index_this = (size_t)0;
						this_current_int_array = it_this->value;
						this_current_int_array_num_of_used_ints = this_current_int_array->num_of_used_ints;
						this_current_int_array_intarr = this_current_int_array->intarr;
					}
					if (index_answer >= answer_current_int_array_intarr_len)
					{
						++num_of_int_arrays_used_in_answer;
						it_answer = it_answer->next;
						index_answer = (size_t)0;
						answer_current_int_array = it_answer->value;
						answer_current_int_array->set_num_of_used_ints_to_maximum();
						answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
						answer_current_int_array_intarr = answer_current_int_array->intarr;
					}
					const size_t remaining_intarr_len_this = this_current_int_array_num_of_used_ints - index_this;
					const size_t remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
					if (remaining_intarr_len_answer < remaining_intarr_len_this)
						next_stop += remaining_intarr_len_answer;
					else
						next_stop += remaining_intarr_len_this;
					continue;
				}
				*answer_current_int_array_intarr = *this_current_int_array_intarr;
				++this_current_int_array_intarr;
				++answer_current_int_array_intarr;
				++int_num;
			}
		}
	}
	answer->num_of_used_ints = int_num;
	answer->num_of_intarrays_used = num_of_int_arrays_used_in_answer;
	answer_current_int_array->num_of_used_ints = index_answer;
	answer->cutoff_leading_zeros(it_answer);
#if DEBUG_MODE == 2
	std::cout << "\nEnd of subtract:";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const\"");
#endif
}
