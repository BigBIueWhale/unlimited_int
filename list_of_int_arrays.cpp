#include "list_of_int_arrays.h"
int_array_piggy_bank list_of_int_arrays::bank_storage;
void list_of_int_arrays::increase_by_one_array_and_zero()
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"increase_by_one_array_and_zero()\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	many_bits current_len_arr_to_add;
	if (this->num_of_ints > MIN_ALLOC) { current_len_arr_to_add = this->num_of_ints; }
	else { current_len_arr_to_add = MIN_ALLOC; }
	if (MAX_ALLOC < current_len_arr_to_add) { current_len_arr_to_add = MAX_ALLOC; }
	this->num_of_ints += current_len_arr_to_add;
	int_array* new_int_array = new int_array();
	new_int_array->resize_and_fillzero(current_len_arr_to_add);
	Node* new_node = new Node(new_int_array);
	if (this->intarrays.length == 0)
	{
		this->intarrays.first = new_node;
		this->intarrays.last = new_node;
	}
	else
	{
		this->intarrays.last->next = new_node;
		new_node->previous = this->intarrays.last;
		this->intarrays.last = new_node;
	}
	++this->intarrays.length;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"increase_by_one_array_and_zero()\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void list_of_int_arrays::increase_by_one_array()
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"increase_by_one_array()\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	many_bits current_len_arr_to_add;
	if (this->num_of_ints > MIN_ALLOC) { current_len_arr_to_add = this->num_of_ints; }
	else { current_len_arr_to_add = MIN_ALLOC; }
	if (MAX_ALLOC < current_len_arr_to_add) { current_len_arr_to_add = MAX_ALLOC; }
	this->num_of_ints += current_len_arr_to_add;
	int_array* new_int_array = new int_array;
	new_int_array->resize(current_len_arr_to_add);
	Node* new_node = new Node(new_int_array);
	if (this->intarrays.length == 0)
	{
		this->intarrays.first = new_node;
		this->intarrays.last = new_node;
	}
	else
	{
		this->intarrays.last->next = new_node;
		new_node->previous = this->intarrays.last;
		this->intarrays.last = new_node;
	}
	++this->intarrays.length;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"increase_by_one_array()\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void list_of_int_arrays::increase_until_num_of_ints(many_bits num_of_ints_to_increase_until)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"increase_until_num_of_ints(many_bits num_of_ints_to_increase_until)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if (num_of_ints_to_increase_until <= this->num_of_ints) { return; }
	list_of_int_arrays::bank_storage.withdraw(*this, num_of_ints_to_increase_until);
	while ((this->num_of_ints) < num_of_ints_to_increase_until)
	{
		this->increase_by_one_array();
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"increase_until_num_of_ints(many_bits num_of_ints_to_increase_until)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
list_of_int_arrays::list_of_int_arrays(const list_of_int_arrays& num_to_assign)
{
	this->intarrays = num_to_assign.intarrays;
	this->num_of_ints = num_to_assign.num_of_ints;
}
void list_of_int_arrays::fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(many_bits fill_0_until)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(many_bits fill_0_until)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
	if (this->intarrays.length == 0) { return; }
	Node* it = this->intarrays.first;
	many_bits num_of_ints_zeroed_until_now = 0;
	int_array* current_int_array;
	if (this->num_of_ints < fill_0_until) { fill_0_until = this->num_of_ints; }
	while (true)
	{
		current_int_array = it->value;
		many_bits sum = num_of_ints_zeroed_until_now + current_int_array->intarr_len;
		if (sum >= fill_0_until)
		{
			many_bits num_to_fill_until = fill_0_until - num_of_ints_zeroed_until_now;
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
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(many_bits fill_0_until)\":";
#endif
#if DEBUG_MODE > 0
	this->find_inconsistencies();
#endif
}
void list_of_int_arrays::increase_by_one_array_to_insignificant()
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"increase_by_one_array_to_insignificant()\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies()) {
		std::cerr << "\nThe inconsistency was found in start of function \"list_of_int_arrays::increase_by_one_array_to_insignificant()\"";
	}
#endif
	many_bits current_len_arr_to_add;
	if (this->num_of_ints > MIN_ALLOC) { current_len_arr_to_add = this->num_of_ints; }
	else { current_len_arr_to_add = MIN_ALLOC; }
	if (MAX_ALLOC < current_len_arr_to_add) { current_len_arr_to_add = MAX_ALLOC; }
	this->num_of_ints += current_len_arr_to_add;
	int_array* new_int_array = new int_array(current_len_arr_to_add);
	Node* new_node = new Node(new_int_array);
	if (this->intarrays.length == 0)
	{
		this->intarrays.first = new_node;
		this->intarrays.last = new_node;
	}
	else
	{
		this->intarrays.first->previous = new_node;
		new_node->next = this->intarrays.first;
		this->intarrays.first = new_node;
	}
	++this->intarrays.length;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"increase_by_one_array_to_insignificant()\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies()) {
		std::cerr << "\nThe inconsistency was found in end of function \"list_of_int_arrays::increase_by_one_array_to_insignificant()\"";
	}
#endif
}