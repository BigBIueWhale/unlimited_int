#include "int_array.hpp"
using namespace unlimited;
#if DEBUG_MODE > 0
#include <iostream>
#include <sstream>
#include <iomanip>
#endif
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
many_bits int_array::num_of_ints_created = 0;
many_bits int_array::num_of_ints_destroyed = 0;
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
std::mutex int_array::num_of_ints_created_guard;
std::mutex int_array::num_of_ints_destroyed_guard;
#endif
#endif
void int_array::assign(const few_bits num_to_assign)
{
	if (num_to_assign == 0) { this->num_of_used_ints = 0; }
	else
	{
		this->num_of_used_ints = 1;
		if (this->intarr_len == 0) {this->resize(MIN_ALLOC); }
		this->intarr[0] = num_to_assign;
	}
}
void int_array::fillzero()
{
	if (this->intarr_len == 0) { return; }
	few_bits* this_intarr = this->intarr;
	few_bits* stop_ptr = &this_intarr[this->intarr_len - 1];
	while (this_intarr != stop_ptr)
	{
		*this_intarr = 0;
		++this_intarr;
	}
	*this_intarr = 0;
}
void int_array::fillzero_until(many_bits num_of_ints_to_zero)
{
	if (num_of_ints_to_zero > this->intarr_len) { num_of_ints_to_zero = this->intarr_len; }
	if (num_of_ints_to_zero == 0) { return; }
	few_bits* this_intarr = this->intarr;
	few_bits* stop_ptr = &this_intarr[num_of_ints_to_zero - 1];
	while (this_intarr != stop_ptr)
	{
		*this_intarr = 0;
		++this_intarr;
	}
	*this_intarr = 0;
}
#if DEBUG_MODE > 0
bool int_array::find_inconsistencies()
{
	if (this->intarr == nullptr)
	{
		if (this->intarr_len != 0)
		{
			std::cerr << "\nError found by function int_array::find_inconsistencies, this->intarr_len is not 0 even though this->intarr == nullptr";
			return true;
		}
		if (this->num_of_used_ints != 0)
		{
			std::cerr << "\nError found by function int_array::find_inconsistencies, this->num_of_used_ints is not 0 even though this->intarr == nullptr";
			return true;
		}
	}
	else
	{
		if (this->intarr_len == 0)
		{
			std::cerr << "\nError found by function int_array::find_inconsistencies, this->intarr_len is 0";
			return true;
		}
		if (this->intarr_len < this->num_of_used_ints)
		{
			std::cerr << "\nError found by function int_array::find_inconsistencies, this->intarr_len is smaller than this->num_of_used_ints";
			return true;
		}
	}
	return false;
}
void int_array::print_all() const
{
	std::stringstream message;
	if (this->intarr_len == 0)
	{
		message << " 0xNOTHING";
	}
	else
	{
		message << "  this->intarr_len: " << this->intarr_len;
		message << "  this->num_of_used_ints: " << this->num_of_used_ints;
		message << "  0x";
		bool is_first = true;
		for (many_bits_signed index_of_intarr = this->intarr_len - 1; index_of_intarr >= 0; index_of_intarr--)
		{
			if (is_first) { is_first = false; }
			else { message << " "; }
			char fill_char = '0';
			message << std::setfill(fill_char) << std::setw(NUM_OF_BITS_few_bits / 4) << std::hex << ((this->intarr)[index_of_intarr]) << std::dec;
		}
	}
	std::cout << message.str();
}
#endif
bool int_array::is_all_used_zeros() const
{
	if (this->num_of_used_ints == 0) { return true; }
	few_bits* current_address = this->intarr;
	few_bits* stop_at = &this->intarr[this->num_of_used_ints - 1];
	while (current_address != stop_at) { if (*current_address != 0) { return false; }; ++current_address; }
	if (*current_address != 0) { return false; }
	return true;
}
many_bits_signed int_array::find_first_used_not_zero() const
{
	if (this->num_of_used_ints == 0) { return -1; }
	few_bits* current_address = &this->intarr[this->num_of_used_ints - 1];
	few_bits* stop_at = this->intarr;
	many_bits_signed index = this->num_of_used_ints - 1;
	while (current_address != stop_at)
	{
		if (*current_address != 0) { return index; };
		--current_address;
		--index;
	}
	if (*current_address != 0) { return 0; }
	return -1;
}
void int_array::shift_right(many_bits num_of_ints_to_shift_right_by)
{
	if (num_of_ints_to_shift_right_by == 0) { return; }
	if (num_of_ints_to_shift_right_by >= this->num_of_used_ints) { this->num_of_used_ints = 0; return; }
	few_bits* it_read = &this->intarr[num_of_ints_to_shift_right_by];
	few_bits* it_write = this->intarr;
	many_bits num_of_ints_in_new_int_array = this->num_of_used_ints - num_of_ints_to_shift_right_by;
	few_bits* stop_ptr = &this->intarr[num_of_ints_in_new_int_array];
	while (it_write != stop_ptr)
	{
		*it_write = *it_read;
		++it_read;
		++it_write;
	}
	this->num_of_used_ints = num_of_ints_in_new_int_array;
}
void int_array::shift_left_by_one()
{
	if (this->num_of_used_ints == 0) { *this->intarr = 0; this->num_of_used_ints = 1; return; }
	if (this->num_of_used_ints == 1) { this->intarr[1] = *this->intarr; *this->intarr = 0; this->num_of_used_ints = 2; return; }
	if (this->is_full())
		throw "Error in function \"void int_array::shift_left_by_one()\". Can\'t shift left, there's no room to shift left.";
	few_bits* it_read = &this->intarr[this->num_of_used_ints - 1];
	few_bits* it_write = &this->intarr[this->num_of_used_ints];
	few_bits* it_stop_read = this->intarr;
	while (it_read != it_stop_read)
	{
		*it_write = *it_read;
		--it_write;
		--it_read;
	}
	*it_write = *it_read;
	*this->intarr = 0;
	++this->num_of_used_ints;
}
void int_array::resize_and_fillzero(many_bits size_to_make)
{
	if (size_to_make == 0)
	{
		this->num_of_used_ints = 0;
	}
	else
	{
		this->intarr_len = size_to_make;
		this->intarr = new few_bits[size_to_make];
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		int_array::num_of_ints_created_guard.lock();
#endif
		int_array::num_of_ints_created += size_to_make;
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		int_array::num_of_ints_created_guard.unlock();
#endif
#endif
		this->fillzero();
	}
}
void int_array::resize(many_bits size_to_make)
{
	if (size_to_make == 0) { this->num_of_used_ints = 0; }
	else
	{
		this->intarr_len = size_to_make;
		this->intarr = new few_bits[size_to_make];
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		int_array::num_of_ints_created_guard.lock();
#endif
		int_array::num_of_ints_created += size_to_make;
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		int_array::num_of_ints_created_guard.unlock();
#endif
#endif
	}
}
void int_array::destroy()
{
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
	int_array::num_of_ints_destroyed_guard.lock();
#endif
	int_array::num_of_ints_destroyed += this->intarr_len;
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
	int_array::num_of_ints_destroyed_guard.unlock();
#endif
#endif
	delete[] intarr;
	intarr = nullptr;
}
