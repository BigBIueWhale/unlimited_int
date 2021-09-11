#include "unlimited_int.hpp"
using namespace unlimited;
unsigned unlimited_int::char_to_number(const char ch, const unsigned base)
{
	if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')))
		throw std::out_of_range(std::string("\nError in function \"char_to_number(char ch, int base)\". Invalid character: \'") + ch + "\'." + " with ASCII value of: " + std::to_string(ch));
	if ((base <= 0) || (base > 36))
		throw std::out_of_range(std::string("\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\". Base: ") + std::to_string(base));
	int value;
	const int ch_int = static_cast<int>(ch);
	if (ch_int >= static_cast<int>('0') && ch_int <= static_cast<int>('9')) //is a number
		value = ch_int - static_cast<int>('0');
	else if (ch_int >= static_cast<int>('A') && ch_int <= static_cast<int>('Z')) //is an upper-case letter
		value = ch_int - static_cast<int>('A') + 10;
	else //is a lower-case letter
		value = ch_int - static_cast<int>('a') + 10;
	if (static_cast<unsigned>(value) >= base || value < 0)
		throw std::invalid_argument(std::string("\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nthe number of char ch is not a digit in the specified base. The character: ") + ch + " ASCII number: " + std::to_string(ch) + " The base: " + std::to_string(base));
	return value;
}
unlimited_int unlimited_int::from_string(const char* str, const unsigned int base)
{
	if (base <= 0 || base > 36)
		throw std::out_of_range("\nError in function \"from_c_string\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\"");
	if (*str == '\0')
		throw std::invalid_argument("Can\'t convert empty string to unlimited_int");
	bool set_is_negative_to_true = false;
	if (str[0] == '-')
	{
		set_is_negative_to_true = true;
		++str;
	}
	while (*str == '0')
		++str;
	if (*str == '\0')
		return unlimited_int();
	unlimited_int base_ui((few_bits)base);
	const char* it_ch = str;
	size_t counter = (size_t)0;
	while (*it_ch != '\0')
	{
		++it_ch;
		++counter;
	}
	size_t index = counter - (size_t)1;
	unlimited_int multiplicand((few_bits)1);
	unlimited_int answer;
	while (true)
	{
		unsigned value = unlimited_int::char_to_number(str[index], base);
		constexpr unsigned MAX_BASE = 36U;
		static_assert(MAX_BASE > 0U, "Base 0 doesn\'t exist.");
		if (value > MAX_BASE - 1U) //until base 36 (including)
			throw std::invalid_argument("\nError in function \"unlimited_int::from_c_string(char*, const int base)\": Invalid char in the char array");
		{
			unlimited_int result_multiplication_by_few_bits;
			static_assert(MAX_few_bits_NUM >= MAX_BASE - 1U, "few_bits isn\'t big enough to hold a single digit in the maximum supported base, MAX_BASE");
			multiplicand.multiply(static_cast<few_bits>(value), &result_multiplication_by_few_bits);
			answer += result_multiplication_by_few_bits;
		}
		multiplicand *= base_ui;
		if (index-- == (size_t)0)
			break;
	}
	answer._is_negative = set_is_negative_to_true;
	return answer;
}
