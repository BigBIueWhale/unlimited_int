#include "unlimited_int.hpp"
using namespace unlimited;
int unlimited_int::char_to_number(const char ch, const int base)
{
	if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')))
		throw std::out_of_range(std::string("\nError in function \"char_to_number(char ch, int base)\". Invalid character: \'") + ch + "\'." + " with ASCII value of: " + std::to_string(ch));
	if ((base <= 0) || (base > 36))
		throw std::out_of_range(std::string("\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\". Base: ") + std::to_string(base));
	int value;
	if (ch >= '0' && ch <= '9') //is a number
		value = ch - '0';
	else if (ch >= 'A' && ch <= 'Z') //is an upper-case letter
		value = ch - 'A' + 10;
	else //is a lower-case letter
		value = ch - 'a' + 10;
	if (value >= base || value < 0)
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
		int value = unlimited_int::char_to_number(str[index], base);
		if (value < 0 || value > 35) //until base 36 (including)
			throw std::invalid_argument("\nError in function \"unlimited_int::from_c_string(char*, const int base)\": Invalid char in the char array");
		answer += multiplicand * (few_bits)value;
		multiplicand *= base_ui;
		if (index-- == (size_t)0)
			break;
	}
	answer.is_negative = set_is_negative_to_true;
	return answer;
}
