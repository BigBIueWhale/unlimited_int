#ifndef UNLIMITED_INT_ARRAY_LIST_H
#define UNLIMITED_INT_ARRAY_LIST_H true
#include "custom_linked_list.hpp"
#include "int_array.hpp"
namespace unlimited
{
	class int_array_list : public custom_linked_list<int_array>
	{
	public:
		struct list_location {
			custom_linked_list_node<int_array>* node;
			size_t index;
			size_t num_array;
		};
		size_t num_of_ints;
		int_array_list() : num_of_ints((size_t)0) {}
		int_array_list(const custom_linked_list<int_array>& other) : custom_linked_list(other) { num_of_ints = (size_t)0; }
		int_array_list(const int_array_list& other) : custom_linked_list(other) { this->num_of_ints = other.num_of_ints; }
		void swap(int_array_list& int_array_list_to_swap_with);
		void append(int_array_list&);
		void prepend(int_array_list&);
		custom_linked_list_node<int_array>* erase(custom_linked_list_node<int_array>* node_to_remove);
		int_array_list* sublist(custom_linked_list_node<int_array>* start_sublist, custom_linked_list_node<int_array>* end_sublist, size_t sublist_len, size_t sum_ints);
		//finds the integer from the insignificant side. Ignores num_used_ints.
		//For example: giving this function 1 will give you the most insignificant int in the list. Giving this function 0 will throw an exception because we're counting from 1 over here.
		list_location find_num_of_int_from_insignificant(size_t num_int_to_find);
		void clear();
		~int_array_list();
#if DEBUG_MODE > 0
		size_t double_check_length();
#endif
	};
}
#endif
