//This is just an example file. "The best way to learn is by example"
#include "unlimited/unlimited.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
//void helper();
void test_unlimited_int();
int main(int argc, char* argv[])
{
	//helper();
	test_unlimited_int();
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
//void helper()
//{
//	const size_t num_of_bits = 512;
//	unlimited_int min = unlimited_int((few_bits)1) << num_of_bits - (size_t)1;
//	unlimited_int max = (unlimited_int((few_bits)1) << num_of_bits) - unlimited_int(1);
//	const auto start_time = std::chrono::steady_clock::now();
//	constexpr int NUM_ITERATIONS = 40;
//	for (int counter = 0; counter < NUM_ITERATIONS; ++counter)
//	{
//		std::cout << "\nnumber " << std::setw(4) << std::setfill(' ') << counter + 1 << " / " << NUM_ITERATIONS << ": ";
//		std::cout.flush();
//		std::cout << unlimited_int::generate_random_prime_multithreaded(min, max);
//		std::cout.flush();
//	}
//	const auto end_time = std::chrono::steady_clock::now();
//	const auto time_duration = end_time - start_time;
//	const long double time_duration_seconds = static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(time_duration).count()) / static_cast<long double>(1000000000);
//	std::cout << "\nTime took: " << time_duration_seconds << " seconds.";
//}
unlimited_int generate_random()
{
	const unlimited_int max(unlimited_int::pow(unlimited_int("10"), unlimited_int("100")));
	const unlimited_int min(-max);
	return unlimited_int::generate_random(min, max);
}
int generate_random_int(int max_int)
{
	if (max_int < 0)
		throw std::invalid_argument("Min < Max");
	else if (max_int == 0)
		return 0;
	else
		return static_cast<int>(generate_random().abs() % (unlimited_int(max_int) + unlimited_int(1)));
}
void test_unlimited_int()
{
	enum class number_types { signed_short_int, signed_int, signed_long_int, signed_long_long_int, unsigned_short_int, unsigned_int, unsigned_long_int, unsigned_long_long_int, unlimited_int, end_enum };
	enum class operator_types_binary { addition, subtraction, multiplication, division, remainder, bitwise_and, bitwise_or, bitwise_xor, swap, comparison, pow, end_enum };
	enum class operator_types_unary { bitwise_not_and_inversion, unary_plus_plus_and_minus_minus, change_or_flip_sign, get_length_in_bits, is_zero, modulo_2, power2, bitwise_shift, end_enum};
	for (int counter = 0; counter < 1000; ++counter)
	{
		auto assign_num = [](number_types num_type, unlimited_int& num)
		{
			switch (num_type)
			{
			case number_types::signed_short_int:
				num = (signed short int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<signed short int>::min()), unlimited_int(std::numeric_limits<signed short int>::max()));
				break;
			case number_types::signed_int:
				num = (signed int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<signed int>::min()), unlimited_int(std::numeric_limits<signed int>::max()));
				break;
			case number_types::signed_long_int:
				num = (signed long int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<signed long int>::min()), unlimited_int(std::numeric_limits<signed long int>::max()));
				break;
			case number_types::signed_long_long_int:
				num = (signed long long int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<signed long long int>::min()), unlimited_int(std::numeric_limits<signed long long int>::max()));
				break;
			case number_types::unsigned_short_int:
				num = (unsigned short int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<unsigned short int>::min()), unlimited_int(std::numeric_limits<unsigned short int>::max()));
				break;
			case number_types::unsigned_int:
				num = (unsigned int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<unsigned int>::min()), unlimited_int(std::numeric_limits<unsigned int>::max()));
				break;
			case number_types::unsigned_long_int:
				num = (unsigned long int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<unsigned long int>::min()), unlimited_int(std::numeric_limits<unsigned long int>::max()));
				break;
			case number_types::unsigned_long_long_int:
				num = (unsigned long long int)unlimited_int::generate_random(unlimited_int(std::numeric_limits<unsigned long long int>::min()), unlimited_int(std::numeric_limits<unsigned long long int>::max()));
				break;
			case number_types::unlimited_int:
				num = unlimited_int::generate_random(unlimited_int("-10000000000000000000000000000000000000000000", 16), unlimited_int("10000000000000000000000000000000000000000000", 16));
				break;
			default:
				throw std::logic_error("This isn't supposed to happen.");
			}
		};
		std::string binary_or_unary_operator = (generate_random_int(1) == 0) ? "binary" : "unary";
		if (binary_or_unary_operator == "binary")
		{
			int num1_type = generate_random_int(static_cast<int>(number_types::end_enum) - 1);
			int num2_type = generate_random_int(static_cast<int>(number_types::end_enum) - 1);
			unlimited_int num1;
			unlimited_int num2;
			assign_num(static_cast<number_types>(num1_type), num1);
			assign_num(static_cast<number_types>(num2_type), num2);
			operator_types_binary operator_type = static_cast<operator_types_binary>(generate_random_int(static_cast<int>(operator_types_binary::end_enum) - 1));
			switch (operator_type)
			{
			case operator_types_binary::addition:
				break;
			case operator_types_binary::subtraction:
				break;
			case operator_types_binary::multiplication:
				break;
			case operator_types_binary::division:
				break;
			case operator_types_binary::remainder:
				break;
			case operator_types_binary::bitwise_and:
				break;
			case operator_types_binary::bitwise_or:
				break;
			case operator_types_binary::bitwise_xor:
				break;
			case operator_types_binary::swap:
				break;
			case operator_types_binary::comparison:
				break;
			case operator_types_binary::pow:
				break;
			default:
				throw std::logic_error("It\'s not supposed to be possible to get here.");
			}
		}
		else if (binary_or_unary_operator == "unary")
		{
			int num_type = generate_random_int(static_cast<int>(number_types::end_enum) - 1);
			unlimited_int num;
			assign_num(static_cast<number_types>(num_type), num);
			operator_types_unary operator_type = static_cast<operator_types_unary>(generate_random_int(static_cast<int>(operator_types_unary::end_enum) - 1));
			switch (operator_type)
			{
			case operator_types_unary::bitwise_not_and_inversion:
				break;
			case operator_types_unary::unary_plus_plus_and_minus_minus:
				break;
			case operator_types_unary::change_or_flip_sign:
				break;
			case operator_types_unary::get_length_in_bits:
				break;
			case operator_types_unary::is_zero:
				break;
			case operator_types_unary::modulo_2:
				break;
			case operator_types_unary::power2:
				break;
			case operator_types_unary::bitwise_shift:
				break;
			default:
				throw std::logic_error("It\'s not supposed to be possible to get here.");
			}
		}
	}
}
