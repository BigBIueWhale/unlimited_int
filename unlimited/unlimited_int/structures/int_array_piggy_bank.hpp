#ifndef UNLIMITED_INT_ARRAY_PIGGY_BANK_H
#define UNLIMITED_INT_ARRAY_PIGGY_BANK_H true
#include "int_array_list.hpp"
namespace unlimited
{
	class int_array_piggy_bank : public int_array_list
	{
	public:
		int_array_piggy_bank() {}
		void withdraw(int_array_list& deposit_to, size_t amount_to_withdraw);
		bool withdraw_one_Node_to_significant(int_array_list& deposit_to);
		bool withdraw_one_Node_to_insignificant(int_array_list& deposit_to);
		void deposit(int_array_list& source)
		{
			this->append(source);
			this->obey_PIGGY_BANK_MAXIMUM();
		}
		void deposit_from_significant(int_array_list& source, size_t num_of_nodes_to_deposit);
		void deposit_from_insignificant(int_array_list& source, size_t num_of_nodes_to_deposit);
		void obey_PIGGY_BANK_MAXIMUM();
	};
}
#endif
