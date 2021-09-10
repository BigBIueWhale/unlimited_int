#include "unlimited_int.hpp"
using namespace unlimited;
unlimited_int::operator few_bits() const
{
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to few_bits because the unlimited_int object is too big.");
	if (this->_is_negative)
		throw std::overflow_error("Invalid conversion from unlimited_int to few_bits because the unlimited_int object is negative.");
	return this->get_least_significant_few_bits();
}
unlimited_int::operator few_bits_signed() const
{
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to few_bits_signed because the unlimited_int object is too big.");
	const few_bits this_as_few_bits = this->get_least_significant_few_bits();
	static_assert(static_cast<few_bits_signed>(MAX_few_bits_signed_NUM) == -(static_cast<few_bits_signed>(MIN_few_bits_signed_NUM) + (few_bits_signed)1),
		"The funtion unlimited_int::operator few_bits_signed() makes a basic assumption about how signed numbers work, and that assumption is crucial for expected operation of the unlimited library.");
	//If we need to create a negative result, and the result can fit as the negative version, but not as the positive version.
	//For example: a regular 64bit signed int can hold the number -0x8000000000000000 but not the number +0x8000000000000000, because it's only until +0x7FFFFFFFFFFFFFFF (see above assersion)
	//That's why I need a special check for that situation.
	if (this->_is_negative && this_as_few_bits == static_cast<few_bits>(MAX_few_bits_signed_NUM) + (few_bits)1)
		return static_cast<few_bits_signed>(MIN_few_bits_signed_NUM);
	//In any other situation except that extreme situation above:
	if (this_as_few_bits > static_cast<few_bits>(MAX_few_bits_signed_NUM))
		throw std::overflow_error("Invalid conversion from unlimited_int to few_bits_signed because the unlimited_int object is too big (or too small) to fully fit inside few_bits_signed.");
	const few_bits_signed this_as_few_bits_signed = static_cast<few_bits_signed>(this_as_few_bits);
	if (this->_is_negative)
		return -this_as_few_bits_signed;
	return this_as_few_bits_signed;
}
unlimited_int::operator many_bits() const
{
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits because the unlimited_int object is too big.");
	if (this->_is_negative)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits because the unlimited_int object is negative.");
	return this->get_least_significant_many_bits();
}
unlimited_int::operator many_bits_signed() const
{
	if (this->num_of_used_ints > (size_t)1)
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits_signed because the unlimited_int object is too big to fit even in many_bits (unsigned)");
	const many_bits this_as_many_bits = this->get_least_significant_many_bits();
	//If we need to create a negative result, and the result can fit as the negative version, but not as the positive version.
	//For example: a regular 32bit signed int can hold the number -0x80000000 but not the number +0x80000000, because it's only until +0x7FFFFFFF (see above assersion)
	//That's why I need a special check for that situation.
	static_assert(static_cast<many_bits_signed>(MAX_many_bits_signed_NUM) == -(static_cast<many_bits_signed>(MIN_many_bits_signed_NUM) + (many_bits_signed)1),
		"The funtion unlimited_int::operator many_bits_signed() makes a basic assumption about how signed numbers work, and that assumption is crucial for expected operation of the unlimited library.");
	if (this->_is_negative && this_as_many_bits == static_cast<many_bits>(MAX_many_bits_signed_NUM) + (many_bits)1)
		return static_cast<many_bits_signed>(MIN_many_bits_signed_NUM);
	//In any other situation except that extreme situation above:
	if (this_as_many_bits > static_cast<many_bits>(MAX_many_bits_signed_NUM))
		throw std::overflow_error("Invalid conversion from unlimited_int to many_bits_signed because the unlimited_int object is too big (or too small) to fully fit inside many_bits_signed.");
	const many_bits_signed this_as_many_bits_signed = static_cast<many_bits_signed>(this_as_many_bits);
	if (this->_is_negative)
		return -this_as_many_bits_signed;
	return this_as_many_bits_signed;
}
