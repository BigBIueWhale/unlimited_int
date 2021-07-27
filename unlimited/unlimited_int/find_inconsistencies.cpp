#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE > 0
#include <iostream>
void unlimited_int::print_properties() const
{
	std::cout << "\nthis: " << this;
	std::cout << "\nthis->auto_destroy: " << this->auto_destroy;
	std::cout << "\nthis->is_negative: " << this->is_negative;
	std::cout << "\nthis->num_of_intarrays_used: " << this->num_of_intarrays_used;
	std::cout << "\nthis->num_of_used_ints: " << this->num_of_used_ints;
	this->intarrays.print_properties();
}
void unlimited_int::print() const
{
	this->print_properties();
	std::cout << std::endl << std::hex << (*this) << std::dec;
}
void unlimited_int::compact_print() const
{
	std::cout << std::hex << (*this) << std::dec;
}
bool unlimited_int::find_inconsistencies() const
{
	bool to_return_true = false;
	to_return_true = this->intarrays.find_inconsistencies();
	if ((this->intarrays.intarrays.length == 0) && ((this->is_negative) || (this->num_of_used_ints != 0) || (this->num_of_intarrays_used != 0)))
	{
		std::cerr << "\nError found by function \"find_inconsistencies\": \"this->intarrays.intarrays.length\" == 0 and one of \"is_negative\", \"num_of_used_ints\", \"num_of_intarrays_used\" doesn\'t fit with that";
		to_return_true = true;
	}
	if ((this->num_of_intarrays_used == 0) != (this->num_of_used_ints == 0))
	{
		std::cerr << "\nError found by function \"find_inconsistencies\": \"this->num_of_intarrays_used\" and \"this->num_of_used_ints\" disagree on whether the number equals zero";
		to_return_true = true;
	}
	if (this->intarrays.intarrays.length < this->num_of_intarrays_used)
	{
		std::cerr << "\nError found by function \"find_inconsistencies\": \"this->num_of_intarrays_used\" is bigger than \"this->intarrays.intarray.length\"";
		to_return_true = true;
	}
	if (this->intarrays.num_of_ints < this->num_of_used_ints)
	{
		std::cerr << "\nError found by function \"find_inconsistencies\": \"this->num_of_used_ints\" is bigger than \"this->intarrays.num_of_ints\"";
		to_return_true = true;
	}
	many_bits size_of_this_intarrays_intarrays = this->intarrays.intarrays.double_check_length();
	if (this->intarrays.intarrays.length != size_of_this_intarrays_intarrays)
	{
		std::cerr << "\nError found by function \"find_inconsistencies\": \"this->intarrays.intarrays.length\" isn\'t correct. Because size based on the \"ListArrays::double_check_length\" function is: " << size_of_this_intarrays_intarrays
			<< " and the variable \"this->intarrays.intarrays.length\" equals: " << this->intarrays.intarrays.length;
		to_return_true = true;
	}
	else
	{
		bool this_intarrays_intarrays_empty = this->intarrays.intarrays.length == 0;
		if (this_intarrays_intarrays_empty && ((this->is_negative) || (this->num_of_used_ints != 0) || (this->num_of_intarrays_used != 0)))
		{
			std::cerr << "\nError found by function \"find_inconsistencies\": \"this->intarrays.intarrays\" is empty, and one of \"is_negative\", \"num_of_used_ints\", \"num_of_intarrays_used\" doesn\'t fit with that";
			to_return_true = true;
		}
		if (!this_intarrays_intarrays_empty)
		{
			Node* it = this->intarrays.intarrays.first;
			many_bits num_of_ints_counter = 0;
			many_bits num_of_ints_used = 0;
			many_bits num_of_int_arrays_went_through_so_far = 0;
			while (it != nullptr)
			{
				int_array* current_int_array = it->value;
				if (current_int_array == nullptr)
				{
					std::cerr << "\nError found by function \"find_inconsistencies\": One of the \"int_array\" objects in list: \"this->intarrays.intarrays\" is nullptr\n";
					this->print_properties();
					return true;
				}
				else if (current_int_array->intarr == nullptr)
				{
					std::cerr << "\nError found by function \"find_inconsistencies\": the pointer to one of the actual integer arrays inside of an \"int_array\" object in list: \"this->intarrays.intarrays\" is nullptr\n";
					this->print_properties();
					return true;
				}
				num_of_ints_counter += current_int_array->intarr_len;
				if (num_of_int_arrays_went_through_so_far < this->num_of_intarrays_used)
				{
					num_of_ints_used += current_int_array->num_of_used_ints;
					if (current_int_array->num_of_used_ints == 0)
					{
						std::cerr << "\nError found by function \"find_inconsistencies\": One of the \"current_int_array\" objects that is labeled as used by \"this->num_of_intarrays_used\" contains zero used ints \"based on \"num_of_used_ints\" inside the \"int_array\" object\"";
					}
					else
					{
						if (num_of_int_arrays_went_through_so_far + 1 == this->num_of_intarrays_used)
						{
							if (current_int_array->intarr[current_int_array->num_of_used_ints - 1] == 0)
							{
								std::cerr << "\nError found by function \"find_inconsistencies\": The most significant digit in the whole \"unlimited_int\" type number is 0";
								std::cerr << "\nNow printing the faulty number: ";
								this->print();
								std::cout << "\n";
								to_return_true = true;
							}
						}
					}
				}
				it = it->next;
				++num_of_int_arrays_went_through_so_far;
			}
			if (num_of_ints_counter != this->intarrays.num_of_ints)
			{
				std::cerr << "\nError found by function \"find_inconsistencies\": the sum of the \"intarr_len\" variables in the \"int_array\"s pointed to in \"this->intarr.intarr\" list, doesn\'t equal \"this->num_of_ints\""
					<< " because the sum equals: " << num_of_ints_counter << " and the official variable \"this->intarrays.num_of_ints\" equals: " << this->intarrays.num_of_ints;
				to_return_true = true;
			}
			if (num_of_ints_used != this->num_of_used_ints)
			{
				std::cerr << "\nError found by function \"find_inconsistencies\": the sum of the \"num_of_used_ints\" variables in the \"int_array\"s pointed to in \"this->intarr.intarr\" list, doesn\'t equal \"this->num_of_used_ints\""
					<< " because the sum equals: " << num_of_ints_used << " and the official variable \"this->num_of_used_ints\" equals: " << this->num_of_used_ints;
				to_return_true = true;
			}
		}
	}
	if (to_return_true)
	{
		std::cout << "\n";
		this->print_properties();
	}
	return to_return_true;
}
#endif