#include "int_array_piggy_bank.h"
void int_array_piggy_bank::deposit_from_significant(int_array_list& source, many_bits num_of_nodes_to_deposit)
{
#if DEBUG_MODE == 2
	std::cout << "\nstart of \"deposit_from_significant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || source.find_inconsistencies()) {
		std::cerr << "\nThe inconsistency was found at the start of \"deposit_from_significant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
	}
#endif
	many_bits num_of_nodes_in_source = source.intarrays.length;
	if (num_of_nodes_in_source == 0) { return; }
	if (num_of_nodes_to_deposit >= num_of_nodes_in_source) { this->deposit(source); return; }
	Node* it_source = source.intarrays.last;
	many_bits num_of_nodes_so_far = 1, sum_ints = it_source->value->intarr_len;
	while (num_of_nodes_so_far < num_of_nodes_to_deposit)
	{
		it_source = it_source->previous;
		sum_ints += it_source->value->intarr_len;
		++num_of_nodes_so_far;
	}
	if (this->intarrays.length == 0)
	{
		this->intarrays.first = it_source;
		this->intarrays.last = source.intarrays.last;
		source.intarrays.last = it_source->previous;
		source.intarrays.last->next = nullptr;
		it_source->previous = nullptr;
	}
	else
	{
		this->intarrays.last->next = it_source;
		Node* it_source_previous = it_source->previous;
		it_source->previous = this->intarrays.last;
		this->intarrays.last = source.intarrays.last;
		source.intarrays.last = it_source_previous;
		it_source_previous->next = nullptr;
	}
	source.num_of_ints -= sum_ints;
	source.intarrays.length -= num_of_nodes_to_deposit;
	this->num_of_ints += sum_ints;
	this->intarrays.length += num_of_nodes_to_deposit;
	this->obey_PIGGY_BANK_MAXIMUM();
#if DEBUG_MODE == 2
	std::cout << "\nend of \"deposit_from_significant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || source.find_inconsistencies()) {
		std::cerr << "\nThe inconsistency was found at the end of \"deposit_from_significant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
	}
#endif
}
void int_array_piggy_bank::deposit_from_insignificant(int_array_list& source, many_bits num_of_nodes_to_deposit)
{
#if DEBUG_MODE == 2
	std::cout << "\nstart of \"deposit_from_insignificant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || source.find_inconsistencies())
		std::cerr << "\nThe inconsistency was found in start of function: \"deposit_from_insignificant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
#endif
	many_bits num_of_nodes_in_source = source.intarrays.length;
	if (num_of_nodes_in_source == 0) { return; }
	if (num_of_nodes_to_deposit >= num_of_nodes_in_source) { this->deposit(source); return; }
	if (num_of_nodes_to_deposit == 0) { return; }
	Node* it_source = source.intarrays.first;
	many_bits num_of_nodes_so_far = 1, sum_ints = it_source->value->intarr_len;
	while (num_of_nodes_so_far < num_of_nodes_to_deposit)
	{
		it_source = it_source->next;
		sum_ints += it_source->value->intarr_len;
		++num_of_nodes_so_far;
	}
	if (this->intarrays.length == 0)
	{
		this->intarrays.first = source.intarrays.first;
		this->intarrays.last = it_source;
		source.intarrays.first = it_source->next;
		if (source.intarrays.first == nullptr) { source.intarrays.last = nullptr; }
		else { source.intarrays.first->previous = nullptr; it_source->next = nullptr; }
	}
	else
	{
		this->intarrays.last->next = source.intarrays.first;
		Node* it_source_next = it_source->next;
		it_source->next = nullptr;
		source.intarrays.first->previous = this->intarrays.last;
		this->intarrays.last = it_source;
		source.intarrays.first = it_source_next;
		if (it_source_next == nullptr) { source.intarrays.last = nullptr; }
		else { it_source_next->previous = nullptr; }
	}
	source.num_of_ints -= sum_ints;
	source.intarrays.length -= num_of_nodes_to_deposit;
	this->num_of_ints += sum_ints;
	this->intarrays.length += num_of_nodes_to_deposit;
	this->obey_PIGGY_BANK_MAXIMUM();
#if DEBUG_MODE == 2
	std::cout << "\nend of \"deposit_from_insignificant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || source.find_inconsistencies())
		std::cerr << "\nThe inconsistency was found in end of function: \"deposit_from_insignificant(int_array_list& source, many_bits num_of_nodes_to_deposit)\"";
#endif
}
void int_array_piggy_bank::withdraw(int_array_list& deposit_to, many_bits amount_to_make)
{
#if DEBUG_MODE == 2
	std::cout << "\nWithdrawing " << (amount_to_make - deposit_to.num_of_ints) << " ints from: " << this->num_of_ints;
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || deposit_to.find_inconsistencies()) {
		std::cerr << "\nThe inconsistency was found at the start of \"withdraw(int_array_list& deposit_to, many_bits amount_to_make)\"";
	}
#endif
	if ((this->intarrays.length == 0) || (deposit_to.num_of_ints >= amount_to_make)) { return; }
	many_bits amount_to_increase_by = amount_to_make - deposit_to.num_of_ints;
	if (this->num_of_ints < amount_to_increase_by)
	{
		deposit_to.append(*this);
		return;
	}
	Node* it_this = this->intarrays.first;
	many_bits sum = 0, num_int_array = 0;
	while (sum < amount_to_increase_by)
	{
		sum += it_this->value->intarr_len;
		it_this = it_this->next;
		++num_int_array;
	}
	if (deposit_to.intarrays.last == nullptr) { deposit_to.intarrays.first = this->intarrays.first; }
	else
	{
		deposit_to.intarrays.last->next = this->intarrays.first;
		this->intarrays.first->previous = deposit_to.intarrays.last;
	}
	if (it_this == nullptr)
	{
		deposit_to.intarrays.last = this->intarrays.last;
		this->intarrays.first = nullptr;
		this->intarrays.last = nullptr;
	}
	else
	{
		deposit_to.intarrays.last = it_this->previous;
		deposit_to.intarrays.last->next = nullptr;
		this->intarrays.first = it_this;
		this->intarrays.first->previous = nullptr;
	}
	deposit_to.num_of_ints += sum;
	deposit_to.intarrays.length += num_int_array;
	this->num_of_ints -= sum;
	this->intarrays.length -= num_int_array;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"int_array_piggy_bank::withdraw(int_array_list* deposite_to, many_bits amount_to_make)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || deposit_to.find_inconsistencies()) {
		std::cerr << "\nThe inconsistency was found at the end of \"withdraw(int_array_list& deposit_to, many_bits amount_to_make)\"";
	}
#endif
}
void int_array_piggy_bank::obey_PIGGY_BANK_MAXIMUM()
{
	Node* it_this = this->intarrays.last;
	while ((it_this != nullptr) && (this->num_of_ints > PIGGY_BANK_MAXIMUM))
	{
		int_array* it_this_value = it_this->value;
		--this->intarrays.length;
		this->num_of_ints -= it_this_value->intarr_len;
		it_this_value->destroy();
		delete it_this_value;
		Node* it_this_cpy = it_this;
		it_this = it_this->previous;
		delete it_this_cpy;
	}
	this->intarrays.last = it_this;
	if (this->intarrays.last == nullptr) { this->intarrays.first = nullptr; }
	else { this->intarrays.last->next = nullptr; }
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"int_array_piggy_bank::obey_PIGGY_BANK_MAXIMUM()\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies()) {
		std::cerr << "\nThe inconsistency was found at the end of \"int_array_piggy_bank::obey_PIGGY_BANK_MAXIMUM()\"";
	}
#endif
}
#if DEBUG_MODE > 0
bool int_array_piggy_bank::find_inconsistencies()
{
	bool to_return_true = false;
	if (this->num_of_ints > PIGGY_BANK_MAXIMUM)
	{
		std::cerr << "\nError found by function \"int_array_piggy_bank::find_inconsistencies()\", num_of_ints > PIGGY_BANK_MAXIMUM";
		to_return_true = true;
	}
	return (this->int_array_list::find_inconsistencies() || to_return_true);
}
#endif
bool int_array_piggy_bank::withdraw_one_Node(int_array_list& deposit_to)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"withdraw_one_Node(int_array_list& deposit_to, many_bits amount_to_withdraw)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || deposit_to.find_inconsistencies())
		std::cerr << "\nThe inconsistency was found in start of function: \"withdraw_one_Node(int_array_list& deposit_to)\"";
#endif
	if (this->intarrays.length == 0) { return false; }
	many_bits num_of_ints_withdrawn = this->intarrays.first->value->intarr_len;
	if (deposit_to.intarrays.length == 0)
	{
		deposit_to.intarrays.first = this->intarrays.first;
		deposit_to.intarrays.last = this->intarrays.first;
		this->intarrays.first = this->intarrays.first->next;
		deposit_to.intarrays.last->next = nullptr;
		if (this->intarrays.first != nullptr) { this->intarrays.first->previous = nullptr; }
		else { this->intarrays.last = nullptr; }
	}
	else
	{
		deposit_to.intarrays.last->next = this->intarrays.first;
		this->intarrays.first->previous = deposit_to.intarrays.last;
		deposit_to.intarrays.last = this->intarrays.first;
		this->intarrays.first = this->intarrays.first->next;
		deposit_to.intarrays.last->next = nullptr;
		if (this->intarrays.first != nullptr) { this->intarrays.first->previous = nullptr; }
		else { this->intarrays.last = nullptr; }
	}
	--this->intarrays.length;
	++deposit_to.intarrays.length;
	this->num_of_ints -= num_of_ints_withdrawn;
	deposit_to.num_of_ints += num_of_ints_withdrawn;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"withdraw_one_Node(int_array_list& deposit_to, many_bits amount_to_withdraw)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || deposit_to.find_inconsistencies())
		std::cerr << "\nThe inconsistency was found in end of function: \"withdraw_one_Node(int_array_list& deposit_to)\"";
#endif
	return true;
}
bool int_array_piggy_bank::withdraw_one_Node_to_insignificant(int_array_list& deposit_to)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"withdraw_one_Node_to_insignificant(int_array_list& deposit_to, many_bits amount_to_withdraw)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || deposit_to.find_inconsistencies())
		std::cerr << "\nThe inconsistency was found in start of function: \"withdraw_one_Node_to_insignificant(int_array_list& deposit_to)\"";
#endif
	if (this->intarrays.length == 0) { return false; }
	many_bits num_of_ints_withdrawn = this->intarrays.first->value->intarr_len;
	if (deposit_to.intarrays.length == 0)
	{
		deposit_to.intarrays.first = this->intarrays.first;
		deposit_to.intarrays.last = this->intarrays.first;
		this->intarrays.first = this->intarrays.first->next;
		deposit_to.intarrays.last->next = nullptr;
		if (this->intarrays.first != nullptr) { this->intarrays.first->previous = nullptr; }
		else { this->intarrays.last = nullptr; }
	}
	else
	{
		deposit_to.intarrays.first->previous = this->intarrays.first;
		Node* this_first_next = this->intarrays.first->next;
		this->intarrays.first->next = deposit_to.intarrays.first;
		deposit_to.intarrays.first = this->intarrays.first;
		this->intarrays.first = this_first_next;
		if (this_first_next != nullptr) { this_first_next->previous = nullptr; }
		else { this->intarrays.last = nullptr; }
	}
	--this->intarrays.length;
	++deposit_to.intarrays.length;
	this->num_of_ints -= num_of_ints_withdrawn;
	deposit_to.num_of_ints += num_of_ints_withdrawn;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"withdraw_one_Node(int_array_list& deposit_to, many_bits amount_to_withdraw)\"";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies() || deposit_to.find_inconsistencies())
		std::cerr << "\nThe inconsistency was found in end of function: \"withdraw_one_Node(int_array_list& deposit_to)\"";
#endif
	return true;
}
