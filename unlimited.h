#ifndef UNLIMITED_H
#define UNLIMITED_H
#include "unlimited_int.h"
namespace unlimited
{
	//Deletes all dynamically allocated memory that is tied to static variables such as unlimited::list_of_int_arrays::piggy_bank and such as unlimited::unlimited_int::current_random.
	//This function prepares the program to shut down, allows for the debugging process to figure out whether there was a memory leak in the program.
	//Everything will continue working after calling this function. It will however restart the random number generator (if it's called again) by recreating a seed.
	static void delete_all_static_memory()
	{
		unlimited::unlimited_int::flush_current_random();
		unlimited::list_of_int_arrays::flush_piggy_bank();
	}
}
#endif