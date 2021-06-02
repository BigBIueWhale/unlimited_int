#include "unlimited_int.h"
//I need to design and write a squaring basecase multiplication algorithm
//The most important thing, is to make the division algorithm more efficient, and at least to make the binary search faster.
void unlimited_int::forget_memory()
{
	this->intarrays.num_of_ints = 0;
	this->intarrays.intarrays.length = 0;
	this->intarrays.intarrays.first = nullptr;
	this->intarrays.intarrays.last = nullptr;
	this->set_to_zero();
}
few_bits unlimited_int::from_hex_char_to_int(char c_int)
{
	if ((48 <= c_int) && (c_int <= 57))
	{
		return (c_int - 48);
	}
	else if ((97 <= c_int) && (c_int <= 102))
	{
		return (c_int - 87);
	}
	else if ((65 <= c_int) && (c_int <= 70))
	{
		return (c_int - 54);
	}
	return 0;
}
void unlimited_int::assign_hex(std::string num)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory(); }
	this->auto_destroy = true;
	many_bits num_of_chars = ((many_bits)num.length());
	if (num_of_chars == 0) { this->set_to_zero(); }
	else
	{
		if (num[0] == '-')
		{
			this->is_negative = true;
			num_of_chars--;
			if (num_of_chars == 0)
			{
				this->set_to_zero();
				return;
			}
			num.erase(0, 1);
		}
		else
		{
			this->is_negative = false;
		}
		many_bits first_num_not_zero = num.find_first_not_of('0');
		num_of_chars -= first_num_not_zero;
		if (num_of_chars == 0)
		{
			this->set_to_zero();
			return;
		}
		num.erase(0, first_num_not_zero);
		many_bits num_of_ints_needed;
		if (num_of_chars % (NUM_OF_BITS_few_bits / 4) == 0)
		{
			num_of_ints_needed = num_of_chars / (NUM_OF_BITS_few_bits / 4);
		}
		else
		{
			num_of_ints_needed = (num_of_chars / (NUM_OF_BITS_few_bits / 4)) + 1;
		}
		this->intarrays.increase_until_num_of_ints(num_of_ints_needed);
		this->num_of_used_ints = num_of_ints_needed;
		many_bits num_of_intarrays_used_counter = 1;
		Node* it = this->intarrays.intarrays.first;
		int_array* current_int_array = it->value;
		current_int_array->set_num_of_used_ints_to_maximum();
		many_bits length_of_current_int_array = current_int_array->intarr_len;
		many_bits index_intarr = 0;
		many_bits_signed index_string = num_of_chars - 1;
		many_bits index_string_foward = 0;
		for (many_bits num_int = 0; num_int < num_of_ints_needed; )
		{
			if (index_intarr == length_of_current_int_array)
			{
				num_of_intarrays_used_counter++;
				index_intarr = 0;
				it = it->next;
				current_int_array = it->value;
				current_int_array->set_num_of_used_ints_to_maximum();
				length_of_current_int_array = current_int_array->intarr_len;
				continue;
			}
			many_bits power_of_16 = 1;
			few_bits num_to_assign = 0;
			current_int_array->intarr[index_intarr] = 0;
			while (true)
			{
				few_bits num_to_add = (unlimited_int::from_hex_char_to_int(num[index_string]));
				num_to_assign += (num_to_add * power_of_16);
				++index_string_foward;
				--index_string;
				if ((index_string_foward % (NUM_OF_BITS_few_bits / 4) == 0) || index_string < 0)
				{
					break;
				}
				power_of_16 *= 16;
			}
			current_int_array->intarr[index_intarr] = num_to_assign;
			++num_int;
			++index_intarr;
		}
		current_int_array->num_of_used_ints = index_intarr;
		this->num_of_intarrays_used = num_of_intarrays_used_counter;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"assign_hex(std::string num)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
unlimited_int::unlimited_int(few_bits_signed num_to_assign)
{
	bool set_to_negative;
	if (num_to_assign == 0) { this->set_to_zero(); }
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
	this->find_inconsistencies();
#endif
}
unlimited_int::unlimited_int(many_bits_signed num_to_assign)
{
	bool set_to_negative;
	if (num_to_assign == 0)
	{
		this->set_to_zero();
	}
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
	this->find_inconsistencies();
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
	this->find_inconsistencies();
#endif
}
void unlimited_int::operator=(unlimited_int& num_to_assign)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory(); }
	this->auto_destroy = true;
	num_to_assign.auto_destroy = false;
	this->intarrays = num_to_assign.intarrays;
	this->is_negative = num_to_assign.is_negative;
	this->num_of_intarrays_used = num_to_assign.num_of_intarrays_used;
	this->num_of_used_ints = num_to_assign.num_of_used_ints;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void unlimited_int::print_properties()
{
	std::cout << "\nthis: " << this;
	std::cout << "\nthis->auto_destroy: " << this->auto_destroy;
	std::cout << "\nthis->is_negative: " << this->is_negative;
	std::cout << "\nthis->num_of_intarrays_used: " << this->num_of_intarrays_used;
	std::cout << "\nthis->num_of_used_ints: " << this->num_of_used_ints;
	this->intarrays.print_properties();
}
void unlimited_int::print()
{
	this->print_properties();
	std::cout << "\n";
	if (this->num_of_intarrays_used == 0)
	{
		std::cout << "0xNOTHING_IN_UNLIMITED_INT";
	}
	else
	{
		if (this->is_negative)
		{
			std::cout << "MINUS ";
		}
		many_bits_signed num_arr_counter = this->intarrays.intarrays.length - 1;
		Node* it;
		for (it = this->intarrays.intarrays.last; it != nullptr; it = it->previous, --num_arr_counter)
		{
			if (num_arr_counter < this->num_of_intarrays_used)
			{
				if (num_arr_counter == this->intarrays.intarrays.length - 1)
				{
					std::cout << "arr index in list: " << num_arr_counter << ": ";
				}
				else
				{
					std::cout << "\narr index in list: " << num_arr_counter << ": ";
				}
				char* to_print = it->value->create_string();
				std::cout << to_print;
				delete[] to_print;
			}
		}
	}
}
void unlimited_int::compact_print()
{
	if (this->num_of_intarrays_used == 0)
	{
		std::cout << "0xNOTHING_IN_UNLIMITED_INT";
	}
	else
	{
		if (this->is_negative)
		{
			std::cout << "-";
		}
		many_bits_signed num_arr_counter = this->intarrays.intarrays.length - 1;
		Node* it;
		for (it = this->intarrays.intarrays.last; it != nullptr; it = it->previous, num_arr_counter--)
		{
			if (num_arr_counter < ((many_bits_signed)this->num_of_intarrays_used))
			{
				char *str_of_int_array = it->value->create_string();
				std::cout << str_of_int_array;
				delete[] str_of_int_array;
			}
		}
	}
}
void unlimited_int::flush()
{
	this->auto_destroy = true;
	this->intarrays.flush_to_piggy_bank();
	this->set_to_zero();
}
void unlimited_int::destroy()
{
	this->auto_destroy = true;
	this->intarrays.flush();
	this->set_to_zero();
}
void unlimited_int::fill_0_until_num_of_ints(many_bits fill_0_until)
{
	if (!this->auto_destroy) { this->forget_memory(); }
	Node* it = this->intarrays.intarrays.first;
	many_bits num_of_ints_zeroed_until_now = 0;
	int_array* current_int_array;
	if (this->intarrays.num_of_ints < fill_0_until) { fill_0_until = this->intarrays.num_of_ints; }
	while (true)
	{
		current_int_array = it->value;
		many_bits sum = num_of_ints_zeroed_until_now + current_int_array->intarr_len;
		if (sum >= fill_0_until)
		{
			current_int_array->fillzero_until(fill_0_until - num_of_ints_zeroed_until_now);
			break;
		}
		else
		{
			num_of_ints_zeroed_until_now = sum;
			current_int_array->fillzero();
		}
		it = it->next;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"fill_0_until_num_of_ints(many_bits fill_0_until)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void unlimited_int::fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	Node* it = this->intarrays.intarrays.first;
	many_bits num_of_ints_zeroed_until_now = 0;
	int_array* current_int_array;
	if (this->intarrays.num_of_ints < fill_0_until) { fill_0_until = this->intarrays.num_of_ints; }
	while (true)
	{
		current_int_array = it->value;
		current_int_array->set_num_of_used_ints_to_maximum();
		many_bits sum = num_of_ints_zeroed_until_now + current_int_array->intarr_len;
		if (sum >= fill_0_until)
		{
			current_int_array->fillzero_until(fill_0_until - num_of_ints_zeroed_until_now);
			break;
		}
		else
		{
			num_of_ints_zeroed_until_now = sum;
			current_int_array->fillzero();
		}
		it = it->next;
	}
}
void unlimited_int::copy_to(unlimited_int& num_to_paste_into)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"copy_to\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	num_to_paste_into.flush();
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
	current_int_array_paste->set_num_of_used_ints_to_maximum();
	many_bits current_int_array_this_intarr_len = current_int_array_this->num_of_used_ints;
	many_bits current_int_array_paste_intarr_len = current_int_array_paste->intarr_len;
	few_bits* current_int_array_paste_intarr = current_int_array_paste->intarr;
	few_bits* current_int_array_this_intarr = current_int_array_this->intarr;
	many_bits stop_at = this_num_of_used_ints;
	if (current_int_array_this_intarr_len < stop_at)
	{
		stop_at = current_int_array_this_intarr_len;
	}
	if (current_int_array_paste_intarr_len < stop_at)
	{
		stop_at = current_int_array_paste_intarr_len;
	}
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this += previous_num_int;
			index_paste += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_num_of_used_ints) { break; }
			if (index_this == current_int_array_this_intarr_len)
			{
				it_this = it_this->next;
				current_int_array_this = it_this->value;
				current_int_array_this_intarr_len = current_int_array_this->num_of_used_ints;
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
			many_bits this_intarr_len_left_to_go_through = current_int_array_this_intarr_len - index_this;
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
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"copy_to\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_paste_into.find_inconsistencies();
#endif
}
__list_location__ unlimited_int::find_num_used_int_from_significant(many_bits num_int_to_find)
{
	__list_location__ ll;
	this->flush_unused();
	if (num_int_to_find > this->num_of_used_ints) { ll.index = 0; ll.num_array = 0; ll.node = nullptr; return ll; }
	Node* it = this->intarrays.intarrays.last;
	many_bits sum_used = 0;
	many_bits num_array = this->num_of_intarrays_used + 1;
	while (true)
	{
		sum_used += it->value->num_of_used_ints;
		--num_array;
		if (sum_used >= num_int_to_find) { break; }
		else { it = it->previous; }
	}
	ll.num_array = num_array;
	ll.node = it;
	//ll.index = it->value->num_of_used_ints - ((num_int_to_find - (sum_used - it->value->num_of_used_ints)) - 1) - 1;
	ll.index = sum_used - num_int_to_find;
	return ll;
}
void unlimited_int::copy_most_significant_to(unlimited_int& num_to_paste_into, many_bits num_of_ints_to_copy)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"copy_most_significant_to\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	num_to_paste_into.is_negative = false;
	if (this->num_of_intarrays_used == 0)
	{
		num_to_paste_into.set_to_zero();
		return;
	}
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if (num_of_ints_to_copy > this_num_of_used_ints) { num_of_ints_to_copy = this_num_of_used_ints; }
	num_to_paste_into.intarrays.increase_until_num_of_ints(num_of_ints_to_copy);
	num_to_paste_into.num_of_used_ints = num_of_ints_to_copy;
	__list_location__ ll = num_to_paste_into.intarrays.find_num_of_int_from_insignificant(num_of_ints_to_copy);
	Node* it_paste = ll.node;
	many_bits_signed index_paste = ll.index;
	num_to_paste_into.num_of_intarrays_used = ll.num_array;
	num_to_paste_into.num_of_used_ints = num_of_ints_to_copy;
	int_array* current_int_array_paste = it_paste->value;
	current_int_array_paste->num_of_used_ints = index_paste + 1;
	this->flush_unused();
	Node* it_this = this->intarrays.intarrays.last;
	int_array* current_int_array_this = it_this->value;
	many_bits_signed index_this = current_int_array_this->num_of_used_ints - 1;
	many_bits num_int = 0, stop_at, previous_num_int = 0;
	many_bits num_of_ints_left_for_this = index_this + 1;
	many_bits num_of_ints_left_for_paste = index_paste + 1;
	few_bits* current_intarr_this = &current_int_array_this->intarr[index_this];
	few_bits* current_intarr_paste = &current_int_array_paste->intarr[index_paste];
	if (num_of_ints_left_for_this < num_of_ints_left_for_paste) { stop_at = num_of_ints_left_for_this; }
	else { stop_at = num_of_ints_left_for_paste; }
	if (num_of_ints_to_copy < stop_at) { stop_at = num_of_ints_to_copy; }
	while (true)
	{
		if (num_int >= stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_paste -= previous_num_int;
			index_this -= previous_num_int;
			previous_num_int = num_int;
			if (num_int >= num_of_ints_to_copy) { break; }
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
			if (num_of_ints_to_copy < stop_at) { stop_at = num_of_ints_to_copy; }
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
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe error was found in function \"copy_most_significant_to\" with this.";
	}
	if (num_to_paste_into.find_inconsistencies())
	{
		std::cout << "\nThe error was found in function \"copy_most_significant_to\" with num_to_paste_into.";
	}
#endif
}
void unlimited_int::swap(unlimited_int& num_to_swap_with)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"swap\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_swap_with.find_inconsistencies();
#endif

	bool temp_bool;
	temp_bool = num_to_swap_with.is_negative;
	num_to_swap_with.is_negative = this->is_negative;
	this->is_negative = temp_bool;

	many_bits temp_int = num_to_swap_with.num_of_intarrays_used;
	num_to_swap_with.num_of_intarrays_used = this->num_of_intarrays_used;
	this->num_of_intarrays_used = temp_int;

	temp_int = num_to_swap_with.num_of_used_ints;
	num_to_swap_with.num_of_used_ints = this->num_of_used_ints;
	this->num_of_used_ints = temp_int;

	this->intarrays.swap(num_to_swap_with.intarrays);

#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"swap\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_swap_with.find_inconsistencies();
#endif
}
char unlimited_int::estimate_compare_to(unlimited_int& num_to_compare_to)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"estimate_compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_compare_to.find_inconsistencies();
#endif
	if ((this->is_negative) && (num_to_compare_to.is_negative == false))
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"estimate_compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
		this->find_inconsistencies();
		num_to_compare_to.find_inconsistencies();
#endif
		return 'S';
	}
	if ((this->is_negative == false) && (num_to_compare_to.is_negative))
	{
#if DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"estimate_compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
		this->find_inconsistencies();
		num_to_compare_to.find_inconsistencies();
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
		std::cout << "\nFinding inconsistencies in end of function \"estimate_compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
		this->find_inconsistencies();
		num_to_compare_to.find_inconsistencies();
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
		std::cout << "\nFinding inconsistencies in end of function \"estimate_compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
		this->find_inconsistencies();
		num_to_compare_to.find_inconsistencies();
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
	std::cout << "\nFinding inconsistencies in end of function \"estimate_compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_compare_to.find_inconsistencies();
#endif
	return 'E';
}
#if DEBUG_MODE > 0
bool unlimited_int::find_inconsistencies()
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
char unlimited_int::compare_to(unlimited_int& num_to_compare_to)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"compare_to(unlimited_int* num_to_compare_to)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_compare_to.find_inconsistencies();
#endif
	char value_compare_estimation = this->estimate_compare_to(num_to_compare_to);
	if (value_compare_estimation != 'E')
	{
		return value_compare_estimation;
	}
	bool both_are_negative = false;
	if (this->is_negative)
	{
		both_are_negative = true;
	}
	if (this->num_of_intarrays_used == 0) //meaning they're both zero
	{
		return 'E';
	}
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
	this->find_inconsistencies();
	num_to_compare_to.find_inconsistencies();
#endif
	return 'E';
}
void unlimited_int::subtract(unlimited_int* num_to_subtract, unlimited_int* answer)
{
#if DEBUG_MODE == 2
	std::cout << "\nStart of subtract:";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || num_to_subtract->find_inconsistencies())
	{
		std::cout << "\nThe error was found in start of function subtract";
	}
#endif
	if (num_to_subtract == this)
	{
		answer->set_to_zero();
		return;
	}
	if (this->num_of_intarrays_used == 0)
	{
		if (num_to_subtract != answer)
		{
			if (num_to_subtract->num_of_used_ints == 0) { answer->set_to_zero(); }
			else
			{
				num_to_subtract->copy_to(*answer);
				answer->is_negative = !answer->is_negative;
			}
			return;
		}
	}
	if (num_to_subtract->num_of_intarrays_used == 0)
	{
		if (this != answer) { this->copy_to(*answer); }
		return;
	}
	bool num_to_subtract_is_negative = num_to_subtract->is_negative;
	bool this_is_negative = this->is_negative;
	unlimited_int this_copy(*this, false);
	unlimited_int num_to_subtract_copy(*num_to_subtract, false);
	this_copy.is_negative = false;
	num_to_subtract_copy.is_negative = false;
	few_bits num_of_negatives = 0;
	if (this_is_negative) { num_of_negatives++; }
	if (num_to_subtract_is_negative) { num_of_negatives++; }
	if (num_of_negatives == 2) { this_copy.swap(num_to_subtract_copy); }
	else if (num_of_negatives == 1)
	{
		if (this_is_negative)
		{
			this_copy.add(&num_to_subtract_copy, answer);
			answer->is_negative = true;
		}
		else { this_copy.add(&num_to_subtract_copy, answer); }
		return;
	}
	char compare_result = this_copy.compare_to(num_to_subtract_copy);
	if (compare_result == 'E')
	{
		answer->set_to_zero();
		return;
	}
	if (compare_result == 'L') { answer->is_negative = false; }
	else //if compare_result == 'S'
	{
		this_copy.swap(num_to_subtract_copy);
		answer->is_negative = true;
	}
	many_bits num_of_used_ints_this = this_copy.num_of_used_ints;
	answer->intarrays.increase_until_num_of_ints(num_of_used_ints_this);
	Node* it_this = this_copy.intarrays.intarrays.first;
	Node* it_subtract = num_to_subtract_copy.intarrays.intarrays.first;
	Node* it_answer = answer->intarrays.intarrays.first;
	int_array* this_current_int_array = it_this->value;
	int_array* subtract_current_int_array = it_subtract->value;
	int_array* answer_current_int_array = it_answer->value;
	many_bits num_of_int_arrays_used_in_answer = 1;
	many_bits index_this = 0, index_subtract = 0, index_answer = 0;
	many_bits this_current_int_array_intarr_len = this_current_int_array->num_of_used_ints;
	many_bits subtract_current_int_array_intarr_len = subtract_current_int_array->num_of_used_ints;
	many_bits answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
	few_bits* this_current_int_array_intarr = this_current_int_array->intarr;
	few_bits* subtract_current_int_array_intarr = subtract_current_int_array->intarr;
	few_bits* answer_current_int_array_intarr = answer_current_int_array->intarr;
	many_bits int_num = 0, previous_int_num = 0;
	many_bits num_of_used_ints_subtract = num_to_subtract_copy.num_of_used_ints;
	many_bits int_num_of_start_of_current_intarr_answer = 0;
	many_bits next_stop = num_of_used_ints_subtract;
	if (subtract_current_int_array_intarr_len < next_stop) { next_stop = subtract_current_int_array_intarr_len; }
	if (this_current_int_array_intarr_len < next_stop) { next_stop = this_current_int_array_intarr_len; }
	if (answer_current_int_array_intarr_len < next_stop) { next_stop = answer_current_int_array_intarr_len; }
	many_bits remaining_intarr_len_this;
	many_bits remaining_intarr_len_answer;
	bool is_carry = false;
	answer_current_int_array->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (int_num == next_stop)
		{
			previous_int_num = int_num - previous_int_num;
			index_answer += previous_int_num;
			index_subtract += previous_int_num;
			index_this += previous_int_num;
			previous_int_num = int_num;
			if (int_num == num_of_used_ints_subtract) { break; }
			if (index_this == this_current_int_array_intarr_len)
			{
				it_this = it_this->next;
				index_this = 0;
				this_current_int_array = it_this->value;
				this_current_int_array_intarr_len = this_current_int_array->num_of_used_ints;
				this_current_int_array_intarr = this_current_int_array->intarr;
			}
			if (index_subtract == subtract_current_int_array_intarr_len)
			{
				it_subtract = it_subtract->next;
				index_subtract = 0;
				subtract_current_int_array = it_subtract->value;
				subtract_current_int_array_intarr_len = subtract_current_int_array->num_of_used_ints;
				subtract_current_int_array_intarr = subtract_current_int_array->intarr;
			}
			if (index_answer == answer_current_int_array_intarr_len)
			{
				int_num_of_start_of_current_intarr_answer = int_num;
				num_of_int_arrays_used_in_answer++;
				it_answer = it_answer->next;
				index_answer = 0;
				answer_current_int_array = it_answer->value;
				answer_current_int_array->set_num_of_used_ints_to_maximum();
				answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
				answer_current_int_array_intarr = answer_current_int_array->intarr;
			}
			many_bits remaining_intarr_len_subtract = subtract_current_int_array_intarr_len - index_subtract;
			remaining_intarr_len_this = this_current_int_array_intarr_len - index_this;
			remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
			bool subtract_smaller_than_this = remaining_intarr_len_subtract < remaining_intarr_len_this;
			bool answer_smaller_than_this = remaining_intarr_len_answer < remaining_intarr_len_this;
			bool answer_smaller_than_subtract = remaining_intarr_len_answer < remaining_intarr_len_subtract;
			if (subtract_smaller_than_this && !answer_smaller_than_subtract)
			{
				next_stop += remaining_intarr_len_subtract;
			}
			else if (answer_smaller_than_this && answer_smaller_than_subtract)
			{
				next_stop += remaining_intarr_len_answer;
			}
			else { next_stop += remaining_intarr_len_this; }
			continue;
		}
		many_bits_signed current_value_in_this;
		many_bits_signed current_value_in_subtract = *subtract_current_int_array_intarr;
		if (is_carry)
		{
			current_value_in_this = (many_bits_signed)(*this_current_int_array_intarr) - 1;
		}
		else { current_value_in_this = *this_current_int_array_intarr; }
		if (current_value_in_this < current_value_in_subtract)
		{
			*answer_current_int_array_intarr = current_value_in_this + MAX_few_bits_NUM_PLUS_ONE - current_value_in_subtract;
			is_carry = true;
		}
		else
		{
			*answer_current_int_array_intarr = current_value_in_this - current_value_in_subtract;
			is_carry = false;
		}
		++answer_current_int_array_intarr;
		++this_current_int_array_intarr;
		++subtract_current_int_array_intarr;
		++int_num;
	}
	if (int_num != num_of_used_ints_this)
	{
		if (index_this == this_current_int_array_intarr_len)
		{
			it_this = it_this->next;
			index_this = 0;
			this_current_int_array = it_this->value;
			this_current_int_array_intarr_len = this_current_int_array->num_of_used_ints;
			this_current_int_array_intarr = this_current_int_array->intarr;
		}
		if (index_answer == answer_current_int_array_intarr_len)
		{
			int_num_of_start_of_current_intarr_answer = int_num;
			num_of_int_arrays_used_in_answer++;
			it_answer = it_answer->next;
			index_answer = 0;
			answer_current_int_array = it_answer->value;
			answer_current_int_array->set_num_of_used_ints_to_maximum();
			answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
			answer_current_int_array_intarr = answer_current_int_array->intarr;
		}
		remaining_intarr_len_this = this_current_int_array_intarr_len - index_this;
		remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
		if (remaining_intarr_len_this < remaining_intarr_len_answer)
		{
			next_stop = remaining_intarr_len_this + int_num;
		}
		else { next_stop = remaining_intarr_len_answer + int_num; }
		if (is_carry)
		{
			while (true)
			{
				if (int_num == next_stop)
				{
					if (int_num == num_of_used_ints_this) { break; }
					previous_int_num = int_num - previous_int_num;
					index_answer += previous_int_num;
					index_this += previous_int_num;
					previous_int_num = int_num;
					if (index_this == this_current_int_array_intarr_len)
					{
						it_this = it_this->next;
						index_this = 0;
						this_current_int_array = it_this->value;
						this_current_int_array_intarr_len = this_current_int_array->num_of_used_ints;
						this_current_int_array_intarr = this_current_int_array->intarr;
					}
					if (index_answer == answer_current_int_array_intarr_len)
					{
						int_num_of_start_of_current_intarr_answer = int_num;
						num_of_int_arrays_used_in_answer++;
						it_answer = it_answer->next;
						index_answer = 0;
						answer_current_int_array = it_answer->value;
						answer_current_int_array->set_num_of_used_ints_to_maximum();
						answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
						answer_current_int_array_intarr = answer_current_int_array->intarr;
					}
					remaining_intarr_len_this = this_current_int_array_intarr_len - index_this;
					remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
					if (remaining_intarr_len_answer < remaining_intarr_len_this)
					{
						next_stop += remaining_intarr_len_answer;
					}
					else { next_stop += remaining_intarr_len_this; }
					continue;
				}
				if (!is_carry) { break; }
				few_bits current_value_in_this;
				current_value_in_this = *this_current_int_array_intarr;
				if (current_value_in_this == 0)
				{
					*answer_current_int_array_intarr = MAX_few_bits_NUM;
					is_carry = true;
				}
				else
				{
					*answer_current_int_array_intarr = current_value_in_this - 1;
					is_carry = false;
				}
				++this_current_int_array_intarr;
				++answer_current_int_array_intarr;
				++int_num;
			}
			previous_int_num = int_num - previous_int_num;
			index_answer += previous_int_num;
			index_this += previous_int_num;
			previous_int_num = int_num;
		}
		if (int_num != num_of_used_ints_this)
		{
			if (index_this == this_current_int_array_intarr_len)
			{
				it_this = it_this->next;
				index_this = 0;
				this_current_int_array = it_this->value;
				this_current_int_array_intarr_len = this_current_int_array->num_of_used_ints;
				this_current_int_array_intarr = this_current_int_array->intarr;
			}
			if (index_answer == answer_current_int_array_intarr_len)
			{
				int_num_of_start_of_current_intarr_answer = int_num;
				num_of_int_arrays_used_in_answer++;
				it_answer = it_answer->next;
				index_answer = 0;
				answer_current_int_array = it_answer->value;
				answer_current_int_array->set_num_of_used_ints_to_maximum();
				answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
				answer_current_int_array_intarr = answer_current_int_array->intarr;
			}
			remaining_intarr_len_this = this_current_int_array_intarr_len - index_this;
			remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
			if (remaining_intarr_len_this < remaining_intarr_len_answer)
			{
				next_stop = remaining_intarr_len_this + int_num;
			}
			else { next_stop = remaining_intarr_len_answer + int_num; }
			while (true)
			{
				if (int_num == next_stop)
				{
					previous_int_num = int_num - previous_int_num;
					index_answer += previous_int_num;
					index_this += previous_int_num;
					previous_int_num = int_num;
					if (int_num == num_of_used_ints_this) { break; }
					if (index_this == this_current_int_array_intarr_len)
					{
						it_this = it_this->next;
						index_this = 0;
						this_current_int_array = it_this->value;
						this_current_int_array_intarr_len = this_current_int_array->num_of_used_ints;
						this_current_int_array_intarr = this_current_int_array->intarr;
					}
					if (index_answer == answer_current_int_array_intarr_len)
					{
						int_num_of_start_of_current_intarr_answer = int_num;
						num_of_int_arrays_used_in_answer++;
						it_answer = it_answer->next;
						index_answer = 0;
						answer_current_int_array = it_answer->value;
						answer_current_int_array->set_num_of_used_ints_to_maximum();
						answer_current_int_array_intarr_len = answer_current_int_array->intarr_len;
						answer_current_int_array_intarr = answer_current_int_array->intarr;
					}
					remaining_intarr_len_this = this_current_int_array_intarr_len - index_this;
					remaining_intarr_len_answer = answer_current_int_array_intarr_len - index_answer;
					if (remaining_intarr_len_answer < remaining_intarr_len_this)
					{
						next_stop += remaining_intarr_len_answer;
					}
					else { next_stop += remaining_intarr_len_this; }
					continue;
				}
				*answer_current_int_array_intarr = *this_current_int_array_intarr;
				++this_current_int_array_intarr;
				++answer_current_int_array_intarr;
				++int_num;
			}
		}
	}
	answer->num_of_used_ints = int_num;
	answer->num_of_intarrays_used = num_of_int_arrays_used_in_answer;
	answer_current_int_array->num_of_used_ints = index_answer;
	answer->cutoff_leading_zeros(it_answer);
#if DEBUG_MODE == 2
	std::cout << "\nEnd of subtract:";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
	{
		std::cout << "\nThe inconsistencies were found at the end of subtract.";
	}
#endif
}
void unlimited_int::add(unlimited_int* num_to_add, unlimited_int* answer)
{
#if DEBUG_MODE == 2
	std::cout << "\nStart of add:";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_add->find_inconsistencies();
#endif
	if (num_to_add == this)
	{
		this->multiply((few_bits)2, answer);
		return;
	}
	if (this->num_of_intarrays_used == 0)
	{
		if (num_to_add != answer)
		{
			num_to_add->copy_to(*answer);
		}
		return;
	}
	if (num_to_add->num_of_intarrays_used == 0)
	{
		if (this != answer)
		{
			this->copy_to(*answer);
		}
		return;
	}
	few_bits num_of_negatives = 0;
	bool this_is_negative = this->is_negative, add_is_negative = num_to_add->is_negative, set_answer_to_negative;
	if (this_is_negative) { num_of_negatives++; }
	if (add_is_negative) { num_of_negatives++; }
	unlimited_int this_copy = *this;
	unlimited_int num_to_add_copy = *num_to_add;
	this_copy.is_negative = false;
	this_copy.auto_destroy = false;
	num_to_add_copy.is_negative = false;
	num_to_add_copy.auto_destroy = false;
	if (num_of_negatives == 2)
	{
		set_answer_to_negative = true;
	}
	else if (num_of_negatives == 1)
	{
		if (this_is_negative)
		{
			num_to_add_copy.subtract(&this_copy, answer);
		}
		else
		{
			this_copy.subtract(&num_to_add_copy, answer);
		}
		return;
	}
	else
	{
		set_answer_to_negative = false;
	}
	unlimited_int* smaller_num, * larger_num;
	char compare_result = this_copy.estimate_compare_to(num_to_add_copy);
	if (compare_result == 'L')
	{
		larger_num = &this_copy;
		smaller_num = &num_to_add_copy;
	}
	else
	{
		smaller_num = &this_copy;
		larger_num = &num_to_add_copy;
	}
	many_bits bigger_num_used_ints = larger_num->num_of_used_ints;
	many_bits max_num_of_ints_needed_for_answer = bigger_num_used_ints + 1;
	answer->intarrays.increase_until_num_of_ints(max_num_of_ints_needed_for_answer);
	Node* it_bigger = larger_num->intarrays.intarrays.first;
	Node* it_smaller = smaller_num->intarrays.intarrays.first;
	Node* it_answer = answer->intarrays.intarrays.first;
	int_array* current_intarray_for_bigger = it_bigger->value;
	int_array* current_intarray_for_smaller = it_smaller->value;
	int_array* current_intarray_for_answer = it_answer->value;
	many_bits num_array_in_answer = 0;
	many_bits num_int = 0;
	many_bits index_bigger = 0, index_smaller = 0, index_answer = 0;
	many_bits num_of_intarrays_used_for_answer = 1;
	many_bits carry = 0;
	few_bits* current_intarr_for_bigger = current_intarray_for_bigger->intarr;
	few_bits* current_intarr_for_smaller = current_intarray_for_smaller->intarr;
	few_bits* current_intarr_for_answer = current_intarray_for_answer->intarr;
	many_bits intarr_len_answer = current_intarray_for_answer->intarr_len;
	many_bits intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
	many_bits intarr_len_smaller = current_intarray_for_smaller->num_of_used_ints;
	many_bits smaller_num_used_ints = smaller_num->num_of_used_ints;
	many_bits next_stop = intarr_len_smaller;
	if (intarr_len_bigger < next_stop) { next_stop = intarr_len_bigger; }
	if (intarr_len_answer < next_stop) { next_stop = intarr_len_answer; }
	many_bits remaining_intarr_len_bigger;
	many_bits remaining_intarr_len_answer;
	many_bits previous_num_int = 0;
	current_intarray_for_answer->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int == next_stop) {
			previous_num_int = num_int - previous_num_int;
			index_bigger += previous_num_int;
			index_smaller += previous_num_int;
			index_answer += previous_num_int;
			previous_num_int = num_int;
			if (num_int == smaller_num_used_ints) { break; }
			if (index_smaller == intarr_len_smaller)
			{
				it_smaller = it_smaller->next;
				current_intarray_for_smaller = it_smaller->value;
				current_intarr_for_smaller = current_intarray_for_smaller->intarr;
				intarr_len_smaller = current_intarray_for_smaller->num_of_used_ints;
				index_smaller = 0;
			}
			if (index_bigger == intarr_len_bigger)
			{
				it_bigger = it_bigger->next;
				current_intarray_for_bigger = it_bigger->value;
				current_intarr_for_bigger = current_intarray_for_bigger->intarr;
				intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
				index_bigger = 0;
			}
			if (index_answer == intarr_len_answer)
			{
				num_of_intarrays_used_for_answer++;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarr_for_answer = current_intarray_for_answer->intarr;
				intarr_len_answer = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = 0;
			}
			many_bits remaining_intarr_len_smaller = intarr_len_smaller - index_smaller;
			remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
			remaining_intarr_len_answer = intarr_len_answer - index_answer;
			bool smaller_smaller_than_bigger = remaining_intarr_len_smaller < remaining_intarr_len_bigger;
			bool answer_smaller_than_bigger = remaining_intarr_len_answer < remaining_intarr_len_bigger;
			bool answer_smaller_than_smaller = remaining_intarr_len_answer < remaining_intarr_len_smaller;
			if (smaller_smaller_than_bigger && !answer_smaller_than_smaller)
			{
				next_stop += remaining_intarr_len_smaller;
			}
			else if (answer_smaller_than_bigger && answer_smaller_than_smaller)
			{
				next_stop += remaining_intarr_len_answer;
			}
			else { next_stop += remaining_intarr_len_bigger; }
			continue;
		}
		carry += ((many_bits)(*current_intarr_for_bigger)) + ((many_bits)(*current_intarr_for_smaller));
		*current_intarr_for_answer = carry;
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarr_for_bigger;
		++current_intarr_for_smaller;
		++current_intarr_for_answer;
		++num_int;
	}
	if (num_int != bigger_num_used_ints)
	{
		if (index_bigger == intarr_len_bigger)
		{
			it_bigger = it_bigger->next;
			current_intarray_for_bigger = it_bigger->value;
			current_intarr_for_bigger = current_intarray_for_bigger->intarr;
			intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
			index_bigger = 0;
		}
		if (index_answer == intarr_len_answer)
		{
			num_of_intarrays_used_for_answer++;
			it_answer = it_answer->next;
			current_intarray_for_answer = it_answer->value;
			current_intarr_for_answer = current_intarray_for_answer->intarr;
			intarr_len_answer = current_intarray_for_answer->intarr_len;
			current_intarray_for_answer->set_num_of_used_ints_to_maximum();
			index_answer = 0;
		}
		remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
		remaining_intarr_len_answer = intarr_len_answer - index_answer;
		if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
		{
			next_stop = remaining_intarr_len_bigger + num_int;
		}
		else { next_stop = remaining_intarr_len_answer + num_int; }
		if (bigger_num_used_ints < next_stop) { next_stop = bigger_num_used_ints; }
		while (carry != 0)
		{
			if (num_int == next_stop) {
				if (num_int == bigger_num_used_ints) { break; }
				previous_num_int = num_int - previous_num_int;
				index_bigger += previous_num_int;
				index_answer += previous_num_int;
				previous_num_int = num_int;
				if (index_bigger == intarr_len_bigger)
				{
					it_bigger = it_bigger->next;
					current_intarray_for_bigger = it_bigger->value;
					current_intarr_for_bigger = current_intarray_for_bigger->intarr;
					intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
					index_bigger = 0;
				}
				if (index_answer == intarr_len_answer)
				{
					num_of_intarrays_used_for_answer++;
					it_answer = it_answer->next;
					current_intarray_for_answer = it_answer->value;
					current_intarr_for_answer = current_intarray_for_answer->intarr;
					intarr_len_answer = current_intarray_for_answer->intarr_len;
					current_intarray_for_answer->set_num_of_used_ints_to_maximum();
					index_answer = 0;
				}
				remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
				remaining_intarr_len_answer = intarr_len_answer - index_answer;
				if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
				{
					next_stop += remaining_intarr_len_bigger;
				}
				else { next_stop += remaining_intarr_len_answer; }
				continue;
			}
			carry += ((many_bits)(*current_intarr_for_bigger));
			*current_intarr_for_answer = carry;
			carry >>= NUM_OF_BITS_few_bits;
			++current_intarr_for_bigger;
			++current_intarr_for_answer;
			++num_int;
		}
		previous_num_int = num_int - previous_num_int;
		index_bigger += previous_num_int;
		index_answer += previous_num_int;
		previous_num_int = num_int;
		if (num_int != bigger_num_used_ints)
		{
			if (index_bigger == intarr_len_bigger)
			{
				it_bigger = it_bigger->next;
				current_intarray_for_bigger = it_bigger->value;
				current_intarr_for_bigger = current_intarray_for_bigger->intarr;
				intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
				index_bigger = 0;
			}
			if (index_answer == intarr_len_answer)
			{
				num_of_intarrays_used_for_answer++;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarr_for_answer = current_intarray_for_answer->intarr;
				intarr_len_answer = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = 0;
			}
			remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
			remaining_intarr_len_answer = intarr_len_answer - index_answer;
			if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
			{
				next_stop = remaining_intarr_len_bigger + num_int;
			}
			else { next_stop = remaining_intarr_len_answer + num_int; }
			while (true)
			{
				if (num_int == next_stop) {
					previous_num_int = num_int - previous_num_int;
					index_bigger += previous_num_int;
					index_answer += previous_num_int;
					previous_num_int = num_int;
					if (num_int == bigger_num_used_ints) { break; }
					if (index_bigger == intarr_len_bigger)
					{
						it_bigger = it_bigger->next;
						current_intarray_for_bigger = it_bigger->value;
						current_intarr_for_bigger = current_intarray_for_bigger->intarr;
						intarr_len_bigger = current_intarray_for_bigger->num_of_used_ints;
						index_bigger = 0;
					}
					if (index_answer == intarr_len_answer)
					{
						num_of_intarrays_used_for_answer++;
						it_answer = it_answer->next;
						current_intarray_for_answer = it_answer->value;
						current_intarr_for_answer = current_intarray_for_answer->intarr;
						intarr_len_answer = current_intarray_for_answer->intarr_len;
						current_intarray_for_answer->set_num_of_used_ints_to_maximum();
						index_answer = 0;
					}
					remaining_intarr_len_bigger = intarr_len_bigger - index_bigger;
					remaining_intarr_len_answer = intarr_len_answer - index_answer;
					if (remaining_intarr_len_bigger < remaining_intarr_len_answer)
					{
						next_stop += remaining_intarr_len_bigger;
					}
					else { next_stop += remaining_intarr_len_answer; }
					continue;
				}
				*current_intarr_for_answer = *current_intarr_for_bigger;
				++current_intarr_for_answer;
				++current_intarr_for_bigger;
				num_int++;
			}
		}
	}
	if (carry == 0)
	{
		answer->num_of_used_ints = bigger_num_used_ints;
		current_intarray_for_answer->num_of_used_ints = index_answer;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
	else
	{
		if (index_answer == intarr_len_answer)
		{
			++num_of_intarrays_used_for_answer;
			it_answer = it_answer->next;
			current_intarray_for_answer = it_answer->value;
			index_answer = 0;
		}
		answer->num_of_used_ints = bigger_num_used_ints + 1;
		current_intarray_for_answer->intarr[index_answer] = (few_bits)carry;
		current_intarray_for_answer->num_of_used_ints = index_answer + 1;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
	answer->is_negative = set_answer_to_negative;
#if DEBUG_MODE == 2
	std::cout << "\nEnd of add:";
#endif
#if DEBUG_MODE > 0
	answer->find_inconsistencies();
#endif
}
void unlimited_int::assign(few_bits value_to_assign)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory();  this->auto_destroy = true; }
	if (value_to_assign == 0) { return; }
	else
	{
		this->intarrays.increase_until_num_of_ints(1);
		this->num_of_intarrays_used = 1;
		this->is_negative = false;
		this->num_of_used_ints = 1;
		this->intarrays.intarrays.first->value->assign(value_to_assign);
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"assign(few_ints value_to_assign)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void unlimited_int::assign(many_bits value_to_assign)
{
	if (this->auto_destroy) { this->flush(); }
	else { this->forget_memory();  this->auto_destroy = true; }
	if (value_to_assign == 0) { return; }
	else
	{
		this->is_negative = false;
		few_bits remainder = value_to_assign; //value_to_assign & MASK_LOW_BITS
		few_bits carry = value_to_assign >> NUM_OF_BITS_few_bits;
		if (carry != 0)
		{
			this->num_of_used_ints = 2;
			this->intarrays.increase_until_num_of_ints(2);
			Node* it = this->intarrays.intarrays.first;
			int_array* int_array_to_change = it->value;
			int_array_to_change->assign(remainder);
			if (int_array_to_change->intarr_len == 1)
			{
				this->num_of_intarrays_used = 2;
				it = it->next;
				it->value->assign(carry);
			}
			else
			{
				this->num_of_intarrays_used = 1;
				int_array_to_change->intarr[1] = carry;
				int_array_to_change->num_of_used_ints = 2;
			}
		}
		else
		{
			this->num_of_used_ints = 1;
			this->num_of_intarrays_used = 1;
			this->intarrays.increase_until_num_of_ints(1);
			this->intarrays.intarrays.first->value->assign(remainder);
		}
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"assign(many_bits value_to_assign)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void unlimited_int::multiply_both_only_one_array(few_bits num_to_mult, unlimited_int* answer)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_both_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	int_array* this_int_array = this->intarrays.intarrays.first->value;
	few_bits* this_intarr = this_int_array->intarr;
	int_array* answer_int_array = answer->intarrays.intarrays.first->value;
	few_bits* answer_intarr = answer_int_array->intarr;
	few_bits* stop_ptr = &this_intarr[this_int_array->num_of_used_ints - 1];
	many_bits carry = 0;
	while (this_intarr != stop_ptr)
	{
		carry += ((many_bits)(*this_intarr)) * num_to_mult_many;
		*answer_intarr = carry;
		carry >>= NUM_OF_BITS_few_bits;
		++this_intarr;
		++answer_intarr;
	}
	carry += ((many_bits)(*this_intarr)) * num_to_mult_many;
	*answer_intarr = carry;
	carry >>= NUM_OF_BITS_few_bits;
	++this_intarr;
	++answer_intarr;
	many_bits unum_of_used_ints_answer;
	if (carry == 0) { unum_of_used_ints_answer = this->num_of_used_ints; }
	else
	{
		unum_of_used_ints_answer = this->num_of_used_ints + 1;
		*answer_intarr = carry;
	}
	answer_int_array->num_of_used_ints = unum_of_used_ints_answer;
	answer->num_of_used_ints = unum_of_used_ints_answer;
	answer->num_of_intarrays_used = 1;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_both_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	answer->find_inconsistencies();
#endif
}
void unlimited_int::multiply_this_only_one_array(few_bits num_to_mult, unlimited_int* answer)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_this_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	Node* it_answer = answer->intarrays.intarrays.first;
	int_array* current_intarray_for_answer = it_answer->value;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits index_answer = 0;
	many_bits num_of_intarrays_used_for_answer = 1;
	many_bits carry = 0;
	many_bits current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
	few_bits* current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
	many_bits this_used_ints = this->num_of_used_ints;
	few_bits* current_intarray_for_this_intarr = this->intarrays.intarrays.first->value->intarr;
	many_bits stop_at = this_used_ints;
	if (current_intarray_for_answer_intarr_len < stop_at)
	{
		stop_at = current_intarray_for_answer_intarr_len;
	}
	current_intarray_for_answer->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_answer += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_used_ints) { break; }
			if (index_answer == current_intarray_for_answer_intarr_len)
			{
				num_of_intarrays_used_for_answer++;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = 0;
			}
			stop_at += current_intarray_for_answer_intarr_len - index_answer;
			if (this_used_ints < stop_at)
			{
				stop_at = this_used_ints;
			}
			continue;
		}
		carry += ((many_bits)(*current_intarray_for_this_intarr)) * num_to_mult_many;
		*current_intarray_for_answer_intarr = carry;
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarray_for_this_intarr;
		++current_intarray_for_answer_intarr;
		++num_int;
	}
	if (index_answer == current_intarray_for_answer_intarr_len)
	{
		++num_of_intarrays_used_for_answer;
		it_answer = it_answer->next;
		index_answer = 0;
		current_intarray_for_answer = it_answer->value;
	}
	if (carry == 0)
	{
		answer->num_of_used_ints = this_used_ints;
		current_intarray_for_answer->num_of_used_ints = index_answer;
		if (index_answer == 0)
		{
			answer->num_of_intarrays_used = num_of_intarrays_used_for_answer - 1;
		}
		else { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer; }
	}
	else
	{
		answer->num_of_used_ints = this_used_ints + 1;
		current_intarray_for_answer->intarr[index_answer] = (few_bits)carry;
		current_intarray_for_answer->num_of_used_ints = index_answer + 1;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_this_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	answer->find_inconsistencies();
	this->find_inconsistencies();
#endif
}
void unlimited_int::multiply_answer_only_one_array(few_bits num_to_mult, unlimited_int* answer)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_answer_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
#if DEBUG_MODE == 2
	unlimited_int ui_mult = num_to_mult;
	unlimited_int answer_kar = (*this) * ui_mult;
#endif
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	Node* it_this = this->intarrays.intarrays.first;
	int_array* current_intarray_for_this = it_this->value;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits index_this = 0;
	many_bits carry = 0;
	many_bits current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
	few_bits* current_intarray_for_this_intarr = current_intarray_for_this->intarr;
	many_bits this_used_ints = this->num_of_used_ints;
	int_array* answer_first_int_array = answer->intarrays.intarrays.first->value;
	few_bits* first_intarray_for_answer_intarr = answer_first_int_array->intarr;
	many_bits stop_at = this_used_ints;
	if (current_intarray_for_this_intarr_len < stop_at) { stop_at = current_intarray_for_this_intarr_len; }
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_used_ints) { break; }
			if (index_this == current_intarray_for_this_intarr_len)
			{
				it_this = it_this->next;
				current_intarray_for_this = it_this->value;
				current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
				current_intarray_for_this_intarr = current_intarray_for_this->intarr;
				index_this = 0;
			}
			stop_at += current_intarray_for_this_intarr_len - index_this;
			if (this_used_ints < stop_at) { stop_at = this_used_ints; }
			continue;
		}
		carry += ((many_bits)(*current_intarray_for_this_intarr)) * num_to_mult_many;
		*first_intarray_for_answer_intarr = carry;
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarray_for_this_intarr;
		++first_intarray_for_answer_intarr;
		++num_int;
	}
	if (carry == 0)
	{
		answer->num_of_used_ints = this_used_ints;
		answer_first_int_array->num_of_used_ints = this_used_ints;
	}
	else
	{
		answer->num_of_used_ints = this_used_ints + 1;
		*first_intarray_for_answer_intarr = carry;
		answer_first_int_array->num_of_used_ints = this_used_ints + 1;
	}
	answer->num_of_intarrays_used = 1;
#if DEBUG_MODE == 2
	if (answer_kar != *answer)
	{
		std::cout << "\nMistake in \"multiply_this_only_one_array\", because the answer given is: " << *answer << "  and the correct answer based on karatsuba is: " << answer_kar;
	}
#endif
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_answer_only_one_array(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies()) { std::cout << "I somehow ruined this in function \"multiply_answer_only_one_array\""; }
	if (answer->find_inconsistencies()) { std::cout << "I somehow ruined answer in function \"multiply_answer_only_one_array\""; }
#endif
}
void unlimited_int::multiply(few_bits num_to_mult, unlimited_int* answer)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if ((this->num_of_intarrays_used == 0) || (num_to_mult == 0))
	{
		answer->set_to_zero();
		return;
	}
	many_bits max_num_of_ints_needed_for_answer = this->num_of_used_ints + 1;
	answer->intarrays.increase_until_num_of_ints(max_num_of_ints_needed_for_answer);
	bool this_uses_only_one_array = this->num_of_intarrays_used == 1;
	bool answer_needs_only_one_array = max_num_of_ints_needed_for_answer <= answer->intarrays.intarrays.first->value->intarr_len;
	if (this_uses_only_one_array || answer_needs_only_one_array)
	{
		if (this_uses_only_one_array && answer_needs_only_one_array)
		{
			this->multiply_both_only_one_array(num_to_mult, answer);
		}
		else if (this_uses_only_one_array)
		{
			this->multiply_this_only_one_array(num_to_mult, answer);
		}
		else
		{
			this->multiply_answer_only_one_array(num_to_mult, answer);
		}
		answer->is_negative = this->is_negative;
		return;
	}
	answer->is_negative = this->is_negative;
	many_bits num_to_mult_many = (many_bits)num_to_mult;
	Node* it_this = this->intarrays.intarrays.first;
	Node* it_answer = answer->intarrays.intarrays.first;
	int_array* current_intarray_for_this = it_this->value;
	int_array* current_intarray_for_answer = it_answer->value;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits index_this = 0, index_answer = 0;
	many_bits num_of_intarrays_used_for_answer = 1;
	many_bits carry = 0;
	many_bits current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
	many_bits current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
	few_bits* current_intarray_for_this_intarr = current_intarray_for_this->intarr;
	few_bits* current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
	many_bits this_used_ints = this->num_of_used_ints;
	many_bits stop_at = this_used_ints;
	if (current_intarray_for_answer_intarr_len < stop_at)
	{
		stop_at = current_intarray_for_answer_intarr_len;
	}
	if (current_intarray_for_this_intarr_len < stop_at)
	{
		stop_at = current_intarray_for_this_intarr_len;
	}
	current_intarray_for_answer->set_num_of_used_ints_to_maximum();
	while (true)
	{
		if (num_int == stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this += previous_num_int;
			index_answer += previous_num_int;
			previous_num_int = num_int;
			if (num_int == this_used_ints) { break; }
			if (index_this == current_intarray_for_this_intarr_len)
			{
				it_this = it_this->next;
				current_intarray_for_this = it_this->value;
				current_intarray_for_this_intarr_len = current_intarray_for_this->num_of_used_ints;
				current_intarray_for_this_intarr = current_intarray_for_this->intarr;
				index_this = 0;
			}
			if (index_answer == current_intarray_for_answer_intarr_len)
			{
				++num_of_intarrays_used_for_answer;
				it_answer = it_answer->next;
				current_intarray_for_answer = it_answer->value;
				current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
				current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
				current_intarray_for_answer->set_num_of_used_ints_to_maximum();
				index_answer = 0;
			}
			many_bits remaining_length_of_this_intarr = current_intarray_for_this_intarr_len - index_this;
			many_bits remaining_length_of_answer_intarr = current_intarray_for_answer_intarr_len - index_answer;
			if (remaining_length_of_this_intarr < remaining_length_of_answer_intarr)
			{
				stop_at += remaining_length_of_this_intarr;
			}
			else { stop_at += remaining_length_of_answer_intarr; }
			if (this_used_ints < stop_at) { stop_at = this_used_ints; }
			continue;
		}
		carry += ((many_bits)(*current_intarray_for_this_intarr)) * num_to_mult_many;
		*current_intarray_for_answer_intarr = carry; //carry & MASK_LOW_BITS
		carry >>= NUM_OF_BITS_few_bits;
		++current_intarray_for_this_intarr;
		++current_intarray_for_answer_intarr;
		++num_int;
	}
	if (index_answer == current_intarray_for_answer_intarr_len)
	{
		++num_of_intarrays_used_for_answer;
		it_answer = it_answer->next;
		index_answer = 0;
		current_intarray_for_answer = it_answer->value;
	}
	if (carry == 0)
	{
		answer->num_of_used_ints = this_used_ints;
		current_intarray_for_answer->num_of_used_ints = index_answer;
		if (index_answer == 0) { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer - 1; }
		else { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer; }
	}
	else
	{
		answer->num_of_used_ints = max_num_of_ints_needed_for_answer;
		current_intarray_for_answer->intarr[index_answer] = (few_bits)carry;
		current_intarray_for_answer->num_of_used_ints = index_answer + 1;
		answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply(few_bits num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	answer->find_inconsistencies();
#endif
}
void unlimited_int::multiply_basecase(unlimited_int* num_to_mult, unlimited_int* answer)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"multiply_basecase(unlimited_int* num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
	num_to_mult->find_inconsistencies();
#endif
	if ((num_to_mult->num_of_intarrays_used == 0) || (this->num_of_intarrays_used == 0))
	{
		answer->set_to_zero();
		return;
	}
	bool answer_is_this = false, answer_is_mult = false;
	if (this == answer)
	{
		answer_is_this = true;
		answer = new unlimited_int;
	}
	else if (answer == num_to_mult)
	{
		answer_is_mult = true;
		answer = new unlimited_int;
	}
	if (this->is_negative != num_to_mult->is_negative) { answer->is_negative = true; }
	unlimited_int* smaller_num, * bigger_num;
	char value_of_comparison = this->estimate_compare_to(*num_to_mult);
	if (value_of_comparison == 'L')
	{
		bigger_num = this;
		smaller_num = num_to_mult;
	}
	else
	{
		bigger_num = num_to_mult;
		smaller_num = this;
	}
	many_bits smaller_num_of_used_ints = smaller_num->num_of_used_ints;
	many_bits bigger_used_ints = bigger_num->num_of_used_ints;
	many_bits num_of_ints_needed_for_answer = bigger_used_ints + smaller_num_of_used_ints;
	answer->intarrays.increase_until_num_of_ints(num_of_ints_needed_for_answer);
	answer->fill_0_until_num_of_ints_and_set_used_ints_to_maximum(num_of_ints_needed_for_answer);
	Node* it_smaller = smaller_num->intarrays.intarrays.first;
	int_array* current_int_array_for_smaller = it_smaller->value;
	many_bits current_int_array_len_for_smaller = current_int_array_for_smaller->num_of_used_ints;
	few_bits* current_actual_int_array_in_smaller = current_int_array_for_smaller->intarr;
	Node* it_start_answer = answer->intarrays.intarrays.first;
	int_array* current_int_array_for_start_answer = it_start_answer->value;
	many_bits current_int_array_len_for_start_answer = current_int_array_for_start_answer->intarr_len;
	few_bits* current_actual_int_array_in_start_answer = current_int_array_for_start_answer->intarr;
	many_bits num_of_intarrays_used_start_answer = 1;
	many_bits num_int_outer_loop = 0, previous_num_int_outer_loop = 0;
	many_bits next_stop_outer_loop;
	if (current_int_array_len_for_start_answer < current_int_array_len_for_smaller)
	{
		next_stop_outer_loop = current_int_array_len_for_start_answer;
	}
	else { next_stop_outer_loop = current_int_array_len_for_smaller; }
	if (smaller_num_of_used_ints < next_stop_outer_loop) { next_stop_outer_loop = smaller_num_of_used_ints; }
	many_bits index_smaller = 0, index_start_answer = 0;
	many_bits num_of_used_ints_in_most_significant_int_array = 0;
	int_array* current_intarray_for_answer = current_int_array_for_start_answer;
	bool last_digit_is_zero = true;
	while (true)
	{
		if (num_int_outer_loop == next_stop_outer_loop)
		{
			previous_num_int_outer_loop = num_int_outer_loop - previous_num_int_outer_loop;
			index_smaller += previous_num_int_outer_loop;
			previous_num_int_outer_loop = num_int_outer_loop;
			if (num_int_outer_loop == smaller_num_of_used_ints) { break; }
			if (index_smaller == current_int_array_len_for_smaller)
			{
				index_smaller = 0;
				it_smaller = it_smaller->next;
				current_int_array_for_smaller = it_smaller->value;
				current_int_array_len_for_smaller = current_int_array_for_smaller->num_of_used_ints;
				current_actual_int_array_in_smaller = current_int_array_for_smaller->intarr;
			}
			if (index_start_answer == current_int_array_len_for_start_answer)
			{
				index_start_answer = 0;
				it_start_answer = it_start_answer->next;
				current_int_array_for_start_answer = it_start_answer->value;
				current_int_array_len_for_start_answer = current_int_array_for_start_answer->intarr_len;
				current_actual_int_array_in_start_answer = current_int_array_for_start_answer->intarr;
				num_of_intarrays_used_start_answer++;
			}
			many_bits remaining_length_of_current_intarr_smaller = current_int_array_len_for_smaller - index_smaller;
			many_bits remaining_length_of_current_intarr_start_answer = current_int_array_len_for_start_answer - index_start_answer;
			if (remaining_length_of_current_intarr_smaller < remaining_length_of_current_intarr_start_answer)
			{
				next_stop_outer_loop += remaining_length_of_current_intarr_smaller;
			}
			else { next_stop_outer_loop += remaining_length_of_current_intarr_start_answer; }
			if (smaller_num_of_used_ints < next_stop_outer_loop) { next_stop_outer_loop = smaller_num_of_used_ints; }
			continue;
		}
		many_bits num_to_mult_many = *current_actual_int_array_in_smaller;
		if (num_to_mult_many != 0)
		{
			Node* it_bigger = bigger_num->intarrays.intarrays.first;
			Node* it_answer = it_start_answer;
			int_array* current_intarray_for_bigger = it_bigger->value;
			current_intarray_for_answer = current_int_array_for_start_answer;
			many_bits num_int = 0, previous_num_int = 0;
			many_bits index_bigger = 0, index_answer = index_start_answer;
			many_bits carry = 0;
			many_bits current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
			many_bits current_intarray_for_bigger_intarr_len = current_intarray_for_bigger->num_of_used_ints;
			few_bits* current_intarray_for_bigger_intarr = current_intarray_for_bigger->intarr;
			few_bits* current_intarray_for_answer_intarr = current_actual_int_array_in_start_answer;
			many_bits num_of_intarrays_used_for_answer = num_of_intarrays_used_start_answer;
			many_bits remaining_length_of_answer_intarr = current_intarray_for_answer_intarr_len - index_answer;
			many_bits stop_at;
			if (remaining_length_of_answer_intarr < current_intarray_for_bigger_intarr_len)
			{
				stop_at = remaining_length_of_answer_intarr;
			}
			else { stop_at = current_intarray_for_bigger_intarr_len; }
			if (bigger_used_ints < stop_at) { stop_at = bigger_used_ints; }
			while (true)
			{
				if (num_int == stop_at)
				{
					previous_num_int = num_int - previous_num_int;
					index_bigger += previous_num_int;
					index_answer += previous_num_int;
					previous_num_int = num_int;
					if (num_int == bigger_used_ints) { break; }
					if (index_bigger == current_intarray_for_bigger_intarr_len)
					{
						it_bigger = it_bigger->next;
						current_intarray_for_bigger = it_bigger->value;
						current_intarray_for_bigger_intarr_len = current_intarray_for_bigger->num_of_used_ints;
						current_intarray_for_bigger_intarr = current_intarray_for_bigger->intarr;
						index_bigger = 0;
					}
					if (index_answer == current_intarray_for_answer_intarr_len)
					{
						++num_of_intarrays_used_for_answer;
						it_answer = it_answer->next;
						current_intarray_for_answer = it_answer->value;
						current_intarray_for_answer_intarr_len = current_intarray_for_answer->intarr_len;
						current_intarray_for_answer_intarr = current_intarray_for_answer->intarr;
						index_answer = 0;
					}
					many_bits remaining_length_of_bigger_intarr = current_intarray_for_bigger_intarr_len - index_bigger;
					remaining_length_of_answer_intarr = current_intarray_for_answer_intarr_len - index_answer;
					if (remaining_length_of_bigger_intarr < remaining_length_of_answer_intarr)
					{
						stop_at += remaining_length_of_bigger_intarr;
					}
					else { stop_at += remaining_length_of_answer_intarr; }
					if (bigger_used_ints < stop_at) { stop_at = bigger_used_ints; }
					continue;
				}
				carry += ((many_bits)(*current_intarray_for_bigger_intarr)) * num_to_mult_many + ((many_bits)(*current_intarray_for_answer_intarr));
				*current_intarray_for_answer_intarr = carry;
				carry >>= NUM_OF_BITS_few_bits;
				++current_intarray_for_bigger_intarr;
				++current_intarray_for_answer_intarr;
				++num_int;
			}
			if (index_answer == current_intarray_for_answer_intarr_len)
			{
				++num_of_intarrays_used_for_answer;
				it_answer = it_answer->next;
				index_answer = 0;
				current_intarray_for_answer = it_answer->value;
			}
			if (carry == 0)
			{
				last_digit_is_zero = true;
				if (index_answer == 0) { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer - 1; }
				else { answer->num_of_intarrays_used = num_of_intarrays_used_for_answer; }
				num_of_used_ints_in_most_significant_int_array = index_answer;
			}
			else
			{
				last_digit_is_zero = false;
				current_intarray_for_answer->intarr[index_answer] = (few_bits)carry;
				answer->num_of_intarrays_used = num_of_intarrays_used_for_answer;
				num_of_used_ints_in_most_significant_int_array = index_answer + 1;
			}
		}
		++num_int_outer_loop;
		++current_actual_int_array_in_smaller;
		++current_actual_int_array_in_start_answer;
		++index_start_answer;
	}
	if (last_digit_is_zero) { answer->num_of_used_ints = num_of_ints_needed_for_answer - 1; }
	else { answer->num_of_used_ints = num_of_ints_needed_for_answer; }
	current_intarray_for_answer->num_of_used_ints = num_of_used_ints_in_most_significant_int_array;
	if (answer_is_this)
	{
		answer->swap(*this);
		answer->flush();
		delete answer;
	}
	else if (answer_is_mult)
	{
		answer->swap(*num_to_mult);
		answer->flush();
		delete answer;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"multiply_basecase(unlimited_int* num_to_mult, unlimited_int* answer)\":";
#endif
#if DEBUG_MODE > 0
	if (answer_is_this)
	{
		if (this->find_inconsistencies())
		{
			std::cout << "\nThe inconsistencies were found at the end of multiply_basecase with this.";
		}
	}
	else if (answer_is_mult)
	{
		if (num_to_mult->find_inconsistencies())
		{
			std::cout << "\nThe inconsistencies were found at the end of multiply_basecase with num_to_mult.";
		}
	}
	else
	{
		if (answer->find_inconsistencies())
		{
			std::cout << "\nThe inconsistencies were found at the end of multiply_basecase with answer.";
		}
	}
#endif
}
void unlimited_int::split_at_and_use_original(many_bits index, unlimited_int* high, unlimited_int* low)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"split_at_and_use_original(many_bits index, unlimited_int* high, unlimited_int* low)\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	many_bits size_of_this = this->num_of_used_ints;
	if (size_of_this == 0)
	{
		low->set_to_zero();
		high->set_to_zero();
		this->flush();
		return;
	}
	else if (index == 0)
	{
		this->swap(*high);
		this->flush();
		low->set_to_zero();
		return;
	}
	else if (index >= size_of_this)
	{
		this->swap(*low);
		this->flush();
		high->set_to_zero();
		return;
	}
	many_bits this_num_of_used_int_arrays = this->num_of_intarrays_used;
	Node* it_low = low->intarrays.intarrays.first;
	Node* it_this = this->intarrays.intarrays.first;
	many_bits num_of_ints_filled_lower = 0;
	many_bits num_of_taken_int_arrays_from_this = 0;
	many_bits size_to_make_high = size_of_this - index;
	many_bits sum_used = 0;
	while (true)
	{
		num_of_ints_filled_lower += it_this->value->intarr_len;
		sum_used += it_this->value->num_of_used_ints;
		++num_of_taken_int_arrays_from_this;
		if (sum_used >= index) { break; }
		it_this = it_this->next;
	}
	int_array* current_int_array_this = it_this->value;
	if (low->intarrays.intarrays.length == 0)
	{
		low->intarrays.intarrays.first = this->intarrays.intarrays.first;
		this->intarrays.intarrays.first = it_this->next;
		if (this->intarrays.intarrays.first == nullptr) { this->intarrays.intarrays.last = nullptr; }
		else { this->intarrays.intarrays.first->previous = nullptr; }
		low->intarrays.intarrays.last = it_this;
		low->intarrays.intarrays.last->next = nullptr;
	}
	else
	{
		Node* temp_node = it_this->next;
		it_this->next = low->intarrays.intarrays.first;
		low->intarrays.intarrays.first->previous = it_this;
		low->intarrays.intarrays.first = this->intarrays.intarrays.first;
		this->intarrays.intarrays.first = temp_node;
		if (this->intarrays.intarrays.first == nullptr) { this->intarrays.intarrays.last = nullptr; }
		else { this->intarrays.intarrays.first->previous = nullptr; }
	}
	this->intarrays.num_of_ints -= num_of_ints_filled_lower;
	low->intarrays.num_of_ints += num_of_ints_filled_lower;
	this->intarrays.intarrays.length -= num_of_taken_int_arrays_from_this;
	low->intarrays.intarrays.length += num_of_taken_int_arrays_from_this;
	low->num_of_intarrays_used = num_of_taken_int_arrays_from_this;
	low->num_of_used_ints = index;
	Node* it_high;
	many_bits num_of_ints_left_in_shared_int_array = sum_used - index;
	current_int_array_this->num_of_used_ints -= num_of_ints_left_in_shared_int_array;
	list_of_int_arrays high_prepend;
	if (num_of_ints_left_in_shared_int_array > 0)
	{
		many_bits stop_at, num_int = 0, previous_num_int = 0;
		high_prepend.increase_until_num_of_ints(num_of_ints_left_in_shared_int_array);
		few_bits* intarr_this = &current_int_array_this->intarr[current_int_array_this->num_of_used_ints];
		many_bits index_high = 0;
		it_high = high_prepend.intarrays.first;
		int_array* current_high_int_array = it_high->value;
		many_bits current_num_of_ints_in_high_int_array = current_high_int_array->intarr_len;
		current_high_int_array->set_num_of_used_ints_to_maximum();
		few_bits* high_intarr = current_high_int_array->intarr;
		if (current_num_of_ints_in_high_int_array < num_of_ints_left_in_shared_int_array)
		{
			stop_at = current_num_of_ints_in_high_int_array;
		}
		else { stop_at = num_of_ints_left_in_shared_int_array; }
		while (true)
		{
			if (num_int == stop_at)
			{
				previous_num_int = num_int - previous_num_int;
				index_high += previous_num_int;
				previous_num_int = num_int;
				if (num_int == num_of_ints_left_in_shared_int_array) { break; }
				if (index_high >= current_num_of_ints_in_high_int_array)
				{
					index_high = 0;
					it_high = it_high->next;
					current_high_int_array = it_high->value;
					current_high_int_array->set_num_of_used_ints_to_maximum();
					current_num_of_ints_in_high_int_array = current_high_int_array->intarr_len;
					high_intarr = current_high_int_array->intarr;
				}
				many_bits current_num_of_ints_left_in_high_int_array = current_num_of_ints_in_high_int_array - index_high;
				stop_at += current_num_of_ints_left_in_high_int_array;
				if (num_of_ints_left_in_shared_int_array < stop_at)
				{
					stop_at = num_of_ints_left_in_shared_int_array;
				}
				continue;
			}
			*high_intarr = *intarr_this;
			++num_int;
			++high_intarr;
			++intarr_this;
		}
		current_high_int_array->num_of_used_ints = index_high;
	}
	many_bits this_num_of_used_int_arrays_left = this_num_of_used_int_arrays - num_of_taken_int_arrays_from_this;
	if (this_num_of_used_int_arrays_left != 0)
	{
		many_bits num_of_transfered_int_arrays_to_high = 0, num_of_transfered_ints_to_high = 0;
		many_bits current_len_int_array_this;
		it_this = this->intarrays.intarrays.first;
		while (true)
		{
			num_of_transfered_ints_to_high += it_this->value->intarr_len;
			++num_of_transfered_int_arrays_to_high;
			if (num_of_transfered_int_arrays_to_high >= this_num_of_used_int_arrays_left) { break; }
			it_this = it_this->next;
		}
		if (high_prepend.intarrays.last == nullptr)
		{
			high_prepend.intarrays.first = this->intarrays.intarrays.first;
		}
		else
		{
			high_prepend.intarrays.last->next = this->intarrays.intarrays.first;
			this->intarrays.intarrays.first->previous = high_prepend.intarrays.last;
		}
		high_prepend.intarrays.last = it_this;
		this->intarrays.intarrays.first = it_this->next;
		high_prepend.intarrays.last->next = nullptr;
		if (this->intarrays.intarrays.first == nullptr) { this->intarrays.intarrays.last = nullptr; }
		else { this->intarrays.intarrays.first->previous = nullptr; }
		this->intarrays.intarrays.length -= num_of_transfered_int_arrays_to_high;
		high_prepend.intarrays.length += num_of_transfered_int_arrays_to_high;
		this->intarrays.num_of_ints -= num_of_transfered_ints_to_high;
		high_prepend.num_of_ints += num_of_transfered_ints_to_high;
	}
	high->num_of_intarrays_used = high_prepend.intarrays.length;
	high->intarrays.prepend(high_prepend);
	high->num_of_used_ints = size_to_make_high;
	if (it_low == nullptr) { it_low = low->intarrays.intarrays.last; }
	else { it_low = it_low->previous; }
	low->cutoff_leading_zeros(it_low);
	this->flush();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"split_at_and_use_original(many_bits index, unlimited_int* high, unlimited_int* low)\"";
#endif
#if DEBUG_MODE > 0
	if (high->find_inconsistencies() || low->find_inconsistencies())
	{
		std::cout << "\nThe inconsistencies were found at the end of split_at_and_use_original.";
	}
#endif
}
void unlimited_int::split_at(many_bits index, unlimited_int* high, unlimited_int* low)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"split_at(many_bits index, unlimited_int* low, unlimited_int* high)\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if (index >= this_num_of_used_ints)
	{
		this->copy_to(*low);
		high->set_to_zero();
		return;
	}
	else if (index == 0)
	{
		this->copy_to(*high);
		low->set_to_zero();
		return;
	}
	high->is_negative = false;
	low->is_negative = false;
	many_bits length_of_high = this_num_of_used_ints - index;
	many_bits length_of_low = index;
	low->intarrays.increase_until_num_of_ints(length_of_low);
	low->num_of_used_ints = length_of_low;
	high->intarrays.increase_until_num_of_ints(length_of_high);
	high->num_of_used_ints = length_of_high;
	Node* it_this = this->intarrays.intarrays.first;
	many_bits num_int = 0, previous_num_int = 0;
	many_bits this_index = 0;
	int_array* current_this_int_array = it_this->value;
	many_bits current_this_int_array_length = current_this_int_array->num_of_used_ints;
	Node* it_low = low->intarrays.intarrays.first;
	many_bits low_index = 0;
	int_array* current_low_int_array = it_low->value;
	current_low_int_array->set_num_of_used_ints_to_maximum();
	many_bits current_low_int_array_length = current_low_int_array->intarr_len;
	many_bits num_of_used_int_arrays_low = 1;
	few_bits* current_intarr_this = current_this_int_array->intarr;
	few_bits* current_intarr_low = current_low_int_array->intarr;
	many_bits stop_at;
	if (current_this_int_array_length < current_low_int_array_length)
	{
		stop_at = current_this_int_array_length;
	}
	else { stop_at = current_low_int_array_length; }
	if (length_of_low < stop_at) { stop_at = length_of_low; }
	while (true)
	{
		if (num_int >= stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			this_index += previous_num_int;
			low_index += previous_num_int;
			previous_num_int = num_int;
			if (num_int >= length_of_low) { break; }
			if (this_index >= current_this_int_array_length)
			{
				this_index = 0;
				it_this = it_this->next;
				current_this_int_array = it_this->value;
				current_this_int_array_length = current_this_int_array->num_of_used_ints;
				current_intarr_this = current_this_int_array->intarr;
			}
			if (low_index >= current_low_int_array_length)
			{
				++num_of_used_int_arrays_low;
				low_index = 0;
				it_low = it_low->next;
				current_low_int_array = it_low->value;
				current_low_int_array->set_num_of_used_ints_to_maximum();
				current_low_int_array_length = current_low_int_array->intarr_len;
				current_intarr_low = current_low_int_array->intarr;
			}
			many_bits num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
			many_bits num_of_ints_left_for_current_low = current_low_int_array_length - low_index;
			if (num_of_ints_left_for_current_low < num_of_ints_left_for_current_this)
			{
				stop_at += num_of_ints_left_for_current_low;
			}
			else { stop_at += num_of_ints_left_for_current_this; }
			if (length_of_low < stop_at) { stop_at = length_of_low; }
			continue;
		}
		*current_intarr_low = *current_intarr_this;
		++current_intarr_low;
		++current_intarr_this;
		++num_int;
	}
	current_low_int_array->num_of_used_ints = low_index;
	low->num_of_intarrays_used = num_of_used_int_arrays_low;
	low->cutoff_leading_zeros(it_low);
	Node* it_high = high->intarrays.intarrays.first;
	many_bits high_index = 0;
	int_array* current_high_int_array = it_high->value;
	current_high_int_array->set_num_of_used_ints_to_maximum();
	many_bits current_high_int_array_length = current_high_int_array->intarr_len;
	many_bits num_of_used_int_arrays_high = 1;
	few_bits* current_intarr_high = current_high_int_array->intarr;
	many_bits num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
	if (num_of_ints_left_for_current_this < current_high_int_array_length)
	{
		stop_at += num_of_ints_left_for_current_this;
	}
	else { stop_at += current_high_int_array_length; }
	if (this_num_of_used_ints < stop_at) { stop_at = this_num_of_used_ints; }
	while (true)
	{
		if (num_int >= stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			this_index += previous_num_int;
			high_index += previous_num_int;
			previous_num_int = num_int;
			if (num_int >= this_num_of_used_ints) { break; }
			if (this_index >= current_this_int_array_length)
			{
				this_index = 0;
				it_this = it_this->next;
				current_this_int_array = it_this->value;
				current_this_int_array_length = current_this_int_array->num_of_used_ints;
				current_intarr_this = current_this_int_array->intarr;
			}
			if (high_index >= current_high_int_array_length)
			{
				++num_of_used_int_arrays_high;
				high_index = 0;
				it_high = it_high->next;
				current_high_int_array = it_high->value;
				current_high_int_array->set_num_of_used_ints_to_maximum();
				current_high_int_array_length = current_high_int_array->intarr_len;
				current_intarr_high = current_high_int_array->intarr;
			}
			num_of_ints_left_for_current_this = current_this_int_array_length - this_index;
			many_bits num_of_ints_left_for_current_high = current_high_int_array_length - high_index;
			if (num_of_ints_left_for_current_high < num_of_ints_left_for_current_this)
			{
				stop_at += num_of_ints_left_for_current_high;
			}
			else { stop_at += num_of_ints_left_for_current_this; }
			if (this_num_of_used_ints < stop_at) { stop_at = this_num_of_used_ints; }
			continue;
		}
		*current_intarr_high = *current_intarr_this;
		++current_intarr_high;
		++current_intarr_this;
		++num_int;
	}
	high->num_of_intarrays_used = num_of_used_int_arrays_high;
	current_high_int_array->num_of_used_ints = high_index;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"split_at(many_bits index, unlimited_int* low, unlimited_int* high)\"";
#endif
#if DEBUG_MODE > 0
	if (high->find_inconsistencies() || low->find_inconsistencies() || this->find_inconsistencies())
	{
		std::cout << "\nThe inconsistencies were found at the end of split_at.";
	}
#endif
}
/*
import math
def size_base10(num):
	return len(str(num))
def split_at(num, length_of_low):
	str_num = str(num)
	low_str = str_num[-length_of_low:]
	high_str = str_num[:len(str_num) - length_of_low]
	return int(high_str), int(low_str)
def karatsuba(num1, num2):
	if (num1 < 10) or (num2 < 10):
		return (num1 * num2)

	#Calculates the size of the numbers.
	m = min(size_base10(num1), size_base10(num2))
	m2 = math.floor(m / 2)
	#m2 = ceil(m / 2) will also work

	#Split the digit sequences in the middle.
	high1, low1 = split_at(num1, m2)
	high2, low2 = split_at(num2, m2)

	#3 calls made to numbers approximately half the size.
	z0 = karatsuba(low1, low2)
	z1 = karatsuba((low1 + high1), (low2 + high2))
	z2 = karatsuba(high1, high2)

	return (z2 * (10 ** (m2 * 2))) + ((z1 - z2 - z0) * (10 ** m2)) + z0
*/
#define MIN_KARATSUBA 80
unlimited_int* unlimited_int::multiply_karatsuba(unlimited_int* num_to_mult)
{
	many_bits num_to_mult_num_of_used_ints = num_to_mult->num_of_used_ints;
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if ((num_to_mult_num_of_used_ints <= MIN_KARATSUBA) || (this_num_of_used_ints <= MIN_KARATSUBA))
	{
		unlimited_int* answer = new unlimited_int;
		this->multiply_basecase(num_to_mult, answer);
		return answer;
	}
	many_bits m2;
	if (num_to_mult_num_of_used_ints < this_num_of_used_ints) { m2 = num_to_mult_num_of_used_ints / 2; }
	else { m2 = this_num_of_used_ints / 2; }
	unlimited_int high1, low1, high2, low2;
	this->split_at(m2, &high1, &low1);
	num_to_mult->split_at(m2, &high2, &low2);
	unlimited_int* z0 = low1.multiply_karatsuba(&low2);
	//The next two commands will be saving space by reusing "low1" and "low2"
	low1.add(&high1, &low1); //now low1 = low1 + high1
	low2.add(&high2, &low2); //now low2 = low2 + high2
	//the following command destroys (sacrifices) low1 and low2
	unlimited_int* z1 = low1.multiply_karatsuba_destroy_this_and_num_to_mult(&low2);
	//the following command destroys (sacrifices) high1 and high2
	unlimited_int* z2 = high1.multiply_karatsuba_destroy_this_and_num_to_mult(&high2);
	//the next lines will basically do this: return (((z2 * (10 * *(m2 * 2))) + ((z1 - z2 - z0) * (10 * *m2)) + z0) // multiplied_by)
	z1->subtract(z2, z1);
	z1->subtract(z0, z1); //z1 will become z1 - z2 - z0
	z1->shift_left(m2);
	z2->shift_left(m2 * 2);
	z2->add(z1, z2);
	delete z1;
	z2->add(z0, z2); //z2 now holds the final answer to return
	delete z0;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in recursive function \"multiply karatuba\"";
#endif
#if DEBUG_MODE > 0
	z2->find_inconsistencies();
#endif
	return z2;
}
unlimited_int* unlimited_int::multiply_karatsuba_destroy_this_and_num_to_mult(unlimited_int* num_to_mult)
{
	many_bits num_to_mult_num_of_used_ints = num_to_mult->num_of_used_ints;
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if ((num_to_mult_num_of_used_ints <= MIN_KARATSUBA) || (this_num_of_used_ints <= MIN_KARATSUBA))
	{
		unlimited_int* answer = new unlimited_int;
		this->multiply_basecase(num_to_mult, answer);
		this->flush();
		num_to_mult->flush();
		return answer;
	}
	many_bits m2;
	if (num_to_mult_num_of_used_ints < this_num_of_used_ints) { m2 = num_to_mult_num_of_used_ints / 2; }
	else { m2 = this_num_of_used_ints / 2; }
	unlimited_int high1, low1, high2, low2;
	this->split_at_and_use_original(m2, &high1, &low1);
	num_to_mult->split_at_and_use_original(m2, &high2, &low2);
	unlimited_int* z0 = low1.multiply_karatsuba(&low2);
	//The next two commands will be saving space by reusing "low1" and "low2"
	low1.add(&high1, &low1); //now low1 = low1 + high1
	low2.add(&high2, &low2); //now low2 = low2 + high2
	//the following command destroys (sacrifices) low1 and low2
	unlimited_int* z1 = low1.multiply_karatsuba_destroy_this_and_num_to_mult(&low2);
	//the following command destroys (sacrifices) high1 and high2
	unlimited_int* z2 = high1.multiply_karatsuba_destroy_this_and_num_to_mult(&high2);
	//the next lines will basically do this: return (((z2 * (10 * *(m2 * 2))) + ((z1 - z2 - z0) * (10 * *m2)) + z0) // multiplied_by)
	z1->subtract(z2, z1);
	z1->subtract(z0, z1); //z1 will become z1 - z2 - z0
	z1->shift_left(m2);
	z2->shift_left(m2 * 2);
	z2->add(z1, z2);
	delete z1;
	z2->add(z0, z2); //z2 now holds the final answer to return
	delete z0;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in recursive function \"multiply karatuba\"";
#endif
#if DEBUG_MODE > 0
	z2->find_inconsistencies();
#endif
	return z2;
}
unlimited_int* unlimited_int::multiply_karatsuba_destroy_this(unlimited_int* num_to_mult)
{
	many_bits num_to_mult_num_of_used_ints = num_to_mult->num_of_used_ints;
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if ((num_to_mult_num_of_used_ints <= MIN_KARATSUBA) || (this_num_of_used_ints <= MIN_KARATSUBA))
	{
		unlimited_int* answer = new unlimited_int;
		this->multiply_basecase(num_to_mult, answer);
		this->flush();
		return answer;
	}
	many_bits m2;
	if (num_to_mult_num_of_used_ints < this_num_of_used_ints) { m2 = num_to_mult_num_of_used_ints / 2; }
	else { m2 = this_num_of_used_ints / 2; }
	unlimited_int high1, low1, high2, low2;
	num_to_mult->split_at(m2, &high2, &low2);
	this->split_at_and_use_original(m2, &high1, &low1);
	unlimited_int* z0 = low1.multiply_karatsuba(&low2);
	//The next two commands will be saving space by reusing "low1" and "low2"
	low1.add(&high1, &low1); //now low1 = low1 + high1
	low2.add(&high2, &low2); //now low2 = low2 + high2
	//the following command destroys (sacrifices) low1 and low2
	unlimited_int* z1 = low1.multiply_karatsuba_destroy_this_and_num_to_mult(&low2);
	//the following command destroys (sacrifices) high1 and high2
	unlimited_int* z2 = high1.multiply_karatsuba_destroy_this_and_num_to_mult(&high2);
	//the next lines will basically do this: return (((z2 * (10 * *(m2 * 2))) + ((z1 - z2 - z0) * (10 * *m2)) + z0) // multiplied_by)
	z1->subtract(z2, z1);
	z1->subtract(z0, z1); //z1 will become z1 - z2 - z0
	z1->shift_left(m2);
	z2->shift_left(m2 * 2);
	z2->add(z1, z2);
	delete z1;
	z2->add(z0, z2); //z2 now holds the final answer to return
	delete z0;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in recursive function \"multiply karatuba\"";
#endif
#if DEBUG_MODE > 0
	z2->find_inconsistencies();
#endif
	return z2;
}
#define MIN_KARATSUBA_SQUARING 80
unlimited_int* unlimited_int::power2()
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of recursive function \"power2\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if (this->num_of_used_ints <= MIN_KARATSUBA_SQUARING)
	{
		unlimited_int* answer = new unlimited_int;
		this->multiply_basecase(this, answer);
		return answer;
	}
	unlimited_int high, low;
	many_bits m2 = this->num_of_used_ints / 2;
	this->split_at(m2, &high, &low);
	unlimited_int* square_of_high = high.power2();
	unlimited_int* square_of_low = low.power2();
	square_of_high->shift_left(2 * m2);
	unlimited_int* answer = *square_of_high + *square_of_low;
	delete square_of_high;
	delete square_of_low;
	unlimited_int* high_times_low = high.multiply_karatsuba_destroy_this_and_num_to_mult(&low);
	*high_times_low <<= (((many_bits)1) + (m2 * NUM_OF_BITS_few_bits));
	*answer += *high_times_low;
	delete high_times_low;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of recursive function \"power2\"";
#endif
#if DEBUG_MODE > 0
	answer->find_inconsistencies();
	this->find_inconsistencies();
#endif
	return answer;
}
unlimited_int* unlimited_int::power2_destroy_this()
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of recursive function \"power2_destroy_this\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if (this->num_of_used_ints <= MIN_KARATSUBA_SQUARING)
	{
		unlimited_int* answer = new unlimited_int;
		this->multiply_basecase(this, answer);
		this->flush();
		return answer;
	}
	unlimited_int high, low;
	many_bits m2 = this->num_of_used_ints / 2;
	this->split_at_and_use_original(m2, &high, &low);
	unlimited_int* square_of_high = high.power2();
	unlimited_int* square_of_low = low.power2();
	square_of_high->shift_left(2 * m2);
	unlimited_int* answer = *square_of_high + *square_of_low;
	delete square_of_high;
	delete square_of_low;
	unlimited_int* high_times_low = high.multiply_karatsuba_destroy_this_and_num_to_mult(&low);
	*high_times_low <<= (((many_bits)1) + (m2 * NUM_OF_BITS_few_bits));
	*answer += *high_times_low;
	delete high_times_low;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of recursive function \"power2_destroy_this\"";
#endif
#if DEBUG_MODE > 0
	answer->find_inconsistencies();
	this->find_inconsistencies();
#endif
	return answer;
}
void unlimited_int::shift_right_one_by_one(many_bits shift_by) //basically divide by 0x10000000000000000 (if you're in 64 bits mode) and ignore the remainder
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_right_one_by_one(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in start of function \"shift_right_one_by_one(many_bits shift_by)\"";
	}
#endif
	many_bits this_num_of_used_ints = this->num_of_used_ints;
	if ((this->num_of_intarrays_used == 0) || (shift_by == 0)) { return; }
	if (shift_by >= this_num_of_used_ints) { this->set_to_zero(); return; }
	//Start by finding the index of the most insignificant digit of the newly to-be-created shifted number
	Node* it_read = this->intarrays.intarrays.first;
	Node* it_write = it_read;
	many_bits used_ints_counter = 0;
	int_array* current_read_int_array;
	many_bits current_length_read_int_array;
	while (true)
	{
		current_read_int_array = it_read->value;
		current_length_read_int_array = current_read_int_array->num_of_used_ints;
		many_bits sum = used_ints_counter + current_length_read_int_array;
		if (sum > shift_by) { break; }
		used_ints_counter = sum;
		it_read = it_read->next;
	}
	many_bits index_read = shift_by - used_ints_counter;
	many_bits index_write = 0;
	int_array* current_write_int_array = it_write->value;
	many_bits current_length_write_int_array = current_write_int_array->intarr_len;
	current_write_int_array->set_num_of_used_ints_to_maximum();
	many_bits int_num = 0, stop_next, length_of_shifted_num = this_num_of_used_ints - shift_by;
	many_bits previous_int_num = 0;
	many_bits len_left_for_current_write_intarr = current_length_write_int_array;
	many_bits len_left_for_current_read_intarr = current_length_read_int_array - index_read;
	if (len_left_for_current_write_intarr < len_left_for_current_read_intarr)
	{
		stop_next = len_left_for_current_write_intarr;
	}
	else { stop_next = len_left_for_current_read_intarr; }
	if (length_of_shifted_num < stop_next) { stop_next = length_of_shifted_num; }
	many_bits new_num_of_used_int_arrays = 1;
	few_bits* current_intarr_read = &current_read_int_array->intarr[index_read];
	few_bits* current_intarr_write = current_write_int_array->intarr;
	while (true)
	{
		if (int_num >= stop_next)
		{
			previous_int_num = int_num - previous_int_num;
			index_read += previous_int_num;
			index_write += previous_int_num;
			previous_int_num = int_num;
			if (int_num >= length_of_shifted_num) { break; }
			if (index_read >= current_length_read_int_array)
			{
				index_read = 0;
				it_read = it_read->next;
				current_read_int_array = it_read->value;
				current_length_read_int_array = current_read_int_array->num_of_used_ints;
				current_intarr_read = current_read_int_array->intarr;
			}
			if (index_write >= current_length_write_int_array)
			{
				++new_num_of_used_int_arrays;
				index_write = 0;
				it_write = it_write->next;
				current_write_int_array = it_write->value;
				current_write_int_array->set_num_of_used_ints_to_maximum();
				current_length_write_int_array = current_write_int_array->intarr_len;
				current_intarr_write = current_write_int_array->intarr;
			}
			len_left_for_current_write_intarr = current_length_write_int_array - index_write;
			len_left_for_current_read_intarr = current_length_read_int_array - index_read;
			if (len_left_for_current_write_intarr < len_left_for_current_read_intarr)
			{
				stop_next += len_left_for_current_write_intarr;
			}
			else { stop_next += len_left_for_current_read_intarr; }
			if (length_of_shifted_num < stop_next) { stop_next = length_of_shifted_num; }
			continue;
		}
		*current_intarr_write = *current_intarr_read;
		++int_num;
		++current_intarr_write;
		++current_intarr_read;
	}
	this->num_of_used_ints = length_of_shifted_num;
	this->num_of_intarrays_used = new_num_of_used_int_arrays;
	current_write_int_array->num_of_used_ints = index_write;
	this->flush_unused();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_right_one_by_one(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function \"shift_right_one_by_one(many_bits shift_by)\"";
	}
#endif
}
void unlimited_int::shift_left(many_bits shift_by) //basically multiply by (0x10000000000000000 ^ shift_by) (if you're in 64 bits mode)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_left(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if ((this->num_of_intarrays_used == 0) || (shift_by == 0)) { return; }
	list_of_int_arrays list_to_prepend;
	list_to_prepend.increase_until_num_of_ints(shift_by);
	list_to_prepend.fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(shift_by);
	this->num_of_intarrays_used += list_to_prepend.intarrays.length;
	this->intarrays.prepend(list_to_prepend);
	this->num_of_used_ints += shift_by;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_left(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
few_bits unlimited_int::generate_random_few_bits()
{
#if IS_64_BIT_SYSTEM
	return (((((few_bits)rand()) & 0xf))
		+ ((((few_bits)rand()) & 0xf) << 4)
		+ ((((few_bits)rand()) & 0xf) << 8)
		+ ((((few_bits)rand()) & 0xf) << 12)
		+ ((((few_bits)rand()) & 0xf) << 16)
		+ ((((few_bits)rand()) & 0xf) << 20)
		+ ((((few_bits)rand()) & 0xf) << 24)
		+ ((((few_bits)rand()) & 0xf) << 28));
#else
	return (((((few_bits)rand()) & 0xf))
		+ ((((few_bits)rand()) & 0xf) << 4)
		+ ((((few_bits)rand()) & 0xf) << 8)
		+ ((((few_bits)rand()) & 0xf) << 12));
#endif
}
many_bits unlimited_int::generate_random_many_bits()
{
#if IS_64_BIT_SYSTEM
	return (((((many_bits)rand()) & 0xf))
		+ ((((many_bits)rand()) & 0xf) << 4)
		+ ((((many_bits)rand()) & 0xf) << 8)
		+ ((((many_bits)rand()) & 0xf) << 12)
		+ ((((many_bits)rand()) & 0xf) << 16)
		+ ((((many_bits)rand()) & 0xf) << 20)
		+ ((((many_bits)rand()) & 0xf) << 24)
		+ ((((many_bits)rand()) & 0xf) << 28)
		+ ((((many_bits)rand()) & 0xf) << 32)
		+ ((((many_bits)rand()) & 0xf) << 36)
		+ ((((many_bits)rand()) & 0xf) << 40)
		+ ((((many_bits)rand()) & 0xf) << 44)
		+ ((((many_bits)rand()) & 0xf) << 48)
		+ ((((many_bits)rand()) & 0xf) << 52)
		+ ((((many_bits)rand()) & 0xf) << 56)
		+ ((((many_bits)rand()) & 0xf) << 60));
#else
	return (((((many_bits)rand()) & 0xf))
		+ ((((many_bits)rand()) & 0xf) << 4)
		+ ((((many_bits)rand()) & 0xf) << 8)
		+ ((((many_bits)rand()) & 0xf) << 12)
		+ ((((many_bits)rand()) & 0xf) << 16)
		+ ((((many_bits)rand()) & 0xf) << 20)
		+ ((((many_bits)rand()) & 0xf) << 24)
		+ ((((many_bits)rand()) & 0xf) << 28));
#endif
}
void unlimited_int::randomize(many_bits num_of_digits)
{
	if (num_of_digits == 0) { this->set_to_zero(); return; }
	this->intarrays.increase_until_num_of_ints(num_of_digits);
	this->num_of_used_ints = num_of_digits;
	Node* it = this->intarrays.intarrays.first;
	int_array* current_int_array = it->value;
	current_int_array->set_num_of_used_ints_to_maximum();
	many_bits current_length_int_array = current_int_array->intarr_len;
	few_bits* current_intarr = current_int_array->intarr;
	many_bits num_int = 0, stop_at, index_this = 0, this_num_of_intarrays_used = 1, previous_num_int = 0;
	if (current_length_int_array < num_of_digits) { stop_at = current_length_int_array; }
	else { stop_at = num_of_digits; }
	while (true)
	{
		if (num_int >= stop_at)
		{
			previous_num_int = num_int - previous_num_int;
			index_this += previous_num_int;
			previous_num_int = num_int;
			if (num_int >= num_of_digits) { break; }
			if (index_this >= current_length_int_array)
			{
				++this_num_of_intarrays_used;
				index_this = 0;
				it = it->next;
				current_int_array = it->value;
				current_int_array->set_num_of_used_ints_to_maximum();
				current_length_int_array = current_int_array->intarr_len;
				current_intarr = current_int_array->intarr;
			}
			many_bits num_of_ints_left = current_length_int_array - index_this;
			stop_at += num_of_ints_left;
			if (num_of_digits < stop_at) { stop_at = num_of_digits; }
			continue;
		}
		*current_intarr = unlimited_int::generate_random_few_bits();
		++current_intarr;
		++num_int;
	}
	current_int_array->num_of_used_ints = index_this;
	this->num_of_intarrays_used = this_num_of_intarrays_used;
	this->cutoff_leading_zeros(it);
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"randomize(many_bits num_of_digits)\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
unlimited_int* unlimited_int::operator*(unlimited_int& other)
{
	unlimited_int* answer = this->multiply_karatsuba(&other);
	if (this->num_of_used_ints == 0) { answer->is_negative = false; }
	else if (this->is_negative != other.is_negative) { answer->is_negative = true; }
	return answer;
}
unlimited_int* unlimited_int::operator/(unlimited_int& denominator)
{
	if (denominator.num_of_used_ints == 0) { return (new unlimited_int(((few_bits_signed)0))); }
	unlimited_int* answer = this->divide_by(denominator);
	if (this->num_of_used_ints == 0) { answer->is_negative = false; }
	else if (this->is_negative != denominator.is_negative) { answer->is_negative = true; }
	return answer;
}
unlimited_int* unlimited_int::operator*(few_bits num)
{
	unlimited_int* answer = new unlimited_int;
	this->multiply(num, answer);
	return answer;
}
unlimited_int* operator*(few_bits num, unlimited_int& ui)
{
	unlimited_int* answer = new unlimited_int;
	ui.multiply(num, answer);
	return answer;
}
void unlimited_int::operator*=(unlimited_int& other)
{
	unlimited_int* answer = this->multiply_karatsuba_destroy_this(&other);
	(*this) = answer;
}
unlimited_int* unlimited_int::operator+(unlimited_int& other)
{
	unlimited_int* answer = new unlimited_int;
	this->add(&other, answer);
	return answer;
}
unlimited_int* unlimited_int::operator-(unlimited_int& other)
{
	unlimited_int* answer = new unlimited_int;
	this->subtract(&other, answer);
	return answer;
}
void unlimited_int::operator++()
{
#if DEBUG_MODE == 2
	std::cout << "Finding inconsistencies in start of function \"unlimited_int::operator++()\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if (this->num_of_used_ints == 0) { this->assign((few_bits)1); return; }
	few_bits* current_intarr = this->intarrays.intarrays.first->value->intarr;
	if (current_intarr[0] < MAX_few_bits_NUM) { ++current_intarr[0]; return; }
	else
	{
		unlimited_int one((few_bits)1);
		this->add(&one, this);
	}
#if DEBUG_MODE == 2
	std::cout << "Finding inconsistencies in end of function \"unlimited_int::operator++()\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void unlimited_int::operator--()
{
#if DEBUG_MODE == 2
	std::cout << "Finding inconsistencies in start of function \"unlimited_int::operator--()\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if (this->num_of_used_ints == 0) { this->assign((few_bits_signed)-1); return; }
	few_bits* current_intarr = this->intarrays.intarrays.first->value->intarr;
	if (current_intarr[0] > 0) { --current_intarr[0]; return; }
	else
	{
		unlimited_int one((few_bits)1);
		this->subtract(&one, this);
	}
#if DEBUG_MODE == 2
	std::cout << "Finding inconsistencies in end of function \"unlimited_int::operator--()\"";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
unlimited_int* unlimited_int::operator%(unlimited_int& ui)
{
	unlimited_int* answer_divide = this->divide_by(ui);
	unlimited_int* answer_multiply = (*answer_divide) * ui;
	delete answer_divide;
	unlimited_int* answer = new unlimited_int;
	this->subtract(answer_multiply, answer);
	delete answer_multiply;
	return answer;
}
void unlimited_int::operator%=(unlimited_int& ui)
{
	unlimited_int* answer_divide = this->divide_by(ui);
	unlimited_int* answer_multiply = (*answer_divide) * ui;
	delete answer_divide;
	this->subtract(answer_multiply, this);
	delete answer_multiply;
}
unlimited_int* unlimited_int::operator+(many_bits num)
{
	unlimited_int num_ui(num);
	unlimited_int* answer = new unlimited_int;
	this->add(&num_ui, answer);
	return answer;
}
unlimited_int* operator+(many_bits num, unlimited_int& ui)
{
	unlimited_int num_ui(num);
	unlimited_int* answer = new unlimited_int;
	ui.add(&num_ui, answer);
	return answer;
}
unlimited_int* operator-(many_bits num, unlimited_int& ui)
{
	unlimited_int num_ui(num);
	unlimited_int* answer = new unlimited_int;
	ui.subtract(&num_ui, answer);
	return answer;
}
/*
std::ostream& operator<<(std::ostream& os, const unlimited_int& ui)
{
	if (ui.num_of_intarrays_used == 0) { os << "0"; }
	else
	{
		if (ui.is_negative) { os << '-'; }
		many_bits_signed num_arr_counter = ui.intarrays.intarrays.length - 1;
		Node* it;
		for (it = ui.intarrays.intarrays.last; it != nullptr; it = it->previous, num_arr_counter--)
		{
			if (num_arr_counter < ui.num_of_intarrays_used)
			{
				char* str_of_int_array = it->value->create_string();
				os << str_of_int_array;
				delete[] str_of_int_array;
			}
		}
	}
	return os;
}
*/
std::ostream& operator<<(std::ostream& os, unlimited_int& ui)
{
	char* str_of_this = ui.to_string();
	os << str_of_this;
	delete[]str_of_this;
	return os;
}
void unlimited_int::cutoff_leading_zeros(Node* int_array_to_start_at)
{
	if (int_array_to_start_at == nullptr) { return; }
	many_bits num_of_int_arrays_went_through = 0;
	many_bits num_of_ints_went_through = 0;
	Node* it = int_array_to_start_at;
	while (it != nullptr)
	{
		int_array* current_int_array = it->value;
		if (current_int_array->is_all_used_zeros())
		{
			num_of_ints_went_through += current_int_array->num_of_used_ints;
			++num_of_int_arrays_went_through;
		}
		else
		{
			many_bits_signed first_not_zero = current_int_array->find_first_used_not_zero();
			if (first_not_zero == -1)
			{
				num_of_ints_went_through += current_int_array->num_of_used_ints;
				++num_of_int_arrays_went_through;
			}
			else
			{
				num_of_ints_went_through += current_int_array->num_of_used_ints;
				current_int_array->num_of_used_ints = first_not_zero + 1;
				num_of_ints_went_through -= current_int_array->num_of_used_ints;
				break;
			}
		}
		it = it->previous;
	}
	this->num_of_intarrays_used -= num_of_int_arrays_went_through;
	this->num_of_used_ints -= num_of_ints_went_through;
	this->flush_unused();
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"cutoff_leading_zeros(Node* int_array_to_start_at)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe error was found in end of function \"cutoff_leading_zeros(Node* int_array_to_start_at)\"";
	}
#endif
}
void unlimited_int::shift_left_by_bits(many_bits num_of_bits_to_shift_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_left_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe error was found in start of function \"shift_left_by_bits()\"";
	}
#endif
	if (this->num_of_used_ints == 0 || num_of_bits_to_shift_by == 0) { return; }
	this->shift_left(num_of_bits_to_shift_by / NUM_OF_BITS_few_bits);
	if (this->num_of_used_ints == 0) { return; }
	register const int micro_shift = num_of_bits_to_shift_by % NUM_OF_BITS_few_bits;
	if (micro_shift != 0)
	{
		const int amount_to_shift_remainder = NUM_OF_BITS_few_bits - micro_shift;
		few_bits mask_of_shift_builder = 0;
		for (int bit_counter = NUM_OF_BITS_few_bits - 1; bit_counter >= amount_to_shift_remainder; --bit_counter)
		{
			mask_of_shift_builder += 1 << bit_counter;
		}
		register const few_bits mask_of_shift = mask_of_shift_builder;
		few_bits remainder = 0;
		this->flush_unused();
		Node* it_this = this->intarrays.intarrays.first;
		while (it_this != nullptr)
		{
			int_array* current_int_array = it_this->value;
			register few_bits* it_ptr = current_int_array->intarr;
			const register few_bits* stop_at = &current_int_array->intarr[current_int_array->num_of_used_ints - 1];
			register few_bits current_value;
			while (it_ptr != stop_at)
			{
				current_value = *it_ptr;
				*it_ptr <<= micro_shift;
				*it_ptr += remainder;
				remainder = (current_value & mask_of_shift) >> amount_to_shift_remainder;
				++it_ptr;
			}
			current_value = *it_ptr;
			*it_ptr <<= micro_shift;
			*it_ptr += remainder;
			remainder = (current_value & mask_of_shift) >> amount_to_shift_remainder;
			it_this = it_this->next;
		}
		if (remainder != 0)
		{
			int_array* last_int_array = this->intarrays.intarrays.last->value;
			if (last_int_array->is_full())
			{
				this->intarrays.increase_by_one_array_from_piggy_bank();
				++this->num_of_intarrays_used;
				last_int_array = this->intarrays.intarrays.last->value;
				last_int_array->num_of_used_ints = 0;
			}
			++this->num_of_used_ints;
			last_int_array->intarr[last_int_array->num_of_used_ints] = remainder;
			++last_int_array->num_of_used_ints;
		}
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_left_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe error was found in end of function \"shift_left_by_bits()\"";
	}
#endif
}
void unlimited_int::shift_right_by_bits(many_bits num_of_bits_to_shift_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_right_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe error was found in start of function \"shift_right_by_bits()\"";
	}
#endif
	if (this->num_of_used_ints == 0 || num_of_bits_to_shift_by == 0) { return; }
	this->shift_right(num_of_bits_to_shift_by / NUM_OF_BITS_few_bits);
	if (this->num_of_used_ints == 0) { return; }
	register const int micro_shift = num_of_bits_to_shift_by % NUM_OF_BITS_few_bits;
	if (micro_shift != 0)
	{
		const int amount_to_shift_remainder = NUM_OF_BITS_few_bits - micro_shift;
		few_bits mask_of_shift_builder = 0;
		for (int bit_counter = 0; bit_counter < micro_shift; ++bit_counter)
		{
			mask_of_shift_builder += 1 << bit_counter;
		}
		register const few_bits mask_of_shift = mask_of_shift_builder;
		few_bits remainder = 0;
		this->flush_unused();
		Node* it_this = this->intarrays.intarrays.last;
		while (it_this != nullptr)
		{
			int_array* current_int_array = it_this->value;
			register few_bits* it_ptr = &current_int_array->intarr[current_int_array->num_of_used_ints - 1];
			const register few_bits* stop_at = current_int_array->intarr;
			register few_bits current_value;
			while (it_ptr != stop_at)
			{
				current_value = *it_ptr;
				*it_ptr >>= micro_shift;
				*it_ptr += remainder;
				remainder = (current_value & mask_of_shift) << amount_to_shift_remainder;
				--it_ptr;
			}
			current_value = *it_ptr;
			*it_ptr >>= micro_shift;
			*it_ptr += remainder;
			remainder = (current_value & mask_of_shift) << amount_to_shift_remainder;
			it_this = it_this->previous;
		}
		this->cutoff_leading_zeros(this->intarrays.intarrays.last);
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_right_by_bits()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe error was found in end of function \"shift_right_by_bits()\"";
	}
#endif
}
void unlimited_int::shift_right(many_bits shift_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"shift_right(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in start of function \"shift_right(many_bits shift_by)\"";
	}
#endif
	if (shift_by == 0) { return; }
	if (shift_by >= this->num_of_used_ints) { this->set_to_zero(); return; }
	Node* it_this = this->intarrays.intarrays.first;
	many_bits num_of_ints_went_through = 0, num_of_intarrays_went_through = 0;
	while (true)
	{
		many_bits sum = num_of_ints_went_through + it_this->value->num_of_used_ints;
		if (sum > shift_by) { break; }
		num_of_ints_went_through = sum;
		++num_of_intarrays_went_through;
		if (sum == shift_by) { break; }
		else { it_this = it_this->next; }
	}
	this->intarrays.flush_insignificant_to_piggy_bank(num_of_intarrays_went_through);
	this->intarrays.intarrays.first->value->shift_right(shift_by - num_of_ints_went_through);
	this->num_of_used_ints -= shift_by;
	this->num_of_intarrays_used -= num_of_intarrays_went_through;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"shift_right(many_bits shift_by)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function \"shift_right(many_bits shift_by)\"";
	}
#endif
}
few_bits unlimited_int::binary_search_divide(unlimited_int& num_to_divide_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"binary_search_divide(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if ((this->find_inconsistencies()) || (num_to_divide_by.find_inconsistencies()))
	{
		std::cout << "\nThe inconsistency was found in function \"binary_search_divide(unlimited_int& num_to_divide_by)\"";
	}
#endif
	unlimited_int mult_result;
	few_bits min = 0, max = MAX_few_bits_NUM;
	num_to_divide_by.multiply(min, &mult_result);
	char result_compare = mult_result.compare_to(*this);
	if (result_compare == 'E') { return min; }
	num_to_divide_by.multiply(max, &mult_result);
	result_compare = mult_result.compare_to(*this);
	if ((result_compare == 'E') || (result_compare == 'S')) { return max; }
	while (true)
	{
		few_bits average = (((many_bits)min) + ((many_bits)max)) / 2;
		num_to_divide_by.multiply(average, &mult_result);
		result_compare = mult_result.compare_to(*this);
		if (result_compare == 'E') { return average; }
		if (result_compare == 'L') { max = average; }
		else { min = average; }
		if (max - min <= 1) { return min; }
	}
	return 0;
}
unlimited_int* unlimited_int::divide_by(unlimited_int& num_to_divide_by)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if ((this->find_inconsistencies()) || (num_to_divide_by.find_inconsistencies()))
	{
		std::cout << "\nThe inconsistency was found in start of function \"divide_by(unlimited_int& num_to_divide_by)\"";
	}
	if (num_to_divide_by.num_of_used_ints == 0) { std::cerr << "\nError trying to divide by zero."; }
#endif
	many_bits num_of_used_ints_divide = num_to_divide_by.num_of_used_ints;
	many_bits num_of_used_ints_this = this->num_of_used_ints;
	char result_compare = this->compare_to(num_to_divide_by);
	unlimited_int* answer = new unlimited_int;
	if (result_compare == 'E') { *answer = 1; return answer; }
	if (result_compare == 'S') { return answer; }
	if (num_of_used_ints_this == 0 || num_of_used_ints_divide == 0) { return answer; }
	if (num_of_used_ints_this == num_of_used_ints_divide)
	{
		*answer = this->binary_search_divide(num_to_divide_by);
		return answer;
	}
	unlimited_int partial_this;
	many_bits num_of_ints_taken_from_this_so_far;
	many_bits num_of_ints_currently_using_from_this = num_of_used_ints_divide;
	this->copy_most_significant_to(partial_this, num_of_ints_currently_using_from_this);
	__list_location__ ll_start = this->find_num_used_int_from_significant(num_of_ints_currently_using_from_this + 1);
	Node* it_this = ll_start.node;
	int_array* current_int_array_this = it_this->value;
	many_bits_signed index_this = ll_start.index;
	result_compare = partial_this.compare_to(num_to_divide_by);
	if (result_compare == 'S')
	{
		partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
		++num_of_ints_currently_using_from_this;
		result_compare = partial_this.compare_to(num_to_divide_by);
		--index_this;
		if (index_this < 0)
		{
			it_this = it_this->previous;
			if (it_this != nullptr)
			{
				current_int_array_this = it_this->value;
				index_this = current_int_array_this->num_of_used_ints - 1;
			}
		}
	}
	few_bits binary_search_division_result;
	unlimited_int result_of_multiplication;
	while (true)
	{
		binary_search_division_result = partial_this.binary_search_divide(num_to_divide_by);
		answer->push_to_insignificant(binary_search_division_result);
		num_to_divide_by.multiply(binary_search_division_result, &result_of_multiplication);
		partial_this.subtract(&result_of_multiplication, &partial_this);
		result_of_multiplication.flush();
		if (num_of_ints_currently_using_from_this == num_of_used_ints_this) { break; }
		partial_this.push_to_insignificant(current_int_array_this->intarr[index_this]);
		++num_of_ints_currently_using_from_this;
		--index_this;
		if (index_this < 0)
		{
			if (it_this != nullptr)
			{
				it_this = it_this->previous;
				if (it_this != nullptr)
				{
					current_int_array_this = it_this->value;
					index_this = current_int_array_this->num_of_used_ints - 1;
				}
			}
		}
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function \"divide_by(unlimited_int& num_to_divide_by)\"";
	}
#endif
	return answer;
}
unlimited_int* unlimited_int::divide_using_binary_search(unlimited_int& num_to_divide_by)
{
	unlimited_int* min = new unlimited_int;
	unlimited_int* max = new unlimited_int;
	unlimited_int* answer;
	this->copy_to(*max);
	many_bits_signed permanent_difference = ((many_bits_signed)this->num_of_used_ints) - ((many_bits_signed)num_to_divide_by.num_of_used_ints);
	while (true)
	{
		unlimited_int difference_between_min_and_max = (*max) - (*min);
		if (difference_between_min_and_max <= ((many_bits_signed)1)) { delete max; return min; }
		difference_between_min_and_max.flush();
		unlimited_int* average = (*min) + (*max);
		(*average) >>= 1; //divide by two
		many_bits_signed difference = permanent_difference - ((many_bits_signed)average->num_of_used_ints);
		if (difference < -1) { delete max; max = average; }
		else if (difference > 0) { delete min; min = average; }
		else
		{
			answer = num_to_divide_by.multiply_karatsuba(average);
			char compare_result = answer->compare_to(*this);
			delete answer;
			if (compare_result == 'E')
			{
				delete min; delete max;
				return average;
			}
			else if (compare_result == 'S') { delete min; min = average; }
			else { delete max; max = average; }
		}
	}
}
void unlimited_int::push_to_insignificant(few_bits num_to_push)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"push_to_insignificant(few_bits num_to_push)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in start of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"";
	}
#endif
	if (this->num_of_used_ints == 0) { this->assign(num_to_push); return; }
	int_array* int_array_first = this->intarrays.intarrays.first->value;
	if (int_array_first->is_full())
	{
		this->intarrays.increase_by_one_array_from_piggy_bank_to_insignificant();
		++this->num_of_intarrays_used;
		int_array_first = this->intarrays.intarrays.first->value;
		int_array_first->num_of_used_ints = 0;
	}
	int_array_first->push_to_insignificant(num_to_push);
	++this->num_of_used_ints;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function \"unlimited_int::push_to_insignificant(few_bits num_to_push)\"";
	}
#endif
}
char* unlimited_int::to_string(int base)
{
	if (this->num_of_used_ints == 0)
	{
		char* char_to_return = new char[2];
		char_to_return[0] = '0';
		char_to_return[1] = '\0';
		return char_to_return;
	}
	const int size_of_string = (this->num_of_used_ints) * NUM_OF_BITS_few_bits;
	char* string_base = new char[size_of_string];
	many_bits_signed counter = 0;
	unlimited_int numerator = this->copy(), base_str = ((many_bits_signed)base);
	numerator.is_negative = false;
	while ((numerator > ((many_bits_signed)0)) && (counter < size_of_string))
	{
		unlimited_int current_digit_ui = (numerator % base_str);
		string_base[counter] = unlimited_int::number_to_char((int)current_digit_ui, base);
		numerator /= base_str;
		++counter;
	}
	//by now we have the digits of string_base backwards
	char* final_str;
	char* final_str_original;
	many_bits_signed counter2 = 0;
	if (this->is_negative)
	{
		final_str_original = new char[counter + 2];
		final_str_original[0] = '-';
		final_str = final_str_original + 1;
	}
	else
	{
		final_str_original = new char[counter + 1];
		final_str = final_str_original;
	}
	for (many_bits_signed counter3 = counter - 1; counter2 < counter; ++counter2, --counter3)
	{
		final_str[counter2] = string_base[counter3];
	}
	delete[]string_base;
	final_str[counter] = '\0';
	return final_str_original;
}
unlimited_int* unlimited_int::from_string(char* str, const int base = 10)
{
	unlimited_int* answer = new unlimited_int;
	if (str[0] == '0') { answer->set_to_zero(); return answer; }
	bool set_answer_to_true = false;
	if (str[0] == '-') { set_answer_to_true = true; ++str; }
	if ((base <= 0) || (base > 36)) { std::cerr << "\nError in function \"from_string\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\""; return answer; }
	unlimited_int base_ui = base;
	char* it_ch = str;
	many_bits_signed counter = 0;
	while (*it_ch != '\0') { ++it_ch; ++counter; }
	many_bits_signed index = counter - 1;
	unlimited_int multiplicand = 1;
	while (index >= 0)
	{
		int value = unlimited_int::char_to_number(str[index], base);
		if (value == -1)
		{
			std::cerr << "\nError in function \"unlimited_int::from_string(char*, const int base)\": Invalid char in the char array";
		}
		unlimited_int temp_answer;
		multiplicand.multiply(value, &temp_answer);
		*answer += temp_answer;
		--index;
		multiplicand *= base_ui;
	}
	answer->is_negative = set_answer_to_true;
	return answer;
}
int unlimited_int::char_to_number(char ch, const int base = 10)
{
	if ((ch < 48) || ((ch > 57) && (ch < 65)) || ((ch > 90) && (ch < 97)) || (ch > 122))
	{
		std::cerr << "\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nchar ch is a NON number or letter characters: " << ((int)ch);
		return (-1);
	}
	if ((base <= 0) || (base > 36)) { std::cerr << "\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\""; return -1; }
	int value;
	if (ch <= 57) //is a number
	{
		value = ch - 48;
	}
	else if (ch <= 90) //is an upper-case letter
	{
		value = ch - 55;
	}
	else { value = ch - 87; } //is a lower-case letter
	if (value >= base)
	{
		std::cerr << "\nError in function \"char_to_number(char ch, int base)\" Invalid Argument!\nthe number of char ch: " << value << " is not a digit in the specified base: " << base;
		return (-1);
	}
	return value;
}
char unlimited_int::number_to_char(int num, const int base = 10)
{
	if ((num >= base) || (num < 0)) { std::cout << "\nError in function \"number_to_char\" num out of range."; return '0'; }
	if ((base <= 0) || (base > 36)) { std::cerr << "\nError in function \"number_to_char\" Invalid Argument!\nbase is out of range \"1 <= base <= 36\""; return '0'; }
	if (num <= 9) { return (num + 48); }
	return (num + 87);
}
unlimited_int* unlimited_int::generate_random(unlimited_int& min, unlimited_int& max)
{
	char result_compare = min.compare_to(max);
	if (result_compare == 'L')
	{
		std::cerr << "\nError found in function \"unlimited_int::generate_random(unlimited_int&, unlimited_int&)\": min > max";
		return (min.copy());
	}
	else if (result_compare == 'E')
	{
		return (min.copy());
	}
	unlimited_int result_of_sub = max - min;
	unlimited_int* answer = unlimited_int::generate_random(result_of_sub);
	*answer += min;
	return answer;
}
unlimited_int* unlimited_int::generate_random(unlimited_int& max)
{
	unlimited_int* answer = new unlimited_int;
	if (max.num_of_used_ints == 0) { return answer; }
	__list_location__ ll = max.find_num_used_int_from_significant(1);
	few_bits most_significant_max = ll.node->value->intarr[ll.index];
	answer->randomize(max.num_of_used_ints);
	if (*answer > max)
	{
		__list_location__ ll_answer = answer->find_num_used_int_from_significant(1);
		few_bits* most_significant_answer_ptr = &ll_answer.node->value->intarr[ll_answer.index];
		few_bits num_to_assign = unlimited_int::generate_random_many_bits() % (((many_bits)most_significant_max) + ((many_bits)1));
		*most_significant_answer_ptr = num_to_assign;
		if (num_to_assign == 0) { answer->cutoff_leading_zeros(ll_answer.node); }
		else if (*answer > max)
		{
			num_to_assign = unlimited_int::generate_random_many_bits() % ((many_bits)num_to_assign);
			*most_significant_answer_ptr = num_to_assign;
			if (num_to_assign == 0) { answer->cutoff_leading_zeros(ll_answer.node); }
		}
	}
	return answer;
}
//makes use if the identity (a ⋅ b) mod m = [(a mod m) ⋅ (b mod m)] mod m
unlimited_int* unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || mod.find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
	}
#endif
	unlimited_int* answer = new unlimited_int(((many_bits_signed)1));
	if (power.num_of_used_ints == 0) { return answer; }
	if (base.num_of_used_ints == 0) { answer->set_to_zero(); return answer; }
	if (mod == ((many_bits_signed)1)) { answer->set_to_zero(); return answer; }
	unlimited_int current_power = base.copy();
	current_power %= mod;
	if (current_power.is_zero()) { answer->set_to_zero(); return answer; }
	unlimited_int power_cpy = power.copy();
	if (power_cpy.modulo_2() == 1)
	{
		*answer *= current_power;
		*answer %= mod;
	}
	power_cpy >>= 1;
	while (! power_cpy.is_zero())
	{
		current_power = current_power.power2_destroy_this();
		current_power %= mod;
		if (power_cpy.modulo_2() == 1)
		{
			*answer *= current_power;
			*answer %= mod;
		}
		power_cpy >>= 1;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || mod.find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
	}
	if (answer->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\" in answer";
	}
#endif
	return answer;
}
unlimited_int* unlimited_int::pow(unlimited_int& base, unlimited_int& power)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
	}
#endif
	unlimited_int* answer = new unlimited_int(((many_bits_signed)1));
	if (power.num_of_used_ints == 0) { return answer; }
	if (base.num_of_used_ints == 0) { answer->set_to_zero(); return answer; }
	unlimited_int current_power = base.copy();
	unlimited_int power_cpy = power.copy();
	if (power_cpy.modulo_2() == 1) { *answer *= current_power; }
	power_cpy >>= 1;
	while (!power_cpy.is_zero())
	{
		current_power = current_power.power2_destroy_this();
		if (power_cpy.modulo_2() == 1) { *answer *= current_power; }
		power_cpy >>= 1;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
	}
	if (answer->find_inconsistencies())
	{
		std::cout << "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\" in answer";
	}
#endif
	return answer;
}
