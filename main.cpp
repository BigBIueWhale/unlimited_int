//This is just an example file. "The best way to learn is through example"
#include "unlimited/unlimited.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
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
	std::cin.get();
	return 0;
}
using namespace unlimited;
//void helper()
//{
//	while (true)
//	{
//		unlimited_int num_digits_ui;
//		many_bits num_digits;
//		bool input_is_valid = false;
//		while (!input_is_valid)
//		{
//			input_is_valid = true;
//			std::cout << "\n\nEnter number of digits for prime number (or \"exit\" to exit program): ";
//			std::string user_input;
//			std::getline(std::cin, user_input);
//			if (user_input == "exit" || user_input == "\"exit\"")
//				return; //exit from this damn program
//			try
//			{
//				num_digits_ui = unlimited_int::from_string(user_input);
//				if (num_digits_ui < unlimited_int() || num_digits_ui > unlimited_int(~((many_bits)0)) || num_digits_ui > unlimited_int(~((unsigned long long)0)) || num_digits_ui > unlimited_int::from_string("16384"))
//					throw std::invalid_argument("Input too big or too small");
//				num_digits = std::stoull(num_digits_ui.to_string(10), nullptr, 10);
//			}
//			catch (std::exception& e)
//			{
//				std::cout << "\nError parsing input: " << e.what() << ". Try again.";
//				input_is_valid = false;
//			}
//		}
//		if (num_digits == 1)
//		{
//			std::cout << "\nThere\'s no prime number with one bit only.";
//			continue;
//		}
//		else if (num_digits == 0)
//		{
//			std::cout << "\n0 is not a prime number.";
//			continue;
//		}
//		unlimited_int min(unlimited_int((few_bits)1) << num_digits - (many_bits)1);
//		unlimited_int max(unlimited_int((few_bits)1) << (size_t)num_digits);
//		--max;
//		unlimited_int prime(unlimited_int::generate_random_prime_multithreaded(min, max));
//		std::cout << "\nPrime number: " << prime;
//	}
//}
void helper()
{
	const size_t num_of_bits = 512;
	unlimited_int min = unlimited_int((few_bits)1) << num_of_bits - (size_t)1;
	unlimited_int max = (unlimited_int((few_bits)1) << num_of_bits) - unlimited_int(1);
	const auto start_time = std::chrono::steady_clock::now();
	constexpr int NUM_ITERATIONS = 40;
	for (int counter = 0; counter < NUM_ITERATIONS; ++counter)
	{
		std::cout << "\nnumber " << std::setw(4) << std::setfill(' ') << counter + 1 << " / " << NUM_ITERATIONS << ": ";
		std::cout.flush();
		std::cout << unlimited_int::generate_random_prime_multithreaded(min, max);
		std::cout.flush();
	}
	const auto end_time = std::chrono::steady_clock::now();
	const auto time_duration = end_time - start_time;
	const long double time_duration_seconds = static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(time_duration).count()) / static_cast<long double>(1000000000);
	std::cout << "\nTime took: " << time_duration_seconds << " seconds.";
}
