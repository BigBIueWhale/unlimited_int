#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
std::shared_ptr<unlimited_int> unlimited_int::operator/(const unlimited_int& denominator) const
{
	if (denominator.num_of_used_ints == 0) { throw "\nError in function: \"unlimited_int* unlimited_int::operator/(const unlimited_int& denominator) const\" Can't divide by zero"; }
	std::shared_ptr<unlimited_int> answer = this->divide_by(denominator);
	if (answer->num_of_used_ints == 0) { answer->is_negative = false; }
	else if (this->is_negative != denominator.is_negative) { answer->is_negative = true; }
	return answer;
}
std::shared_ptr<unlimited_int> unlimited_int::operator*(const few_bits num) const
{
	unlimited_int* answer = new unlimited_int;
	this->multiply(num, answer);
	return std::shared_ptr<unlimited_int>(answer);
}
std::shared_ptr<unlimited_int> unlimited::operator*(const few_bits num, const unlimited_int& ui)
{
	unlimited_int* answer = new unlimited_int;
	ui.multiply(num, answer);
	return std::shared_ptr<unlimited_int>(answer);
}
void unlimited_int::operator*=(const unlimited_int& other)
{
	//it's more efficient when either other or this is a power of 2 because then bit shifting can be used instead of the Karatsuba multiplication algorithm.
	many_bits_signed other_log2 = other.find_exact_log_2();
	many_bits_signed this_log2 = this->find_exact_log_2();
	if (other_log2 >= (many_bits_signed)0)
		(*this) <<= other_log2;
	else if (this_log2 >= (many_bits_signed)0)
		(*this) = other << this_log2;
	else //neither numbers are powers of 2.
		(*this) = std::shared_ptr<unlimited_int>(this->multiply_karatsuba_destroy_this(&other));
	if (this->num_of_used_ints == 0) { this->is_negative = false; }
	else if (this->is_negative != other.is_negative) { this->is_negative = true; }
}
std::shared_ptr<unlimited_int> unlimited_int::operator+(const unlimited_int& other) const
{
	unlimited_int* answer = new unlimited_int;
	this->add(&other, answer);
	return std::shared_ptr<unlimited_int>(answer);
}
std::shared_ptr<unlimited_int> unlimited_int::operator-(const unlimited_int& other) const
{
	unlimited_int* answer = new unlimited_int;
	this->subtract(&other, answer);
	return std::shared_ptr<unlimited_int>(answer);
}
void unlimited_int::operator++()
{
	unlimited_int one((few_bits)1);
	this->add(&one, this);
}
void unlimited_int::operator--()
{
	unlimited_int one((few_bits)1);
	this->subtract(&one, this);
}
std::shared_ptr<unlimited_int> unlimited_int::operator-() const
{
	if (this->is_zero())
		return std::shared_ptr<unlimited_int>(new unlimited_int(0));
	unlimited_int* cpy = this->copy();
	cpy->is_negative = !cpy->is_negative;
	return std::shared_ptr<unlimited_int>(cpy);
}
std::shared_ptr<unlimited_int> unlimited_int::abs() const
{
	if (this->is_zero())
		return std::shared_ptr<unlimited_int>(new unlimited_int(0));
	unlimited_int* cpy = this->copy();
	cpy->is_negative = false;
	return std::shared_ptr<unlimited_int>(cpy);
}
std::shared_ptr<unlimited_int> unlimited_int::operator%(const unlimited_int& ui) const
{
	std::shared_ptr<unlimited_int> answer_divide = this->divide_by(ui);
	std::shared_ptr<unlimited_int> answer_multiply = *answer_divide * ui;
	answer_divide.reset();
	unlimited_int* answer = new unlimited_int;
	this->subtract(answer_multiply.get(), answer);
	answer_multiply.reset();
	return std::shared_ptr<unlimited_int>(answer);
}
void unlimited_int::operator%=(const unlimited_int& ui)
{
	std::shared_ptr<unlimited_int> answer_divide = this->divide_by(ui);
	std::shared_ptr<unlimited_int> answer_multiply = *answer_divide * ui;
	answer_divide.reset();
	this->subtract(answer_multiply.get(), this);
}
std::ostream& unlimited::operator<<(std::ostream& os, const unlimited_int& ui)
{
	char* str_of_this;
	auto current_flags = os.flags();
	if (current_flags & std::ios::hex)
		str_of_this = ui.to_c_string(16);
	else if (current_flags & std::ios::dec)
		str_of_this = ui.to_c_string(10);
	else if (current_flags & std::ios::oct)
		str_of_this = ui.to_c_string(8);
	else
		str_of_this = ui.to_c_string(16);
	os << str_of_this;
	delete[]str_of_this;
	return os;
}
