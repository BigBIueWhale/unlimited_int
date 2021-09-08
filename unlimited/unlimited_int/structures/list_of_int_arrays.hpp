#ifndef UNLIMITED_LIST_OF_INT_ARRAYS_H
#define UNLIMITED_LIST_OF_INT_ARRAYS_H
#include "int_array_list.hpp"
#include "int_array_piggy_bank.hpp"
namespace unlimited
{
	class list_of_int_arrays : public int_array_list
	{
	private:
		static thread_local int_array_piggy_bank bank_storage; //definition is in unlimited/unlimited.cpp
	public:
		list_of_int_arrays() {}
		void fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(size_t fill_0_until);
		void flush_to_piggy_bank()
		{
			list_of_int_arrays::bank_storage.deposit(*this);
		}
		void flush_significant_to_piggy_bank(size_t num_of_nodes_to_deposit)
		{
			list_of_int_arrays::bank_storage.deposit_from_significant(*this, num_of_nodes_to_deposit);
		}
		void flush_insignificant_to_piggy_bank(size_t num_of_nodes_to_deposit)
		{
			list_of_int_arrays::bank_storage.deposit_from_insignificant(*this, num_of_nodes_to_deposit);
		}
		void withdraw_from_piggy_bank(size_t length_to_make)
		{
			this->bank_storage.withdraw(*this, length_to_make);
		}
		void increase_by_one_array_from_piggy_bank()
		{
			if (!list_of_int_arrays::bank_storage.withdraw_one_Node_to_significant(*this))
				this->increase_by_one_array();
		}
		void increase_by_one_array_from_piggy_bank_to_insignificant()
		{
			if (!list_of_int_arrays::bank_storage.withdraw_one_Node_to_insignificant(*this))
				this->increase_by_one_array_to_insignificant();
		}
		void increase_by_one_array_and_zero();
		void increase_by_one_array();
		void increase_by_one_array_to_insignificant();
		void increase_until_num_of_ints(const size_t num_of_ints_to_increase_until);
		static void destroy_piggy_bank()
		{
			list_of_int_arrays::bank_storage.clear();
		}
#if DEBUG_MODE > 0
#endif
	};
}
#endif
