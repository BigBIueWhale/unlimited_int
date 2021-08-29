#include "list_of_int_arrays.hpp"
using namespace unlimited;
void list_of_int_arrays::increase_by_one_array_and_zero()
{
	size_t current_len_arr_to_add;
	if (this->num_of_ints > (size_t)MIN_ALLOC)
		current_len_arr_to_add = this->num_of_ints;
	else
		current_len_arr_to_add = (size_t)MIN_ALLOC;
	if ((size_t)MAX_ALLOC < current_len_arr_to_add)
		current_len_arr_to_add = (size_t)MAX_ALLOC;
	int_array *const new_int_array = new int_array();
	new_int_array->resize_and_fillzero(current_len_arr_to_add);
	this->push_back(new_int_array);
	this->num_of_ints += current_len_arr_to_add;
}
void list_of_int_arrays::increase_by_one_array()
{
	size_t current_len_arr_to_add;
	if (this->num_of_ints > (size_t)MIN_ALLOC)
		current_len_arr_to_add = this->num_of_ints;
	else
		current_len_arr_to_add = (size_t)MIN_ALLOC;
	if ((size_t)MAX_ALLOC < current_len_arr_to_add)
		current_len_arr_to_add = (size_t)MAX_ALLOC;
	int_array *const new_int_array = new int_array();
	new_int_array->resize(current_len_arr_to_add);
	this->push_back(new_int_array);
	this->num_of_ints += current_len_arr_to_add;
}
void list_of_int_arrays::increase_until_num_of_ints(const size_t num_of_ints_to_increase_until)
{
	if (num_of_ints_to_increase_until > this->num_of_ints)
	{
		list_of_int_arrays::bank_storage.withdraw(*this, num_of_ints_to_increase_until);
		while (this->num_of_ints < num_of_ints_to_increase_until)
			this->increase_by_one_array();
	}
}
void list_of_int_arrays::fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(size_t fill_0_until)
{
	if (this->_length > 0)
	{
		custom_linked_list_node<int_array>* it = this->first();
		size_t num_of_ints_zeroed_until_now = 0;
		if (this->num_of_ints < fill_0_until)
			fill_0_until = this->num_of_ints;
		while (true)
		{
			int_array *const current_int_array = it->value;
			const size_t sum = num_of_ints_zeroed_until_now + current_int_array->intarr_len;
			if (sum >= fill_0_until)
			{
				const size_t num_to_fill_until = fill_0_until - num_of_ints_zeroed_until_now;
				current_int_array->fillzero_until(num_to_fill_until);
				current_int_array->num_of_used_ints = num_to_fill_until;
				break;
			}
			else
			{
				num_of_ints_zeroed_until_now = sum;
				current_int_array->fillzero();
				current_int_array->set_num_of_used_ints_to_maximum();
			}
			it = it->next;
		}
	}
}
void list_of_int_arrays::increase_by_one_array_to_insignificant()
{
	size_t current_len_arr_to_add;
	if (this->num_of_ints > (size_t)MIN_ALLOC)
		current_len_arr_to_add = this->num_of_ints;
	else
		current_len_arr_to_add = (size_t)MIN_ALLOC;
	if ((size_t)MAX_ALLOC < current_len_arr_to_add)
		current_len_arr_to_add = (size_t)MAX_ALLOC;
	int_array *const new_int_array = new int_array(current_len_arr_to_add);
	this->push_front(new_int_array);
	this->num_of_ints += current_len_arr_to_add;
}
