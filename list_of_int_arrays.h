#ifndef LIST_OF_INT_ARRAYS_STRUCT_WAS_DECLARED
#define LIST_OF_INT_ARRAYS_STRUCT_WAS_DECLARED true
#include "int_array_list.h"
#include "int_array_piggy_bank.h"
class list_of_int_arrays : public int_array_list
{
private:
	static int_array_piggy_bank bank_storage;
public:
	list_of_int_arrays() {}
	list_of_int_arrays(const list_of_int_arrays& num_to_assign);
	void fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(many_bits fill_0_until);
	void flush_to_piggy_bank() { list_of_int_arrays::bank_storage.deposit(*this); }
	void flush_significant_to_piggy_bank(many_bits num_of_nodes_to_deposit) {
		list_of_int_arrays::bank_storage.deposit_from_significant(*this, num_of_nodes_to_deposit); }
	void flush_insignificant_to_piggy_bank(many_bits num_of_nodes_to_deposit) {
		list_of_int_arrays::bank_storage.deposit_from_insignificant(*this, num_of_nodes_to_deposit); }
	void withdraw_from_piggy_bank(many_bits length_to_make) { this->bank_storage.withdraw(*this, length_to_make); }
	void increase_by_one_array_from_piggy_bank() {
		if (! this->bank_storage.withdraw_one_Node(*this)) { this->increase_by_one_array(); }
	}
	void increase_by_one_array_from_piggy_bank_to_insignificant() {
		if (! this->bank_storage.withdraw_one_Node_to_insignificant(*this)) {
			this->increase_by_one_array_to_insignificant(); }
	}
	void increase_by_one_array_and_zero();
	void increase_by_one_array();
	void increase_by_one_array_to_insignificant();
	void increase_until_num_of_ints(many_bits num_of_ints_to_increase_until);
	static void flush_piggy_bank() { list_of_int_arrays::bank_storage.flush(); }
#if DEBUG_MODE > 0
#endif
};

#endif