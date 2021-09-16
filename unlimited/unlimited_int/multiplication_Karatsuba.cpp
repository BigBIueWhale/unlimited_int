#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//Python example code to show how Karatsuba multiplication works:
/*
import math
def size_base10(num):
	return len(str(num))
def split_at(num, length_of_low):
	str_num = str(num)
	low_str = str_num[-length_of_low:]
	high_str = str_num[:len(str_num) - length_of_low]
	return int(high_str), int(low_str)
def karatsuba(num1, num2):
	if (num1 < 10) or (num2 < 10):
		return (num1 * num2)

	#Calculates the size of the numbers.
	m = min(size_base10(num1), size_base10(num2))
	m2 = math.floor(m / 2)
	#m2 = ceil(m / 2) will also work

	#Split the digit sequences in the middle.
	high1, low1 = split_at(num1, m2)
	high2, low2 = split_at(num2, m2)

	#3 calls made to numbers approximately half the size.
	z0 = karatsuba(low1, low2)
	z1 = karatsuba((low1 + high1), (low2 + high2))
	z2 = karatsuba(high1, high2)

	return (z2 * (10 ** (m2 * 2))) + ((z1 - z2 - z0) * (10 ** m2)) + z0
*/
#define MIN_KARATSUBA 80
unlimited_int unlimited_int::multiply_karatsuba(const unlimited_int* num_to_mult) const
{
	const size_t num_to_mult_num_of_used_ints = num_to_mult->num_of_used_ints;
	const size_t this_num_of_used_ints = this->num_of_used_ints;
	if (num_to_mult_num_of_used_ints <= (size_t)MIN_KARATSUBA || this_num_of_used_ints <= (size_t)MIN_KARATSUBA)
	{
		unlimited_int answer;
		this->multiply_basecase(num_to_mult, &answer);
		return answer;
	}
	size_t m2;
	if (num_to_mult_num_of_used_ints < this_num_of_used_ints)
		m2 = num_to_mult_num_of_used_ints / (size_t)2;
	else
		m2 = this_num_of_used_ints / (size_t)2;
	unlimited_int high1, low1, high2, low2;
	this->split_at(m2, &high1, &low1);
	num_to_mult->split_at(m2, &high2, &low2);
	unlimited_int z0 = low1.multiply_karatsuba(&low2);
	//The next two commands will be saving space by reusing "low1" and "low2"
	low1.add(&high1, &low1); //now low1 = low1 + high1
	low2.add(&high2, &low2); //now low2 = low2 + high2
	//the following command destroys (sacrifices) low1 and low2
	unlimited_int z1 = low1.multiply_karatsuba_destroy_this_and_num_to_mult(&low2);
	//the following command destroys (sacrifices) high1 and high2
	unlimited_int z2 = high1.multiply_karatsuba_destroy_this_and_num_to_mult(&high2);
	//the next lines will basically do this: return (((z2 * (10 * *(m2 * 2))) + ((z1 - z2 - z0) * (10 * *m2)) + z0) // multiplied_by)
	z1.subtract(&z2, &z1);
	z1.subtract(&z0, &z1); //z1 will become z1 - z2 - z0
	z1.shift_left(m2);
	z2.shift_left(m2 * (size_t)2);
	z2.add(&z1, &z2);
	z1.flush();
	z2.add(&z0, &z2); //z2 now holds the final answer to return
	z0.flush();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in recursive function \"multiply karatuba\"";
#endif
#if DEBUG_MODE > 0
	if (z2.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"unlimited_int* unlimited_int::multiply_karatsuba(const unlimited_int* num_to_mult) const\"");
#endif
	return z2;
}
unlimited_int unlimited_int::multiply_karatsuba_destroy_this_and_num_to_mult(unlimited_int* num_to_mult)
{
	const size_t num_to_mult_num_of_used_ints = num_to_mult->num_of_used_ints;
	const size_t this_num_of_used_ints = this->num_of_used_ints;
	if (num_to_mult_num_of_used_ints <= (size_t)MIN_KARATSUBA || this_num_of_used_ints <= (size_t)MIN_KARATSUBA)
	{
		unlimited_int answer;
		this->multiply_basecase(num_to_mult, &answer);
		this->flush();
		num_to_mult->flush();
		return answer;
	}
	size_t m2;
	if (num_to_mult_num_of_used_ints < this_num_of_used_ints)
		m2 = num_to_mult_num_of_used_ints / (size_t)2;
	else
		m2 = this_num_of_used_ints / (size_t)2;
	unlimited_int high1, low1, high2, low2;
	this->split_at_and_use_original(m2, &high1, &low1);
	num_to_mult->split_at_and_use_original(m2, &high2, &low2);
	unlimited_int z0 = low1.multiply_karatsuba(&low2);
	//The next two commands will be saving space by reusing "low1" and "low2"
	low1.add(&high1, &low1); //now low1 = low1 + high1
	low2.add(&high2, &low2); //now low2 = low2 + high2
	//the following command destroys (sacrifices) low1 and low2
	unlimited_int z1 = low1.multiply_karatsuba_destroy_this_and_num_to_mult(&low2);
	//the following command destroys (sacrifices) high1 and high2
	unlimited_int z2 = high1.multiply_karatsuba_destroy_this_and_num_to_mult(&high2);
	//the next lines will basically do this: return (((z2 * (10 * *(m2 * 2))) + ((z1 - z2 - z0) * (10 * *m2)) + z0) // multiplied_by)
	z1.subtract(&z2, &z1);
	z1.subtract(&z0, &z1); //z1 will become z1 - z2 - z0
	z1.shift_left(m2);
	z2.shift_left(m2 * (size_t)2);
	z2.add(&z1, &z2);
	z1.flush();
	z2.add(&z0, &z2); //z2 now holds the final answer to return
	z0.flush();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in recursive function \"multiply karatuba\"";
#endif
#if DEBUG_MODE > 0
	if (z2.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"unlimited_int* unlimited_int::multiply_karatsuba_destroy_this_and_num_to_mult(unlimited_int* num_to_mult)\"");
#endif
	return z2;
}
unlimited_int unlimited_int::multiply_karatsuba_destroy_this(const unlimited_int* num_to_mult)
{
	const size_t num_to_mult_num_of_used_ints = num_to_mult->num_of_used_ints;
	const size_t this_num_of_used_ints = this->num_of_used_ints;
	if (num_to_mult_num_of_used_ints <= (size_t)MIN_KARATSUBA || this_num_of_used_ints <= (size_t)MIN_KARATSUBA)
	{
		unlimited_int answer;
		this->multiply_basecase(num_to_mult, &answer); //need to create a special function of power2_basecase
		this->flush();
		return answer;
	}
	size_t m2;
	if (num_to_mult_num_of_used_ints < this_num_of_used_ints)
		m2 = num_to_mult_num_of_used_ints / (size_t)2;
	else
		m2 = this_num_of_used_ints / (size_t)2;
	unlimited_int high1, low1, high2, low2;
	num_to_mult->split_at(m2, &high2, &low2);
	this->split_at_and_use_original(m2, &high1, &low1);
	unlimited_int z0 = low1.multiply_karatsuba(&low2);
	//The next two commands will be saving space by reusing "low1" and "low2"
	low1.add(&high1, &low1); //now low1 = low1 + high1
	low2.add(&high2, &low2); //now low2 = low2 + high2
	//the following command destroys (sacrifices) low1 and low2
	unlimited_int z1 = low1.multiply_karatsuba_destroy_this_and_num_to_mult(&low2);
	//the following command destroys (sacrifices) high1 and high2
	unlimited_int z2 = high1.multiply_karatsuba_destroy_this_and_num_to_mult(&high2);
	//the next lines will basically do this: return (((z2 * (10 * *(m2 * 2))) + ((z1 - z2 - z0) * (10 * *m2)) + z0) // multiplied_by)
	z1.subtract(&z2, &z1);
	z1.subtract(&z0, &z1); //z1 will become z1 - z2 - z0
	z1.shift_left(m2);
	z2.shift_left(m2 * (size_t)2);
	z2.add(&z1, &z2);
	z1.flush();
	z2.add(&z0, &z2); //z2 now holds the final answer to return
	z0.flush();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in recursive function \"multiply karatuba\"";
#endif
#if DEBUG_MODE > 0
	if (z2.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"unlimited_int* unlimited_int::multiply_karatsuba_destroy_this(const unlimited_int* num_to_mult)\"");
#endif
	return z2;
}
#define MIN_KARATSUBA_SQUARING 80
//More efficient Karatsuba multiplication for the special case of multiplying a number by itself.
unlimited_int unlimited_int::power2() const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of recursive function \"power2\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if (this->num_of_used_ints <= (size_t)MIN_KARATSUBA_SQUARING)
	{
		unlimited_int answer;
		this->multiply_basecase(this, &answer);
		return answer;
	}
	unlimited_int high, low;
	const size_t m2 = this->num_of_used_ints / (size_t)2;
	this->split_at(m2, &high, &low);
	unlimited_int square_of_high = high.power2();
	unlimited_int square_of_low = low.power2();
	square_of_high.shift_left(m2 * (size_t)2);
	unlimited_int answer = square_of_high + square_of_low;
	square_of_high.flush();
	square_of_low.flush();
	unlimited_int high_times_low = high.multiply_karatsuba_destroy_this_and_num_to_mult(&low);
	high_times_low <<= (size_t)1 + m2 * (size_t)NUM_OF_BITS_few_bits;
	answer += high_times_low;
	high_times_low.flush();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of recursive function \"power2\"";
#endif
#if DEBUG_MODE > 0
	if (answer.find_inconsistencies() || this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"unlimited_int* unlimited_int::power2() const\"");
#endif
	return answer;
}
unlimited_int unlimited_int::power2_destroy_this()
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of recursive function \"power2_destroy_this\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"unlimited_int* unlimited_int::power2_destroy_this()\"");
#endif
	if (this->num_of_used_ints <= (size_t)MIN_KARATSUBA_SQUARING)
	{
		unlimited_int answer;
		this->multiply_basecase(this, &answer);
		this->flush();
		return answer;
	}
	unlimited_int high, low;
	const size_t m2 = this->num_of_used_ints / (size_t)2;
	this->split_at_and_use_original(m2, &high, &low);
	unlimited_int square_of_high = high.power2();
	unlimited_int square_of_low = low.power2();
	square_of_high.shift_left(m2 * (size_t)2);
	unlimited_int answer = square_of_high + square_of_low;
	square_of_high.flush();
	square_of_low.flush();
	unlimited_int high_times_low = high.multiply_karatsuba_destroy_this_and_num_to_mult(&low);
	high_times_low <<= (size_t)1 + m2 * (size_t)NUM_OF_BITS_few_bits;
	answer += high_times_low;
	high_times_low.flush();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of recursive function \"power2_destroy_this\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || answer.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"unlimited_int* unlimited_int::power2_destroy_this()\"");
#endif
	return answer;
}
unlimited_int unlimited_int::operator*(const unlimited_int& other) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int* unlimited_int::operator*(const unlimited_int& other) const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || other.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function \"unlimited_int* unlimited_int::operator*(const unlimited_int& other) const\"");
#endif
	unlimited_int answer;
	unlimited_int this_positive(*this, false);
	this_positive.self_abs();
	unlimited_int other_positive(other, false);
	other_positive.self_abs();
	const bool this_fits_in_few_bits = this_positive.fits_in_few_bits();
	const bool other_fits_in_few_bits = other_positive.fits_in_few_bits();
	if (this_fits_in_few_bits || other_fits_in_few_bits)
	{
		few_bits this_as_few_bits = MAX_few_bits_NUM; //Initializing to turn off compiler warning.
		few_bits other_as_few_bits = MAX_few_bits_NUM; //Initializing to turn off compiler warning.
		if (this_fits_in_few_bits)
			this_as_few_bits = this_positive.to_few_bits();
		if (other_fits_in_few_bits)
			other_as_few_bits = other_positive.to_few_bits();
		if (this_fits_in_few_bits && other_fits_in_few_bits)
		{
			static_assert(sizeof(many_bits) >= sizeof(few_bits) * 2, "Assuming that many_bits is at least twice as large as few_bits");
			answer = static_cast<many_bits>(this_as_few_bits) * static_cast<many_bits>(other_as_few_bits);
		}
		else if (this_fits_in_few_bits)
			other_positive.multiply(this_as_few_bits, &answer);
		else if (other_fits_in_few_bits)
			this_positive.multiply(other_as_few_bits, &answer);
		answer.self_abs();
		if (this->is_negative() != other.is_negative())
			answer.self_negative();
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"unlimited_int* unlimited_int::operator*(const unlimited_int& other) const\"";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || other.find_inconsistencies() || answer.find_inconsistencies())
			throw std::logic_error("The inconsistency was found in end of function \"unlimited_int* unlimited_int::operator*(const unlimited_int& other) const\"");
#endif
		return answer;
	}
	bool other_is_a_power_of_2, this_is_a_power_of_2;
	//it's more efficient when either other or this is a power of 2 because then bit shifting can be used instead of the Karatsuba multiplication algorithm.
	const size_t other_log2 = other.find_exact_log_2(&other_is_a_power_of_2);
	const size_t this_log2 = this->find_exact_log_2(&this_is_a_power_of_2);
	if (other_is_a_power_of_2)
		answer = this_positive << other_log2;
	else if (this_is_a_power_of_2)
		answer = other_positive << this_log2;
	else //neither numbers are powers of 2.
	{
		bool both_multiplicants_are_same = false;
		if (this == &other) //pointers are pointing to the same object
			both_multiplicants_are_same = true;
		else if (*this == other) //two copies of the same number
			both_multiplicants_are_same = true;
		if (both_multiplicants_are_same)
			answer = this->power2();
		else
			answer = this->multiply_karatsuba(&other);
	}
	if (this->num_of_used_ints == (size_t)0)
		answer._is_negative = false;
	else if (this->_is_negative != other._is_negative)
		answer._is_negative = true;
	else
		answer._is_negative = false;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int* unlimited_int::operator*(const unlimited_int& other) const\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || other.find_inconsistencies() || answer.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function \"unlimited_int* unlimited_int::operator*(const unlimited_int& other) const\"");
#endif
	return answer;
}
