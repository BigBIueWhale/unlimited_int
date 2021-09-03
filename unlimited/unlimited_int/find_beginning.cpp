#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
int_array_list::list_location unlimited_int::find_num_used_int_from_significant(const size_t num_int_to_find) const
{
	int_array_list::list_location ll;
	if (num_int_to_find > this->num_of_used_ints)
		throw std::logic_error("Error in function find_num_used_int_from_significant: the number int in the argument is passed the end of the number.");
	custom_linked_list_node<int_array>* it = this->get_most_significant_used_int_array();
	size_t sum_used = (size_t)0;
	size_t num_array = this->num_of_intarrays_used + (size_t)1;
	while (true)
	{
#if DEBUG_MODE > 0
		if (num_array-- == 0)
		{
			num_array = (size_t)0;
			break; //The exception will be called
		}
#endif
		sum_used += it->value->num_of_used_ints;
		if (sum_used >= num_int_to_find)
			break;
		else
			it = it->previous;
	}
#if DEBUG_MODE > 0
	if (num_array == (size_t)0)
		throw std::logic_error("Error in function: \"__list_location__ unlimited_int::find_num_used_int_from_significant(const many_bits num_int_to_find) const\". \"this->num_of_intarrays_used\" is wrong.");
#endif
	//Num array from insignificant, starting from array 1 which is this->first() (instead of counting from 0, and instead of counting from significant)
	ll.num_array = num_array;
	ll.node = it;
	ll.index = sum_used - num_int_to_find;
	return ll;
}
custom_linked_list_node<int_array>* unlimited_int::get_most_significant_used_int_array() const
{
	if (this->num_of_intarrays_used == (size_t)0)
		throw std::logic_error("\nError in start of function \"Node* unlimited_int::get_last_intarray() const\" there are no used intarrays");
	const size_t num_of_intarrays_unused = this->intarrays->size() - this->num_of_intarrays_used;
	custom_linked_list_node<int_array>* it = this->intarrays->last();
	for (size_t counter = (size_t)0; counter < num_of_intarrays_unused; ++counter)
		it = it->previous;
	return it;
}
