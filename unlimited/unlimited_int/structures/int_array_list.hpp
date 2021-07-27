#ifndef UNLIMITED_INT_ARRAY_LIST_H
#define UNLIMITED_INT_ARRAY_LIST_H true
#include "ListArrays.hpp"
namespace unlimited
{
	class int_array_list
	{
	public:
		ListArrays intarrays;
		many_bits num_of_ints;
		int_array_list() { this->num_of_ints = 0; }
		void operator=(const int_array_list& num_to_assign);
		void destroy_and_reset();
#if DEBUG_MODE > 0
		void print_properties() const;
#endif
		void swap(int_array_list& int_array_list_to_swap_with);
		void append(int_array_list&);
		void prepend(int_array_list&);
		//finds the int index from the insignificant side. Ignores num_used_ints.
		__list_location__ find_num_of_int_from_insignificant(many_bits num_int_to_find);
#if DEBUG_MODE > 0
		bool find_inconsistencies() const;
#endif
	};
}
#endif
