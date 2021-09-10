#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
#define MAXIMUM_NUMERATOR_SIZE_FOR_WHICH_TO_USE_REPEATED_ADDITION 16
unlimited_int unlimited_int::operator/(const unlimited_int& denominator) const
{
	if (denominator.num_of_used_ints == (size_t)0)
		throw std::invalid_argument("\nError in function: \"unlimited_int* unlimited_int::operator/(const unlimited_int& denominator) const\" Can't divide by zero");
	unlimited_int answer;
	if (this->num_of_used_ints <= (size_t)MAXIMUM_NUMERATOR_SIZE_FOR_WHICH_TO_USE_REPEATED_ADDITION)
		answer = this->divide_by_repeated_addition(denominator);
	else
		answer = this->divide_by(denominator);
	if (answer.num_of_used_ints == (size_t)0)
		answer.is_negative = false;
	else if (this->is_negative != denominator.is_negative)
		answer.is_negative = true;
	return answer;
}
unlimited_int unlimited_int::operator/(const few_bits divisor) const
{
	if (divisor == (few_bits)0)
		throw std::invalid_argument("\nError in function: \"unlimited_int* unlimited_int::operator/(const few_bits denominator) const\" Can't divide by zero");
	unlimited_int answer = this->divide_by(divisor);
	answer.is_negative = this->is_negative;
	if (answer.is_zero())
		answer.is_negative = false;
	return answer;
}
unlimited_int unlimited_int::operator*(const few_bits num) const
{
	unlimited_int answer;
	this->multiply(num, &answer);
	return answer;
}
unlimited_int unlimited::operator*(const few_bits num, const unlimited_int& ui)
{
	unlimited_int answer;
	ui.multiply(num, &answer);
	return answer;
}
void unlimited_int::operator*=(const unlimited_int& other)
{
	bool other_is_a_power_of_2, this_is_a_power_of_2;
	//it's more efficient when either other or this is a power of 2 because then bit shifting can be used instead of the Karatsuba multiplication algorithm.
	const size_t other_log2 = other.find_exact_log_2(&other_is_a_power_of_2);
	const size_t this_log2 = this->find_exact_log_2(&this_is_a_power_of_2);
	if (other_is_a_power_of_2)
		*this <<= other_log2;
	else if (this_is_a_power_of_2)
		*this = other << this_log2;
	else //neither numbers are powers of 2.
		*this = this->multiply_karatsuba_destroy_this(&other);
	if (this->num_of_used_ints == (size_t)0)
		this->is_negative = false;
	else if (this->is_negative != other.is_negative)
		this->is_negative = true;
}
unlimited_int unlimited_int::operator+(const unlimited_int& other) const
{
	unlimited_int answer;
	this->add(&other, &answer);
	return answer;
}
unlimited_int unlimited_int::operator-(const unlimited_int& other) const
{
	unlimited_int answer;
	this->subtract(&other, &answer);
	return answer;
}
void unlimited_int::operator++()
{
	//Can't do it in one line because the standard says that getting a pointer of an rvalue is undefined behaviour.
	const unlimited_int one((few_bits)1);
	this->add(&one, this);
}
void unlimited_int::operator--()
{
	//Can't do it in one line because the standard says that getting a pointer of an rvalue is undefined behaviour.
	const unlimited_int one((few_bits)1);
	this->subtract(&one, this);
}
unlimited_int unlimited_int::operator-() const
{
	if (this->is_zero())
		return unlimited_int();
	unlimited_int cpy = *this;
	cpy.flip_sign();
	return cpy;
}
unlimited_int unlimited_int::abs() const
{
	if (this->is_zero())
		return unlimited_int();
	unlimited_int cpy = *this;
	cpy.self_abs();
	return cpy;
}
unlimited_int unlimited_int::operator%(const unlimited_int& ui) const
{
	unlimited_int answer_divide = this->divide_by(ui);
	unlimited_int answer_multiply = answer_divide * ui;
	answer_divide.flush();
	unlimited_int answer;
	this->subtract(&answer_multiply, &answer);
	return answer;
}
unlimited_int unlimited_int::operator%(const few_bits divisor) const
{
	unlimited_int answer_divide = this->divide_by(divisor);
	unlimited_int answer_multiply = answer_divide * divisor;
	answer_divide.flush();
	unlimited_int answer;
	this->subtract(&answer_multiply, &answer);
	return answer;
}
void unlimited_int::operator%=(const unlimited_int& ui)
{
	unlimited_int answer_divide = this->divide_by(ui);
	unlimited_int answer_multiply = answer_divide * ui;
	answer_divide.flush();
	this->subtract(&answer_multiply, this);
}
void unlimited_int::operator%=(const few_bits divisor)
{
	unlimited_int answer_divide = this->divide_by(divisor);
	unlimited_int answer_multiply = answer_divide * divisor;
	answer_divide.flush();
	this->subtract(&answer_multiply, this);
}
std::ostream& unlimited::operator<<(std::ostream& os, const unlimited_int& ui)
{
	std::string str_of_this;
	const auto current_flags = os.flags();
	if (current_flags & std::ios::hex)
		str_of_this = ui.to_string(16);
	else if (current_flags & std::ios::dec)
		str_of_this = ui.to_string(10);
	else if (current_flags & std::ios::oct)
		str_of_this = ui.to_string(8);
	else
		str_of_this = ui.to_string(16);
	os << str_of_this;
	return os;
}
bool unlimited::operator<(const few_bits num, const unlimited_int& other) { return other.compare_to(num) == 'L'; }
bool unlimited::operator<(const many_bits num, const unlimited_int& other) { return other.compare_to(num) == 'L'; }
bool unlimited::operator<(const few_bits_signed num, const unlimited_int& other) { return other.compare_to(num) == 'L'; }
bool unlimited::operator<(const many_bits_signed num, const unlimited_int& other) { return other.compare_to(num) == 'L'; }
bool unlimited::operator>(const few_bits num, const unlimited_int& other) { return other.compare_to(num) == 'S'; }
bool unlimited::operator>(const many_bits num, const unlimited_int& other) { return other.compare_to(num) == 'S'; }
bool unlimited::operator>(const few_bits_signed num, const unlimited_int& other) { return other.compare_to(num) == 'S'; }
bool unlimited::operator>(const many_bits_signed num, const unlimited_int& other) { return other.compare_to(num) == 'S'; }
bool unlimited::operator<=(const few_bits num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'L' || result == 'E'; }
bool unlimited::operator<=(const many_bits num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'L' || result == 'E'; }
bool unlimited::operator<=(const few_bits_signed num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'L' || result == 'E'; }
bool unlimited::operator<=(const many_bits_signed num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'L' || result == 'E'; }
bool unlimited::operator>=(const few_bits num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'S' || result == 'E'; }
bool unlimited::operator>=(const many_bits num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'S' || result == 'E'; }
bool unlimited::operator>=(const few_bits_signed num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'S' || result == 'E'; }
bool unlimited::operator>=(const many_bits_signed num, const unlimited_int& other) { const char result = other.compare_to(num); return result == 'S' || result == 'E'; }
bool unlimited::operator==(const few_bits num, const unlimited_int& other) { return other.compare_to(num) == 'E'; }
bool unlimited::operator==(const many_bits num, const unlimited_int& other) { return other.compare_to(num) == 'E'; }
bool unlimited::operator==(const few_bits_signed num, const unlimited_int& other) { return other.compare_to(num) == 'E'; }
bool unlimited::operator==(const many_bits_signed num, const unlimited_int& other) { return other.compare_to(num) == 'E'; }
bool unlimited::operator!=(const few_bits num, const unlimited_int& other) { return other.compare_to(num) != 'E'; }
bool unlimited::operator!=(const many_bits num, const unlimited_int& other) { return other.compare_to(num) != 'E'; }
bool unlimited::operator!=(const few_bits_signed num, const unlimited_int& other) { return other.compare_to(num) != 'E'; }
bool unlimited::operator!=(const many_bits_signed num, const unlimited_int& other) { return other.compare_to(num) != 'E'; }
