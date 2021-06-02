#include "int_array_list.h"
void int_array_list::flush()
{
#if DEBUG_MODE > 1
	std::cout << "\nFlushing " << this->num_of_ints << " ints.";
#endif
	Node* it_this = this->intarrays.first;
	while (it_this != nullptr)
	{
		int_array* it_this_value = it_this->value;
		it_this_value->destroy();
		delete it_this_value;
		Node* it_this_cpy = it_this;
		it_this = it_this->next;
		delete it_this_cpy;
	}
	this->intarrays.length = 0;
	this->num_of_ints = 0;
	this->intarrays.first = nullptr;
	this->intarrays.last = nullptr;
}
void int_array_list::print_properties()
{
	this->intarrays.print_properties();
	std::cout << "\nint_array_list->num_of_ints: " << this->num_of_ints;
}
void int_array_list::swap(int_array_list& int_array_list_to_swap_with)
{
	many_bits temp_int = this->num_of_ints;
	this->num_of_ints = int_array_list_to_swap_with.num_of_ints;
	int_array_list_to_swap_with.num_of_ints = temp_int;

	this->intarrays.swap(int_array_list_to_swap_with.intarrays);
}
#if DEBUG_MODE > 0
bool int_array_list::find_inconsistencies()
{
#if DEBUG_MODE > 1
	std::cout << "\nFinding inconsistencies from \"int_array_list::find_inconsistencies\" of this->intarrays";
#endif
	if (this->intarrays.find_inconsistencies()) { return true; }
	if (this->intarrays.length == 0)
	{
		if (this->num_of_ints != 0)
		{
			std::cerr << "\nError found by function \"int_array_list::find_inconsistencies\", this->num_of_ints is not 0 even though this->intarrays.length == 0";
			return true;
		}
	}
	else
	{
		if (this->num_of_ints < this->intarrays.length)
		{
			std::cerr << "\nError found by function \"int_array_list::find_inconsistencies\", variable this->intarrays_len is: " << this->intarrays.length << " and this->num_of_ints is: " << this->num_of_ints << " which means that there are less ints than arrays of ints!!";
			return true;
		}
		if (!(this->intarrays.length == 0))
		{
			Node* it = this->intarrays.first;
			many_bits num_of_ints_counter = 0;
			many_bits num_of_int_arrays_went_through_so_far = 0;
			while (it != nullptr)
			{
				int_array* current_int_array = it->value;
				if (current_int_array == nullptr)
				{
					std::cerr << "\nError found by function \"int_array_list::find_inconsistencies\": One of the \"int_array\" objects in list: \"this->intarrays\" is nullptr\n";
					return true;
				}
				else
				{
					current_int_array->find_inconsistencies();
					if (current_int_array->intarr == nullptr)
					{
						std::cerr << "\nError found by function \"int_array_list::find_inconsistencies\": the pointer to one of the actual integer arrays inside of an \"int_array\" object in list: \"this->intarrays\" is nullptr\n";
						return true;
					}
				}
				num_of_ints_counter += current_int_array->intarr_len;
				it = it->next;
				++num_of_int_arrays_went_through_so_far;
			}
			if (num_of_int_arrays_went_through_so_far != this->intarrays.length)
			{
				std::cerr << "\nError found by function \"int_array_list::find_inconsistencies\": the number of int_arrays" <<
					" according to \"this->intarrays.length\" is: " << this->intarrays.length << "  but the" <<
					" number of int_arrays found by iterating through the list is: " << num_of_int_arrays_went_through_so_far;
				return true;
			}
			if (num_of_ints_counter != this->num_of_ints)
			{
				std::cerr << "\nError found by function \"int_array_list::find_inconsistencies\": the sum of the \"intarr_len\" variables in the \"int_array\"s pointed to in \"this->intarr\" list, doesn\'t equal \"this->num_of_ints\"" << " because the sum equals: " << num_of_ints_counter <<
					" and the official variable \"this->num_of_ints\" equals: " << this->num_of_ints;
				return true;
			}
		}
	}
	return false;
}
#endif
void int_array_list::append(int_array_list& list_to_append)
{
	this->intarrays.append(list_to_append.intarrays);
	this->num_of_ints += list_to_append.num_of_ints;
	list_to_append.num_of_ints = 0;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"append(list_of_int_arrays list_to_append)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void int_array_list::prepend(int_array_list& list_to_prepend)
{
	this->intarrays.prepend(list_to_prepend.intarrays);
	this->num_of_ints += list_to_prepend.num_of_ints;
	list_to_prepend.num_of_ints = 0;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function \"prepend(list_of_int_arrays list_to_prepend)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
__list_location__ int_array_list::find_num_of_int_from_insignificant(many_bits num_int_to_find)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in function: \"find_num_of_int_from_insignificant(many_bits num_int_to_find)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
	{
		std::cerr << "\nThe inconsistency was found in function: \"find_num_of_int_from_insignificant(many_bits num_int_to_find)\"";
	}
#endif
	__list_location__ ll;
	if (num_int_to_find > this->num_of_ints) { num_int_to_find = this->num_of_ints; }
	Node* it = this->intarrays.first;
	many_bits sum_of_all_ints = 0, num_of_arrays = 1;
	while (true)
	{
		many_bits temp_sum = sum_of_all_ints + it->value->intarr_len;
		if (temp_sum >= num_int_to_find) { break; }
		sum_of_all_ints = temp_sum;
		it = it->next;
		++num_of_arrays;
	}
	ll.node = it;
	ll.index = num_int_to_find - sum_of_all_ints - 1;
	ll.num_array = num_of_arrays;
	return ll;
}