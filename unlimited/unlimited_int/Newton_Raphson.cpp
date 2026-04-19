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
	//Starting value for the Newton Raphson iteration below. The target value that the iteration converges to is T = floor(B^k / N), where B = 2^NUM_OF_BITS_few_bits, N = *this, k = amount_to_shift_by, and d = num_of_used_ints. T sits somewhere in the range (B^(k-d), B^(k-d+1)]. The simplest starting value is just approx = B^(k-d), which is at the bottom of that range. When N's top int is small, T sits near the top of the range and so the simple starting value is up to a factor of B below T. Each Newton Raphson step only roughly doubles approx until approx gets close to T, so that factor-of-B gap can cost up to log2(B) extra iterations (that's 32 iterations on 32-bit platforms, 64 on 64-bit) before the iteration starts converging quickly. Computing the starting value from N's top int closes most of that gap for the cost of one hardware many_bits/few_bits divide, and preserves the property approx <= T that the rest of this function counts on.
	//Why approx <= T still holds: let top be the most-significant used int of *this. Since N has d ints with top int equal to top, N is strictly less than (top + 1) * B^(d-1), so T is strictly greater than B^(k-d+1)/(top + 1). Taking q = floor(B/(top + 1)) from the hardware divide, we have q * (top + 1) <= B, so q * B^(k-d) <= B^(k-d+1)/(top + 1) < B^k/N. Since q * B^(k-d) is a non-negative whole number strictly less than B^k/N, it is at most floor(B^k/N) = T. So approx <= T, exactly the property that the old starting value approx = B^(k-d) also had.
	//Why the new starting value is never worse than the old one: when top = B - 1 the expression floor(B/(top + 1)) evaluates to exactly 1 and we get back the old starting value. For every smaller top the expression is at least 1 and is at least 2 once top + 1 <= B/2, so the new starting value is never below the old one and is strictly greater for any divisor whose top int has its top bit unset.
	custom_linked_list_node<int_array>* approx_top_node = this->get_most_significant_used_int_array();
	const few_bits approx_top_int = approx_top_node->value->intarr[approx_top_node->value->num_of_used_ints - (size_t)1];
	const many_bits B_as_many_bits = ((many_bits)1) << (many_bits)NUM_OF_BITS_few_bits;
	const few_bits approx_leading_coefficient = (few_bits)(B_as_many_bits / ((many_bits)approx_top_int + (many_bits)1));
	unlimited_int approx(approx_leading_coefficient);
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
		unlimited_int::Newton_Raphson_lookup.most_recent.detach(item_in_list);
		//promote the importance of the current fingerprint by moving it to the head of the list (most recently used)
		unlimited_int::Newton_Raphson_lookup.most_recent.push_front(item_in_list);
		const bool is_different_divisor = divisor.calculate_efficient_cryptographic_hash() != *reciprocal.hash_of_dividend;
		if ((reciprocal.reciprocal_info.amount_shifted < (dividend.num_of_used_ints + (size_t)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL)) //reciprocal exists but doesn't have enough precision
			|| is_different_divisor) //or the fingerprint was matching just by chance, a hash collision (that's why we're checking with a cryptographic hash as well)
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
			if (unlimited_int::Newton_Raphson_lookup.most_recent.size() >= (size_t)MAX_NUM_OF_DIVISORS_TO_KEEP_TRACK_OF)
			{
				unlimited_int::Newton_Raphson_lookup.reciprocals_map.erase(*unlimited_int::Newton_Raphson_lookup.most_recent.last()->value);
				unlimited_int::Newton_Raphson_lookup.most_recent.pop_back();
			}
			item_in_list = unlimited_int::Newton_Raphson_lookup.most_recent.push_front(new size_t(fingerprint_divisor));
		}
		try
		{
			reciprocal_iterator_in_map = unlimited_int::Newton_Raphson_lookup.reciprocals_map.emplace(fingerprint_divisor, reciprocal_information_for_database(divisor.calculate_reciprocal_floor(dividend.num_of_used_ints), std::make_unique<unlimited_int>(divisor.calculate_efficient_cryptographic_hash()), item_in_list)).first;
		}
		catch (...)
		{
			//Emplace failed. Remove the LRU node unconditionally to keep the list
			//and map consistent. In the !key_exists_in_list case, the node was just
			//created by push_front above. In the key_exists_in_list case, the old
			//map entry was already erased, so leaving the promoted node
			//in the list would create an orphan that degrades cache effectiveness.
			unlimited_int::Newton_Raphson_lookup.most_recent.erase(item_in_list);
			throw;
		}
	}
	return unlimited_int::divide_using_reciprocal(dividend, reciprocal_iterator_in_map->second.reciprocal_info, divisor, remainder);
}
unlimited_int unlimited_int::remainder_recurring_divison(const unlimited_int& dividend, const unlimited_int& divisor)
{
	unlimited_int result;
	unlimited_int::recurring_division(dividend, divisor, &result); //the division result is purposefully lost here
	return result;
}
