#include "ListArrays.hpp"
using namespace unlimited;
#if DEBUG_MODE > 0
#include <iostream>
#endif
#if DEBUG_MODE > 0
void ListArrays::print_properties() const
{
	std::cout << "\nListArrays->length: " << this->length;
}
#endif
void ListArrays::prepend(ListArrays& la_to_prepend)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"ListArrays::prepend(ListArrays& la_to_prepend)\".";
#endif
#if DEBUG_MODE >= 1
	this->find_inconsistencies();
	la_to_prepend.find_inconsistencies();
#endif
	if (la_to_prepend.length == 0) { return; }
	if (this->length == 0) { this->operator=(la_to_prepend); }
	else
	{
		this->length += la_to_prepend.length;
		this->first->previous = la_to_prepend.last;
		la_to_prepend.last->next = this->first;
		this->first = la_to_prepend.first;
	}
	la_to_prepend.length = 0;
	la_to_prepend.first = nullptr;
	la_to_prepend.last = nullptr;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"ListArrays::prepend(ListArrays& la_to_prepend)\".";
#endif
#if DEBUG_MODE >= 1
	this->find_inconsistencies();
	la_to_prepend.find_inconsistencies();
#endif
}
void ListArrays::append(ListArrays& la_to_append)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"ListArrays::append(ListArrays& la_to_append)\".";
#endif
#if DEBUG_MODE >= 1
	this->find_inconsistencies();
	la_to_append.find_inconsistencies();
#endif
	if (la_to_append.length == 0) { return; }
	if (this->length == 0) { this->operator=(la_to_append); }
	else
	{
		this->length += la_to_append.length;
		this->last->next = la_to_append.first;
		la_to_append.first->previous = this->last;
		this->last = la_to_append.last;
	}
	la_to_append.length = 0;
	la_to_append.first = nullptr;
	la_to_append.last = nullptr;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"ListArrays::append(ListArrays& la_to_append)\".";
#endif
#if DEBUG_MODE >= 1
	this->find_inconsistencies();
	la_to_append.find_inconsistencies();
#endif
}
#if DEBUG_MODE > 0
many_bits ListArrays::double_check_length() const
{
	Node* it = this->first;
	many_bits counter = 0;
	while (it != nullptr)
	{
		++counter;
		it = it->next;
	}
	return counter;
}
bool ListArrays::find_inconsistencies() const
{
	bool to_return_error = false;
	many_bits length_this = this->double_check_length();
	if (length_this != this->length)
	{
		std::cout << "\nError found by function \"ListArrays::find_inconsistencies()\": the actual length of the list is: " << length_this <<
			" and the length in \"this->length\" is: " << this->length << " (They should be equal)";
		to_return_error = true;
	}
	if ((this->first == nullptr) != (this->last == nullptr))
	{
		std::cout << "\nError found by function \"ListArrays::find_inconsistencies()\": "
			<< "(this->first == nullptr) != (this->last == nullptr)";
		to_return_error = true;
	}
	if (this->first != nullptr)
	{
		if (this->first->previous != nullptr)
		{
			std::cout << "\nError found by function \"ListArrays::find_inconsistencies()\": "
				<< "this->first->previous != nullptr";
			to_return_error = true;
		}
		if (this->length == 1 && this->first->next == this->last)
		{
			std::cout << "\nError found by function \"ListArrays::find_inconsistencies()\": "
				<< "this->length == 1 and this->first->next == this->last";
			to_return_error = true;
		}
	}
	if (this->last != nullptr)
	{
		if (this->last->next != nullptr)
		{
			std::cout << "\nError found by function \"ListArrays::find_inconsistencies()\": "
				<< "this->last->next != nullptr";
			to_return_error = true;
		}
		if (this->length == 1 && this->last->previous == this->first)
		{
			std::cout << "\nError found by function \"ListArrays::find_inconsistencies()\": "
				<< "this->length == 1 and this->last->previous == this->first";
			to_return_error = true;
		}
	}
	return to_return_error;
}
void ListArrays::print_all() const
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"ListArrays::print_all()\".";
#endif
#if DEBUG_MODE >= 1
	this->find_inconsistencies();
#endif
	this->print_properties();
	Node* it = this->last;
	many_bits_signed arr_index = this->length - 1;
	while (it != nullptr)
	{
		std::cout << "\narray index: " << arr_index << ":\n";
		it->value->print_all();
		it = it->previous;
		--arr_index;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"ListArrays::print_all()\".";
#endif
#if DEBUG_MODE >= 1
	this->find_inconsistencies();
#endif
}
#endif
