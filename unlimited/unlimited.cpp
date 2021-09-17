#include "unlimited.hpp"
#if (UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0) || (UNLIMITED_INT_LIBRARY_DEBUG_MODE == -2)
std::atomic<uint64_t> unlimited::int_array::num_of_ints_created((uint64_t)0);
std::atomic<uint64_t> unlimited::int_array::num_of_ints_destroyed((uint64_t)0);
unlimited::int_array::find_memory_leaks unlimited::int_array::automatic_memory_leak_checker_on_program_end;
#endif
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
void unlimited::assert_no_memory_leak()
{
	unlimited::delete_all_static_memory();
	unlimited::int_array::automatic_memory_leak_checker_on_program_end.assert_no_memory_leak();
}
static_assert(UNLIMITED_INT_NUM_OF_BITS_few_bits == sizeof(unlimited::few_bits) * 8, "Assertion error: NUM_OF_BITS_few_bits is wrong.");
static_assert(UNLIMITED_INT_NUM_OF_BITS_many_bits == sizeof(unlimited::many_bits) * 8, "Assertion error: NUM_OF_BITS_many_bits is wrong.");
static_assert(UNLIMITED_INT_NUM_OF_BITS_few_bits * 2 == UNLIMITED_INT_NUM_OF_BITS_many_bits, "Assertion error: NUM_OF_BITS_many_bits must have exactly twice the number of bits as NUM_OF_BITS_few_bits");
static_assert(sizeof(unlimited::few_bits_signed) == sizeof(unlimited::few_bits), "Assertion error: few_bits_signed needs to be the signed version of few_bits (needs to have the same number of bytes)");
static_assert(sizeof(unlimited::many_bits_signed) == sizeof(unlimited::many_bits), "Assertion error: many_bits_signed needs to be the signed version of many_bits (needs to have the same number of bytes)");
