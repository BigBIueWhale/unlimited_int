#include "unlimited_int.hpp"
using namespace unlimited;
namespace
{
	unsigned char_to_number_for_from_string(const char ch, const unsigned base)
	{
		if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')))
			throw std::out_of_range(std::string("Error in function \"from_string\". Invalid character: \'") + ch + "\'." + " with ASCII value of: " + std::to_string(ch));
		if ((base < 2) || (base > 36))
			throw std::out_of_range(std::string("Error in function \"from_string\" Invalid Argument!\nbase is out of range \"2 <= base <= 36\". Base: ") + std::to_string(base));
		int value;
		const int ch_int = static_cast<int>(ch);
		if (ch_int >= static_cast<int>('0') && ch_int <= static_cast<int>('9'))
			value = ch_int - static_cast<int>('0');
		else if (ch_int >= static_cast<int>('A') && ch_int <= static_cast<int>('Z'))
			value = ch_int - static_cast<int>('A') + 10;
		else
			value = ch_int - static_cast<int>('a') + 10;
		if (static_cast<unsigned>(value) >= base || value < 0)
			throw std::invalid_argument(std::string("Error in function \"from_string\" Invalid Argument!\nthe number of char ch is not a digit in the specified base. The character: ") + ch + " ASCII number: " + std::to_string(ch) + " The base: " + std::to_string(base));
		return value;
	}
	unlimited_int from_string_range(const char* str, const size_t len, const unsigned int base)
	{
		if (base < 2 || base > 36)
			throw std::out_of_range("Error in function \"from_string\" Invalid Argument!\nbase is out of range \"2 <= base <= 36\"");
		if (str == nullptr)
			throw std::invalid_argument("Can\'t convert null string to unlimited_int");
		if (len == (size_t)0)
			throw std::invalid_argument("Can\'t convert empty string to unlimited_int");
		bool set_is_negative_to_true = false;
		size_t first_digit_index = (size_t)0;
		if (str[first_digit_index] == '-')
		{
			set_is_negative_to_true = true;
			++first_digit_index;
			if (first_digit_index == len)
				throw std::invalid_argument("Can\'t convert \"-\" to unlimited_int, no digits after minus sign");
		}
		while (first_digit_index < len && str[first_digit_index] == '0')
			++first_digit_index;
		if (first_digit_index == len)
			return unlimited_int();
		unlimited_int base_ui((few_bits)base);
		size_t index = len - (size_t)1;
		unlimited_int multiplicand((few_bits)1);
		unlimited_int answer;
		while (true)
		{
			unsigned value = char_to_number_for_from_string(str[index], base);
			constexpr unsigned MAX_BASE = 36U;
			static_assert(MAX_BASE > 0U, "Base 0 doesn\'t exist.");
			if (value > MAX_BASE - 1U) //until base 36 (including)
				throw std::invalid_argument("Error in function \"unlimited_int::from_string\": Invalid char in the character sequence");
			{
				static_assert(MAX_few_bits_NUM >= MAX_BASE - 1U, "few_bits isn\'t big enough to hold a single digit in the maximum supported base, MAX_BASE");
				answer += multiplicand * unlimited_int(static_cast<few_bits>(value));
			}
			multiplicand *= base_ui;
			if (index-- == first_digit_index)
				break;
		}
		if (set_is_negative_to_true)
			answer.self_negative();
		return answer;
	}
}
unsigned unlimited_int::char_to_number(const char ch, const unsigned base)
{
	if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')))
		throw std::out_of_range(std::string("Error in function \"char_to_number(char ch, int base)\". Invalid character: \'") + ch + "\'." + " with ASCII value of: " + std::to_string(ch));
	if ((base < 2) || (base > 36))
		throw std::out_of_range(std::string("Error in function \"char_to_number(char ch, int base)\" Invalid Argument!\nbase is out of range \"2 <= base <= 36\". Base: ") + std::to_string(base));
	int value;
	const int ch_int = static_cast<int>(ch);
	if (ch_int >= static_cast<int>('0') && ch_int <= static_cast<int>('9')) //is a number
		value = ch_int - static_cast<int>('0');
	else if (ch_int >= static_cast<int>('A') && ch_int <= static_cast<int>('Z')) //is an upper-case letter
		value = ch_int - static_cast<int>('A') + 10;
	else //is a lower-case letter
		value = ch_int - static_cast<int>('a') + 10;
	if (static_cast<unsigned>(value) >= base || value < 0)
		throw std::invalid_argument(std::string("Error in function \"char_to_number(char ch, int base)\" Invalid Argument!\nthe number of char ch is not a digit in the specified base. The character: ") + ch + " ASCII number: " + std::to_string(ch) + " The base: " + std::to_string(base));
	return value;
}
unlimited_int unlimited_int::from_string(const char* str, const unsigned int base)
{
	if (str == nullptr)
		throw std::invalid_argument("Can\'t convert null string to unlimited_int");
	size_t counter = (size_t)0;
	while (str[counter] != '\0')
		++counter;
	return from_string_range(str, counter, base);
}
unlimited_int unlimited_int::from_string(const std::string& str, const unsigned int base)
{
	return from_string_range(str.data(), str.size(), base);
}
