#include "unlimited.hpp"
namespace unlimited
{
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
	//defined the piggy bank before current_random so that current_random will be destroyed first, to prevent memory leak.
	thread_local int_array_piggy_bank list_of_int_arrays::bank_storage;
	thread_local unlimited_int unlimited_int::current_random;
	thread_local reciprocals_database unlimited_int::Newton_Raphson_lookup;
#else
	//defined the piggy bank before current_random so that current_random will be destroyed first, to prevent memory leak.
	int_array_piggy_bank list_of_int_arrays::bank_storage;
	unlimited_int unlimited_int::current_random;
	reciprocals_database unlimited_int::Newton_Raphson_lookup;
#endif
	void delete_all_static_memory()
	{
		unlimited::unlimited_int::clear_Newton_Raphson_lookup();
		unlimited::unlimited_int::flush_current_random();
		unlimited::list_of_int_arrays::flush_piggy_bank();
	}
}
