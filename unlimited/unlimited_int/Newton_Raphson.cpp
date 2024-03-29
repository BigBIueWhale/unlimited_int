#include "unlimited_int.hpp"
using namespace unlimited;
//Settings AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL to at least 1 is necessary to generate the correct result in calculate_reciprocal_floor when dealing with small numbers.
//The Newton Raphson iterations require that the initial conditions are precise enough, otherwise it will produce the wrong result (and in my implementation, throw an exception).
//That's why in the case of small numbers, we need AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL to be at least 1, to get that initial precision needed for the Newton Raphson iterations.
static constexpr size_t AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL = 1;
//Like binary search but faster. Also the reciprocal can be saved for later to greatly improve the speed of recurrent division.
//tau[i + 1] = tau[i] * (2 - N * tau[i])
reciprocal_information unlimited_int::calculate_reciprocal_floor(size_t length_dividend_to_support) const
{
	if (this->is_zero())
		throw std::invalid_argument("Error can\'t compute reciprocal of 0 because can\'t divide by 0");
	if (length_dividend_to_support < this->num_of_used_ints)
		return reciprocal_information(std::make_unique<unlimited_int>((few_bits)1), this->num_of_used_ints);
	const size_t amount_to_shift_by = length_dividend_to_support + (size_t)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL;
	unlimited_int two_shifted((few_bits)2);
	two_shifted.shift_left(amount_to_shift_by);
	unlimited_int approx((few_bits)1);
	approx.shift_left(amount_to_shift_by - this->num_of_used_ints);
	unlimited_int previous_approx;
	unlimited_int N(*this, false);
	N._is_negative = false;
	unlimited_int approx_times_N;
	do
	{
		approx_times_N = approx * N;
		const unlimited_int difference = two_shifted - approx_times_N;
		previous_approx = approx;
		approx *= difference;
		approx.shift_right(amount_to_shift_by);
	} while (approx != previous_approx);
//Correction of error by 1
	unlimited_int numerator((few_bits)1);
	numerator.shift_left(amount_to_shift_by);
	char result_comparison = approx_times_N.compare_to(numerator); //They're both non-negative to I don't need to use "compare_to_ignore_sign"
	if (result_comparison == 'S')
	{
		while (result_comparison == 'S')
		{
			approx_times_N += N;
			result_comparison = approx_times_N.compare_to(numerator);
			if (result_comparison != 'L')
				++approx;
		}
	}
	else if (result_comparison == 'L')
		throw std::logic_error("Error in function \"calculate_reciprocal_ceiling\". The reciprocal result given by Newton Raphson is certainly wrong (too big)");
	return reciprocal_information(std::make_unique<unlimited_int>(std::move(approx)), amount_to_shift_by);
}
unlimited_int unlimited_int::divide_using_reciprocal(const unlimited_int& dividend, const reciprocal_information& reciprocal, const unlimited_int& divisor, unlimited_int* remainder)
{
	if (reciprocal.reciprocal->_is_negative)
		throw std::invalid_argument("Error in function divide using reciprocal! The reciprocal is negative");
	if (reciprocal.amount_shifted < (dividend.num_of_used_ints + (size_t)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL))
		throw std::invalid_argument("Error while trying to divide using reciprocal: the reciprocal doesn\'t have enough accuracy relative to the given dividend");
	const size_t amount_to_reduce_reciprocal = reciprocal.amount_shifted - (dividend.num_of_used_ints + (size_t)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL);
	unlimited_int reciprocal_shifted(*reciprocal.reciprocal, false);
	//Only make an independent copy of reciprocal if it needs to be shifted
	if (amount_to_reduce_reciprocal > 0)
	{
		reciprocal_shifted.forget_memory();
		reciprocal_shifted = *reciprocal.reciprocal;
		reciprocal_shifted.shift_right(amount_to_reduce_reciprocal);
	}
	const size_t amount_to_shift_answer = reciprocal.amount_shifted - amount_to_reduce_reciprocal;
	unlimited_int answer = dividend * reciprocal_shifted;
	answer.self_abs();
	answer.shift_right(amount_to_shift_answer);
	//By now, the answer is either correct, or too small. That's because the reciprocal is floored, and shifting right also floors the answer, so it's definitely not too big.
	//Based on observasion, if the result is wrong, it's always too small by 1.
	//Even though I obseved that, I can't actually prove that the result can't be too small by 2 (for example).
	//But I do know the the result is not too big. So later in this function I'll use a while loop to increase the result by 1 until we get the correct result.
	//Based on observasion, that while loop will only run once, and it's just addition so it's not such a big deal.
	//It's not very elegant, but that way we guarantee a correct result, provably.
	unlimited_int divisor_positive(divisor, false);
	divisor_positive.self_abs();
	unlimited_int result_multiplication = answer * divisor_positive;
	const char result_multiplication_comparison_to_dividend = result_multiplication.compare_to_ignore_sign(dividend);
	if (result_multiplication_comparison_to_dividend == 'L')
		throw std::logic_error("Newton Raphson division error in function \"divide_using_reciprocal\". The result is certainly wrong (too big).");
	else if (result_multiplication_comparison_to_dividend == 'S')
	{
		unlimited_int result_multiplication_error_by_1 = result_multiplication + divisor_positive;
		char comparison_result = result_multiplication_error_by_1.compare_to_ignore_sign(dividend);
		while (comparison_result != 'L')
		{
			++answer; //fix error by 1 (the while loop is expected to only run once, but nonetheless it exists because I can't prove that the largest possible error is an error by 1)
			result_multiplication = result_multiplication_error_by_1;
			result_multiplication_error_by_1 += divisor_positive;
			comparison_result = result_multiplication_error_by_1.compare_to_ignore_sign(dividend);
		}
	}
	answer._is_negative = dividend._is_negative != divisor._is_negative;
	if (answer.is_zero())
		answer._is_negative = false;
	if (remainder != nullptr)
	{
		unlimited_int dividend_positive(dividend, false);
		dividend_positive.self_abs();
		*remainder = dividend_positive - result_multiplication;
		if (dividend._is_negative)
			remainder->self_negative(); //because this is remainder of division, not modulo
		else
			remainder->self_abs();
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	unlimited_int long_division_check = dividend / divisor;
	if (answer != long_division_check)
		throw std::logic_error("Error in function: divide_using_reciprocal. The result of division conflicts with the other division method.");
	if (remainder != nullptr)
	{
		unlimited_int check_remainder = dividend % divisor;
		if (*remainder != check_remainder)
			throw std::logic_error("Error in function: divide_using_reciprocal. The result of remainder conflicts with the other division method.");
	}
#endif
	return answer;
}
//You can change this to a higher (or lower) value. The higher the value, the more RAM it'll take.
//Setting to 1 should be enough for efficiently generating prime numbers.
static constexpr size_t MAX_NUM_OF_DIVISORS_TO_KEEP_TRACK_OF = 3;
unlimited_int unlimited_int::recurring_division(const unlimited_int& dividend, const unlimited_int& divisor, unlimited_int* remainder)
{
	if (divisor.is_zero())
		throw std::invalid_argument("Error in function \"std::shared_ptr<unlimited_int> unlimited_int::recurring_division(const unlimited_int& dividend, const unlimited_int& divisor)\" can\'t divide by zero.");
	if (dividend.is_zero())
	{
		if (remainder != nullptr)
			*remainder = (few_bits)0;
		return unlimited_int();
	}
	char result_comparison = dividend.compare_to_ignore_sign(divisor);
	if (result_comparison == 'E')
	{
		unlimited_int result((few_bits)1);
		result._is_negative = dividend._is_negative != divisor._is_negative;
		if (remainder != nullptr)
			*remainder = (few_bits)0;
		return result;
	}
	else if (result_comparison == 'S')
	{
		if (remainder != nullptr)
			*remainder = dividend;
		return unlimited_int();
	}
	bool divisor_is_a_power_of_2;
	const size_t log2_divisor = divisor.find_exact_log_2(&divisor_is_a_power_of_2);
	if (divisor_is_a_power_of_2)
	{
		unlimited_int dividend_positive(dividend, false);
		dividend_positive._is_negative = false;
		unlimited_int result = dividend_positive >> log2_divisor;
		result._is_negative = dividend._is_negative != divisor._is_negative;
		if (result.is_zero())
			result._is_negative = false;
		if (remainder != nullptr)
		{
			unlimited_int result_multiplication = result * divisor;
			*remainder = dividend - result_multiplication;
		}
		return result;
	}
	const size_t fingerprint_divisor = divisor.fingerprint();
	bool need_to_calculate_reciprocal = true;
	bool key_exists_in_list = false;
	custom_linked_list_node<size_t>* item_in_list = nullptr;
	auto reciprocal_iterator_in_map = unlimited_int::Newton_Raphson_lookup.reciprocals_map.find(fingerprint_divisor);
	if (reciprocal_iterator_in_map != unlimited_int::Newton_Raphson_lookup.reciprocals_map.end()) //if the key already exists
	{
		need_to_calculate_reciprocal = false;
		key_exists_in_list = true;
		reciprocal_information_for_database& reciprocal = reciprocal_iterator_in_map->second;
		item_in_list = reciprocal.link_to_list;
		custom_linked_list_node<size_t>* next_in_the_list = unlimited_int::Newton_Raphson_lookup.most_recent.detach(item_in_list);
		if (next_in_the_list != unlimited_int::Newton_Raphson_lookup.most_recent.end())
			next_in_the_list = next_in_the_list->next;
		//advance the "importance" of the current fingerprint by moving it 1 item closer to the head of the list
		unlimited_int::Newton_Raphson_lookup.most_recent.insert(next_in_the_list, item_in_list);
		const bool hashes_match = dividend.calculate_efficient_cryptographic_hash() == *reciprocal.hash_of_dividend;
		if ((reciprocal.amount_shifted < (dividend.num_of_used_ints + (size_t)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL)) //reciprocal exists but doesn't have enough precision
			|| hashes_match) //or the fingerprint was matching just by chance, a hash colision (that's why we're checking with a cryptographic hash as well)
		{
			need_to_calculate_reciprocal = true;
			unlimited_int::Newton_Raphson_lookup.reciprocals_map.erase(reciprocal_iterator_in_map);
		}
		else
			reciprocal_iterator_in_map->second.link_to_list = item_in_list; //update the list iterator in the map
	}
	if (need_to_calculate_reciprocal)
	{
		if (!key_exists_in_list)
		{
			if (unlimited_int::Newton_Raphson_lookup.most_recent.size() > (size_t)MAX_NUM_OF_DIVISORS_TO_KEEP_TRACK_OF)
			{
				unlimited_int::Newton_Raphson_lookup.reciprocals_map.erase(*unlimited_int::Newton_Raphson_lookup.most_recent.last()->value);
				unlimited_int::Newton_Raphson_lookup.most_recent.pop_back();
			}
			item_in_list = unlimited_int::Newton_Raphson_lookup.most_recent.push_front(new size_t(fingerprint_divisor));
		}
		unlimited_int::Newton_Raphson_lookup.reciprocals_map.emplace(fingerprint_divisor, reciprocal_information_for_database(divisor.calculate_reciprocal_floor(dividend.num_of_used_ints), std::make_unique<unlimited_int>(divisor.calculate_efficient_cryptographic_hash()), item_in_list));
		reciprocal_iterator_in_map = unlimited_int::Newton_Raphson_lookup.reciprocals_map.find(fingerprint_divisor);
	}
	return unlimited_int::divide_using_reciprocal(dividend, reciprocal_iterator_in_map->second, divisor, remainder);
}
unlimited_int unlimited_int::remainder_recurring_divison(const unlimited_int& dividend, const unlimited_int& divisor)
{
	unlimited_int result;
	unlimited_int::recurring_division(dividend, divisor, &result); //the division result is purposefully lost here
	return result;
}
