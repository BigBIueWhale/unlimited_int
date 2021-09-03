//This is just an example file. "The best way to learn is through example"
#include "unlimited/unlimited.hpp"
#include <iostream>
#include <string>
#include <sstream>
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
	std::cin.get();
	return 0;
}
using namespace unlimited;
void helper()
{
	const std::string valid_digits = "0123456789";
	while (true)
	{
		int num_digits = -1;
		bool input_is_valid = false;
		while (!input_is_valid)
		{
			input_is_valid = true;
			std::cout << "\n\nEnter number of digits for prime number (or \"exit\" to exit program): ";
			std::string user_input;
			std::getline(std::cin, user_input);
			if (user_input == "exit" || user_input == "\"exit\"")
			{
				return; //exit from this damn program
			}
			if (user_input.length() > 5)
			{
				std::cout << "\nUser input is too long. Try again.";
				input_is_valid = false;
				continue;
			}
			bool all_characters_are_valid = true;
			for (const char& ch : user_input)
			{
				if (valid_digits.find(ch) == std::string::npos) //not a digit
				{
					std::cout << "\nUser input is invalid. (At least) one of the characters isn\'t a digit in base 10. Try again.";
					all_characters_are_valid = false;
					break;
				}
			}
			if (!all_characters_are_valid)
			{
				input_is_valid = false;
				continue;
			}
			std::stringstream user_input_ss;
			user_input_ss << user_input;
			try
			{
				user_input_ss >> num_digits;
			}
			catch (std::exception& e)
			{
				std::cout << "\nError parsing input: " << e.what() << ". Try again.";
				input_is_valid = false;
			}
		}
		if (num_digits == 1)
		{
			std::cout << "\nThere\'s no prime number with one bit only.";
			continue;
		}
		else if (num_digits == 0)
		{
			std::cout << "\n0 is not a prime number.";
			continue;
		}
		else if (num_digits < 0)
		{
			std::cout << "\nThere can\'t be a negative number of bits.";
			continue;
		}
		unlimited_int min(unlimited_int(1) << ((many_bits)num_digits - (many_bits)1));
		unlimited_int max(unlimited_int(1) << (num_digits));
		--max;
		unlimited_int prime(unlimited_int::generate_random_prime_multithreaded(min, max));
		std::cout << "\nPrime number: " << prime;
	}
}
