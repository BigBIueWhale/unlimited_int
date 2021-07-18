#ifndef UNLIMITED_LISTARRAYS_H
#define UNLIMITED_LISTARRAYS_H
#include "Node.h"
namespace unlimited
{
	struct __list_location__ {
		Node* node;
		unlimited::many_bits index;
		unlimited::many_bits num_array;
	};
	class ListArrays
	{
	public:
		Node* first;
		Node* last;
		unlimited::many_bits length;
		ListArrays()
		{
			first = nullptr;
			last = nullptr;
			length = 0;
		}
		ListArrays(const ListArrays& la) { (*this) = la; }
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

			unlimited::many_bits temp_int;
			temp_int = other.length;
			other.length = this->length;
			this->length = temp_int;
		}
#if DEBUG_MODE > 0
		void print_properties() const;
		void print_all() const;
#endif
		void prepend(ListArrays& la_to_prepend);
		void append(ListArrays& la_to_append);
#if DEBUG_MODE > 0
		unlimited::many_bits double_check_length() const;
		bool find_inconsistencies() const;
#endif
	};
}
#endif
