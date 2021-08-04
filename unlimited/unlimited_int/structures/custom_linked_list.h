#ifndef CUSTOM_LINKED_LIST_H
#define CUSTOM_LINKED_LIST_H
#include "settings.h"
namespace unlimited
{
	//template for node for doubly-linked list
	template <typename T>
	class custom_Node
	{
	public:
		custom_Node<T>* previous;
		custom_Node<T>* next;
		T* value;
		//disable the copy constructor
		custom_Node(const custom_Node<T>&) = delete;
		custom_Node()
		{
			this->previous = nullptr;
			this->next = nullptr;
			this->value = nullptr;
		}
		custom_Node(T* value)
		{
			this->next = nullptr;
			this->previous = nullptr;
			this->value = value;
		}
		void swap(custom_Node<T>& other)
		{
			Node* temp_Node = other.previous;
			other.previous = this->previous;
			this->previous = temp_Node;

			temp_Node = other.next;
			other.next = this->next;
			this->next = temp_Node;

			T* temp_int_array;
			temp_int_array = other.value;
			other.value = this->value;
			this->value = temp_int_array;
		}
		~custom_Node()
		{
			if (this->value != nullptr)
				delete this->value;
		}
	};
	//template for doubly-linked list that has "iterators" (pointers to nodes) that are guaranteed to be safe even after manipulating the list,
	//and even after transfering nodes from one array to another using the deatch function.
	//Everything dynamically allocated that exists inside the list is deleted automatically when clear is called and when the destructor is called
	template <typename T>
	class custom_List
	{
	protected:
	//first and last are placeholders, they will never contain a valid value. They're just useful for iteration etc.
		custom_Node<T> _first;
		custom_Node<T> _last;
		many_bits _length;
	public:
		//returns the pointer to the item past the end
		custom_Node<T>* end() { return &this->_last; }
		//returns the pointer to the item just before the beginning
		custom_Node<T>* begin() { return &this->_first; }
		many_bits size() { return this->_length; }
		custom_List()
		{
			_first.next = &_last;
			_last.previous = &_first;
			this->_length = (many_bits)0;
		}
		//disable the copy constructor
		custom_List(const custom_List<T>&) = delete;
		//Safely swaps between two lists
		void swap(custom_List<T>& other)
		{
			other._first.swap(this->_first);
			other._last.swap(this->_last);
			unlimited::many_bits temp_int;
			temp_int = other._length;
			other._length = this->_length;
			this->_length = temp_int;
		}
		//Add a value in the list just after "first". The argument must be a pointer to a dynamically allocated value
		//and it will automatically be deleted by the list
		void push_front(T* value)
		{
			custom_Node<T>* node_to_prepend = new custom_Node<T>(value);
			this->push_front(node_to_prepend);
		}
		//Add a node in the list just after "first". The argument must be a pointer to a dynamically allocated node
		//and it will automatically be deleted by the list
		void push_front(custom_Node<T>* node_to_prepend)
		{
			this->_first.next->previous = node_to_prepend;
			node_to_prepend->next = this->_first.next;
			this->_first.next = node_to_prepend;
			node_to_prepend->previous = &this->_first;
			++this->_length;
		}
		//Add a value in the list just before "last". The argument must be a pointer to a dynamically allocated value
		//and it will automatically be deleted by the list
		void push_back(T* value)
		{
			custom_Node<T>* const node_to_append = new custom_Node<T>(value);
			this->push_back(node_to_append);
		}
		//Add a node in the list just before "last". The argument must be a pointer to a dynamically allocated node
		//and it will automatically be deleted by the list
		void push_back(custom_Node<T>* node_to_append)
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
		custom_Node<T>* erase(custom_Node<T>* node_to_remove)
		{
			custom_Node<T>* const next_element = node_to_remove->next;
			node_to_remove->next->previous = node_to_remove->previous;
			node_to_remove->previous->next = node_to_remove->next;
			--this->_length;
			delete node_to_remove;
			return next_element;
		}
		//Same as erase except that instead of the node being deleted it simply detaches the node from the list
		//The node can then be put inside a different list, or in a different place in this list, or even manually deleted. All the while the pointer (A.K.A iterator) stays "valid".
		//Returns the next item in the list after the node to extract.
		custom_Node<T>* detach(custom_Node<T>* node_to_extract)
		{
			custom_Node<T>* const next_element = node_to_extract->next;
			node_to_extract->next->previous = node_to_extract->previous;
			node_to_extract->previous->next = node_to_extract->next;
			--this->_length;
			return next_element;
		}
		//erases the last element in the list of the size of the list is greater than 0
		void pop_back()
		{
			if (this->_length > (many_bits)0)
			{
				custom_Node<T>* const last_element = this->_last.previous;
				last_element->previous->next = &this->_last;
				this->_last.previous = last_element->previous;
				delete last_element;
				--this->_length;
			}
		}
		//erases the first element in the list of the size of the list is greater than 0
		void pop_front()
		{
			if (this->_length > (many_bits)0)
			{
				custom_Node<T>* const first_element = this->_first.next;
				first_element->next->previous = &this->_first;
				this->_first.next = first_element->next;
				delete first_element;
				--this->_length;
			}
		}
		//insert an element before a specific node in the list.
		//The object that the pointer is pointing to will automatically be deleted.
		//Returns a pointer to the node that was created
		custom_Node<T>* insert(custom_Node<T>* insert_before, T* value_to_insert)
		{
			custom_Node<T>* const node_to_insert = new custom_Node<T>(value_to_insert);
			this->insert(insert_before, node_to_insert);
			return node_to_insert;
		}
		//insert an element before a specific node in the list.
		//The object that the pointer is pointing to will automatically be deleted.
		void insert(custom_Node<T>* insert_before, custom_Node<T>* node_to_insert)
		{
			insert_before->previous->next = node_to_insert;
			node_to_insert->previous = insert_before->previous;
			node_to_insert->next = insert_before;
			insert_before->previous = node_to_insert;
			++this->_length;
		}
		//Goes through all of the nodes in the list and deletes them, and their internal values (using the destructors of the nodes themselves).
		void clear()
		{
			custom_Node<T>* it = this->_first.next;
			while (it != &this->_last)
			{
				custom_Node<T>* const it_next = it->next;
				delete it;
				it = it_next;
			}
			this->_first.next = &this->_last;
			this->_last.previous = &this->_first;
			this->_length = (many_bits)0;
		}
		~custom_List() { this->clear(); }
	};
}
#endif