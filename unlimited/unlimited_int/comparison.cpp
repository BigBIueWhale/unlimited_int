#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in start of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
	if (this->is_zero() && num_to_compare_to.is_zero())
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
			throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
		return 'E';
	}
	if ((this->is_negative) && (num_to_compare_to.is_negative == false))
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
			throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
		return 'S';
	}
	if ((this->is_negative == false) && (num_to_compare_to.is_negative))
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
			throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
		return 'L';
	}
	bool both_negative = false;
	if (this->is_negative)
	{
		both_negative = true;
	}
	if (this->num_of_used_ints > num_to_compare_to.num_of_used_ints)
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
			throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
		if (both_negative)
		{
			return 'S';
		}
		else
		{
			return 'L';
		}
	}
	else if (num_to_compare_to.num_of_used_ints > this->num_of_used_ints)
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
			throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
		if (both_negative)
		{
			return 'L';
		}
		else
		{
			return 'S';
		}
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
	return 'E';
}
char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\"";
#endif
	if (this->num_of_used_ints > num_to_compare_to.num_of_used_ints)
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
			throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\"";
#endif
		return 'L';
	}
	else if (num_to_compare_to.num_of_used_ints > this->num_of_used_ints)
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
		if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
			throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\"";
#endif
		return 'S';
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in end of function \"char unlimited_int::estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\"";
#endif
	return 'E';
}
char unlimited_int::compare_to(const unlimited_int& num_to_compare_to) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0

	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in start of function \"char unlimited_int::compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
	char value_compare_estimation = this->estimate_compare_to(num_to_compare_to);
	if (value_compare_estimation != 'E')
		return value_compare_estimation;
	const bool both_are_negative = this->is_negative;
	if (this->num_of_intarrays_used == 0) //meaning they're both zero
		return 'E';
	Node* it_this = this->intarrays.intarrays.first;
	many_bits this_num_of_used_int_arrays = this->num_of_intarrays_used;
	for (many_bits num_int_array_in_list_this = 1; num_int_array_in_list_this < this_num_of_used_int_arrays; num_int_array_in_list_this++, it_this = it_this->next);
	Node* it_compare = num_to_compare_to.intarrays.intarrays.first;
	many_bits compare_num_of_used_int_arrays = num_to_compare_to.num_of_intarrays_used;
	for (many_bits num_int_array_in_list_compare = 1; num_int_array_in_list_compare < compare_num_of_used_int_arrays; num_int_array_in_list_compare++, it_compare = it_compare->next);
	int_array* current_int_array_this = it_this->value;
	int_array* current_int_array_compare = it_compare->value;
	many_bits_signed index_this = current_int_array_this->num_of_used_ints - 1;
	many_bits_signed index_compare = current_int_array_compare->num_of_used_ints - 1;
	many_bits num_int = 0;
	many_bits_signed previous_num_int = 0;
	many_bits num_of_used_ints_both = this->num_of_used_ints;
	many_bits stop_at;
	many_bits length_left_in_this = index_this + 1, length_left_in_compare = index_compare + 1;
	if (length_left_in_this < length_left_in_compare) { stop_at = length_left_in_this; }
	else { stop_at = length_left_in_compare; }
	if (num_of_used_ints_both < stop_at) { stop_at = num_of_used_ints_both; }
	few_bits* current_int_array_this_intarr = &current_int_array_this->intarr[current_int_array_this->num_of_used_ints - 1];
	few_bits* current_int_array_compare_intarr = &current_int_array_compare->intarr[current_int_array_compare->num_of_used_ints - 1];
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this -= previous_num_int;
			index_compare -= previous_num_int;
			previous_num_int = num_int;
			if (num_int == num_of_used_ints_both) { break; }
			if (index_this < 0)
			{
				it_this = it_this->previous;
				current_int_array_this = it_this->value;
				current_int_array_this_intarr = &current_int_array_this->intarr[current_int_array_this->num_of_used_ints - 1];
				index_this = current_int_array_this->num_of_used_ints - 1;
			}
			if (index_compare < 0)
			{
				it_compare = it_compare->previous;
				current_int_array_compare = it_compare->value;
				current_int_array_compare_intarr = &current_int_array_compare->intarr[current_int_array_compare->num_of_used_ints - 1];
				index_compare = current_int_array_compare->num_of_used_ints - 1;
			}
			length_left_in_this = index_this + 1;
			length_left_in_compare = index_compare + 1;
			if (length_left_in_this < length_left_in_compare) { stop_at += length_left_in_this; }
			else { stop_at += length_left_in_compare; }
			if (num_of_used_ints_both < stop_at) { stop_at = num_of_used_ints_both; }
			continue;
		}
		few_bits this_value_current = *current_int_array_this_intarr;
		few_bits compare_value_current = *current_int_array_compare_intarr;
		if (this_value_current < compare_value_current)
		{
			if (both_are_negative) { return 'L'; }
			else { return 'S'; }
		}
		if (compare_value_current < this_value_current)
		{
			if (both_are_negative) { return 'S'; }
			else { return 'L'; }
		}
		++num_int;
		--current_int_array_this_intarr;
		--current_int_array_compare_intarr;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in end of function \"char unlimited_int::compare_to(const unlimited_int& num_to_compare_to) const\"";
#endif
	return 'E';
}
char unlimited_int::compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in the start of function \"char unlimited_int::compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\"";
#endif
	char value_compare_estimation = this->estimate_compare_to_ignore_sign(num_to_compare_to);
	if (value_compare_estimation != 'E')
		return value_compare_estimation;
	if ((this->num_of_intarrays_used == 0) && (num_to_compare_to.num_of_intarrays_used == 0)) //meaning they're both zero
		return 'E';
	Node* it_this = this->intarrays.intarrays.first;
	many_bits this_num_of_used_int_arrays = this->num_of_intarrays_used;
	for (many_bits num_int_array_in_list_this = 1; num_int_array_in_list_this < this_num_of_used_int_arrays; num_int_array_in_list_this++, it_this = it_this->next);
	Node* it_compare = num_to_compare_to.intarrays.intarrays.first;
	many_bits compare_num_of_used_int_arrays = num_to_compare_to.num_of_intarrays_used;
	for (many_bits num_int_array_in_list_compare = 1; num_int_array_in_list_compare < compare_num_of_used_int_arrays; num_int_array_in_list_compare++, it_compare = it_compare->next);
	int_array* current_int_array_this = it_this->value;
	int_array* current_int_array_compare = it_compare->value;
	many_bits_signed index_this = current_int_array_this->num_of_used_ints - 1;
	many_bits_signed index_compare = current_int_array_compare->num_of_used_ints - 1;
	many_bits num_int = 0;
	many_bits_signed previous_num_int = 0;
	many_bits num_of_used_ints_both = this->num_of_used_ints;
	many_bits stop_at;
	many_bits length_left_in_this = index_this + 1, length_left_in_compare = index_compare + 1;
	if (length_left_in_this < length_left_in_compare) { stop_at = length_left_in_this; }
	else { stop_at = length_left_in_compare; }
	if (num_of_used_ints_both < stop_at) { stop_at = num_of_used_ints_both; }
	few_bits* current_int_array_this_intarr = &current_int_array_this->intarr[current_int_array_this->num_of_used_ints - 1];
	few_bits* current_int_array_compare_intarr = &current_int_array_compare->intarr[current_int_array_compare->num_of_used_ints - 1];
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this -= previous_num_int;
			index_compare -= previous_num_int;
			previous_num_int = num_int;
			if (num_int == num_of_used_ints_both) { break; }
			if (index_this < 0)
			{
				it_this = it_this->previous;
				current_int_array_this = it_this->value;
				current_int_array_this_intarr = &current_int_array_this->intarr[current_int_array_this->num_of_used_ints - 1];
				index_this = current_int_array_this->num_of_used_ints - 1;
			}
			if (index_compare < 0)
			{
				it_compare = it_compare->previous;
				current_int_array_compare = it_compare->value;
				current_int_array_compare_intarr = &current_int_array_compare->intarr[current_int_array_compare->num_of_used_ints - 1];
				index_compare = current_int_array_compare->num_of_used_ints - 1;
			}
			length_left_in_this = index_this + 1;
			length_left_in_compare = index_compare + 1;
			if (length_left_in_this < length_left_in_compare) { stop_at += length_left_in_this; }
			else { stop_at += length_left_in_compare; }
			if (num_of_used_ints_both < stop_at) { stop_at = num_of_used_ints_both; }
			continue;
		}
		few_bits this_value_current = *current_int_array_this_intarr;
		few_bits compare_value_current = *current_int_array_compare_intarr;
		if (this_value_current < compare_value_current)
		{
			return 'S';
		}
		if (compare_value_current < this_value_current)
		{
			return 'L';
		}
		++num_int;
		--current_int_array_this_intarr;
		--current_int_array_compare_intarr;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_compare_to.find_inconsistencies())
		throw "Inconsistencies found in the end of function \"char unlimited_int::compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const\"";
#endif
	return 'E';
}