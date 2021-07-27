#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
unlimited_int::unlimited_int(const few_bits_signed num_to_assign)
{
	bool set_to_negative;
	if (num_to_assign == 0)
		this->set_to_zero();
	else
	{
		few_bits num_to_assign_positive;
		if (num_to_assign < 0)
		{
			num_to_assign_positive = -num_to_assign;
			set_to_negative = true;
		}
		else
		{
			num_to_assign_positive = num_to_assign;
			set_to_negative = false;
		}
		this->assign(num_to_assign_positive);
		this->is_negative = set_to_negative;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"unlimited_int(few_bits_signed num_to_assign)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "Inconsistency found in the end of function \"unlimited_int::unlimited_int(const few_bits_signed num_to_assign)\"";
#endif
}
unlimited_int::unlimited_int(const many_bits_signed num_to_assign)
{
	bool set_to_negative;
	if (num_to_assign == 0)
		this->set_to_zero();
	else
	{
		many_bits num_to_assign_positive = 0;
		if (num_to_assign < 0)
		{
			num_to_assign_positive = -num_to_assign;
			set_to_negative = true;
		}
		else
		{
			num_to_assign_positive = num_to_assign;
			set_to_negative = false;
		}
		this->assign(num_to_assign_positive);
		this->is_negative = set_to_negative;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"unlimited_int(many_bits_signed num_to_assign)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "Inconsistency found in the end of function \"unlimited_int::unlimited_int(const many_bits_signed num_to_assign)\"";
#endif
}
unlimited_int::unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)
{
	this->auto_destroy = auto_destroy_this;
	this->intarrays = num_to_assign.intarrays;
	this->is_negative = num_to_assign.is_negative;
	this->num_of_intarrays_used = num_to_assign.num_of_intarrays_used;
	this->num_of_used_ints = num_to_assign.num_of_used_ints;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "Inconsistency found in the end of function \"unlimited_int::unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)\"";
#endif
}
unlimited_int::unlimited_int(std::shared_ptr<unlimited_int> shared_ptr_ui)
{
	this->auto_destroy = false;
	(*this) = shared_ptr_ui; //now this->auto_destory becomes true again
}
unlimited_int::unlimited_int(uint32_t* arr, many_bits len)
{
	this->auto_destroy = false;
	this->assign(arr, len);
}
unlimited_int::unlimited_int(uint64_t* arr, many_bits len)
{
	this->auto_destroy = false;
	this->assign(arr, len);
}
