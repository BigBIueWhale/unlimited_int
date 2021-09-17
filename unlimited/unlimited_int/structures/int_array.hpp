#ifndef UNLIMITED_INT_ARRAY_H
#define UNLIMITED_INT_ARRAY_H true
#include "settings.hpp" //int_array.hpp is the basic structure of the entire unlimited library. Therefore, settings.hpp must be included here, and doesn't need to be included anywhere else.
#if (UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0) || (UNLIMITED_INT_LIBRARY_DEBUG_MODE == -2) //The atomic is only needed for num_of_ints_created and num_of_ints_destroyed which are debugging variables.
#include <atomic>
#endif
namespace unlimited
{
	struct int_array
	{
#if (UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0) || (UNLIMITED_INT_LIBRARY_DEBUG_MODE == -2)
		static std::atomic<uint64_t> num_of_ints_created;
		static std::atomic<uint64_t> num_of_ints_destroyed;
		class find_memory_leaks
		{
		public:
			find_memory_leaks() noexcept {}
			void assert_no_memory_leak() noexcept(false);
		} static automatic_memory_leak_checker_on_program_end;
#endif
		few_bits* intarr;
		size_t intarr_len;
		//Represents the number of used ints in the array, starting from the beginning of the array which is the most insignificant part of the number.
		//num_of_used_ints will always be smaller or equal to intarr_len.
		size_t num_of_used_ints;
		int_array() { this->set_null(); }
		//Copy constructor- this class has no destructor, so it's not really standard anyways.
		int_array(const int_array& other)
		{
			this->intarr = other.intarr;
			this->intarr_len = other.intarr_len;
			this->num_of_used_ints = other.num_of_used_ints;
		}
		void operator=(const int_array& other)
		{
			this->intarr = other.intarr;
			this->intarr_len = other.intarr_len;
			this->num_of_used_ints = other.num_of_used_ints;
		}
		void set_null()
		{
			this->intarr_len = (size_t)0;
			this->num_of_used_ints = (size_t)0;
			this->intarr = nullptr;
		}
		//Allocates the array if needed, sets the first element of the array, and sets num_of_used_ints to 0
		void assign(const few_bits num_to_assign);
		//Fills the entire array with 0, the actual entire array including the "unused" portion.
		void fillzero();
		//Fills the array with zeroes from the beginning of the array towards the end of the array (the actual end of the array, not according to num_of_used_ints).
		//If the number given is larger than this->intarr_len then the entire number will be zeroed, with no error.
		void fillzero_until(size_t until_index_including);
		//Allocates memory for the array, and fills the entire array's memory with 0. doesn't delete[] a hypotheticaly preexisting array.
		void resize_and_fillzero(const size_t size_to_make);
		//Allocates memory for the array, doesn't delete[] a hypotheticaly preexisting array.
		void resize(const size_t size_to_make);
		//Deletes the array and sets the class member variables back to their default values.
		void destroy();
		//Sets num_of_used_ints to the length of the array.
		void set_num_of_used_ints_to_maximum() { this->num_of_used_ints = this->intarr_len; }
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
		bool find_inconsistencies();
#endif
		//Starts from the most significant used int in the array ("used" is determined by num_of_used_ints),
		//and gradually goes towards the beginning, least significant part of the array.
		//Returns the first occurence of a few_bits integer that isn't 0.
		//Sets the boolean to false if not found.
		size_t find_first_used_not_zero(bool *const found) const;
		//Copies over the used ints to the less significant part of the array, and sets num_of_used_ints accordingly.
		//If the used tries to shift right too much, then the function throws an exception.
		void shift_right(const size_t num_of_ints_to_shift_right_by);
		bool is_full() const { return this->intarr_len == this->num_of_used_ints; }
		//Copies over the ints towards the significant part of the array (the end of the array).
		//Employs to check to see whether the array is long enough, or even whether the array exists.
		//That's why you should check is_full() before using this function.
		void shift_left_by_one();
		//Employs to check to see whether the array is long enough, or even whether the array exists.
		//That's why you should check is_full() before using this function.
		void push_to_insignificant(const few_bits num_to_push)
		{
			this->shift_left_by_one();
			*this->intarr = num_to_push;
		}
	};
}
#endif
