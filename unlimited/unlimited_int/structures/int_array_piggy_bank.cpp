#include "int_array_piggy_bank.hpp"
using namespace unlimited;
void int_array_piggy_bank::deposit_from_significant(int_array_list& source, size_t num_of_nodes_to_deposit)
{
	const size_t num_of_nodes_in_source = source.size();
	if (num_of_nodes_in_source > (size_t)0 && num_of_nodes_to_deposit > (size_t)0)
	{
		if (num_of_nodes_to_deposit >= num_of_nodes_in_source)
		{
			this->deposit(source);
			return;
		}
		custom_linked_list_node<int_array>* it_source = source.last();
		size_t num_of_nodes_so_far = (size_t)1, sum_ints = it_source->value->intarr_len;
		while (num_of_nodes_so_far < num_of_nodes_to_deposit)
		{
			it_source = it_source->previous;
			sum_ints += it_source->value->intarr_len;
			++num_of_nodes_so_far;
		}
		int_array_list *const sublist = source.sublist(it_source, source.last(), num_of_nodes_to_deposit, sum_ints);
		this->append(*sublist);
		delete sublist;
		this->obey_PIGGY_BANK_MAXIMUM();
	}
}
void int_array_piggy_bank::deposit_from_insignificant(int_array_list& source, size_t num_of_nodes_to_deposit)
{
	const size_t num_of_nodes_in_source = source.size();
	if (num_of_nodes_in_source > (size_t)0 && num_of_nodes_to_deposit > (size_t)0)
	{
		if (num_of_nodes_to_deposit >= num_of_nodes_in_source)
		{
			this->deposit(source);
			return;
		}
		custom_linked_list_node<int_array>* it_source = source.first();
		size_t num_of_nodes_so_far = (size_t)1, sum_ints = it_source->value->intarr_len;
		while (num_of_nodes_so_far < num_of_nodes_to_deposit)
		{
			it_source = it_source->next;
			sum_ints += it_source->value->intarr_len;
			++num_of_nodes_so_far;
		}
		int_array_list *const sublist = source.sublist(source.first(), it_source, num_of_nodes_to_deposit, sum_ints);
		this->append(*sublist);
		delete sublist;
		this->obey_PIGGY_BANK_MAXIMUM();
	}
}
void int_array_piggy_bank::withdraw(int_array_list& deposit_to, size_t amount_to_make)
{
	if (this->_length > (size_t)0 && deposit_to.num_of_ints < amount_to_make)
	{
		const size_t amount_to_increase_by = amount_to_make - deposit_to.num_of_ints;
		if (this->num_of_ints < amount_to_increase_by)
		{
			deposit_to.append(*this);
			return;
		}
		custom_linked_list_node<int_array>* it_this = this->begin();
		size_t sum = (size_t)0, num_int_array = (size_t)0;
		while (sum < amount_to_increase_by)
		{
			it_this = it_this->next;
			sum += it_this->value->intarr_len;
			++num_int_array;
		}
		int_array_list* sublist = this->sublist(this->first(), it_this, num_int_array, sum);
		deposit_to.append(*sublist);
		delete sublist;
	}
}
void int_array_piggy_bank::obey_PIGGY_BANK_MAXIMUM()
{
	custom_linked_list_node<int_array>* it_this = this->first();
	while (this->num_of_ints > (size_t)PIGGY_BANK_MAXIMUM)
		it_this = this->erase(it_this);
}
bool int_array_piggy_bank::withdraw_one_Node_to_significant(int_array_list& deposit_to)
{
	if (this->_length == (size_t)0)
		return false;
	custom_linked_list_node<int_array> *const first_node = this->first();
	const size_t num_of_ints_withdrawn = first_node->value->intarr_len;
	this->detach(first_node);
	this->num_of_ints -= num_of_ints_withdrawn;
	deposit_to.push_back(first_node);
	deposit_to.num_of_ints += num_of_ints_withdrawn;
	return true;
}
bool int_array_piggy_bank::withdraw_one_Node_to_insignificant(int_array_list& deposit_to)
{
	if (this->_length == (size_t)0)
		return false;
	custom_linked_list_node<int_array>* const first_node = this->first();
	const size_t num_of_ints_withdrawn = first_node->value->intarr_len;
	this->detach(first_node);
	this->num_of_ints -= num_of_ints_withdrawn;
	deposit_to.push_front(first_node);
	deposit_to.num_of_ints += num_of_ints_withdrawn;
	return true;
}
