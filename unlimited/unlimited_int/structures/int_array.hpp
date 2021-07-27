#ifndef UNLIMITED_INT_ARRAY_H
#define UNLIMITED_INT_ARRAY_H true
#include "settings.h" //int_array.hpp is the basic structure of the entire unlimited library. Therefore, settings.h must be included here, and doesn't need to be included anywhere else.
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2) //The mutex is only needed for num_of_ints_created and num_of_ints_destroyed which are debugging variables.
#include <mutex>
#endif
namespace unlimited
{
	struct int_array
	{
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
		static many_bits num_of_ints_created;
		static many_bits num_of_ints_destroyed;
		//since num_of_ints_created and num_of_ints_destroyed are static variables, they both need an std::mutex to make them thread-safe.
		static std::mutex num_of_ints_created_guard;
		static std::mutex num_of_ints_destroyed_guard;
#endif
		few_bits* intarr;
		many_bits intarr_len;
		many_bits num_of_used_ints;
		int_array() { this->set_null(); }
		int_array(const many_bits size_to_make)
		{
			this->set_null();
			this->resize(size_to_make);
		}
		void set_null()
		{
			this->intarr_len = 0;
			this->intarr = nullptr;
			this->num_of_used_ints = 0;
		}
		void assign(const few_bits num_to_assign);
#if DEBUG_MODE > 0
		void print_all() const;
#endif
		void fillzero();
		void fillzero_until(many_bits until_index_including);
		void resize_and_fillzero(many_bits size_to_make);
		void resize(many_bits size_to_make);
		void destroy();
		void set_num_of_used_ints_to_maximum() { this->num_of_used_ints = this->intarr_len; }
#if DEBUG_MODE > 0
		bool find_inconsistencies();
#endif
		bool is_all_used_zeros() const;
		many_bits_signed find_first_used_not_zero() const; //returns -1 if not found
		void shift_right(many_bits num_of_ints_to_shift_right_by);
		bool is_full() const { return (this->intarr_len == this->num_of_used_ints); }
		void shift_left_by_one();
		void push_to_insignificant(few_bits num_to_push)
		{
			this->shift_left_by_one();
			*this->intarr = num_to_push;
		}
	};
}
#endif
