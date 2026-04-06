#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
//Bitwise NOT using the two's complement identity: ~x = -(x + 1)
//This is the only definition that gives involution (~~x == x), De Morgan's laws, x ^ ~x == -1,
//and consistency with fixed-width types. Every major bigint library (GMP, Boost, Python, Java) uses this.
unlimited_int unlimited_int::operator~() const
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in beginning of function \"unlimited_int* unlimited_int::operator~() const\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in beginning of function: \"unlimited_int* unlimited_int::operator~() const\"");
#endif
	// ~x = -(x + 1)
	unlimited_int result = *this;
	++result;
	if (result.is_zero())
		result._is_negative = false; //~(-1) = 0
	else
		result._is_negative = !result._is_negative;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int* unlimited_int::operator~() const\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (result.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int* unlimited_int::operator~() const\"");
#endif
	return result;
}
//In-place version of operator~. Computes ~x = -(x + 1) directly on this.
void unlimited_int::invert_bits()
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in beginning of function \"void unlimited_int::invert_bits()\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in beginning of function: \"void unlimited_int::invert_bits()\"");
#endif
	// ~x = -(x + 1)
	++(*this);
	if (this->is_zero())
		this->_is_negative = false; //~(-1) = 0
	else
		this->_is_negative = !this->_is_negative;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::invert_bits()\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"void unlimited_int::invert_bits()\"");
#endif
}
