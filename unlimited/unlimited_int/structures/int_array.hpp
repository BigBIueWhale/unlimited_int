#ifndef UNLIMITED_INT_ARRAY_H
#define UNLIMITED_INT_ARRAY_H true
#include "settings.h" //int_array.hpp is the basic structure of the entire unlimited library. Therefore, settings.h must be included here, and doesn't need to be included anywhere else.
#if ((DEBUG_MODE > 0) || (DEBUG_MODE == -2)) && UNLIMITED_INT_SUPPORT_MULTITHREADING //The mutex is only needed for num_of_ints_created and num_of_ints_destroyed which are debugging variables.
#include <mutex>
#endif
namespace unlimited
{
	struct int_array
	{
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
		static uint64_t num_of_ints_created;
		static uint64_t num_of_ints_destroyed;
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		//since num_of_ints_created and num_of_ints_destroyed are static variables, they both need an std::mutex to make them thread-safe.
		static std::mutex num_of_ints_created_guard;
		static std::mutex num_of_ints_destroyed_guard;
#endif
#endif
		few_bits* intarr;
		size_t intarr_len;
		size_t num_of_used_ints;
		int_array() { this->set_null(); }
		int_array(const size_t size_to_make)
		{
			this->set_null();
			this->resize(size_to_make);
		}
		void set_null()
		{
			this->intarr_len = (size_t)0;
			this->num_of_used_ints = (size_t)0;
			this->intarr = nullptr;
		}
		void assign(const few_bits num_to_assign);
		void fillzero();
		void fillzero_until(size_t until_index_including);
		void resize_and_fillzero(size_t size_to_make);
		void resize(size_t size_to_make);
		void destroy();
		void set_num_of_used_ints_to_maximum() { this->num_of_used_ints = this->intarr_len; }
#if DEBUG_MODE > 0
		bool find_inconsistencies();
#endif
		//Sets the boolean to false if not found
		size_t find_first_used_not_zero(bool* found) const;
		void shift_right(size_t num_of_ints_to_shift_right_by);
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
