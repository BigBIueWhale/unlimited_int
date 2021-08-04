#include "unlimited_int.hpp"
using namespace unlimited;
#define AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL 0
//Like binary search but faster. Also the reciprocal can be saved for later to greatly improve the speed of recurrent division.
//tau[i + 1] = tau[i] * (2 - N * tau[i])
reciprocal_information unlimited_int::calculate_reciprocal_ceiling(many_bits length_dividend_to_support) const
{
	if (this->is_zero())
		throw "Error can\'t compute reciprocal of 0 because can\'t divide by 0";
	if (length_dividend_to_support < this->num_of_used_ints)
		return reciprocal_information(std::shared_ptr<unlimited_int>(new unlimited_int((few_bits)1)), this->num_of_used_ints);
	const many_bits amount_to_shift_by = length_dividend_to_support + (many_bits)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL;
	unlimited_int two_shifted((few_bits)2);
	two_shifted.shift_left(amount_to_shift_by);
	std::shared_ptr<unlimited_int> approx(new unlimited_int((few_bits)1));
	approx->shift_left(amount_to_shift_by - this->num_of_used_ints);
	std::shared_ptr<unlimited_int> previous_approx;
	unlimited_int N(*this, false);
	N.is_negative = false;
	unlimited_int approx_times_N;
	do
	{
		approx_times_N = *approx * N;
		unlimited_int difference(two_shifted - approx_times_N);
		previous_approx = approx;
		approx = difference * *approx;
		approx->shift_right(amount_to_shift_by);
	} while (*approx != *previous_approx);
	unlimited_int numerator((few_bits)1);
	numerator.shift_left(amount_to_shift_by);
	if (approx_times_N != numerator)
	{
		++(*approx);
		if (*(approx_times_N + N) < numerator)
			++(*approx);
	}
	//if (*(approx_times_N + N) <= numerator)
	//	++(*approx);
	return reciprocal_information(approx, amount_to_shift_by);
}
std::shared_ptr<unlimited_int> unlimited_int::divide_using_reciprocal(const unlimited_int& dividend, const reciprocal_information reciprocal, const unlimited_int& divisor, unlimited_int* remainder)
{
	if (reciprocal.reciprocal->is_negative)
		throw "Error in function divide using reciprocal! The reciprocal is negative";
	if (reciprocal.amount_shifted < (dividend.num_of_used_ints + (many_bits)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL))
		throw "Error while trying to divide using reciprocal: the reciprocal doesn\'t have enough accuracy relative to the given dividend";
	const many_bits amount_to_reduce_reciprocal = reciprocal.amount_shifted - (dividend.num_of_used_ints + (many_bits)AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL);
	//const many_bits amount_to_reduce_reciprocal = 0;
	unlimited_int reciprocal_shifted(*reciprocal.reciprocal, false);
	if (amount_to_reduce_reciprocal > 0)
	{
		reciprocal_shifted.forget_memory();
		reciprocal_shifted = *reciprocal.reciprocal;
		reciprocal_shifted.shift_right(amount_to_reduce_reciprocal);
	}
	const many_bits amount_to_shift_answer = reciprocal.amount_shifted - amount_to_reduce_reciprocal;
	std::shared_ptr<unlimited_int> answer(dividend * reciprocal_shifted);
	answer->shift_right(amount_to_shift_answer);
	answer->is_negative = false;
	unlimited_int divisor_positive(divisor, false);
	divisor_positive.is_negative = false;
	unlimited_int result_multiplication(*answer * divisor_positive);
	unlimited_int result_multiplication_error_by_1(result_multiplication + divisor_positive);
	const char comparison_result = result_multiplication_error_by_1.compare_to_ignore_sign(dividend);
	if (comparison_result != 'L')
	{
		++(*answer); //fix error by 1
		//std::cout << "\nFixed error by 1";
	}
	answer->is_negative = dividend.is_negative != divisor.is_negative;
	if (remainder != nullptr)
	{
		if (comparison_result != 'L')
			*remainder = dividend - result_multiplication_error_by_1;
		else
			*remainder = dividend - result_multiplication;
		remainder->is_negative = dividend.is_negative; //because this is remainder of division, not modulo
	}
	return answer;
}
#define MAX_NUM_OF_DIVISORS_TO_KEEP_TRACK_OF 20
std::shared_ptr<unlimited_int> unlimited_int::recurring_division(const unlimited_int& dividend, const unlimited_int& divisor, unlimited_int* remainder)
{
	if (divisor.is_zero())
		throw "Error in function \"std::shared_ptr<unlimited_int> unlimited_int::recurring_division(const unlimited_int& dividend, const unlimited_int& divisor)\" can\'t divide by zero.";
	if (dividend.is_zero())
	{
		if (remainder != nullptr)
			*remainder = (few_bits)0;
		return std::shared_ptr<unlimited_int>(new unlimited_int);
	}
	many_bits_signed log2_divisor = divisor.find_exact_log_2();
	if (log2_divisor >= (many_bits_signed)0)
	{
		std::shared_ptr<unlimited_int> result = dividend >> (many_bits)log2_divisor;
		result->is_negative = dividend.is_negative != divisor.is_negative;
		if (remainder != nullptr)
		{
			unlimited_int result_multiplication(*result * divisor);
			*remainder = dividend - result_multiplication;
		}
		return result;
	}
	char result_comparison = dividend.compare_to_ignore_sign(divisor);
	if (result_comparison == 'E')
	{
		std::shared_ptr<unlimited_int> result(new unlimited_int((few_bits)1));
		result->is_negative = dividend.is_negative != divisor.is_negative;
		if (remainder != nullptr)
			*remainder = (few_bits)0;
		return result;
	}
	else if (result_comparison == 'S')
	{
		if (remainder != nullptr)
			*remainder = dividend;
		return std::shared_ptr<unlimited_int>(new unlimited_int);
	}
	const size_t fingerprint_divisor = divisor.fingerprint();
	bool need_to_calculate_reciprocal = true;
	bool key_exists_in_list = false;
	custom_Node<size_t>* item_in_list = nullptr;
	auto reciprocal_iterator_in_map = unlimited_int::Newton_Raphson_lookup.reciprocals_map.find(fingerprint_divisor);
	if (reciprocal_iterator_in_map != unlimited_int::Newton_Raphson_lookup.reciprocals_map.end()) //if the key already exists
	{
		need_to_calculate_reciprocal = false;
		key_exists_in_list = true;
		reciprocal_information_for_database reciprocal(reciprocal_iterator_in_map->second);
		item_in_list = reciprocal.link_to_list;
		custom_Node<size_t>* const next_in_the_list = unlimited_int::Newton_Raphson_lookup.most_recent.detach(item_in_list);
		//advance the "importance" of the current fingerprint by moving it 1 item closer to the head of the list
		unlimited_int::Newton_Raphson_lookup.most_recent.insert(next_in_the_list, item_in_list);
		const bool hashes_match = *dividend.calculate_efficient_cryptographic_hash() == *reciprocal.hash_of_dividend;
		if ((reciprocal.amount_shifted < (dividend.num_of_used_ints + AMOUNT_OF_EXTRA_PRECISION_FOR_RECIPROCAL)) //reciprocal exists but doesn't have enough precision
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
				unlimited_int::Newton_Raphson_lookup.reciprocals_map.erase(*unlimited_int::Newton_Raphson_lookup.most_recent.end()->previous->value);
				unlimited_int::Newton_Raphson_lookup.most_recent.pop_back();
			}
			unlimited_int::Newton_Raphson_lookup.most_recent.push_back(new size_t(fingerprint_divisor));
			item_in_list = unlimited_int::Newton_Raphson_lookup.most_recent.end()->previous;
		}
		unlimited_int::Newton_Raphson_lookup.reciprocals_map[fingerprint_divisor] = reciprocal_information_for_database(divisor.calculate_reciprocal_ceiling((dividend.num_of_used_ints * (many_bits)2) + (many_bits)4), divisor.calculate_efficient_cryptographic_hash(), item_in_list);
		reciprocal_iterator_in_map = unlimited_int::Newton_Raphson_lookup.reciprocals_map.find(fingerprint_divisor);
	}
	return unlimited_int::divide_using_reciprocal(dividend, (reciprocal_information)reciprocal_iterator_in_map->second, divisor, remainder);
}
std::shared_ptr<unlimited_int> unlimited_int::remainder_recurring_divison(const unlimited_int& dividend, const unlimited_int& divisor)
{
	unlimited_int* result = new unlimited_int;
	unlimited_int::recurring_division(dividend, divisor, result); //the division result is purposefully lost here
	return std::shared_ptr<unlimited_int>(result);
}
