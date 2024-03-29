#ifndef UNLIMITED_H
#define UNLIMITED_H
#include "unlimited_int/unlimited_int.hpp"
//You can change unlimited's library settings in file unlimited/unlimited_int/structures/settings.hpp
namespace unlimited
{
	//Deletes all dynamically allocated memory that is tied to static variables such as unlimited::list_of_int_arrays::piggy_bank and such as unlimited::unlimited_int::current_random.
	//This function prepares the program to shut down, allows for the debugging process to figure out whether there was a memory leak in the program.
	//Everything will continue working after calling this function. It will however restart the random number generator (if it's called again) by recreating a seed.
	void delete_all_static_memory();
	void delete_all_thread_local_memory();
	//Will throw std::runtime_error if there was a memory leak (isn't guaranteed to find all memory leaks).
	void assert_no_memory_leak();
}
#endif
