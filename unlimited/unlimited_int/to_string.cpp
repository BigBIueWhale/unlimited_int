#include "unlimited_int.hpp"
#include <algorithm> //For std::reverse
using namespace unlimited;
static constexpr unsigned int MAX_BASE_FOR_UNLIMITED_INT = 36U;
char unlimited_int::number_to_char(const unsigned int num, const unsigned int base = 10)
{
	if (num >= base)
		throw std::out_of_range("Error in function \"number_to_char\" num out of range.");
	if (base > MAX_BASE_FOR_UNLIMITED_INT || base <= 0U)
		throw std::out_of_range("Error in function \"number_to_char\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\"");
	if (num <= 9U)
		return static_cast<char>(num + static_cast<unsigned int>('0'));
	return static_cast<char>(num - 10U + static_cast<unsigned int>('A'));
}
std::string unlimited_int::to_string(const unsigned int base) const
{
	static_assert(std::numeric_limits<unsigned int>::max() >= MAX_BASE_FOR_UNLIMITED_INT, "MAX_BASE_FOR_UNLIMITED_INT should only be 36, because the numbers and english alphabet letters");
	if (base > MAX_BASE_FOR_UNLIMITED_INT || base <= 0U)
		throw std::invalid_argument("Error in function unlimited_int::to_string(unsigned): The maximum base is 36.");
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
	unlimited_int base_str(base);
	unlimited_int zero;
	while (!numerator.is_zero())
	{
		unlimited_int current_digit_ui; //The remainder
		//Based on my test: when printing 512 bit numbers in base 10, recurring division is about 30% faster than using the function of dividing by few_bits.
		numerator = numerator.recurring_division(numerator, base_str, &current_digit_ui);
		const few_bits least_sig_few_bits = current_digit_ui.get_least_significant_few_bits();
		if (least_sig_few_bits >= static_cast<unsigned int>(base))
			throw std::logic_error("Error in function unlimited_int::to_string(unsigned): Either recurring_division or get_least_significant_few_bits gave the wrong result. Failed sanity check, digit our of range for current base.");
		string_intermediate.push_back(unlimited_int::number_to_char(static_cast<unsigned int>(least_sig_few_bits), base));
	}
	//by now we have the digits of the result backwards, in string_intermediate, and without minus symbol.
	std::reverse(string_intermediate.begin(), string_intermediate.end());
	//Now we're only missing the minus symbol
	if (this->_is_negative)
		return '-' + string_intermediate;
	return string_intermediate;
}
