#ifndef INT_ARRAY_LIST_FILE_DEFINED
#define INT_ARRAY_LIST_FILE_DEFINED true
#include "ListArrays.h"
class int_array_list
{
public:
	ListArrays intarrays;
	many_bits num_of_ints;
	int_array_list() { this->num_of_ints = 0; }
	void flush(); //flush is the same as destroy
	void print_properties();
	void swap(int_array_list& int_array_list_to_swap_with);
	void append(int_array_list&);
	void prepend(int_array_list&);
	__list_location__ find_num_of_int_from_insignificant(many_bits num_int_to_find);
#if DEBUG_MODE > 0
	bool find_inconsistencies();
#endif
};
#endif
