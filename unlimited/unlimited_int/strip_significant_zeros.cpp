#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::cutoff_leading_zeros(custom_linked_list_node<int_array> *const int_array_to_start_at)
{
	if (this->intarrays == nullptr)
		throw std::logic_error("Doesn\'t make sense to receive a node of a list that doesn\'t exist.");
	const custom_linked_list_node<int_array> *const this_intarrays_begin = this->intarrays->begin();
	if (int_array_to_start_at == this_intarrays_begin || int_array_to_start_at == this->intarrays->end() || int_array_to_start_at == nullptr)
		throw std::logic_error("Doesn\'t make sense to start cutting off leading zeros from an invalid node.");
	size_t num_of_int_arrays_went_through = (size_t)0;
	size_t num_of_ints_went_through = (size_t)0;
	custom_linked_list_node<int_array>* it = int_array_to_start_at;
	while (it != this_intarrays_begin)
	{
		int_array *const current_int_array = it->value;
		bool found_first_not_zero;
		const size_t first_not_zero = current_int_array->find_first_used_not_zero(&found_first_not_zero);
		if (!found_first_not_zero) //If the entire used part of the curent int_array is filled with zeros.
		{
			num_of_ints_went_through += current_int_array->num_of_used_ints;
			++num_of_int_arrays_went_through;
		}
		else
		{
			num_of_ints_went_through += current_int_array->num_of_used_ints;
			current_int_array->num_of_used_ints = first_not_zero + (size_t)1;
			num_of_ints_went_through -= current_int_array->num_of_used_ints;
			break;
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
		throw std::logic_error("The error was found in end of function \"cutoff_leading_zeros(Node* int_array_to_start_at)\"");
#endif
}