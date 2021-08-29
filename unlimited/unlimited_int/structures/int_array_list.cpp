#include "int_array_list.hpp"
using namespace unlimited;
void int_array_list::swap(int_array_list& int_array_list_to_swap_with)
{
	const size_t temp_int = this->num_of_ints;
	this->num_of_ints = int_array_list_to_swap_with.num_of_ints;
	int_array_list_to_swap_with.num_of_ints = temp_int;

	this->custom_linked_list::swap(int_array_list_to_swap_with);
}
void int_array_list::append(int_array_list& list_to_append)
{
	this->custom_linked_list::append(list_to_append);
	this->num_of_ints += list_to_append.num_of_ints;
	list_to_append.num_of_ints = (size_t)0;
}
void int_array_list::prepend(int_array_list& list_to_prepend)
{
	this->custom_linked_list::prepend(list_to_prepend);
	this->num_of_ints += list_to_prepend.num_of_ints;
	list_to_prepend.num_of_ints = (size_t)0;
}
custom_linked_list_node<int_array>* int_array_list::erase(custom_linked_list_node<int_array>* node_to_remove)
{
	if (node_to_remove->value != nullptr)
		this->num_of_ints -= node_to_remove->value->intarr_len;
	return this->custom_linked_list::erase(node_to_remove);
}
int_array_list* int_array_list::sublist(custom_linked_list_node<int_array>* start_sublist, custom_linked_list_node<int_array>* end_sublist, size_t sublist_len, size_t sum_ints)
{
	custom_linked_list<int_array>* substr_base = this->custom_linked_list::sublist(start_sublist, end_sublist, sublist_len);
	this->num_of_ints -= sum_ints;
	int_array_list* substr_derived = new int_array_list(*substr_base);
	substr_derived->num_of_ints = sum_ints;
	substr_base->reset_without_deleting_nodes();
	delete substr_base;
	return substr_derived;
}
int_array_list::list_location int_array_list::find_num_of_int_from_insignificant(size_t num_int_to_find)
{
	list_location ll;
	if (num_int_to_find > this->num_of_ints)
		throw std::out_of_range("Error in function \"find_num_of_int_from_insignificant\", the num of int to find is too large for the current int_array_list");
	if (num_int_to_find == (size_t)0)
		throw std::out_of_range("Error in function \"find_num_of_int_from_insignificant\" the num of int to find is 0. This function counts from 1.");
	custom_linked_list_node<int_array>* it = this->custom_linked_list::first();
	size_t sum_of_all_ints = (size_t)0, num_of_arrays = (size_t)1;
	while (true)
	{
		const size_t temp_sum = sum_of_all_ints + it->value->intarr_len;
		if (temp_sum >= num_int_to_find)
			break;
		sum_of_all_ints = temp_sum;
		it = it->next;
		++num_of_arrays;
	}
	ll.node = it;
	ll.index = num_int_to_find - sum_of_all_ints - (size_t)1;
	ll.num_array = num_of_arrays;
	return ll;
}
int_array_list::~int_array_list()
{
	this->num_of_ints = (size_t)0;
	custom_linked_list_node<int_array>* it = this->first();
	const custom_linked_list_node<int_array> *const this_end = this->end();
	while (it != this_end)
	{
		it->value->destroy();
		it = it->next;
	}
	//Now the destructor of this->custom_linked_list::~custom_linked_list() will automatically be called.
}
void int_array_list::clear()
{
	this->num_of_ints = (size_t)0;
	custom_linked_list_node<int_array>* it = this->first();
	const custom_linked_list_node<int_array>* const this_end = this->end();
	while (it != this_end)
	{
		it->value->destroy();
		it = it->next;
	}
	this->custom_linked_list::clear();
}
#if DEBUG_MODE > 0
size_t int_array_list::double_check_length()
{
	custom_linked_list_node<int_array>* it = this->first();
	size_t length_counter = 0;
	const custom_linked_list_node<int_array> *const this_end = this->end();
	while (it != this_end)
	{
		it = it->next;
		++length_counter;
	}
	custom_linked_list_node<int_array>* it_backwards = this->last();
	size_t length_counter_reverse = 0;
	const custom_linked_list_node<int_array> *const this_begin = this->begin();
	while (it_backwards != this_begin)
	{
		it_backwards = it_backwards->previous;
		++length_counter_reverse;
	}
	if (length_counter != length_counter_reverse)
		throw std::logic_error("\nError in function \"size_t int_array_list::double_check_length()\" the length of the doubly-linked list going forward is not the same as the length going in reverse.");
	if (length_counter != this->_length)
		throw std::logic_error("\nError in function \"size_t int_array_list::double_check_length()\" the length of the doubly-linked list measured manually is not the same as the _length variable");
	return length_counter;
}
#endif