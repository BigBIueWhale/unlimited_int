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
		answer._is_negative = false;
	else if (this->_is_negative != denominator._is_negative)
		answer._is_negative = true;
	return answer;
}
unlimited_int unlimited_int::operator/(const few_bits divisor) const
{
	if (divisor == (few_bits)0)
		throw std::invalid_argument("\nError in function: \"unlimited_int* unlimited_int::operator/(const few_bits denominator) const\" Can't divide by zero");
	unlimited_int answer = this->divide_by(divisor);
	if (this->is_negative())
		answer.self_negative();
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
		this->_is_negative = false;
	else if (this->_is_negative != other._is_negative)
		this->_is_negative = true;
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
	unlimited_int cpy = *this;
	cpy.self_abs();
	return cpy;
}
unlimited_int unlimited_int::operator%(const unlimited_int& ui) const
{
	unlimited_int answer_divide = *this / ui;
	unlimited_int answer_multiply = answer_divide * ui;
	answer_divide.flush();
	unlimited_int answer;
	this->subtract(&answer_multiply, &answer);
	return answer;
}
unlimited_int unlimited_int::operator%(const few_bits divisor) const
{
	unlimited_int answer_divide = *this / divisor;
	unlimited_int answer_multiply;
	answer_divide.multiply(divisor, &answer_multiply);
	answer_divide.flush();
	unlimited_int answer;
	this->subtract(&answer_multiply, &answer);
	return answer;
}
void unlimited_int::operator%=(const unlimited_int& ui)
{
	unlimited_int answer_divide = *this / ui;
	unlimited_int answer_multiply = answer_divide * ui;
	answer_divide.flush();
	this->subtract(&answer_multiply, this);
}
void unlimited_int::operator%=(const few_bits divisor)
{
	unlimited_int answer_divide = *this / divisor;
	unlimited_int answer_multiply;
	answer_divide.multiply(divisor, &answer_multiply);
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

bool unlimited_int::operator<=(const signed short int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>=(const signed short int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator<(const signed short int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>(const signed short int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator==(const signed short int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator!=(const signed short int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<=(const signed short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>=(const signed short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<(const signed short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>(const signed short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator==(const signed short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator!=(const signed short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}

bool unlimited_int::operator<=(const signed int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>=(const signed int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator<(const signed int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>(const signed int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator==(const signed int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator!=(const signed int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<=(const signed int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>=(const signed int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<(const signed int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>(const signed int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator==(const signed int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator!=(const signed int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}

bool unlimited_int::operator<=(const signed long int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>=(const signed long int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator<(const signed long int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>(const signed long int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator==(const signed long int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator!=(const signed long int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<=(const signed long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>=(const signed long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<(const signed long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>(const signed long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator==(const signed long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator!=(const signed long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}

bool unlimited_int::operator<=(const signed long long int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>=(const signed long long int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator<(const signed long long int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator>(const signed long long int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator==(const signed long long int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited_int::operator!=(const signed long long int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(this->compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<=(const signed long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>=(const signed long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator<(const signed long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator>(const signed long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator==(const signed long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}
bool unlimited::operator!=(const signed long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot compare \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		return fits(ui.compare_to(static_cast<few_bits_signed>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits_signed>(num)));
}

bool unlimited_int::operator<=(const unsigned short int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>=(const unsigned short int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator<(const unsigned short int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>(const unsigned short int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator==(const unsigned short int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator!=(const unsigned short int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<=(const unsigned short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>=(const unsigned short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<(const unsigned short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>(const unsigned short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator==(const unsigned short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator!=(const unsigned short int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}

bool unlimited_int::operator<=(const unsigned int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>=(const unsigned int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator<(const unsigned int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>(const unsigned int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator==(const unsigned int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator!=(const unsigned int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<=(const unsigned int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>=(const unsigned int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<(const unsigned int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>(const unsigned int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator==(const unsigned int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator!=(const unsigned int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}

bool unlimited_int::operator<=(const unsigned long int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>=(const unsigned long int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator<(const unsigned long int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>(const unsigned long int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator==(const unsigned long int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator!=(const unsigned long int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<=(const unsigned long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>=(const unsigned long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<(const unsigned long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>(const unsigned long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator==(const unsigned long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator!=(const unsigned long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}

bool unlimited_int::operator<=(const unsigned long long int num) const
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>=(const unsigned long long int num) const
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator<(const unsigned long long int num) const
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator>(const unsigned long long int num) const
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator==(const unsigned long long int num) const
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited_int::operator!=(const unsigned long long int num) const
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(this->compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(this->compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<=(const unsigned long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L' || ch == 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>=(const unsigned long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S' || ch == 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator<(const unsigned long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'L'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator>(const unsigned long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'S'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator==(const unsigned long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch == 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
bool unlimited::operator!=(const unsigned long long int num, const unlimited_int& ui)
{
	auto fits = [](const char ch) { return ch != 'E'; };
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_few_bits) || fits_in_many_bits,
		"Assertion error: Cannot compare \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		return fits(ui.compare_to(static_cast<few_bits>(num)));
	else //If only fits_in_many_bits_signed
		return fits(ui.compare_to(static_cast<many_bits>(num)));
}
