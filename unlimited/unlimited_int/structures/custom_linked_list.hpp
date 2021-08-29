#ifndef CUSTOM_LINKED_LIST_H
#define CUSTOM_LINKED_LIST_H
#include "settings.h"
namespace unlimited
{
	//template for node for doubly-linked list
	template <typename T>
	class custom_linked_list_node
	{
	public:
		custom_linked_list_node<T>* previous;
		custom_linked_list_node<T>* next;
		T* value;
		//disable the copy constructor
		custom_linked_list_node(const custom_linked_list_node<T>&) = delete;
		custom_linked_list_node()
		{
			this->previous = nullptr;
			this->next = nullptr;
			this->value = nullptr;
		}
		custom_linked_list_node(T* value)
		{
			this->next = nullptr;
			this->previous = nullptr;
			this->value = value;
		}
		void swap(custom_linked_list_node<T>& other)
		{
			custom_linked_list_node<T>* temp_Node = other.previous;
			other.previous = this->previous;
			this->previous = temp_Node;

			temp_Node = other.next;
			other.next = this->next;
			this->next = temp_Node;

			T* const temp_int_array = other.value;
			other.value = this->value;
			this->value = temp_int_array;
		}
		~custom_linked_list_node()
		{
			if (this->value != nullptr)
				delete this->value;
		}
	};
	//template for doubly-linked list that has "iterators" (pointers to nodes) that are guaranteed to be safe even after manipulating the list,
	//and even after transfering nodes from one array to another using the deatch function.
	//Everything dynamically allocated that exists inside the list is deleted automatically when clear is called and when the destructor is called
	template <typename T>
	class custom_linked_list
	{
	protected:
	//_first and _last are placeholders, they will (hopefully) never contain a valid value. They're just useful for iteration etc.
		custom_linked_list_node<T> _first;
		custom_linked_list_node<T> _last;
		size_t _length;
	public:
		//returns the pointer to the item past the end
		custom_linked_list_node<T>* end() { return &this->_last; }
		//returns the pointer to the item just before the beginning
		custom_linked_list_node<T>* begin() { return &this->_first; }
		//Returns the first actual node, or end() if the size is 0
		custom_linked_list_node<T>* first() const { return this->_first.next; }
		//Returns the last actual node, or begin() if the size is 0
		custom_linked_list_node<T>* last() const { return this->_last.previous; }
		size_t size() const { return this->_length; }
		custom_linked_list()
		{
			_first.next = &_last;
			_last.previous = &_first;
			this->_length = (size_t)0;
		}
		//Copy constructor. Don't forget to do .reset_without_deleting_nodes on one of the linked lists
		custom_linked_list(const custom_linked_list<T>& other)
		{
			this->_first.next = other._first.next;
			this->_last.previous = other._last.previous;
			this->_length = other._length;
		}
		//Safely swaps between two lists
		void swap(custom_linked_list<T>& other)
		{
			other._first.swap(this->_first);
			other._last.swap(this->_last);
			const size_t temp_int = other._length;
			other._length = this->_length;
			this->_length = temp_int;
		}
		//Add a value in the list just after "first". The argument must be a pointer to a dynamically allocated value
		//and it will automatically be deleted by the list
		//Returns a pointer to the newly added node.
		custom_linked_list_node<T>* push_front(T* value)
		{
			custom_linked_list_node<T>* node_to_prepend = new custom_linked_list_node<T>(value);
			this->push_front(node_to_prepend);
			return node_to_prepend;
		}
		//Add a node in the list just after "first". The argument must be a pointer to a dynamically allocated node
		//and it will automatically be deleted by the list
		void push_front(custom_linked_list_node<T>* node_to_prepend)
		{
			this->_first.next->previous = node_to_prepend;
			node_to_prepend->next = this->_first.next;
			this->_first.next = node_to_prepend;
			node_to_prepend->previous = &this->_first;
			++this->_length;
		}
		//Add a value in the list just before "last". The argument must be a pointer to a dynamically allocated value
		//and it will automatically be deleted by the list
		//Returns a pointer to the newly added node.
		custom_linked_list_node<T>* push_back(T* value)
		{
			custom_linked_list_node<T> *const node_to_append = new custom_linked_list_node<T>(value);
			this->push_back(node_to_append);
			return node_to_append;
		}
		//Add a node in the list just before "last". The argument must be a pointer to a dynamically allocated node
		//and it will automatically be deleted by the list
		void push_back(custom_linked_list_node<T>* node_to_append)
		{
			this->_last.previous->next = node_to_append;
			node_to_append->previous = this->_last.previous;
			this->_last.previous = node_to_append;
			node_to_append->next = &this->_last;
			++this->_length;
		}
		//Returns the next element after the one that was erased.
		//The node that the pointer in the argument in pointing to, will automatically be deleted.
		//Don't attempt to erase the beginning or end nodes, and don't attempt to erase a node that doesn't exist in this list.
		custom_linked_list_node<T>* erase(custom_linked_list_node<T>* node_to_remove)
		{
			custom_linked_list_node<T> *const next_element = node_to_remove->next;
			node_to_remove->next->previous = node_to_remove->previous;
			node_to_remove->previous->next = node_to_remove->next;
			--this->_length;
			delete node_to_remove;
			return next_element;
		}
		//Same as erase except that instead of the node being deleted it simply detaches the node from the list
		//The node can then be put inside a different list, or in a different place in this list, or even manually deleted. All the while the pointer (A.K.A iterator) stays "valid".
		//Returns the next item in the list after the node to extract.
		custom_linked_list_node<T>* detach(custom_linked_list_node<T>* node_to_extract)
		{
			custom_linked_list_node<T>* const next_element = node_to_extract->next;
			node_to_extract->next->previous = node_to_extract->previous;
			node_to_extract->previous->next = node_to_extract->next;
			--this->_length;
			return next_element;
		}
		//Creates a new list from a chunk of the original list.
		//Receives nodes from the list, they can be the same node, but the first node start_sublist needs to be closer to _first than the second node parameter end_sublist
		//The sublist function returns a pointer to a dynamically allocated linked list, and that pointer needs to be deleted.
		//The function also receives the length of the sublist. Don't lie!
		custom_linked_list<T>* sublist(custom_linked_list_node<T>* start_sublist, custom_linked_list_node<T>* end_sublist, size_t sublist_len)
		{
			custom_linked_list<T>* sublist = new custom_linked_list<T>;
			start_sublist->previous->next = end_sublist->next;
			end_sublist->next->previous = start_sublist->previous;
			start_sublist->previous = &sublist->_first;
			sublist->_first.next = start_sublist;
			end_sublist->next = &sublist->_last;
			sublist->_last.previous = end_sublist;
			this->_length -= sublist_len;
			sublist->_length = sublist_len;
			return sublist;
		}
		//erases the last element in the list of the size of the list is greater than 0
		void pop_back()
		{
			if (this->_length > (size_t)0)
			{
				custom_linked_list_node<T>* const last_element = this->_last.previous;
				last_element->previous->next = &this->_last;
				this->_last.previous = last_element->previous;
				delete last_element;
				--this->_length;
			}
		}
		//erases the first element in the list if the size of the list is greater than 0
		void pop_front()
		{
			if (this->_length > (size_t)0)
			{
				custom_linked_list_node<T>* const first_element = this->_first.next;
				first_element->next->previous = &this->_first;
				this->_first.next = first_element->next;
				delete first_element;
				--this->_length;
			}
		}
		//insert an element before a specific node in the list.
		//The object that the pointer is pointing to will automatically be deleted.
		//Returns a pointer to the node that was created
		custom_linked_list_node<T>* insert(custom_linked_list_node<T>* insert_before, T* value_to_insert)
		{
			custom_linked_list_node<T>* const node_to_insert = new custom_linked_list_node<T>(value_to_insert);
			this->insert(insert_before, node_to_insert);
			return node_to_insert;
		}
		//insert an element before a specific node in the list.
		//The object that the pointer is pointing to will automatically be deleted.
		void insert(custom_linked_list_node<T>* insert_before, custom_linked_list_node<T>* node_to_insert)
		{
			insert_before->previous->next = node_to_insert;
			node_to_insert->previous = insert_before->previous;
			node_to_insert->next = insert_before;
			insert_before->previous = node_to_insert;
			++this->_length;
		}
		//Prepends the values of one list to this one without changing the order of the list (first to last).
		//Resets the source list to an empty list.
		void prepend(custom_linked_list<T>& source)
		{
			if (source._length > 0)
			{
				custom_linked_list_node<T>* const this_first = this->first();
				custom_linked_list_node<T>* const source_first = source.first();
				custom_linked_list_node<T>* const source_last = source.last();
				this_first->previous = source_last;
				source_last->next = this_first;
				source_first->previous = &this->_first;
				this->_first.next = source_first;
				this->_length += source._length;
				source.reset_without_deleting_nodes();
			}
		}
		//Appends the values of one list to this one without changing the order or the list (first to last).
		//Resets the source list to an empty list.
		void append(custom_linked_list<T>& source)
		{
			if (source._length > 0)
			{
				custom_linked_list_node<T>* const this_last = this->last();
				custom_linked_list_node<T>* const source_first = source.first();
				custom_linked_list_node<T>* const source_last = source.last();
				this_last->next = source_first;
				source_first->previous = this_last;
				source_last->next = &this->_last;
				this->_last.previous = source_last;
				this->_length += source._length;
				source.reset_without_deleting_nodes();
			}
		}
		//Goes through all of the nodes in the list and deletes them, and their internal values (using the destructors of the nodes themselves).
		void clear()
		{
			custom_linked_list_node<T>* it = this->_first.next;
			while (it != &this->_last)
			{
				custom_linked_list_node<T>* const it_next = it->next;
				delete it;
				it = it_next;
			}
			this->_first.next = &this->_last;
			this->_last.previous = &this->_first;
			this->_length = (size_t)0;
		}
		//Resets the list. The list will then be valid to use again. Doesn't delete the any of the "custom_Node"s or of the values that exist inside of those nodes.
		void reset_without_deleting_nodes()
		{
			this->_first.next = &this->_last;
			this->_last.previous = &this->_first;
			this->_length = (size_t)0;
		}
		~custom_linked_list() { this->clear(); }
	};
}
#endif