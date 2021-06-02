#ifndef NODEOFLISTARRAYS_HEADER
#define NODEOFLISTARRAYS_HEADER true
#include "int_array.h"
struct Node
{
	Node* previous;
	Node* next;
	int_array* value;
	Node()
	{
		this->previous = nullptr;
		this->next = nullptr;
		this->value = nullptr;
	}
	Node(const Node& la) { this->operator=(la); }
	Node(int_array* value)
	{
		this->value = value;
		this->next = nullptr;
		this->previous = nullptr;
	}
	void operator=(const Node& la)
	{
		this->previous = la.previous;
		this->next = la.next;
		this->value = la.value;
	}
	void swap(Node& other)
	{
		Node* temp_Node = other.previous;
		other.previous = this->previous;
		this->previous = temp_Node;

		temp_Node = other.next;
		other.next = this->next;
		this->next = temp_Node;

		int_array* temp_int_array;
		temp_int_array = other.value;
		other.value = this->value;
		this->value = temp_int_array;
	}
};
#endif