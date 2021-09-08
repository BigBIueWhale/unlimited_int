#include "unlimited.hpp"
//defined the piggy bank before current_random so that current_random will be destroyed first, to prevent memory leak.
thread_local unlimited::int_array_piggy_bank unlimited::list_of_int_arrays::bank_storage;
thread_local unlimited::unlimited_int unlimited::unlimited_int::current_random;
thread_local unlimited::reciprocals_database unlimited::unlimited_int::Newton_Raphson_lookup;
std::vector<unlimited::few_bits> unlimited::unlimited_int::small_prime_numbers;
void unlimited::delete_all_static_memory()
{
	unlimited::unlimited_int::clear_small_prime_numbers_array();
	unlimited::unlimited_int::delete_all_thread_local_memory();
}
void unlimited::delete_all_thread_local_memory()
{
	unlimited::unlimited_int::delete_all_thread_local_memory();
}
