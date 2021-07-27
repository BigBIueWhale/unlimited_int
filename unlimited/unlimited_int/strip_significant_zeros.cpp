#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
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
		throw "\nThe error was found in end of function \"cutoff_leading_zeros(Node* int_array_to_start_at)\"";
#endif
}