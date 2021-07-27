#include "unlimited_int.hpp"
using namespace unlimited;
int unlimited_int::char_to_number(const char ch, const int base)
{
	if ((ch < 48) || ((ch > 57) && (ch < 65)) || ((ch > 90) && (ch < 97)) || (ch > 122))
		throw "\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nchar ch is a NON number or letter characters: ";
	if ((base <= 0) || (base > 36)) { throw "\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\""; }
	int value;
	if (ch <= 57) //is a number
	{
		value = ch - 48;
	}
	else if (ch <= 90) //is an upper-case letter
	{
		value = ch - 55;
	}
	else { value = ch - 87; } //is a lower-case letter
	if (value >= base)
		throw "\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nthe number of char ch is not a digit in the specified base.";
	return value;
}
std::shared_ptr<unlimited_int> unlimited_int::from_string(const char* str, const int base)
{
	bool set_is_negative_to_true = false;
	if (str[0] == '-')
	{
		set_is_negative_to_true = true;
		++str;
	}
	while (*str == '0')
		++str;
	unlimited_int* answer = new unlimited_int;
	if (*str == '\0')
	{
		answer->set_to_zero();
		return std::shared_ptr<unlimited_int>(answer);
	}
	if ((base <= 0) || (base > 36)) { throw "\nError in function \"from_c_string\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\""; }
	unlimited_int base_ui(base);
	const char* it_ch = str;
	many_bits_signed counter = 0;
	while (*it_ch != '\0') { ++it_ch; ++counter; }
	many_bits_signed index = counter - 1;
	unlimited_int multiplicand(1);
	while (index >= 0)
	{
		int value = unlimited_int::char_to_number(str[index], base);
		if (value == -1)
			throw "\nError in function \"unlimited_int::from_c_string(char*, const int base)\": Invalid char in the char array";
		unlimited_int temp_answer;
		multiplicand.multiply(value, &temp_answer);
		*answer += temp_answer;
		--index;
		multiplicand *= base_ui;
	}
	answer->is_negative = set_is_negative_to_true;
	return std::shared_ptr<unlimited_int>(answer);
}
