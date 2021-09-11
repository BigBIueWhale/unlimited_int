#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
unlimited_int::unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)
{
	this->auto_destroy = auto_destroy_this;
	this->intarrays = num_to_assign.intarrays;
	this->_is_negative = num_to_assign._is_negative;
	this->num_of_intarrays_used = num_to_assign.num_of_intarrays_used;
	this->num_of_used_ints = num_to_assign.num_of_used_ints;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("Inconsistency found in the end of function \"unlimited_int::unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)\"");
#endif
}
unlimited_int::unlimited_int(const signed short int num_to_assign)
{
	typedef signed short int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot convert from \"" "signed short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		this->assign(static_cast<few_bits_signed>(num_to_assign));
	else //If only fits_in_many_bits_signed
		this->assign(static_cast<many_bits_signed>(num_to_assign));
}
unlimited_int::unlimited_int(const signed int num_to_assign)
{
	typedef signed int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot convert from \"" "signed int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		this->assign(static_cast<few_bits_signed>(num_to_assign));
	else //If only fits_in_many_bits_signed
		this->assign(static_cast<many_bits_signed>(num_to_assign));
}
unlimited_int::unlimited_int(const signed long int num_to_assign)
{
	typedef signed long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot convert from \"" "signed long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		this->assign(static_cast<few_bits_signed>(num_to_assign));
	else //If only fits_in_many_bits_signed
		this->assign(static_cast<many_bits_signed>(num_to_assign));
}
unlimited_int::unlimited_int(const signed long long int num_to_assign)
{
	typedef signed long long int origin_type;
	constexpr bool fits_in_few_bits_signed = sizeof(origin_type) <= sizeof(few_bits_signed);
	constexpr bool fits_in_many_bits_signed = sizeof(origin_type) <= sizeof(many_bits_signed);
	static_assert((fits_in_few_bits_signed && fits_in_many_bits_signed) || fits_in_many_bits_signed,
		"Assertion error: Cannot convert from \"" "signed long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits_signed)
		this->assign(static_cast<few_bits_signed>(num_to_assign));
	else //If only fits_in_many_bits_signed
		this->assign(static_cast<many_bits_signed>(num_to_assign));
}
unlimited_int::unlimited_int(const unsigned short int num_to_assign)
{
	typedef unsigned short int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_many_bits) || fits_in_many_bits,
		"Assertion error: Cannot convert from \"" "unsigned short int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		this->assign(static_cast<few_bits>(num_to_assign));
	else //If only fits_in_many_bits
		this->assign(static_cast<many_bits>(num_to_assign));
}
unlimited_int::unlimited_int(const unsigned int num_to_assign)
{
	typedef unsigned int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_many_bits) || fits_in_many_bits,
		"Assertion error: Cannot convert from \"" "unsigned int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		this->assign(static_cast<few_bits>(num_to_assign));
	else //If only fits_in_many_bits
		this->assign(static_cast<many_bits>(num_to_assign));
}
unlimited_int::unlimited_int(const unsigned long int num_to_assign)
{
	typedef unsigned long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_many_bits) || fits_in_many_bits,
		"Assertion error: Cannot convert from \"" "unsigned long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		this->assign(static_cast<few_bits>(num_to_assign));
	else //If only fits_in_many_bits
		this->assign(static_cast<many_bits>(num_to_assign));
}
unlimited_int::unlimited_int(const unsigned long long int num_to_assign)
{
	typedef unsigned long long int origin_type;
	constexpr bool fits_in_few_bits = sizeof(origin_type) <= sizeof(few_bits);
	constexpr bool fits_in_many_bits = sizeof(origin_type) <= sizeof(many_bits);
	static_assert((fits_in_few_bits && fits_in_many_bits) || fits_in_many_bits,
		"Assertion error: Cannot convert from \"" "unsigned long long int" "\" to \"unlimited_int\"");
	if (fits_in_few_bits)
		this->assign(static_cast<few_bits>(num_to_assign));
	else //If only fits_in_many_bits
		this->assign(static_cast<many_bits>(num_to_assign));
}
