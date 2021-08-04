#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//Long division, some binary search.
std::shared_ptr<unlimited_int> unlimited_int::divide_by(const unlimited_int& num_to_divide_by) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if ((this->find_inconsistencies()) || (num_to_divide_by.find_inconsistencies()))
		throw "\nThe inconsistency was found in start of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
	if (num_to_divide_by.num_of_used_ints == 0) { throw "\nError trying to divide by zero."; }
	const many_bits num_of_used_ints_divide = num_to_divide_by.num_of_used_ints;
	const many_bits num_of_used_ints_this = this->num_of_used_ints;
	char result_compare = this->compare_to_ignore_sign(num_to_divide_by);
	if (result_compare == 'E') { return std::shared_ptr<unlimited_int>(new unlimited_int(1)); }
	if (result_compare == 'S') { return std::shared_ptr<unlimited_int>(new unlimited_int(0)); }
	if ((num_of_used_ints_this == 0) || (num_of_used_ints_divide == 0)) { return std::shared_ptr<unlimited_int>(new unlimited_int(0)); }
	many_bits_signed exact_power_of_2 = num_to_divide_by.find_exact_log_2();
	if (exact_power_of_2 >= 0) //more efficient method of division when dividing by power of 2
	{
		std::shared_ptr<unlimited_int> answer = (*this) >> (many_bits)exact_power_of_2;
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
		if (answer->find_inconsistencies())
			throw "\nThe inconsistency was found in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
		return answer;
	}
	if (num_of_used_ints_this == num_of_used_ints_divide)
	{
		std::shared_ptr<unlimited_int> answer(new unlimited_int(this->binary_search_divide(num_to_divide_by)));
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
		if (answer->find_inconsistencies())
			throw "\nThe inconsistency was found in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
		return answer;
	}
	unlimited_int* answer = new unlimited_int;
	unlimited_int partial_this;
	many_bits num_of_ints_currently_using_from_this = num_of_used_ints_divide;
	this->copy_most_significant_to(partial_this, num_of_ints_currently_using_from_this);
	__list_location__ ll_start = this->find_num_used_int_from_significant(num_of_ints_currently_using_from_this + 1);
	Node* it_this = ll_start.node;
	int_array* current_int_array_this = it_this->value;
	many_bits_signed index_this = ll_start.index;
	result_compare = partial_this.compare_to_ignore_sign(num_to_divide_by);
	if (result_compare == 'S')
	{
		partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
		++num_of_ints_currently_using_from_this;
		--index_this;
		if (index_this < 0)
		{
			it_this = it_this->previous;
			if (it_this != nullptr)
			{
				current_int_array_this = it_this->value;
				index_this = current_int_array_this->num_of_used_ints - 1;
			}
		}
	}
	unlimited_int result_of_multiplication;
	while (true)
	{
		if (index_this < 0)
		{
			if (it_this != nullptr)
			{
				it_this = it_this->previous;
				if (it_this != nullptr)
				{
					current_int_array_this = it_this->value;
					index_this = (many_bits_signed)current_int_array_this->num_of_used_ints - (many_bits_signed)1;
				}
				else
					index_this = (many_bits_signed)0; //just to stop the loop
				continue;
			}
		}
		const few_bits binary_search_division_result = partial_this.binary_search_divide(num_to_divide_by);
		answer->push_to_insignificant(binary_search_division_result);
		num_to_divide_by.multiply(binary_search_division_result, &result_of_multiplication);
		result_of_multiplication.is_negative = false;
		partial_this.subtract(&result_of_multiplication, &partial_this);
		result_of_multiplication.flush();
		if (num_of_ints_currently_using_from_this >= num_of_used_ints_this)
			break;
		partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
		++num_of_ints_currently_using_from_this;
		--index_this;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
	return std::shared_ptr<unlimited_int>(answer);
}
void unlimited_int::push_to_insignificant(const few_bits num_to_push)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"push_to_insignificant(few_bits num_to_push)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"";
#endif
	if (this->num_of_used_ints == 0) { this->assign(num_to_push); this->self_abs(); return; }
	int_array* int_array_first = this->intarrays.intarrays.first->value;
	if (int_array_first->is_full())
	{
		this->intarrays.increase_by_one_array_from_piggy_bank_to_insignificant();
		++this->num_of_intarrays_used;
		int_array_first = this->intarrays.intarrays.first->value;
		int_array_first->num_of_used_ints = 0;
	}
	int_array_first->push_to_insignificant(num_to_push);
	++this->num_of_used_ints;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"";
#endif
}
//This function only produces the correct result when the difference length between the two unlimited_ints is 1 or 0. Length being the the number of few_bits used.
few_bits unlimited_int::binary_search_divide(const unlimited_int& num_to_divide_by) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"few_bits unlimited_int::binary_search_divide(const unlimited_int& num_to_divide_by) const\"";
#endif
#if DEBUG_MODE > 0
	if ((this->find_inconsistencies()) || (num_to_divide_by.find_inconsistencies()))
		throw "\nThe inconsistency was found in function \"few_bits unlimited_int::binary_search_divide(const unlimited_int& num_to_divide_by) const\"";
#endif
	few_bits min = 0, max = (few_bits)MAX_few_bits_NUM;
	char result_compare = unlimited_int::compare_multiplication_to_num(num_to_divide_by, min, *this);
	if (result_compare == 'E') { return min; }
	result_compare = unlimited_int::compare_multiplication_to_num(num_to_divide_by, max, *this);
	if ((result_compare == 'E') || (result_compare == 'S'))
		return max;
	while (true)
	{
		few_bits average = (((many_bits)min) + ((many_bits)max)) / 2;
		result_compare = unlimited_int::compare_multiplication_to_num(num_to_divide_by, average, *this);
		if (result_compare == 'E') { return average; }
		if (result_compare == 'L') { max = average; }
		else { min = average; }
		if (max - min <= 1) { return min; }
	}
	return (few_bits)0;
}
//starts the multiplication from the right, and compares while multiplying. That way this function is closer to O(1) than to O(n)
char unlimited_int::compare_multiplication_to_num(const unlimited_int& multiplicand, const few_bits multiplier, const unlimited_int& result_target)
{
	if (multiplicand.is_zero() || (multiplier == (few_bits)0))
		return (unlimited_int(0).compare_to_ignore_sign(result_target));
	else if (result_target.num_of_used_ints < multiplicand.num_of_used_ints)
		return 'L';
	else if (result_target.num_of_used_ints >= multiplicand.num_of_used_ints + (many_bits)2)
		return 'S';
	//create copy of multiplicand to store the result of the multiplication. Normally we'll reach a verdict of the comparison much before we're finished calculating the entire result of the multiplication.
	unlimited_int result; //using result just as a temporary holder. Won't be in a usable state when the function exits. Will still flush successfully.
	result.intarrays.increase_until_num_of_ints(multiplicand.num_of_used_ints + (many_bits)1);
	Node* current_int_array_Node_multiplicand = multiplicand.get_most_significant_used_int_array();
	Node* current_int_array_Node_result_target = result_target.get_most_significant_used_int_array();
	Node* current_int_array_Node_result = result.intarrays.intarrays.last;
	int_array current_int_array_multiplicand = *current_int_array_Node_multiplicand->value;
	int_array current_int_array_result_target = *current_int_array_Node_result_target->value;
	int_array current_int_array_result = *current_int_array_Node_result->value;
	many_bits_signed index_multiplicand = (many_bits_signed)current_int_array_multiplicand.num_of_used_ints - (many_bits_signed)1;
	many_bits_signed index_result_target = (many_bits_signed)current_int_array_result_target.num_of_used_ints - (many_bits_signed)1;
	many_bits_signed index_result = (many_bits_signed)current_int_array_result.intarr_len - (many_bits_signed)1;
	many_bits int_counter = 0;
	many_bits num_of_ints_in_result_that_have_been_compared = 0; //can't retroactively change even with the most extreme carry
	bool firsts_were_compared = false;
	few_bits first_in_result_target = (few_bits)0;
	if (result_target.num_of_used_ints == multiplicand.num_of_used_ints + (many_bits)1)
	{
		first_in_result_target = current_int_array_result_target.intarr[index_result_target];
		--index_result_target;
		if (index_result_target < (many_bits_signed)0)
		{
			current_int_array_Node_result_target = current_int_array_Node_result_target->previous;
			current_int_array_result_target = *current_int_array_Node_result_target->value;
			index_result_target = current_int_array_result_target.num_of_used_ints - (many_bits)1;
		}
	}
	few_bits* first_in_result = &current_int_array_result.intarr[index_result];
	*first_in_result = (few_bits)0;
	--index_result;
	if (index_result < (many_bits_signed)0)
	{
		current_int_array_Node_result = current_int_array_Node_result->previous;
		current_int_array_result = *current_int_array_Node_result->value;
		index_result = current_int_array_result.intarr_len - (many_bits)1;
	}
	many_bits stop_at_for_multiplicand = current_int_array_multiplicand.num_of_used_ints;
	many_bits stop_at_for_result_target = (many_bits)index_result_target + (many_bits)1;
	many_bits stop_at_for_result = (many_bits)index_result + (many_bits)1;
	many_bits stop_at = stop_at_for_multiplicand;
	if (stop_at_for_result_target < stop_at)
		stop_at = stop_at_for_result_target;
	if (stop_at_for_result < stop_at)
		stop_at = stop_at_for_result;
	const many_bits stop_all = multiplicand.num_of_used_ints;
	const many_bits multiplier_many_bits = (many_bits)multiplier;
	Node* compare_from_int_array_Node_result_target = current_int_array_Node_result_target;
	Node* compare_from_int_array_Node_result = current_int_array_Node_result;
	int_array compare_from_int_array_result_target = current_int_array_result_target;
	int_array compare_from_int_array_result = current_int_array_result;
	many_bits compare_from_index_result_target = index_result_target;
	many_bits compare_from_index_result = index_result;
	while (true)
	{
		if (int_counter >= stop_at)
		{
			if (int_counter >= stop_all)
				break;
			if (int_counter >= stop_at_for_multiplicand)
			{
				current_int_array_Node_multiplicand = current_int_array_Node_multiplicand->previous;
				current_int_array_multiplicand = *current_int_array_Node_multiplicand->value;
				stop_at_for_multiplicand = int_counter + current_int_array_multiplicand.num_of_used_ints;
				index_multiplicand = (many_bits_signed)current_int_array_multiplicand.num_of_used_ints - (many_bits_signed)1;
			}
			if (int_counter >= stop_at_for_result_target)
			{
				current_int_array_Node_result_target = current_int_array_Node_result_target->previous;
				current_int_array_result_target = *current_int_array_Node_result_target->value;
				stop_at_for_result_target = int_counter + current_int_array_result_target.num_of_used_ints;
				index_result_target = (many_bits_signed)current_int_array_result_target.num_of_used_ints - (many_bits_signed)1;
			}
			if (int_counter >= stop_at_for_result)
			{
				current_int_array_Node_result = current_int_array_Node_result->previous;
				current_int_array_result = *current_int_array_Node_result->value;
				stop_at_for_result = int_counter + current_int_array_result.intarr_len;
				index_result = (many_bits_signed)current_int_array_result.intarr_len - (many_bits_signed)1;
			}
			stop_at = stop_at_for_multiplicand;
			if (stop_at_for_result_target < stop_at)
				stop_at = stop_at_for_result_target;
			if (stop_at_for_result < stop_at)
				stop_at = stop_at_for_result;
			continue;
		}
		const many_bits result_multiplication = (many_bits)current_int_array_multiplicand.intarr[index_multiplicand] * multiplier_many_bits;
		const few_bits multiplication_remainder = (few_bits)result_multiplication;
		few_bits carry = (few_bits)(result_multiplication >> NUM_OF_BITS_few_bits);
		current_int_array_result.intarr[index_result] = multiplication_remainder;
		//Now iterating through in reverse (from insignificant to significant) for the purpose of "scattering" the carry throughout the result.
		const many_bits num_of_ints_filled_in_result = int_counter + (many_bits)2;
		const many_bits stop_all_reverse = num_of_ints_filled_in_result - (many_bits)1;
		many_bits int_counter_reverse = 0;
		many_bits index_result_reverse = (many_bits)index_result + (many_bits)1;
		Node* current_int_array_Node_result_reverse = current_int_array_Node_result;
		int_array current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
		many_bits stop_at_for_result_reverse = current_int_array_result_reverse.intarr_len - index_result_reverse;
		while (carry > (few_bits)0)
		{
			if (int_counter_reverse >= stop_at_for_result_reverse)
			{
				if (int_counter_reverse >= stop_all_reverse)
					break;
				if (index_result_reverse >= current_int_array_result_reverse.intarr_len)
				{
					index_result_reverse = (many_bits)0;
					current_int_array_Node_result_reverse = current_int_array_Node_result_reverse->next;
					current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
					stop_at_for_result_reverse = int_counter_reverse + current_int_array_result_reverse.intarr_len;
				}
				continue;
			}
			const many_bits result_addition = (many_bits)current_int_array_result_reverse.intarr[index_result_reverse] + carry;
			const few_bits addition_remainder_reverse = (few_bits)result_addition;
			carry = (few_bits)(result_addition >> NUM_OF_BITS_few_bits);
			current_int_array_result_reverse.intarr[index_result_reverse] = addition_remainder_reverse;
			++int_counter_reverse;
			++index_result_reverse;
		}
		const many_bits difference_between_current_and_already_certain = num_of_ints_filled_in_result - num_of_ints_in_result_that_have_been_compared;
		few_bits max_hypothetical_carry;
		bool at_last = false;
		if (int_counter >= stop_all - (many_bits)1)
		{
			max_hypothetical_carry = (few_bits)0; //there can't be any carry if it's the end
			at_last = true;
		}
		else
			max_hypothetical_carry = (few_bits)MAX_few_bits_NUM;
		//Now iterating in reverse for the purpose of finding certainties
		int_counter_reverse = 0;
		index_result_reverse = (many_bits)index_result;
		current_int_array_Node_result_reverse = current_int_array_Node_result;
		current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
		stop_at_for_result_reverse = current_int_array_result_reverse.intarr_len - index_result_reverse;
		many_bits index_result_target_reverse = (many_bits)index_result_target;
		Node* current_int_array_Node_result_target_reverse = current_int_array_Node_result_target;
		int_array current_int_array_result_target_reverse = *current_int_array_Node_result_target_reverse->value;
		many_bits stop_at_for_result_target_reverse = current_int_array_result_target_reverse.num_of_used_ints - index_result_target_reverse;
		many_bits stop_at_reverse = stop_at_for_result_reverse;
		if (stop_at_for_result_target_reverse < stop_at_reverse)
			stop_at_reverse = stop_at_for_result_target_reverse;
		if (difference_between_current_and_already_certain < stop_at_reverse)
			stop_at_reverse = difference_between_current_and_already_certain;
		while (true)
		{
			if (int_counter_reverse >= stop_at_reverse)
			{
				if (int_counter_reverse >= difference_between_current_and_already_certain)
					break;
				if (int_counter_reverse >= stop_at_for_result_reverse)
				{
					index_result_reverse = (many_bits)0;
					current_int_array_Node_result_reverse = current_int_array_Node_result_reverse->next;
					current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
					stop_at_for_result_reverse = int_counter_reverse + current_int_array_result_reverse.intarr_len;
				}
				if (int_counter_reverse >= stop_at_for_result_target_reverse)
				{
					index_result_target_reverse = (many_bits)0;
					current_int_array_Node_result_target_reverse = current_int_array_Node_result_target_reverse->next; //ignore the "dereferencing null pointer" warning
					if (current_int_array_Node_result_target_reverse != nullptr)
					{
						current_int_array_result_target_reverse = *current_int_array_Node_result_target_reverse->value;
						stop_at_for_result_target_reverse = int_counter_reverse + current_int_array_result_target_reverse.num_of_used_ints;
					}
				}
				stop_at_reverse = stop_at_for_result_reverse;
				if (current_int_array_Node_result_target_reverse != nullptr)
					if (stop_at_for_result_target_reverse < stop_at_reverse)
						stop_at_reverse = stop_at_for_result_target_reverse;
				if (difference_between_current_and_already_certain < stop_at_reverse)
					stop_at_reverse = difference_between_current_and_already_certain;
				continue;
			}
			const few_bits current_value_result = current_int_array_result_reverse.intarr[index_result_reverse];
			max_hypothetical_carry = (few_bits)(((many_bits)current_value_result + (many_bits)max_hypothetical_carry) >> NUM_OF_BITS_few_bits);
			if (max_hypothetical_carry == (few_bits)0)
				break;
			++int_counter_reverse;
			++index_result_reverse;
			++index_result_target_reverse;
		}
		if (max_hypothetical_carry == (few_bits)0)
		{
			many_bits num_of_new_comparisons_to_make;
			if (at_last)
				num_of_new_comparisons_to_make = difference_between_current_and_already_certain;
			else
				num_of_new_comparisons_to_make = difference_between_current_and_already_certain - (int_counter_reverse + (many_bits)1);
			num_of_ints_in_result_that_have_been_compared += num_of_new_comparisons_to_make;
			if (num_of_new_comparisons_to_make > (many_bits)0)
			{
				if (!firsts_were_compared)
				{
					firsts_were_compared = true;
					--num_of_new_comparisons_to_make;
					const few_bits value_of_first_of_result = *first_in_result;
					if (value_of_first_of_result > first_in_result_target)
						return 'L';
					else if (value_of_first_of_result < first_in_result_target)
						return 'S';
				}
				if (num_of_new_comparisons_to_make > (many_bits)0)
				{
					//go through from compare_from_int_array_Node_result_target and from compare_from_int_array_Node_result all the way down num_of_new_comparisons_to_make times.
					Node* comparing_int_array_Node_result_target = compare_from_int_array_Node_result_target;
					int_array comparing_int_array_result_target = compare_from_int_array_result_target;
					many_bits_signed comparing_index_result_target = compare_from_index_result_target;
					Node* comparing_int_array_Node_result = compare_from_int_array_Node_result;
					int_array comparing_int_array_result = compare_from_int_array_result;
					many_bits_signed comparing_index_result = compare_from_index_result;
					//adjusting starting points of next comparison iteration
					compare_from_int_array_Node_result_target = current_int_array_Node_result_target_reverse;
					compare_from_int_array_result_target = current_int_array_result_target_reverse;
					compare_from_index_result_target = index_result_target_reverse;
					compare_from_int_array_Node_result = current_int_array_Node_result_reverse;
					compare_from_int_array_result = current_int_array_result_reverse;
					compare_from_index_result = index_result_reverse;
					const many_bits stop_all_compare = num_of_new_comparisons_to_make;
					many_bits stop_compare_for_result_target = (many_bits)comparing_index_result_target + (many_bits)1;
					many_bits stop_compare_for_result = (many_bits)comparing_index_result + (many_bits)1;
					many_bits int_counter_compare = (many_bits)0;
					many_bits stop_at_compare = stop_compare_for_result_target;
					if (stop_compare_for_result < stop_at_compare)
						stop_at_compare = stop_compare_for_result;
					if (stop_all_compare < stop_at_compare)
						stop_at_compare = stop_all_compare;
					while (true)
					{
						if (int_counter_compare >= stop_at_compare)
						{
							if (int_counter_compare >= stop_all_compare)
								break;
							if (int_counter_compare >= stop_compare_for_result_target)
							{
								comparing_int_array_Node_result_target = comparing_int_array_Node_result_target->previous;
								comparing_int_array_result_target = *comparing_int_array_Node_result_target->value;
								comparing_index_result_target = (many_bits_signed)comparing_int_array_result_target.num_of_used_ints - (many_bits_signed)1;
								stop_compare_for_result_target = int_counter_compare + comparing_int_array_result_target.num_of_used_ints;
							}
							if (int_counter_compare >= stop_compare_for_result)
							{
								comparing_int_array_Node_result = comparing_int_array_Node_result->previous;
								comparing_int_array_result = *comparing_int_array_Node_result->value;
								comparing_index_result = (many_bits_signed)comparing_int_array_result.intarr_len - (many_bits_signed)1;
								stop_compare_for_result = int_counter_compare + comparing_int_array_result.intarr_len;
							}
							stop_at_compare = stop_compare_for_result_target;
							if (stop_compare_for_result < stop_at_compare)
								stop_at_compare = stop_compare_for_result;
							if (stop_all_compare < stop_at_compare)
								stop_at_compare = stop_all_compare;
							continue;
						}
						const few_bits current_num_compare_result_target = comparing_int_array_result_target.intarr[comparing_index_result_target];
						const few_bits current_num_compare_result = comparing_int_array_result.intarr[comparing_index_result];
						if (current_num_compare_result > current_num_compare_result_target)
							return 'L';
						else if (current_num_compare_result < current_num_compare_result_target)
							return 'S';
						++int_counter_compare;
						--comparing_index_result_target;
						--comparing_index_result;
					}
				}
			}
		}
		//now check until where is already certain, and iterate through result_target as well.
		++int_counter;
		--index_multiplicand;
		--index_result_target;
		--index_result;
	}
	return 'E';
}
