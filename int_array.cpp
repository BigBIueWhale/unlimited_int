#include "int_array.h"
void int_array::assign(few_bits num_to_assign)
{
	if (num_to_assign == 0) { this->num_of_used_ints = 0; }
	else
	{
		this->num_of_used_ints = 1;
		if (this->intarr_len == 0) {this->resize(MIN_ALLOC); }
		this->intarr[0] = num_to_assign;
	}
}
void int_array::compact_print()
{
	std::stringstream message;
	if (this->num_of_used_ints == 0)
	{
		message << " 0xNOTHING";
	}
	else
	{
		message << " 0x";
		bool is_first = true;
		for (many_bits_signed index_of_intarr = this->num_of_used_ints - 1; index_of_intarr >= 0; index_of_intarr--)
		{
			if (is_first) { is_first = false; }
			else { message << " "; }
			char fill_char = '0';
			message << std::setfill(fill_char) << std::setw(NUM_OF_BITS_few_bits / 4) << std::hex << ((this->intarr)[index_of_intarr]) << std::dec;
		}
	}
	std::cout << message.str();
	message.clear();
}
char* int_array::create_string()
{
	char* str_to_return;
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if (this_num_of_used_ints == 0)
	{
		str_to_return = new char[2];
		str_to_return[0] = '0';
		str_to_return[1] = '\0';
	}
	else
	{
		char* chunk = new char[NUM_OF_BITS_few_bits / 4 + 1];
		chunk[NUM_OF_BITS_few_bits / 4] = '\0';
		str_to_return = new char[this_num_of_used_ints * (NUM_OF_BITS_few_bits / 4) + 1];
		few_bits* this_intarr = this->intarr;
		many_bits str_to_return_index = 0;
		for (many_bits_signed index = this_num_of_used_ints - 1; index >= 0; --index)
		{
			sprintf(chunk, "%x", this_intarr[index]);
			int counter;
			for (counter = 0; chunk[counter] != '\0'; ++counter);
			int num_of_zeros_to_prepend = (NUM_OF_BITS_few_bits / 4) - counter;
			for (int counter2 = 0; counter2 < num_of_zeros_to_prepend; ++counter2, ++str_to_return_index)
			{
				str_to_return[str_to_return_index] = '0';
			}
			for (int counter3 = 0; counter3 < counter; ++counter3, ++str_to_return_index)
			{
				str_to_return[str_to_return_index] = chunk[counter3];
			}
		}
		delete[] chunk;
		str_to_return[str_to_return_index] = '\0';
	}
	return str_to_return;
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
#endif
void int_array::print_all()
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
	message.clear();
}
bool int_array::is_all_used_zeros()
{
	if (this->num_of_used_ints == 0) { return true; }
	few_bits* current_address = this->intarr;
	few_bits* stop_at = &this->intarr[this->num_of_used_ints - 1];
	while (current_address != stop_at) { if (*current_address != 0) { return false; }; ++current_address; }
	if (*current_address != 0) { return false; }
	return true;
}
many_bits_signed int_array::find_first_used_not_zero()
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
