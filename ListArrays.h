#ifndef LISTARRAYS_HEADER
#define LISTARRAYS_HEADER true
#include "Node.h"
struct __list_location__{
	Node* node;
	many_bits index;
	many_bits num_array;
};
class ListArrays
{
public:
	Node* first;
	Node* last;
	many_bits length;
	ListArrays()
	{
		first = nullptr;
		last = nullptr;
		length = 0;
	}
	ListArrays(const ListArrays& la) { this->operator=(la); }
	void operator=(const ListArrays& la)
	{
		this->first = la.first;
		this->last = la.last;
		this->length = la.length;
	}
	void swap(ListArrays& other)
	{
		Node* temp_Node = other.first;
		other.first = this->first;
		this->first = temp_Node;

		temp_Node = other.last;
		other.last = this->last;
		this->last = temp_Node;

		many_bits temp_int;
		temp_int = other.length;
		other.length = this->length;
		this->length = temp_int;
	}
	void print_properties();
	void print_all();
	void prepend(ListArrays& la_to_prepend);
	void append(ListArrays& la_to_append);
#if DEBUG_MODE > 0
	many_bits double_check_length();
	bool find_inconsistencies();
#endif
};
#endif