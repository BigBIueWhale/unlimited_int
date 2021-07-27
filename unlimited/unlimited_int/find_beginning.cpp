#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
__list_location__ unlimited_int::find_num_used_int_from_significant(const many_bits num_int_to_find) const
{
	__list_location__ ll;
	if (num_int_to_find > this->num_of_used_ints) { ll.index = 0; ll.num_array = 0; ll.node = nullptr; return ll; }
	Node* it = this->get_most_significant_used_int_array();
	many_bits sum_used = 0;
	many_bits_signed num_array = this->num_of_intarrays_used + 1;
	while (true)
	{
		sum_used += it->value->num_of_used_ints;
		--num_array;
		if (sum_used >= num_int_to_find) { break; }
		else { it = it->previous; }
	}
	if (num_array < 1)
		throw "Error in function: \"__list_location__ unlimited_int::find_num_used_int_from_significant(const many_bits num_int_to_find) const\". \"this->num_of_intarrays_used\" is wrong.";
	ll.num_array = num_array;
	ll.node = it;
	ll.index = sum_used - num_int_to_find;
	return ll;
}
Node* unlimited_int::get_most_significant_used_int_array() const
{
	if (this->num_of_intarrays_used == 0)
		throw "\nError in start of function \"Node* unlimited_int::get_last_intarray() const\" there are no used intarrays";
	many_bits difference = this->intarrays.intarrays.length - this->num_of_intarrays_used;
	Node* it = this->intarrays.intarrays.last;
	for (many_bits counter = 0; counter < difference; ++counter)
		it = it->previous;
	return it;
}
