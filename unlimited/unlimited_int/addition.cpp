#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::add(const unlimited_int* num_to_add, unlimited_int* answer) const
{
	static_assert(sizeof(few_bits) * 2 == sizeof(many_bits), "Assertion error: NUM_OF_BITS_many_bits must have exactly twice the number of bits as NUM_OF_BITS_few_bits");
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nStart of add:";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_add->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"void unlimited_int::add(const unlimited_int* num_to_add, unlimited_int* answer) const\"");
#endif
	if (num_to_add == this)
	{
		*answer = (*this) << 1;
		return;
	}
	if (this->num_of_intarrays_used == (size_t)0)
	{
		if (num_to_add != answer)
		{
			num_to_add->copy_to(*answer);
		}
		return;
	}
	if (num_to_add->num_of_intarrays_used == (size_t)0)
	{
		if (this != answer)
		{
			this->copy_to(*answer);
		}
		return;
	}
	int num_of_negatives = 0;
	bool this_is_negative = this->_is_negative, add_is_negative = num_to_add->_is_negative, set_answer_to_negative;
	if (this_is_negative)
		++num_of_negatives;
	if (add_is_negative)
		++num_of_negatives;
	unlimited_int this_copy(*this, false);
	unlimited_int num_to_add_copy(*num_to_add, false);
	this_copy._is_negative = false;
	this_copy.auto_destroy = false;
	num_to_add_copy._is_negative = false;
	num_to_add_copy.auto_destroy = false;
	if (num_of_negatives == 2)
		set_answer_to_negative = true;
	else if (num_of_negatives == 1)
	{
		if (this_is_negative)
		{
			num_to_add_copy.subtract(&this_copy, answer);
		}
		else
		{
			this_copy.subtract(&num_to_add_copy, answer);
		}
		return;
	}
	else
		set_answer_to_negative = false;
	unlimited_int* smaller_num, * larger_num;
	char compare_result = this_copy.estimate_compare_to_ignore_sign(num_to_add_copy);
	if (compare_result == 'L')
	{
		larger_num = &this_copy;
		smaller_num = &num_to_add_copy;
	}
	else
	{
		smaller_num = &this_copy;
		larger_num = &num_to_add_copy;
	}
	if (smaller_num->fits_in_few_bits() && larger_num->fits_in_few_bits())
	{
		*answer = static_cast<many_bits>(smaller_num->to_few_bits()) + static_cast<many_bits>(larger_num->to_few_bits());
		if (set_answer_to_negative)
			answer->self_negative();
		return;
	}
	const size_t bigger_num_used_ints = larger_num->num_of_used_ints;
	const size_t max_num_of_ints_needed_for_answer = bigger_num_used_ints + (size_t)1;
	answer->increase_until_num_of_ints(max_num_of_ints_needed_for_answer);
	custom_linked_list_node<int_array>* it_bigger = larger_num->intarrays->first();
	custom_linked_list_node<int_array>* it_smaller = smaller_num->intarrays->first();
	custom_linked_list_node<int_array>* it_answer = answer->intarrays->first();
	int_array* current_intarray_for_bigger = it_bigger->value;
	int_array* current_intarray_for_smaller = it_smaller->value;
	int_array* current_intarray_for_answer = it_answer->value;
	size_t num_int = (size_t)0;
	size_t index_bigger = (size_t)0, index_smaller = (size_t)0, index_answer = (size_t)0;
	size_t num_of_intarrays_used_for_answer = (size_t)1;
	many_bits carry = (many_bits)0;
	few_bits* current_intarr_for_bigger = current_intarray_for_bigger->intarr;
	few_bits* current_intarr_for_smaller = current_intarray_for_smaller->intarr;
	few_bits* current_intarr_for_answer = current_intarray_for_answer->intarr;
	size_t intarr_len_answer = current_intarray_for_answer->intarr_len;
	size_t intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
	size_t intarr_len_smaller = current_intarray_for_smaller->num_of_used_ints;
	const size_t smaller_num_used_ints = smaller_num->num_of_used_ints;
	size_t next_stop = intarr_len_smaller;
	if (intarr_len_bigger < next_stop)
		next_stop = intarr_len_bigger;
	if (intarr_len_answer < next_stop)
		next_stop = intarr_len_answer;
	size_t previous_num_int = (size_t)0;
	current_intarray_for_answer->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int >= next_stop) {
			const size_t difference = num_int - previous_num_int;
			index_bigger += difference;
			index_smaller += difference;
			index_answer += difference;
			previous_num_int = num_int;
			if (num_int >= smaller_num_used_ints)
				break;
			if (index_smaller >= intarr_len_smaller)
			{
				it_smaller = it_smaller->next;
				current_intarray_for_smaller = it_smaller->value;
				current_intarr_for_smaller = current_intarray_for_smaller->intarr;
				intarr_len_smaller = current_intarray_for_smaller->num_of_used_ints;
				index_smaller = (size_t)0;
			}
			if (index_bigger >= intarr_len_bigger)
			{
				it_bigger = it_bigger->next;
				current_intarray_for_bigger = it_bigger->value;
				current_intarr_for_bigger = current_intarray_for_bigger->intarr;
				intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
				index_bigger = (size_t)0;
			}
			if (index_answer >= intarr_len_answer)
			{
				num_of_intarrays_used_for_answer++;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarr_for_answer = current_intarray_for_answer->intarr;
				intarr_len_answer = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = (size_t)0;
			}
			const size_t remaining_intarr_len_smaller = intarr_len_smaller - index_smaller;
			const size_t remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
			const size_t remaining_intarr_len_answer = intarr_len_answer - index_answer;
			const bool smaller_smaller_than_bigger = remaining_intarr_len_smaller < remaining_intarr_len_bigger;
			const bool answer_smaller_than_bigger = remaining_intarr_len_answer < remaining_intarr_len_bigger;
			const bool answer_smaller_than_smaller = remaining_intarr_len_answer < remaining_intarr_len_smaller;
			if (smaller_smaller_than_bigger && !answer_smaller_than_smaller)
				next_stop += remaining_intarr_len_smaller;
			else if (answer_smaller_than_bigger && answer_smaller_than_smaller)
				next_stop += remaining_intarr_len_answer;
			else
				next_stop += remaining_intarr_len_bigger;
			continue;
		}
		carry += static_cast<many_bits>(*current_intarr_for_bigger) + static_cast<many_bits>(*current_intarr_for_smaller);
		*current_intarr_for_answer = static_cast<few_bits>(carry);
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarr_for_bigger;
		++current_intarr_for_smaller;
		++current_intarr_for_answer;
		++num_int;
	}
	if (num_int < bigger_num_used_ints)
	{
		if (index_bigger >= intarr_len_bigger)
		{
			it_bigger = it_bigger->next;
			current_intarray_for_bigger = it_bigger->value;
			current_intarr_for_bigger = current_intarray_for_bigger->intarr;
			intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
			index_bigger = (size_t)0;
		}
		if (index_answer >= intarr_len_answer)
		{
			num_of_intarrays_used_for_answer++;
			it_answer = it_answer->next;
			current_intarray_for_answer = it_answer->value;
			current_intarr_for_answer = current_intarray_for_answer->intarr;
			intarr_len_answer = current_intarray_for_answer->intarr_len;
			current_intarray_for_answer->set_num_of_used_ints_to_maximum();
			index_answer = (size_t)0;
		}
		const size_t remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
		const size_t remaining_intarr_len_answer = intarr_len_answer - index_answer;
		if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
			next_stop = remaining_intarr_len_bigger + num_int;
		else
			next_stop = remaining_intarr_len_answer + num_int;
		if (bigger_num_used_ints < next_stop)
			next_stop = bigger_num_used_ints;
		while (carry != 0)
		{
			if (num_int >= next_stop) {
				if (num_int >= bigger_num_used_ints)
					break;
				const size_t difference = num_int - previous_num_int;
				index_bigger += difference;
				index_answer += difference;
				previous_num_int = num_int;
				if (index_bigger >= intarr_len_bigger)
				{
					it_bigger = it_bigger->next;
					current_intarray_for_bigger = it_bigger->value;
					current_intarr_for_bigger = current_intarray_for_bigger->intarr;
					intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
					index_bigger = (size_t)0;
				}
				if (index_answer >= intarr_len_answer)
				{
					num_of_intarrays_used_for_answer++;
					it_answer = it_answer->next;
					current_intarray_for_answer = it_answer->value;
					current_intarr_for_answer = current_intarray_for_answer->intarr;
					intarr_len_answer = current_intarray_for_answer->intarr_len;
					current_intarray_for_answer->set_num_of_used_ints_to_maximum();
					index_answer = (size_t)0;
				}
				const size_t remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
				const size_t remaining_intarr_len_answer = intarr_len_answer - index_answer;
				if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
					next_stop += remaining_intarr_len_bigger;
				else
					next_stop += remaining_intarr_len_answer;
				continue;
			}
			carry += static_cast<many_bits>(*current_intarr_for_bigger);
			*current_intarr_for_answer = static_cast<few_bits>(carry);
			carry >>= NUM_OF_BITS_few_bits;
			++current_intarr_for_bigger;
			++current_intarr_for_answer;
			++num_int;
		}
		const size_t difference = num_int - previous_num_int;
		index_bigger += difference;
		index_answer += difference;
		previous_num_int = num_int;
		if (num_int < bigger_num_used_ints)
		{
			if (index_bigger >= intarr_len_bigger)
			{
				it_bigger = it_bigger->next;
				current_intarray_for_bigger = it_bigger->value;
				current_intarr_for_bigger = current_intarray_for_bigger->intarr;
				intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
				index_bigger = (size_t)0;
			}
			if (index_answer >= intarr_len_answer)
			{
				num_of_intarrays_used_for_answer++;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarr_for_answer = current_intarray_for_answer->intarr;
				intarr_len_answer = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = (size_t)0;
			}
			const size_t remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
			const size_t remaining_intarr_len_answer = intarr_len_answer - index_answer;
			if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
				next_stop = remaining_intarr_len_bigger + num_int;
			else
				next_stop = remaining_intarr_len_answer + num_int;
			while (true)
			{
				if (num_int >= next_stop) {
					const size_t difference = num_int - previous_num_int;
					index_bigger += difference;
					index_answer += difference;
					previous_num_int = num_int;
					if (num_int >= bigger_num_used_ints)
						break;
					if (index_bigger >= intarr_len_bigger)
					{
						it_bigger = it_bigger->next;
						current_intarray_for_bigger = it_bigger->value;
						current_intarr_for_bigger = current_intarray_for_bigger->intarr;
						intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
						index_bigger = (size_t)0;
					}
					if (index_answer >= intarr_len_answer)
					{
						num_of_intarrays_used_for_answer++;
						it_answer = it_answer->next;
						current_intarray_for_answer = it_answer->value;
						current_intarr_for_answer = current_intarray_for_answer->intarr;
						intarr_len_answer = current_intarray_for_answer->intarr_len;
						current_intarray_for_answer->set_num_of_used_ints_to_maximum();
						index_answer = (size_t)0;
					}
					const size_t remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
					const size_t remaining_intarr_len_answer = intarr_len_answer - index_answer;
					if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
						next_stop += remaining_intarr_len_bigger;
					else
						next_stop += remaining_intarr_len_answer;
					continue;
				}
				*current_intarr_for_answer = *current_intarr_for_bigger;
				++current_intarr_for_answer;
				++current_intarr_for_bigger;
				++num_int;
			}
		}
	}
	if (carry == (many_bits)0)
	{
		answer->num_of_used_ints = bigger_num_used_ints;
		current_intarray_for_answer->num_of_used_ints = index_answer;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
	else
	{
		if (index_answer >= intarr_len_answer)
		{
			++num_of_intarrays_used_for_answer;
			it_answer = it_answer->next;
			current_intarray_for_answer = it_answer->value;
			index_answer = (size_t)0;
		}
		answer->num_of_used_ints = bigger_num_used_ints + (size_t)1;
		current_intarray_for_answer->intarr[index_answer] = static_cast<few_bits>(carry);
		current_intarray_for_answer->num_of_used_ints = index_answer + (size_t)1;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
	answer->_is_negative = set_answer_to_negative;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nEnd of add:";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"void unlimited_int::add(const unlimited_int* num_to_add, unlimited_int* answer) const\"");
#endif
}
