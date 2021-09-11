#include "int_array.hpp"
using namespace unlimited;
#if DEBUG_MODE > 0
#include <iostream>
#include <sstream>
#include <iomanip>
#endif
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
std::atomic<uint64_t> int_array::num_of_ints_created((uint64_t)0);
std::atomic<uint64_t> int_array::num_of_ints_destroyed((uint64_t)0);
#endif
void int_array::assign(const few_bits num_to_assign)
{
	this->num_of_used_ints = (size_t)1;
	if (this->intarr_len == (size_t)0) {this->resize((size_t)MIN_ALLOC); }
	this->intarr[(size_t)0] = num_to_assign;
}
void int_array::fillzero()
{
	if (this->intarr_len > (size_t)0)
	{
		few_bits* this_intarr = this->intarr;
		for (const few_bits* const stop_ptr = this_intarr + (this->intarr_len - (size_t)1); this_intarr <= stop_ptr; ++this_intarr)
			*this_intarr = 0;
	}
}
void int_array::fillzero_until(size_t num_of_ints_to_zero)
{
	if (num_of_ints_to_zero > this->intarr_len) { num_of_ints_to_zero = this->intarr_len; }
	if (num_of_ints_to_zero > (size_t)0)
	{
		few_bits* this_intarr = this->intarr;
		for (const few_bits *const stop_ptr = this_intarr + (num_of_ints_to_zero - (size_t)1); this_intarr <= stop_ptr; ++this_intarr)
			*this_intarr = 0;
	}
}
#if DEBUG_MODE > 0
bool int_array::find_inconsistencies()
{
	if (this->intarr == nullptr)
	{
		if (this->intarr_len != (size_t)0)
		{
			std::cerr << "\nError found by function int_array::find_inconsistencies, this->intarr_len is not 0 even though this->intarr == nullptr";
			return true;
		}
		if (this->num_of_used_ints != (size_t)0)
		{
			std::cerr << "\nError found by function int_array::find_inconsistencies, this->num_of_used_ints is not 0 even though this->intarr == nullptr";
			return true;
		}
	}
	else
	{
		if (this->intarr_len == (size_t)0)
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
#endif
size_t int_array::find_first_used_not_zero(bool *const found) const
{
	if (this->num_of_used_ints == (size_t)0)
	{
		*found = false; 
		return MAX_size_t_NUM;
	}
	few_bits* current_address = this->intarr + (this->num_of_used_ints - (size_t)1);
	const few_bits *const stop_at = this->intarr;
	size_t index = this->num_of_used_ints - (size_t)1;
	while (current_address >= stop_at)
	{
		if (*current_address != (few_bits)0)
		{
			*found = true;
			return index;
		}
		--current_address;
		--index;
	}
	*found = false;
	return MAX_size_t_NUM;
}
void int_array::shift_right(const size_t num_of_ints_to_shift_right_by)
{
	if (num_of_ints_to_shift_right_by > (size_t)0)
	{
		if (num_of_ints_to_shift_right_by > this->num_of_used_ints)
			throw std::logic_error("Error in function int_array::shift_right, can\'t shift right by more than the entire length of the array.");
		else if (num_of_ints_to_shift_right_by == this->num_of_used_ints)
			this->num_of_used_ints = (size_t)0;
		else
		{
			few_bits* it_read = this->intarr + num_of_ints_to_shift_right_by;
			few_bits* it_write = this->intarr;
			const size_t num_of_ints_in_new_int_array = this->num_of_used_ints - num_of_ints_to_shift_right_by;
			const few_bits *const stop_ptr = this->intarr + (num_of_ints_in_new_int_array - (size_t)1);
			while (it_write <= stop_ptr)
			{
				*it_write = *it_read;
				++it_read;
				++it_write;
			}
			this->num_of_used_ints = num_of_ints_in_new_int_array;
		}
	}
}
void int_array::shift_left_by_one()
{
	if (this->num_of_used_ints == (size_t)0)
	{
		*this->intarr = (few_bits)0;
		this->num_of_used_ints = (size_t)1;
	}
	else if (this->num_of_used_ints == (size_t)1)
	{
		this->intarr[1] = *this->intarr;
		*this->intarr = (few_bits)0;
		this->num_of_used_ints = (size_t)2;
	}
	else
	{
		few_bits* it_read = this->intarr + (this->num_of_used_ints - (size_t)1);
		few_bits* it_write = this->intarr + this->num_of_used_ints;
		const few_bits *const it_stop_read = this->intarr;
		while (it_read >= it_stop_read)
		{
			*it_write = *it_read;
			--it_write;
			--it_read;
		}
		*this->intarr = (few_bits)0;
		++this->num_of_used_ints;
	}
}
void int_array::resize_and_fillzero(const size_t size_to_make)
{
	if (size_to_make == (size_t)0)
		this->num_of_used_ints = (size_t)0;
	else
	{
		this->intarr = new few_bits[size_to_make];
		this->intarr_len = size_to_make;
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
		int_array::num_of_ints_created += (uint64_t)size_to_make;
#endif
		this->fillzero();
	}
}
void int_array::resize(const size_t size_to_make)
{
	if (size_to_make == (size_t)0)
		this->num_of_used_ints = (size_t)0;
	else
	{
		this->intarr = new few_bits[size_to_make];
		this->intarr_len = size_to_make;
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
		int_array::num_of_ints_created += (uint64_t)size_to_make;
#endif
	}
}
void int_array::destroy()
{
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
	int_array::num_of_ints_destroyed += (uint64_t)this->intarr_len;
#endif
	delete[] intarr;
	this->set_null();
}
