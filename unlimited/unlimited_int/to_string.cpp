#include "unlimited_int.hpp"
using namespace unlimited;
char unlimited_int::number_to_char(const int num, const int base = 10)
{
	if ((num >= base) || (num < 0)) { throw "\nError in function \"number_to_char\" num out of range."; }
	if ((base <= 0) || (base > 36)) { throw "\nError in function \"number_to_char\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\""; }
	if (num <= 9) { return (num + 48); }
	return (num + 87);
}
std::string unlimited_int::to_string(const int base) const
{
	char* c_str_received = this->to_c_string(base);
	std::string str_to_return(c_str_received);
	delete[]c_str_received;
	return str_to_return;
}
char* unlimited_int::to_c_string(const int base) const
{
	if (this->num_of_used_ints == 0)
	{
		char* char_to_return = new char[2];
		char_to_return[0] = '0';
		char_to_return[1] = '\0';
		return char_to_return;
	}
	const many_bits_signed size_of_string = (this->num_of_used_ints) * (many_bits)NUM_OF_BITS_few_bits;
	char* string_base = new char[size_of_string];
	many_bits_signed counter = 0;
	std::shared_ptr<unlimited_int> numerator(this->copy());
	unlimited_int base_str = unlimited_int((many_bits_signed)base);
	numerator->is_negative = false;
	while ((*numerator > unlimited_int(0)) && (counter < size_of_string))
	{
		std::shared_ptr<unlimited_int> current_digit_ui = (*numerator % base_str);
		string_base[counter] = unlimited_int::number_to_char((int)current_digit_ui->get_least_significant(), base);
		*numerator /= base_str;
		++counter;
	}
	//by now we have the digits of string_base backwards
	char* final_str;
	char* final_str_original;
	if (this->is_negative)
	{
		final_str_original = new char[counter + 2ULL];
		final_str_original[0] = '-';
		final_str = final_str_original + 1;
	}
	else
	{
		final_str_original = new char[counter + 1ULL];
		final_str = final_str_original;
	}
	for (many_bits_signed counter2 = 0, counter3 = counter - 1; counter2 < counter; ++counter2, --counter3)
	{
		final_str[counter2] = string_base[counter3];
	}
	delete[]string_base;
	final_str[counter] = '\0';
	return final_str_original;
}