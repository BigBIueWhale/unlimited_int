#ifndef UNLIMITED_LIST_OF_INT_ARRAYS_H
#define UNLIMITED_LIST_OF_INT_ARRAYS_H
#include "int_array_list.hpp"
#include "int_array_piggy_bank.hpp"
namespace unlimited
{
	class list_of_int_arrays : public int_array_list
	{
	private:
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		static thread_local int_array_piggy_bank bank_storage; //definition is in unlimited/unlimited.cpp
#else
		static int_array_piggy_bank bank_storage; //definition is in unlimited/unlimited.cpp
#endif
	public:
		list_of_int_arrays() {}
		list_of_int_arrays(const list_of_int_arrays& num_to_assign);
		void fill_0_until_num_of_ints_and_set_variables_for_used_accordingly(many_bits fill_0_until);
		void flush_to_piggy_bank();
		void flush_significant_to_piggy_bank(many_bits num_of_nodes_to_deposit);
		void flush_insignificant_to_piggy_bank(many_bits num_of_nodes_to_deposit);
		void withdraw_from_piggy_bank(many_bits length_to_make);
		void increase_by_one_array_from_piggy_bank();
		void increase_by_one_array_from_piggy_bank_to_insignificant();
		void increase_by_one_array_and_zero();
		void increase_by_one_array();
		void increase_by_one_array_to_insignificant();
		void increase_until_num_of_ints(many_bits num_of_ints_to_increase_until);
		static void flush_piggy_bank();
#if DEBUG_MODE > 0
#endif
	};
}
#endif
