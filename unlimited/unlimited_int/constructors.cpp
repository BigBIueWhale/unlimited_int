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
