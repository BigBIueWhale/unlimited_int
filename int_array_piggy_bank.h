#ifndef UNLIMITED_INT_ARRAY_PIGGY_BANK_H
#define UNLIMITED_INT_ARRAY_PIGGY_BANK_H true
#include "int_array_list.h"
namespace unlimited
{
	class int_array_piggy_bank : public int_array_list
	{
	public:
		int_array_piggy_bank() {}
		void withdraw(int_array_list& deposit_to, many_bits amount_to_withdraw);
		bool withdraw_one_Node(int_array_list& deposit_to);
		bool withdraw_one_Node_to_insignificant(int_array_list& deposit_to);
		void deposit(int_array_list& source) {
			this->prepend(source);
			if (this->num_of_ints > PIGGY_BANK_MAXIMUM) { this->obey_PIGGY_BANK_MAXIMUM(); }
		}
		void deposit_from_significant(int_array_list& source, many_bits num_of_nodes_to_deposit);
		void deposit_from_insignificant(int_array_list& source, many_bits num_of_nodes_to_deposit);
		void obey_PIGGY_BANK_MAXIMUM();
#if DEBUG_MODE > 0
		bool find_inconsistencies();
#endif
		~int_array_piggy_bank() { this->flush(); }
	};
}
#endif
