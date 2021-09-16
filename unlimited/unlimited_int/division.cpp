#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//Division without binary search, similar approach to the pow(base, power) algorithm. Returns positive number (or zero) whether or not the answer was supposed to be negative.
unlimited_int unlimited_int::divide_by_repeated_addition(const unlimited_int& num_to_divide_by) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"divide_by_repeated_addition(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if ((this->find_inconsistencies()) || (num_to_divide_by.find_inconsistencies()))
		throw std::logic_error("The inconsistency was found in start of function \"divide_by_repeated_addition(unlimited_int& num_to_divide_by)\"");
#endif
	if (num_to_divide_by.is_zero())
		throw std::invalid_argument("Error trying to divide by zero.");
	const char dividend_comparison_to_divisor = this->compare_to_ignore_sign(num_to_divide_by);
	if (this->is_zero() || dividend_comparison_to_divisor == 'S')
		return unlimited_int();
	if (dividend_comparison_to_divisor == 'E')
		return unlimited_int((few_bits)1);
	bool was_exact_power_of_2;
	const size_t exact_power_of_2 = num_to_divide_by.find_exact_log_2(&was_exact_power_of_2);
	if (was_exact_power_of_2) //more efficient method of division when dividing by power of 2
	{
		unlimited_int this_positive(*this, false);
		this_positive._is_negative = false;
		return this_positive >> exact_power_of_2;
	}
	//Should eventually get close to the size of dividend
	unlimited_int sum_amounts;
	//The amount to add to sum_amounts every time
	unlimited_int current_addition_amount(num_to_divide_by);
	current_addition_amount.self_abs();
	//Will eventaully contain the result
	unlimited_int sum_magnitudes;
	//The amount to add the sum_magnitudes every time
	unlimited_int current_shift_magnitude((few_bits)1);
	bool reached_exact_result_while_going_up = false;
	//Going up
	while (true)
	{
		sum_amounts += current_addition_amount;
		const char sum_comparison_to_dividend = sum_amounts.compare_to_ignore_sign(*this);
		if (sum_comparison_to_dividend == 'E')
		{
			sum_magnitudes += current_shift_magnitude;
			reached_exact_result_while_going_up = true;
			break;
		}
		else if (sum_comparison_to_dividend == 'S')
		{
			current_addition_amount <<= (size_t)1;
			sum_magnitudes += current_shift_magnitude;
			current_shift_magnitude <<= (size_t)1;
		}
		else //Reached max
		{
			sum_amounts -= current_addition_amount;
			current_addition_amount <<= (size_t)1;
			current_shift_magnitude <<= (size_t)1;
			break;
		}
	}
	if (!reached_exact_result_while_going_up)
	{
		//Going down
		while (true)
		{
			sum_amounts += current_addition_amount;
			const char sum_comparison_to_dividend = sum_amounts.compare_to_ignore_sign(*this);
			if (sum_comparison_to_dividend == 'E')
			{
				sum_magnitudes += current_shift_magnitude;
				break;
			}
			else if (sum_comparison_to_dividend == 'S')
				sum_magnitudes += current_shift_magnitude;
			else
				sum_amounts -= current_addition_amount;
			current_shift_magnitude >>= (size_t)1;
			if (current_shift_magnitude.is_zero())
				break;
			current_addition_amount >>= (size_t)1;
		}
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"divide_by_repeated_addition(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if (sum_magnitudes.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"divide_by_repeated_addition(unlimited_int& num_to_divide_by)\"");
	unlimited_int multiplication_check(sum_magnitudes * num_to_divide_by);
	multiplication_check.self_abs();
	unlimited_int num_to_divide_by_cpy(num_to_divide_by, false);
	num_to_divide_by_cpy.self_abs();
	if (!(multiplication_check.compare_to_ignore_sign(*this) != 'L' && (multiplication_check + num_to_divide_by_cpy).compare_to_ignore_sign(*this) != 'S'))
		throw std::logic_error("Wrong answer in function unlimited_int::divide_by_repeated_addition");
#endif
	return sum_magnitudes;
}
//Long division, some binary search. Returns positive number (or zero) whether or not the answer was supposed to be negative.
unlimited_int unlimited_int::divide_by(const unlimited_int& num_to_divide_by) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if ((this->find_inconsistencies()) || (num_to_divide_by.find_inconsistencies()))
		throw std::logic_error("The inconsistency was found in start of function \"divide_by(unlimited_int& num_to_divide_by)\"");
#endif
	if (num_to_divide_by.num_of_used_ints == (size_t)0)
		throw std::invalid_argument("Error trying to divide by zero in function \"unlimited_int::divide_by(const unlimited_int&)\"");
	const size_t num_of_used_ints_divide = num_to_divide_by.num_of_used_ints;
	const size_t num_of_used_ints_this = this->num_of_used_ints;
	const char result_compare = this->compare_to_ignore_sign(num_to_divide_by);
	if (result_compare == 'E')
		return unlimited_int((few_bits)1);
	if (result_compare == 'S')
		return unlimited_int();
	if (num_of_used_ints_this == (size_t)0)
		return unlimited_int();
	bool was_exact_power_of_2;
	const size_t exact_power_of_2 = num_to_divide_by.find_exact_log_2(&was_exact_power_of_2);
	if (was_exact_power_of_2) //more efficient method of division when dividing by power of 2
	{
		unlimited_int this_positive(*this, false);
		this_positive._is_negative = false;
		return this_positive >> exact_power_of_2;
	}
	//If the lengths are the same, the result is guaranteed to fit into a few_bits
	if (num_of_used_ints_this == num_of_used_ints_divide)
		return unlimited_int(this->binary_search_divide(num_to_divide_by));
	unlimited_int answer;
	unlimited_int partial_this;
	size_t num_of_ints_currently_using_from_this = num_of_used_ints_divide;
	this->copy_most_significant_to(partial_this, num_of_ints_currently_using_from_this);
	int_array_list::list_location ll_start = this->find_num_used_int_from_significant(num_of_ints_currently_using_from_this + (size_t)1);
	custom_linked_list_node<int_array>* it_this = ll_start.node;
	int_array* current_int_array_this = it_this->value;
	const custom_linked_list_node<int_array> *const this_intarrays_begin =  this->intarrays->begin();
	size_t index_this = ll_start.index;
	const char result_compare2 = partial_this.compare_to_ignore_sign(num_to_divide_by);
	if (result_compare2 == 'S')
	{
		partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
		++num_of_ints_currently_using_from_this;
		if (index_this-- == (size_t)0)
		{
			it_this = it_this->previous;
			if (it_this != this_intarrays_begin)
			{
				current_int_array_this = it_this->value;
				index_this = current_int_array_this->num_of_used_ints - (size_t)1;
			}
		}
	}
	unlimited_int result_of_multiplication;
	bool reached_beginning_this = false;
	while (true)
	{
		if (reached_beginning_this)
		{
			reached_beginning_this = false;
			if (it_this != this_intarrays_begin)
			{
				it_this = it_this->previous;
				if (it_this != this_intarrays_begin)
				{
					current_int_array_this = it_this->value;
					index_this = current_int_array_this->num_of_used_ints - (size_t)1;
				}
				else
					index_this = (size_t)0; //just to stop the loop
				continue;
			}
		}
		const few_bits binary_search_division_result = partial_this.binary_search_divide(num_to_divide_by);
		answer.push_to_insignificant(binary_search_division_result);
		num_to_divide_by.multiply(binary_search_division_result, &result_of_multiplication);
		result_of_multiplication._is_negative = false;
		partial_this.subtract(&result_of_multiplication, &partial_this);
		result_of_multiplication.flush();
		if (num_of_ints_currently_using_from_this >= num_of_used_ints_this)
			break;
		partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
		++num_of_ints_currently_using_from_this;
		if (index_this-- == (size_t)0)
			reached_beginning_this = true;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if (answer.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"divide_by(unlimited_int& num_to_divide_by)\"");
	unlimited_int multiplication_check(answer * num_to_divide_by);
	multiplication_check._is_negative = false;
	unlimited_int num_to_divide_by_cpy(num_to_divide_by, false);
	num_to_divide_by_cpy._is_negative = false;
	if (!(multiplication_check.compare_to_ignore_sign(*this) != 'L' && (multiplication_check + num_to_divide_by_cpy).compare_to_ignore_sign(*this) != 'S'))
		throw std::logic_error("Wrong answer in function unlimited_int::divide_by");
#endif
	return answer;
}
unlimited_int unlimited_int::divide_by_respect_sign(const few_bits num_to_divide_by) const
{
	if (num_to_divide_by == (few_bits)0)
		throw std::invalid_argument("Error in function: \"unlimited_int* unlimited_int::divide_by_respect_sign(const few_bits num_to_divide_by) const\" Can't divide by zero");
	unlimited_int answer = this->divide_by(num_to_divide_by);
	if (this->is_negative())
		answer.self_negative();
	return answer;
}
//Long division, some binary search. Returns positive number (or zero) whether or not the answer was supposed to be negative.
unlimited_int unlimited_int::divide_by(const few_bits num_to_divide_by) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"divide_by(const few_bits num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"divide_by(const few_bits num_to_divide_by)\"");
#endif
	if (num_to_divide_by == (few_bits)0)
		throw std::invalid_argument("Error trying to divide by zero.");
	const size_t num_of_used_ints_this = this->num_of_used_ints;
	const char result_compare = this->compare_to_ignore_sign(num_to_divide_by);
	if (result_compare == 'E')
		return unlimited_int((few_bits)1);
	if (result_compare == 'S')
		return unlimited_int();
	if (num_of_used_ints_this == (size_t)0)
		return unlimited_int();
	bool was_exact_power_of_2;
	const size_t exact_power_of_2 = (size_t)unlimited_int::find_exact_log_2(num_to_divide_by, &was_exact_power_of_2);
	if (was_exact_power_of_2) //more efficient method of division when dividing by power of 2
	{
		unlimited_int this_positive(*this, false);
		this_positive.self_abs();
		return this_positive >> exact_power_of_2;
	}
	unlimited_int answer;
	unlimited_int partial_this;
	size_t num_of_ints_currently_using_from_this = (size_t)1;
	this->copy_most_significant_to(partial_this, num_of_ints_currently_using_from_this);
	custom_linked_list_node<int_array>* it_this = nullptr;
	int_array* current_int_array_this = nullptr; //Initializing to rediculous values
	size_t index_this = MAX_size_t_NUM;          //just to make sure that there's an error if the variables don't end up getting a valid value.
	const custom_linked_list_node<int_array>* const this_intarrays_begin = this->intarrays->begin();
	if (num_of_ints_currently_using_from_this < this->num_of_used_ints)
	{
		int_array_list::list_location ll_start = this->find_num_used_int_from_significant(num_of_ints_currently_using_from_this + (size_t)1);
		it_this = ll_start.node;
		current_int_array_this = it_this->value;
		index_this = ll_start.index;
		const char result_compare2 = partial_this.compare_to_ignore_sign(num_to_divide_by);
		if (result_compare2 == 'S')
		{
			partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
			++num_of_ints_currently_using_from_this;
			if (index_this-- == 0)
			{
				it_this = it_this->previous;
				if (it_this != this_intarrays_begin)
				{
					current_int_array_this = it_this->value;
					index_this = current_int_array_this->num_of_used_ints - (size_t)1;
				}
			}
		}
	}
	const many_bits divisor = (many_bits)num_to_divide_by;
	bool reached_beginning_this = false;
	while (true)
	{
		if (reached_beginning_this)
		{
			reached_beginning_this = false;
			if (it_this != this_intarrays_begin)
			{
				it_this = it_this->previous;
				if (it_this != this_intarrays_begin)
				{
					current_int_array_this = it_this->value;
					index_this = current_int_array_this->num_of_used_ints - (size_t)1;
				}
				else
					index_this = (size_t)0; //just to stop the loop
				continue;
			}
		}
		const few_bits mini_division_result = partial_this.binary_search_divide(num_to_divide_by);
		answer.push_to_insignificant(mini_division_result);
		const many_bits result_of_multiplication = (many_bits)mini_division_result * divisor;
		const unlimited_int result_of_multiplication_ui(result_of_multiplication);
		partial_this.subtract(&result_of_multiplication_ui, &partial_this);
		if (num_of_ints_currently_using_from_this >= num_of_used_ints_this)
			break;
		partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
		++num_of_ints_currently_using_from_this;
		if (index_this-- == (size_t)0)
			reached_beginning_this = true;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"divide_by(const few_bits num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if (answer.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"divide_by(const few_bits num_to_divide_by)\"");
	unlimited_int multiplication_check;
	answer.multiply(num_to_divide_by, &multiplication_check);
	multiplication_check.self_abs();
	if (!(multiplication_check.compare_to_ignore_sign(*this) != 'L' && (multiplication_check + unlimited_int(num_to_divide_by)).compare_to_ignore_sign(*this) != 'S'))
		throw std::logic_error("Wrong answer in function unlimited_int::divide_by");
#endif
	return answer;
}
void unlimited_int::push_to_insignificant(const few_bits num_to_push)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"push_to_insignificant(few_bits num_to_push)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"");
#endif
	if (this->num_of_used_ints == (size_t)0)
	{
		this->assign(num_to_push);
		this->self_abs();
		return;
	}
	int_array* int_array_first = this->intarrays->first()->value;
	if (int_array_first->is_full())
	{
		this->intarrays->increase_by_one_array_from_piggy_bank_to_insignificant();
		++this->num_of_intarrays_used;
		int_array_first = this->intarrays->first()->value;
		int_array_first->num_of_used_ints = (size_t)0;
	}
	int_array_first->push_to_insignificant(num_to_push);
	++this->num_of_used_ints;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"");
#endif
}
//This function only produces the correct result when the difference length between the two unlimited_ints is 1 or 0. Length being the the number of few_bits used.
few_bits unlimited_int::binary_search_divide(const unlimited_int& num_to_divide_by) const
{
	static_assert(sizeof(few_bits) * 2 == sizeof(many_bits), "Assertion error: NUM_OF_BITS_many_bits must have exactly twice the number of bits as NUM_OF_BITS_few_bits");
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"few_bits unlimited_int::binary_search_divide(const unlimited_int& num_to_divide_by) const\"";
#endif
#if DEBUG_MODE > 0
	if ((this->find_inconsistencies()) || (num_to_divide_by.find_inconsistencies()))
		throw std::logic_error("The inconsistency was found in function \"few_bits unlimited_int::binary_search_divide(const unlimited_int& num_to_divide_by) const\"");
#endif
	few_bits min = (few_bits)0, max = (few_bits)MAX_few_bits_NUM;
	const char result_compare = unlimited_int::compare_multiplication_to_num(num_to_divide_by, min, *this);
	if (result_compare == 'E')
		return min;
	//Variable shadowing isn't permitted in the same level scope.
	const char result_compare2 = unlimited_int::compare_multiplication_to_num(num_to_divide_by, max, *this);
	if (result_compare2 == 'E' || result_compare2 == 'S')
		return max;
	while (true)
	{
		const few_bits average = (few_bits)(((many_bits)min + (many_bits)max) / (many_bits)2);
		//Variable shadowing. This is permitted because it's in a more inner scope.
		const char result_compare = unlimited_int::compare_multiplication_to_num(num_to_divide_by, average, *this);
		if (result_compare == 'E')
			return average;
		if (result_compare == 'L')
			max = average;
		else
			min = average;
		if (max - min <= (few_bits)1)
			return min;
	}
	return (few_bits)0;
}
//This function only produces the correct result when the difference length between the two unlimited_ints is 1 or 0. Length being the the number of few_bits used.
few_bits unlimited_int::binary_search_divide(const few_bits num_to_divide_by) const
{
	static_assert(sizeof(few_bits) * 2 == sizeof(many_bits), "Assertion error: NUM_OF_BITS_many_bits must have exactly twice the number of bits as NUM_OF_BITS_few_bits");
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"few_bits unlimited_int::binary_search_divide(const few_bits num_to_divide_by) const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in function \"few_bits unlimited_int::binary_search_divide(const few_bits num_to_divide_by) const\"");
#endif
	const many_bits divisor = (many_bits)num_to_divide_by;
	many_bits min = (many_bits)0, max = (many_bits)MAX_few_bits_NUM;
	const char result_compare = this->compare_to_ignore_sign(min * divisor);
	if (result_compare == 'E')
		return (few_bits)min;
	//Variable shadowing isn't permitted in the same level scope.
	const char result_compare2 = this->compare_to_ignore_sign(max * divisor);
	if (result_compare2 == 'E' || result_compare2 == 'L')
		return (few_bits)max;
	while (true)
	{
		const many_bits average = (min + max) / (many_bits)2;
		//Variable shadowing. This is permitted because it's in a more inner scope.
		const char result_compare = this->compare_to_ignore_sign(average * divisor);
		if (result_compare == 'E')
			return (few_bits)average;
		if (result_compare == 'S')
			max = average;
		else
			min = average;
		if (max - min <= (many_bits)1)
			return (few_bits)min;
	}
	return (few_bits)0;
}
//starts the multiplication from the right, and compares while multiplying. That way this function is closer to O(1) than to O(n)
char unlimited_int::compare_multiplication_to_num(const unlimited_int& multiplicand, const few_bits multiplier, const unlimited_int& result_target)
{
	static_assert(sizeof(few_bits) * 2 == sizeof(many_bits), "Assertion error: NUM_OF_BITS_many_bits must have exactly twice the number of bits as NUM_OF_BITS_few_bits");
	if (multiplicand.is_zero() || (multiplier == (few_bits)0))
		return (unlimited_int().compare_to_ignore_sign(result_target));
	else if (result_target.num_of_used_ints < multiplicand.num_of_used_ints)
		return 'L';
	//Difference of more than two. The multiplication result can only be 1 few_bits longer than the length of mulitplicand
	else if (result_target.num_of_used_ints >= multiplicand.num_of_used_ints + (size_t)2)
		return 'S';
	//create copy of multiplicand to store the result of the multiplication. Normally we'll reach a verdict of the comparison much before we're finished calculating the entire result of the multiplication.
	unlimited_int result; //using result just as a temporary holder. Won't be in a usable state when the function exits. Will still flush successfully.
	result.increase_until_num_of_ints(multiplicand.num_of_used_ints + (size_t)1);
	custom_linked_list_node<int_array>* current_int_array_Node_multiplicand = multiplicand.get_most_significant_used_int_array();
	custom_linked_list_node<int_array>* current_int_array_Node_result_target = result_target.get_most_significant_used_int_array();
	custom_linked_list_node<int_array>* current_int_array_Node_result = result.intarrays->last();
	int_array current_int_array_multiplicand = *current_int_array_Node_multiplicand->value;
	int_array current_int_array_result_target = *current_int_array_Node_result_target->value;
	int_array current_int_array_result = *current_int_array_Node_result->value;
	size_t index_multiplicand = current_int_array_multiplicand.num_of_used_ints - (size_t)1;
	size_t index_result_target = current_int_array_result_target.num_of_used_ints - (size_t)1;
	size_t index_result = current_int_array_result.intarr_len - (size_t)1;
	size_t int_counter = (size_t)0;
	size_t num_of_ints_in_result_that_have_been_compared = (size_t)0; //can't retroactively change even with the most extreme carry
	bool firsts_were_compared = false;
	few_bits first_in_result_target = (few_bits)0;
	if (result_target.num_of_used_ints == multiplicand.num_of_used_ints + (size_t)1)
	{
		first_in_result_target = current_int_array_result_target.intarr[index_result_target];
		if (index_result_target-- == (size_t)0)
		{
			current_int_array_Node_result_target = current_int_array_Node_result_target->previous;
			current_int_array_result_target = *current_int_array_Node_result_target->value;
			index_result_target = current_int_array_result_target.num_of_used_ints - (size_t)1;
		}
	}
	few_bits* first_in_result = current_int_array_result.intarr + index_result;
	*first_in_result = (few_bits)0;
	if (index_result-- == (size_t)0)
	{
		current_int_array_Node_result = current_int_array_Node_result->previous;
		current_int_array_result = *current_int_array_Node_result->value;
		index_result = current_int_array_result.intarr_len - (size_t)1;
	}
	size_t stop_at_for_multiplicand = current_int_array_multiplicand.num_of_used_ints;
	size_t stop_at_for_result_target = index_result_target + (size_t)1;
	size_t stop_at_for_result = index_result + (size_t)1;
	size_t stop_at = stop_at_for_multiplicand;
	if (stop_at_for_result_target < stop_at)
		stop_at = stop_at_for_result_target;
	if (stop_at_for_result < stop_at)
		stop_at = stop_at_for_result;
	const size_t stop_all = multiplicand.num_of_used_ints;
	const many_bits multiplier_many_bits = (many_bits)multiplier;
	custom_linked_list_node<int_array>* compare_from_int_array_Node_result_target = current_int_array_Node_result_target;
	custom_linked_list_node<int_array>* compare_from_int_array_Node_result = current_int_array_Node_result;
	int_array compare_from_int_array_result_target = current_int_array_result_target;
	int_array compare_from_int_array_result = current_int_array_result;
	size_t compare_from_index_result_target = index_result_target;
	size_t compare_from_index_result = index_result;
	const custom_linked_list_node<int_array> *const result_target_intarrays_end = result_target.intarrays->end();
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
				index_multiplicand = (size_t)current_int_array_multiplicand.num_of_used_ints - (size_t)1;
			}
			if (int_counter >= stop_at_for_result_target)
			{
				current_int_array_Node_result_target = current_int_array_Node_result_target->previous;
				current_int_array_result_target = *current_int_array_Node_result_target->value;
				stop_at_for_result_target = int_counter + current_int_array_result_target.num_of_used_ints;
				index_result_target = (size_t)current_int_array_result_target.num_of_used_ints - (size_t)1;
			}
			if (int_counter >= stop_at_for_result)
			{
				current_int_array_Node_result = current_int_array_Node_result->previous;
				current_int_array_result = *current_int_array_Node_result->value;
				stop_at_for_result = int_counter + current_int_array_result.intarr_len;
				index_result = (size_t)current_int_array_result.intarr_len - (size_t)1;
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
		const size_t num_of_ints_filled_in_result = int_counter + (size_t)2;
		const size_t stop_all_reverse = num_of_ints_filled_in_result - (size_t)1;
		size_t int_counter_reverse = (size_t)0;
		size_t index_result_reverse = index_result + (size_t)1;
		custom_linked_list_node<int_array>* current_int_array_Node_result_reverse = current_int_array_Node_result;
		int_array current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
		size_t stop_at_for_result_reverse = current_int_array_result_reverse.intarr_len - index_result_reverse;
		while (carry > (few_bits)0)
		{
			if (int_counter_reverse >= stop_at_for_result_reverse)
			{
				if (int_counter_reverse >= stop_all_reverse)
					break;
				if (index_result_reverse >= current_int_array_result_reverse.intarr_len)
				{
					index_result_reverse = (size_t)0;
					current_int_array_Node_result_reverse = current_int_array_Node_result_reverse->next;
					current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
					stop_at_for_result_reverse = int_counter_reverse + current_int_array_result_reverse.intarr_len;
				}
				continue;
			}
			const many_bits result_addition = (many_bits)current_int_array_result_reverse.intarr[index_result_reverse] + (many_bits)carry;
			const few_bits addition_remainder_reverse = (few_bits)result_addition;
			carry = (few_bits)(result_addition >> NUM_OF_BITS_few_bits);
			current_int_array_result_reverse.intarr[index_result_reverse] = addition_remainder_reverse;
			++int_counter_reverse;
			++index_result_reverse;
		}
		const size_t difference_between_current_and_already_certain = num_of_ints_filled_in_result - num_of_ints_in_result_that_have_been_compared;
		few_bits max_hypothetical_carry;
		bool at_last = false;
		if (int_counter >= stop_all - (size_t)1)
		{
			max_hypothetical_carry = (few_bits)0; //there can't be any carry if it's the end
			at_last = true;
		}
		else
			max_hypothetical_carry = (few_bits)MAX_few_bits_NUM;
		//Now iterating in reverse for the purpose of finding certainties
		int_counter_reverse = (size_t)0;
		index_result_reverse = (size_t)index_result;
		current_int_array_Node_result_reverse = current_int_array_Node_result;
		current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
		stop_at_for_result_reverse = current_int_array_result_reverse.intarr_len - index_result_reverse;
		size_t index_result_target_reverse = index_result_target;
		custom_linked_list_node<int_array>* current_int_array_Node_result_target_reverse = current_int_array_Node_result_target;
		int_array current_int_array_result_target_reverse = *current_int_array_Node_result_target_reverse->value;
		size_t stop_at_for_result_target_reverse = current_int_array_result_target_reverse.num_of_used_ints - index_result_target_reverse;
		size_t stop_at_reverse = stop_at_for_result_reverse;
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
					index_result_reverse = (size_t)0;
					current_int_array_Node_result_reverse = current_int_array_Node_result_reverse->next;
					current_int_array_result_reverse = *current_int_array_Node_result_reverse->value;
					stop_at_for_result_reverse = int_counter_reverse + current_int_array_result_reverse.intarr_len;
				}
				if (int_counter_reverse >= stop_at_for_result_target_reverse)
				{
					index_result_target_reverse = (size_t)0;
					current_int_array_Node_result_target_reverse = current_int_array_Node_result_target_reverse->next; //ignore the "dereferencing null pointer" warning
					if (current_int_array_Node_result_target_reverse != result_target_intarrays_end)
					{
						current_int_array_result_target_reverse = *current_int_array_Node_result_target_reverse->value;
						stop_at_for_result_target_reverse = int_counter_reverse + current_int_array_result_target_reverse.num_of_used_ints;
					}
				}
				stop_at_reverse = stop_at_for_result_reverse;
				if (current_int_array_Node_result_target_reverse != result_target_intarrays_end)
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
			size_t num_of_new_comparisons_to_make;
			if (at_last)
				num_of_new_comparisons_to_make = difference_between_current_and_already_certain;
			else
				num_of_new_comparisons_to_make = difference_between_current_and_already_certain - (int_counter_reverse + (size_t)1);
			num_of_ints_in_result_that_have_been_compared += num_of_new_comparisons_to_make;
			if (num_of_new_comparisons_to_make > (size_t)0)
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
				if (num_of_new_comparisons_to_make > (size_t)0)
				{
					//go through from compare_from_int_array_Node_result_target and from compare_from_int_array_Node_result all the way down num_of_new_comparisons_to_make times.
					custom_linked_list_node<int_array>* comparing_int_array_Node_result_target = compare_from_int_array_Node_result_target;
					int_array comparing_int_array_result_target = compare_from_int_array_result_target;
					size_t comparing_index_result_target = compare_from_index_result_target;
					custom_linked_list_node<int_array>* comparing_int_array_Node_result = compare_from_int_array_Node_result;
					int_array comparing_int_array_result = compare_from_int_array_result;
					size_t comparing_index_result = compare_from_index_result;
					//adjusting starting points of next comparison iteration
					compare_from_int_array_Node_result_target = current_int_array_Node_result_target_reverse;
					compare_from_int_array_result_target = current_int_array_result_target_reverse;
					compare_from_index_result_target = index_result_target_reverse;
					compare_from_int_array_Node_result = current_int_array_Node_result_reverse;
					compare_from_int_array_result = current_int_array_result_reverse;
					compare_from_index_result = index_result_reverse;
					const size_t stop_all_compare = num_of_new_comparisons_to_make;
					size_t stop_compare_for_result_target = (size_t)comparing_index_result_target + (size_t)1;
					size_t stop_compare_for_result = (size_t)comparing_index_result + (size_t)1;
					size_t int_counter_compare = (size_t)0;
					size_t stop_at_compare = stop_compare_for_result_target;
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
								comparing_index_result_target = (size_t)comparing_int_array_result_target.num_of_used_ints - (size_t)1;
								stop_compare_for_result_target = int_counter_compare + comparing_int_array_result_target.num_of_used_ints;
							}
							if (int_counter_compare >= stop_compare_for_result)
							{
								comparing_int_array_Node_result = comparing_int_array_Node_result->previous;
								comparing_int_array_result = *comparing_int_array_Node_result->value;
								comparing_index_result = (size_t)comparing_int_array_result.intarr_len - (size_t)1;
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
