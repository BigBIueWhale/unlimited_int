#include "unlimited_int.hpp"
using namespace unlimited;
unlimited_int::operator few_bits()
{
	if (this->is_zero())
		return (few_bits)0;
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to few_bits because the unlimited_int object is too big.");
	if (this->is_negative)
		throw std::overflow_error("Invalid conversion from unlimited_int to few_bits because the unlimited_int object is negative.");
	return this->get_least_significant_few_bits();
}
unlimited_int::operator few_bits_signed()
{
	if (this->is_zero())
		return (few_bits)0;
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to few_bits_signed because the unlimited_int object is too big.");
	few_bits_signed this_as_few_bits_signed = static_cast<few_bits_signed>(this->get_least_significant_few_bits());
	if (this->is_negative)
		this_as_few_bits_signed = -this_as_few_bits_signed;
	if ((*this) != this_as_few_bits_signed)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits_signed because the unlimited_int object is too big (or too small) to fully fit inside many_bits_signed.");
	return this_as_few_bits_signed;
}
unlimited_int::operator many_bits()
{
	if (this->is_zero())
		return (few_bits)0;
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits because the unlimited_int object is too big.");
	if (this->is_negative)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits because the unlimited_int object is negative.");
	return this->get_least_significant_many_bits();
}
unlimited_int::operator many_bits_signed()
{
	if (this->is_zero())
		return (few_bits)0;
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits_signed because the unlimited_int object is too big.");
	many_bits_signed this_as_many_bits_signed = static_cast<many_bits_signed>(this->get_least_significant_many_bits());
	if (this->is_negative)
		this_as_many_bits_signed = -this_as_many_bits_signed;
	if ((*this) != this_as_many_bits_signed)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits_signed because the unlimited_int object is too big (or too small) to fully fit inside many_bits_signed.");
	return this_as_many_bits_signed;
}
