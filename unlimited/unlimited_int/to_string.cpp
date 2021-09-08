#include "unlimited_int.hpp"
#include <algorithm> //For std::reverse
using namespace unlimited;
char unlimited_int::number_to_char(const int num, const int base = 10)
{
	if (num >= base || num < 0)
		throw std::out_of_range("\nError in function \"number_to_char\" num out of range.");
	if (base <= 0 || base > 36)
		throw std::out_of_range("\nError in function \"number_to_char\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\"");
	if (num <= 9)
		return static_cast<char>(num + 48);
	return static_cast<char>(num + 87);
}
std::string unlimited_int::to_string(const unsigned int base) const
{
	if (this->num_of_used_ints == (size_t)0)
	{
		std::string string_to_return(2, '\0');
		string_to_return[0] = '0';
		string_to_return[1] = '\0';
		return string_to_return;
	}
	std::string string_intermediate;
	unlimited_int numerator = *this;
	numerator.self_abs();
	unlimited_int base_str((many_bits)base);
	unlimited_int zero;
	while (!numerator.is_zero())
	{
		unlimited_int current_digit_ui; //The remainder
		//Based on my test: when printing 512 bit numbers in base 10, recurring division is about 30% faster than using the function of dividing by few_bits.
		numerator = numerator.recurring_division(numerator, base_str, &current_digit_ui);
		string_intermediate.push_back(unlimited_int::number_to_char((int)current_digit_ui.get_least_significant(), base));
	}
	//by now we have the digits of the result backwards, in string_intermediate, and without minus symbol.
	std::reverse(string_intermediate.begin(), string_intermediate.end());
	//Now we're only missing the minus symbol
	if (this->is_negative)
		return '-' + string_intermediate;
	return string_intermediate;
}
