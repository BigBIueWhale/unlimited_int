#ifndef INT_ARRAY_STRUCT_WAS_DECLARED
#define INT_ARRAY_STRUCT_WAS_DECLARED true
#define IS_64_BIT_SYSTEM true
#define DEBUG_MODE 0 //0 for no error scanning using function(s) "find_inconsistencies", 1 for yes, 2 for scan with print, -2 for only memory counting
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#undef _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <string>
#include <iomanip>
#include <sstream>
#if IS_64_BIT_SYSTEM
typedef uint32_t few_bits;
typedef uint64_t many_bits;
#define NUM_OF_BITS_few_bits 32
#define NUM_OF_BITS_many_bits 64
typedef int32_t few_bits_signed;
typedef int64_t many_bits_signed;
#define MASK_LOW_BITS (many_bits)0xffffffff
#define MAX_few_bits_NUM (many_bits_signed)0xffffffff
#define MAX_few_bits_NUM_PLUS_ONE (many_bits_signed)0x100000000
#define MAX_many_bits_NUM (many_bits)0xffffffffffffffff
#else
typedef uint16_t few_bits;
typedef uint32_t many_bits;
#define NUM_OF_BITS_few_bits 16
#define NUM_OF_BITS_many_bits 32
typedef int16_t few_bits_signed;
typedef int32_t many_bits_signed;
#define MASK_LOW_BITS (many_bits)0xffff
#define MAX_few_bits_NUM (many_bits_signed)0xffff
#define MAX_few_bits_NUM_PLUS_ONE (many_bits_signed)0x10000
#define MAX_many_bits_NUM (many_bits)0xffffffff
#endif
#define PIGGY_BANK_MAXIMUM ((many_bits)(805306368)) //in num of ints, or 3 gigabytes (each int is 32 bits in 64bit mode)
//#define PIGGY_BANK_MAXIMUM ((many_bits)(100000000000000000)) //in num of ints, or too much to matter
//#define MAX_ALLOC 1048576 //2^20
#define MAX_ALLOC 16384 //per single int array.
#define MIN_ALLOC 4096 //per single int array (2048 32bits)^2 = (64 32bit integers)^2
//#define MIN_ALLOC 1
//#define MIN_ALLOC 20
struct int_array
{
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
	static many_bits num_of_ints_created;
	static many_bits num_of_ints_destroyed;
#endif
	few_bits* intarr;
	many_bits intarr_len;
	many_bits num_of_used_ints;
	int_array() { this->set_null(); }
	int_array(many_bits size_to_make)
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
	void assign(few_bits num_to_assign);
	void compact_print();
	char* create_string();
	void print_all();
	void fillzero();
	void fillzero_until(many_bits until_index_including);
	void resize_and_fillzero(many_bits size_to_make)
	{
		if (size_to_make == 0)
		{
			this->num_of_used_ints = 0;
		}
		else
		{
			this->intarr_len = size_to_make;
			this->intarr = new few_bits[size_to_make];
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
			int_array::num_of_ints_created += size_to_make;
#endif
			this->fillzero();
		}
	}
	void resize(many_bits size_to_make)
	{
		if (size_to_make == 0) { this->num_of_used_ints = 0; }
		else
		{
			this->intarr_len = size_to_make;
			this->intarr = new few_bits[size_to_make];
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
			int_array::num_of_ints_created += size_to_make;
#endif
		}
	}
	void destroy()
	{
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
		int_array::num_of_ints_destroyed += this->intarr_len;
#endif
		delete[] intarr;
		intarr = nullptr;
	}
	void set_num_of_used_ints_to_maximum() { this->num_of_used_ints = this->intarr_len; }
#if DEBUG_MODE > 0
	bool find_inconsistencies();
#endif
	bool is_all_used_zeros();
	many_bits_signed find_first_used_not_zero(); //returns -1 if not found
	void shift_right(many_bits num_of_ints_to_shift_right_by);
	bool is_full() { return (this->intarr_len == this->num_of_used_ints); }
	void shift_left_by_one();
	void push_to_insignificant(few_bits num_to_push)
	{
		this->shift_left_by_one();
		*this->intarr = num_to_push;
	}
};
#endif
