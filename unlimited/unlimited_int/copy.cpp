#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::copy_to(unlimited_int& num_to_paste_into) const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"copy_to\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "inconsistency found in the beginning of \"void unlimited_int::copy_to(unlimited_int& num_to_paste_into) const\"";
#endif
	num_to_paste_into.auto_destroy = true;
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	num_to_paste_into.intarrays.increase_until_num_of_ints(this_num_of_used_ints);
	num_to_paste_into.num_of_used_ints = this_num_of_used_ints;
	num_to_paste_into.is_negative = this->is_negative;
	if (this->num_of_intarrays_used == 0)
	{
		num_to_paste_into.set_to_zero();
		return;
	}
	many_bits num_of_used_arrays_in_paste = 1;
	Node* it_this = this->intarrays.intarrays.first;
	Node* it_paste = num_to_paste_into.intarrays.intarrays.first;
	int_array* current_int_array_this = it_this->value;
	int_array* current_int_array_paste = it_paste->value;
	many_bits index_this = 0, index_paste = 0;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits current_int_array_this_num_of_used_ints = current_int_array_this->num_of_used_ints;
	many_bits current_int_array_paste_intarr_len = current_int_array_paste->intarr_len;
	few_bits* current_int_array_paste_intarr = current_int_array_paste->intarr;
	few_bits* current_int_array_this_intarr = current_int_array_this->intarr;
	many_bits stop_at = this_num_of_used_ints;
	if (current_int_array_this_num_of_used_ints < stop_at)
		stop_at = current_int_array_this_num_of_used_ints;
	if (current_int_array_paste_intarr_len < stop_at)
		stop_at = current_int_array_paste_intarr_len;
	current_int_array_paste->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this += previous_num_int;
			index_paste += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_num_of_used_ints) { break; }
			if (index_this == current_int_array_this_num_of_used_ints)
			{
				it_this = it_this->next;
				current_int_array_this = it_this->value;
				current_int_array_this_num_of_used_ints = current_int_array_this->num_of_used_ints;
				current_int_array_this_intarr = current_int_array_this->intarr;
				index_this = 0;
			}
			if (index_paste == current_int_array_paste_intarr_len)
			{
				it_paste = it_paste->next;
				current_int_array_paste = it_paste->value;
				num_of_used_arrays_in_paste++;
				current_int_array_paste->set_num_of_used_ints_to_maximum();
				current_int_array_paste_intarr_len = current_int_array_paste->intarr_len;
				current_int_array_paste_intarr = current_int_array_paste->intarr;
				index_paste = 0;
			}
			many_bits paste_intarr_len_left_to_go_through = current_int_array_paste_intarr_len - index_paste;
			many_bits this_intarr_len_left_to_go_through = current_int_array_this_num_of_used_ints - index_this;
			if (paste_intarr_len_left_to_go_through < this_intarr_len_left_to_go_through)
			{
				stop_at += paste_intarr_len_left_to_go_through;
			}
			else { stop_at += this_intarr_len_left_to_go_through; }
			if (this_num_of_used_ints < stop_at) { stop_at = this_num_of_used_ints; }
			continue;
		}
		*current_int_array_paste_intarr = *current_int_array_this_intarr;
		++current_int_array_paste_intarr;
		++current_int_array_this_intarr;
		++num_int;
	}
	num_to_paste_into.num_of_intarrays_used = num_of_used_arrays_in_paste;
	current_int_array_paste->num_of_used_ints = index_paste;
	num_to_paste_into.flush_unused();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"copy_to\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_paste_into.find_inconsistencies())
		throw "Inconsistency was found in the end of \"void unlimited_int::copy_to(unlimited_int& num_to_paste_into) const\"";
#endif
}
void unlimited_int::copy_most_significant_to(unlimited_int& num_to_paste_into, const many_bits num_of_ints_to_copy) const
{
	many_bits num_of_ints_to_copy_cpy = num_of_ints_to_copy;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"copy_most_significant_to\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "Inconsistency was found in start of function \"void unlimited_int::copy_most_significant_to(unlimited_int& num_to_paste_into, const many_bits num_of_ints_to_copy) const\"";
#endif
	num_to_paste_into.is_negative = false;
	if (this->num_of_intarrays_used == 0)
	{
		num_to_paste_into.set_to_zero();
		return;
	}
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if (num_of_ints_to_copy_cpy > this_num_of_used_ints) { num_of_ints_to_copy_cpy = this_num_of_used_ints; }
	num_to_paste_into.intarrays.increase_until_num_of_ints(num_of_ints_to_copy_cpy);
	num_to_paste_into.num_of_used_ints = num_of_ints_to_copy_cpy;
	__list_location__ ll = num_to_paste_into.intarrays.find_num_of_int_from_insignificant(num_of_ints_to_copy_cpy);
	Node* it_paste = ll.node;
	many_bits_signed index_paste = ll.index;
	num_to_paste_into.num_of_intarrays_used = ll.num_array;
	num_to_paste_into.num_of_used_ints = num_of_ints_to_copy_cpy;
	int_array* current_int_array_paste = it_paste->value;
	current_int_array_paste->num_of_used_ints = index_paste + 1;
	Node* it_this = this->get_most_significant_used_int_array();
	int_array* current_int_array_this = it_this->value;
	many_bits_signed index_this = current_int_array_this->num_of_used_ints - 1;
	many_bits num_int = 0, stop_at, previous_num_int = 0;
	many_bits num_of_ints_left_for_this = index_this + 1;
	many_bits num_of_ints_left_for_paste = index_paste + 1;
	few_bits* current_intarr_this = &current_int_array_this->intarr[index_this];
	few_bits* current_intarr_paste = &current_int_array_paste->intarr[index_paste];
	if (num_of_ints_left_for_this < num_of_ints_left_for_paste) { stop_at = num_of_ints_left_for_this; }
	else { stop_at = num_of_ints_left_for_paste; }
	if (num_of_ints_to_copy_cpy < stop_at) { stop_at = num_of_ints_to_copy_cpy; }
	while (true)
	{
		if (num_int >= stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_paste -= previous_num_int;
			index_this -= previous_num_int;
			previous_num_int = num_int;
			if (num_int >= num_of_ints_to_copy_cpy) { break; }
			if (index_this < 0)
			{
				it_this = it_this->previous;
				current_int_array_this = it_this->value;
				index_this = current_int_array_this->num_of_used_ints - 1;
				current_intarr_this = &current_int_array_this->intarr[index_this];
			}
			if (index_paste < 0)
			{
				it_paste = it_paste->previous;
				current_int_array_paste = it_paste->value;
				index_paste = current_int_array_paste->intarr_len - 1;
				current_intarr_paste = &current_int_array_paste->intarr[index_paste];
				current_int_array_paste->set_num_of_used_ints_to_maximum();
			}
			num_of_ints_left_for_this = index_this + 1;
			num_of_ints_left_for_paste = index_paste + 1;
			if (num_of_ints_left_for_this < num_of_ints_left_for_paste) { stop_at += num_of_ints_left_for_this; }
			else { stop_at += num_of_ints_left_for_paste; }
			if (num_of_ints_to_copy_cpy < stop_at) { stop_at = num_of_ints_to_copy_cpy; }
			continue;
		}
		*current_intarr_paste = *current_intarr_this;
		--current_intarr_paste;
		--current_intarr_this;
		++num_int;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"copy_most_significant_to\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_paste_into.find_inconsistencies())
		throw "Inconsistency was found in the end of function \"void unlimited_int::copy_most_significant_to(unlimited_int& num_to_paste_into, const many_bits num_of_ints_to_copy) const\"";
#endif
}
unlimited_int* unlimited_int::to_dynamic()
{
	unlimited_int* cpy = new unlimited_int;
	cpy->absorb(*this);
	return cpy;
}
void unlimited_int::absorb(unlimited_int& other)
{
	if (this->auto_destroy)
		this->flush();
	else
		this->forget_memory();
	this->num_of_used_ints = other.num_of_used_ints;
	this->num_of_intarrays_used = other.num_of_intarrays_used;
	this->is_negative = other.is_negative;
	this->intarrays = other.intarrays;
	other.forget_memory();
	other.auto_destroy = true;
}
void unlimited_int::swap(unlimited_int& num_to_swap_with)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"swap\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_swap_with.find_inconsistencies())
		throw "Inconsistency was found in start of function \"void unlimited_int::swap(unlimited_int& num_to_swap_with)\"";
#endif

	const bool temp_is_negative = this->is_negative;
	this->is_negative = num_to_swap_with.is_negative;
	num_to_swap_with.is_negative = temp_is_negative;

	const bool temp_auto_destroy = this->auto_destroy;
	this->auto_destroy = num_to_swap_with.auto_destroy;
	num_to_swap_with.auto_destroy = temp_auto_destroy;

	const many_bits temp_num_of_intarrays_used = num_to_swap_with.num_of_intarrays_used;
	num_to_swap_with.num_of_intarrays_used = this->num_of_intarrays_used;
	this->num_of_intarrays_used = temp_num_of_intarrays_used;

	const many_bits temp_num_to_swap_with = num_to_swap_with.num_of_used_ints;
	num_to_swap_with.num_of_used_ints = this->num_of_used_ints;
	this->num_of_used_ints = temp_num_to_swap_with;

	this->intarrays.swap(num_to_swap_with.intarrays);

#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"swap\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_swap_with.find_inconsistencies())
		throw "Inconsistency was found in end of function \"void unlimited_int::swap(unlimited_int& num_to_swap_with)\"";
#endif
}
