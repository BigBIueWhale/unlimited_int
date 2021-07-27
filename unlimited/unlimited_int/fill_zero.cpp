#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
void unlimited_int::fill_0_until_num_of_ints(const many_bits fill_0_until)
{
	many_bits fill_0_until_cpy = fill_0_until;
	//if (!this->auto_destroy) { this->forget_memory(); } this line of code might be important
	Node* it = this->intarrays.intarrays.first;
	many_bits num_of_ints_zeroed_until_now = 0;
	int_array* current_int_array;
	if (this->intarrays.num_of_ints < fill_0_until_cpy) { fill_0_until_cpy = this->intarrays.num_of_ints; }
	while (true)
	{
		current_int_array = it->value;
		many_bits sum = num_of_ints_zeroed_until_now + current_int_array->intarr_len;
		if (sum >= fill_0_until_cpy)
		{
			current_int_array->fillzero_until(fill_0_until_cpy - num_of_ints_zeroed_until_now);
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
	if (this->find_inconsistencies())
		throw "inconsistency found in the end of \"void unlimited_int::fill_0_until_num_of_ints(const many_bits fill_0_until)\"";
#endif
}
void unlimited_int::fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "inconsistency found in the beginning of \"void unlimited_int::fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until)\"";
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
