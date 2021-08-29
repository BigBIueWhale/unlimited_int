#include "unlimited_int.hpp"
using namespace unlimited;
char unlimited_int::number_to_char(const int num, const int base = 10)
{
	if (num >= base || num < 0)
		throw std::out_of_range("\nError in function \"number_to_char\" num out of range.");
	if (base <= 0 || base > 36)
		throw std::out_of_range("\nError in function \"number_to_char\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\"");
	if (num <= 9)
		return num + 48;
	return num + 87;
}
std::string unlimited_int::to_string(const unsigned int base) const
{
	char* c_str_received = this->to_c_string(base);
	std::string str_to_return(c_str_received);
	delete[] c_str_received;
	return str_to_return;
}
char* unlimited_int::to_c_string(const unsigned int base) const
{
	if (this->num_of_used_ints == (size_t)0)
	{
		char* char_to_return = new char[2];
		char_to_return[0] = '0';
		char_to_return[1] = '\0';
		return char_to_return;
	}
	const size_t size_of_string = this->num_of_used_ints * (size_t)NUM_OF_BITS_few_bits;
	char* string_base = new char[size_of_string];
	size_t counter = (size_t)0;
	unlimited_int numerator(std::shared_ptr<unlimited_int>(this->copy()));
	unlimited_int base_str((many_bits)base);
	numerator.is_negative = false;
	unlimited_int zero;
	while (numerator > zero && counter < size_of_string)
	{
		unlimited_int current_digit_ui; //The remainder
		numerator = numerator.recurring_division(numerator, base_str, &current_digit_ui);
		string_base[counter] = unlimited_int::number_to_char((int)current_digit_ui.get_least_significant(), base);
		++counter;
	}
	//by now we have the digits of string_base backwards
	char* final_str;
	char* final_str_original;
	if (this->is_negative)
	{
		final_str_original = new char[counter + (size_t)2];
		final_str_original[0] = '-';
		final_str = final_str_original + (size_t)1;
	}
	else
	{
		final_str_original = new char[counter + (size_t)1];
		final_str = final_str_original;
	}
	for (size_t counter2 = (size_t)0, counter3 = counter - (size_t)1; counter2 < counter; ++counter2, --counter3)
		final_str[counter2] = string_base[counter3];
	delete[] string_base;
	final_str[counter] = '\0';
	return final_str_original;
}