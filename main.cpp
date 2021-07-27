//This is just an example file. "The best way to learn is through example"
#include "unlimited/unlimited.hpp"
#include <iostream>
#include <chrono>
void helper();
int main(int argc, char* argv[])
{
	helper();

#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
	unlimited::delete_all_static_memory(); //not actually required, it's just so that when we print num_of_ints_created and num_of_ints_destroyed we can ensure that there's no memory leak.
	std::cout << "\n\nDEBUG INFORMATION:\n" << std::dec;
	std::cout << "\n$$num_of_ints_created: " << unlimited::int_array::num_of_ints_created;
	std::cout << "\nnum_of_ints_destroyed: " << unlimited::int_array::num_of_ints_destroyed << "\n";
#endif
	return 0;
}
using namespace unlimited;
void helper()
{
	std::cout << "\nEnter the number of bits for prime number: ";
	int bits;
	std::cin >> bits;
	unlimited_int min = unlimited_int::pow(unlimited_int(2), unlimited_int(bits - 1));
	unlimited_int max = *unlimited_int::pow(unlimited_int(2), unlimited_int(bits)) - unlimited_int(1);
	long double sum_seconds = 0.0;
	const int num_of_primes = 20;
	for (int counter = 0; counter < num_of_primes; ++counter)
	{
		const auto start_time = std::chrono::high_resolution_clock::now();
		unlimited_int prime = unlimited_int::generate_random_prime(min, max);
		std::cout << "\nPrime number " << (counter + 1) << ": " << std::dec << prime;
		const auto end_time = std::chrono::high_resolution_clock::now();
		const auto difference_time = end_time - start_time;
		const auto difference_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(difference_time);
		const auto time_took = difference_time_seconds.count();
		sum_seconds += (long double)time_took;
		std::cout << "\nTime took for previous prime: " << time_took;
	}
	const long double average_seconds = sum_seconds / (long double)num_of_primes;
	std::cout << "\nAverage amount of time took: " << average_seconds << " seconds.\n";
}
