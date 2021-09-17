//This is just an example file. "The best way to learn is by example"
#include "unlimited/unlimited.hpp"
using unlimited::unlimited_int;
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
static void test_unlimited_int();
int main(int argc, char* argv[])
{
	for (int i = 0; i < 80000; ++i)
	{
		//try
		//{
			test_unlimited_int();
		//}
		//catch (std::exception& e)
		//{
		//	std::cout << "\nCaught exception: " << e.what() << "\nPress ENTER to continue" << std::endl;
		//	std::cout.flush();
		//	std::cin.get();
		//}
	}
	//const auto start_time = std::chrono::steady_clock::now();
	//for (int counter = 0; counter < 40; ++counter)
	//{
	//	if (counter % 1 == 0)
	//		std::cout << "  " << counter + 1;
	//	unlimited_int result = unlimited_int::generate_random_prime(unlimited_int(), unlimited_int("100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"));
	//}
	//const long double time_took_seconds = static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time).count()) / static_cast<long double>(1000000000);
	//std::cout << "\nTime took seconds: " << time_took_seconds;
	try
	{
		unlimited::assert_no_memory_leak();
	}
	catch (std::exception& e)
	{
		std::cout << "\nMemory leak: " << e.what();
		std::cout.flush();
	}
	std::cout << "\nDone test.\n";
	std::cout.flush();
	std::cin.get();
	return 0;
}
static unlimited_int generate_random()
{
	const unlimited_int max(unlimited_int::pow(unlimited_int("10"), unlimited_int("100")));
	const unlimited_int min(-max);
	return unlimited_int::generate_random(min, max);
}
static int generate_random_int(int max_int)
{
	if (max_int < 0)
		throw std::invalid_argument("Min < Max");
	else if (max_int == 0)
		return 0;
	else
	{
		unlimited_int random_num = generate_random();
		random_num.self_abs();
		return static_cast<int>(random_num % (unlimited_int(max_int) + unlimited_int(1)));
	}
}
template <typename T>
void comparison_works(unlimited_int& num)
{
	const unlimited_int ui_version = unlimited_int::generate_random(unlimited_int(std::numeric_limits<T>::min()), unlimited_int(std::numeric_limits<T>::max()));
	T integer_instance = static_cast<T>(ui_version);
	if ((num == integer_instance) != (num == ui_version))
		throw std::logic_error("Failed comparison with integer type ==");
	if ((num != integer_instance) != (num != ui_version))
		throw std::logic_error("Failed comparison with integer type !=");
	if ((num <= integer_instance) != (num <= ui_version))
		throw std::logic_error("Failed comparison with integer type <=");
	if ((num >= integer_instance) != (num >= ui_version))
		throw std::logic_error("Failed comparison with integer type >=");
	if ((num > integer_instance) != (num > ui_version))
		throw std::logic_error("Failed comparison with integer type >");
	if ((num < integer_instance) != (num < ui_version))
		throw std::logic_error("Failed comparison with integer type <");

	if ((integer_instance == num) != (ui_version == num))
		throw std::logic_error("Failed comparison with integer type ==");
	if ((integer_instance != num) != (ui_version != num))
		throw std::logic_error("Failed comparison with integer type !=");
	if ((integer_instance >= num) != (ui_version >= num))
		throw std::logic_error("Failed comparison with integer type >=");
	if ((integer_instance <= num) != (ui_version <= num))
		throw std::logic_error("Failed comparison with integer type <=");
	if ((integer_instance < num) != (ui_version < num))
		throw std::logic_error("Failed comparison with integer type <");
	if ((integer_instance > num) != (ui_version > num))
		throw std::logic_error("Failed comparison with integer type >");
}
static void test_unlimited_int()
{
	static unsigned long long testing_counter = 0ULL;
	++testing_counter;
	std::cout << "  " << testing_counter;
	std::cout.flush();
	enum class number_types { signed_short_int, signed_int, signed_long_int, signed_long_long_int, unsigned_short_int, unsigned_int, unsigned_long_int, unsigned_long_long_int, unlimited_int, end_enum };
	enum class operator_types_binary { addition, subtraction, multiplication, division, remainder, bitwise_and, bitwise_or, bitwise_xor, swap, comparison, end_enum };
	enum class operator_types_unary { bitwise_not_and_inversion, unary_plus_plus_and_minus_minus, change_or_flip_sign, get_length_in_bits, modulo_2, power2, bitwise_shift, end_enum};
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
			{
				const int random_power_of_bits = generate_random_int(184);
				const unlimited_int limit_positive = unlimited_int(1) << random_power_of_bits;
				num = unlimited_int::generate_random(-limit_positive, limit_positive);
				break;
			}
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
			const int random_chooser = generate_random_int(400);
			if (random_chooser == 0)
				num1 = 0;
			else if (random_chooser == 1)
				num1 = 1;
			else if (random_chooser == 2)
				num1 = -1;
			else if (random_chooser == 3)
				num1 = 2;
			else if (random_chooser == 4)
				num1 = -2;
			const int random_chooser2 = generate_random_int(400);
			if (random_chooser2 == 0)
				num2 = 0;
			else if (random_chooser2 == 1)
				num2 = 1;
			else if (random_chooser2 == 2)
				num2 = -1;
			else if (random_chooser2 == 3)
				num2 = 2;
			else if (random_chooser2 == 4)
				num2 = -2;
			operator_types_binary operator_type = static_cast<operator_types_binary>(generate_random_int(static_cast<int>(operator_types_binary::end_enum) - 1));
			switch (operator_type)
			{
			case operator_types_binary::addition:
			{
				unlimited_int num1_cpy = num1;
				unlimited_int num2_cpy = num2;
				num1_cpy += num2;
				num2_cpy += num1;
				const unlimited_int result = num1 + num2;
				if (num1_cpy != num2_cpy || result != num1_cpy || result - num1 != num2 || result - num2 != num1)
				{
					std::cout << "\nAddition failed:\nnum1: " << num1 << "\nnum2: " << num2;
					throw std::logic_error("Addition failed");
				}
				break;
			}
			case operator_types_binary::subtraction:
			{
				unlimited_int num1_cpy = num1;
				unlimited_int num2_cpy = num2;
				num1_cpy -= num2;
				num2_cpy -= num1;
				const unlimited_int result_1_minus_2 = num1 - num2;
				const unlimited_int result_2_minus_1 = num2 - num1;
				if (num1_cpy != result_1_minus_2 || num2_cpy != result_2_minus_1 || result_1_minus_2 + num2 != num1 || result_2_minus_1 + num1 != num2)
				{
					std::cout << "\nSubtraction failed:\nnum1: " << num1 << "\nnum2: " << num2;
					throw std::logic_error("Subtraction failed");
				}
				break;
			}
			case operator_types_binary::multiplication:
			{
				unlimited_int num1_cpy = num1;
				unlimited_int num2_cpy = num2;
				num1_cpy *= num2;
				num2_cpy *= num1;
				const unlimited_int result = num1 * num2;
				if (num1_cpy != result || num2_cpy != result)
				{
					std::cout << "\nMultiplication result:\nresult: " << result << "\nnum1_cpy: " << num1_cpy << "\nnum2_cpy: " << num2_cpy;
					throw std::logic_error("Multiplication failed");
				}
				if (num1 != 0)
				{
					const unlimited_int division_result = result / num1;
					if (division_result != num2)
					{
						std::cout << "\nMultiplication result num1 != 0:\nresult: " << result << "\nnum1: " << num1 << "\nnum2: " << num2 << "\ndivision_result: " << division_result;
						throw std::logic_error("Multiplication doesn\'t agree with division");
					}
				}
				if (num2 != 0)
				{
					const unlimited_int division_result = result / num2;
					if (division_result != num1)
					{
						std::cout << "\nMultiplication result num2 != 0:\nresult: " << result << "\nnum1: " << num1 << "\nnum2: " << num2 << "\ndivision_result: " << division_result;
						throw std::logic_error("Multiplication doesn\'t agree with division");
					}
				}
				break;
			}
			case operator_types_binary::division:
			{
				if (num2 != 0)
				{
					unlimited_int num1_cpy = num1;
					num1_cpy /= num2;
					const unlimited_int result = num1 / num2;
					const unlimited_int recurring_division_result = unlimited_int::recurring_division(num1, num2);
					if (result != num1_cpy || result != recurring_division_result)
					{
						std::cout << "\nDivision result num2 != 0:\nresult: " << result << "\nnum1_cpy: " << num1_cpy << "\nrecurring_division_result: " << recurring_division_result
							<< "\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("Division failed");
					}
				}
				if (num1 != 0)
				{
					unlimited_int num2_cpy = num2;
					num2_cpy /= num1;
					const unlimited_int result = num2 / num1;
					const unlimited_int recurring_division_result = unlimited_int::recurring_division(num2, num1);
					if (result != num2_cpy || result != recurring_division_result)
					{
						std::cout << "\nDivision result num2 != 0:\nresult: " << result << "\nnum2_cpy: " << num2_cpy << "\nrecurring_division_result: " << recurring_division_result
							<< "\nnum1: " << num1 << "\nnum2: " << num2;
						unlimited_int::recurring_division(num2, num1);
						throw std::logic_error("Division failed");
					}
				}
				break;
			}
			case operator_types_binary::remainder:
			{
				if (num2 != 0)
				{
					unlimited_int num1_cpy = num1;
					num1_cpy %= num2;
					const unlimited_int result = num1 % num2;
					unlimited_int recurring_division_result;
					unlimited_int::recurring_division(num1, num2, &recurring_division_result);
					if (result != num1_cpy || result != recurring_division_result)
					{
						std::cout << "\nRemainder result num2 != 0:\nresult: " << result << "\nnum1_cpy: " << num1_cpy << "\nrecurring_division_result: " << recurring_division_result
							<< "\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("Remainder failed");
					}
				}
				if (num1 != 0)
				{
					unlimited_int num2_cpy = num2;
					num2_cpy %= num1;
					const unlimited_int result = num2 % num1;
					unlimited_int recurring_division_remainder_result;
					unlimited_int::recurring_division(num2, num1, &recurring_division_remainder_result);
					if (result != num2_cpy || result != recurring_division_remainder_result)
					{
						std::cout << "\nRemainder result num1 != 0:\nresult: " << result << "\nnum2_cpy: " << num2_cpy << "\nrecurring_division_result: " << recurring_division_remainder_result
							 << "\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("Remainder failed");
					}
				}
				break;
			}
			case operator_types_binary::bitwise_and:
			{
				bool threw_exception = false;
				try
				{
					unlimited_int num1_cpy = num1;
					unlimited_int num2_cpy = num2;
					const unlimited_int result = num1 & num2;
					num1_cpy &= num2;
					num2_cpy &= num1;
					if (num1_cpy != num2_cpy || result != num1_cpy || (result & num1) != result || (result & num2) != result)
					{
						std::cout << "\n\"Bitwise\" and failed.\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("\"Bitwise and\" failed.");
					}
				}
				catch (std::invalid_argument& e)
				{
					threw_exception = true;
					if (num1 >= 0 && num2 >= 0)
					{
						std::cout << "\n\"Bitwise\" and failed.\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("Bitwise and threw exception even though the numbers aren\'t negative.");
					}
				}
				if ((num1 < 0 || num2 < 0) && !threw_exception)
				{
					std::cout << "\n\"Bitwise\" and failed.\nnum1: " << num1 << "\nnum2: " << num2;
					throw std::logic_error("Bitwise and didn\'t throw exception even though the numbers are negative.");
				}
				break;
			}
			case operator_types_binary::bitwise_or:
			{
				bool threw_exception = false;
				try
				{
					unlimited_int num1_cpy = num1;
					unlimited_int num2_cpy = num2;
					const unlimited_int result = num1 | num2;
					num1_cpy |= num2;
					num2_cpy |= num1;
					if (num1_cpy != num2_cpy || result != num1_cpy || ((result & num1) | (result & num2)) != result)
					{
						std::cout << "\n\"Bitwise\" or failed.\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("\"Bitwise or\" failed.");
					}
				}
				catch (std::invalid_argument& e)
				{
					threw_exception = true;
					if (num1 >= 0 && num2 >= 0)
					{
						std::cout << "\n\"Bitwise\" or failed.\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("Bitwise or threw exception even though the numbers aren\'t negative.");
					}
				}
				if ((num1 < 0 || num2 < 0) && !threw_exception)
				{
					std::cout << "\n\"Bitwise\" or failed.\nnum1: " << num1 << "\nnum2: " << num2;
					throw std::logic_error("Bitwise or didn\'t throw exception even though the numbers are negative.");
				}
				break;
			}
			case operator_types_binary::bitwise_xor:
			{
				bool threw_exception = false;
				try
				{
					unlimited_int num1_cpy = num1;
					unlimited_int num2_cpy = num2;
					const unlimited_int result = num1 ^ num2;
					num1_cpy ^= num2;
					num2_cpy ^= num1;
					if (num1_cpy != num2_cpy || result != num1_cpy || (result ^ num1) != num2 || (result ^ num2) != num1)
					{
						std::cout << "\n\"Bitwise\" xor failed.\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("\"Bitwise xor\" failed.");
					}
				}
				catch (std::invalid_argument& e)
				{
					threw_exception = true;
					if (num1 >= 0 && num2 >= 0)
					{
						std::cout << "\n\"Bitwise\" xor failed.\nnum1: " << num1 << "\nnum2: " << num2;
						throw std::logic_error("Bitwise xor threw exception even though the numbers aren\'t negative.");
					}
				}
				if ((num1 < 0 || num2 < 0) && !threw_exception)
				{
					std::cout << "\n\"Bitwise\" xor failed.\nnum1: " << num1 << "\nnum2: " << num2;
					throw std::logic_error("Bitwise xor didn\'t throw exception even though the numbers are negative.");
				}
				break;
			}
			case operator_types_binary::swap:
			{
				unlimited_int num1_cpy = num1;
				unlimited_int num2_cpy = num2;
				num1_cpy.swap(num2_cpy);
				std::swap(num1_cpy, num2_cpy);
				num1_cpy.swap(num2_cpy);
				if (num1_cpy != num2 || num2_cpy != num1)
				{
					std::cout << "\nFailed swap.\nnum1: " << num1 << "\nnum2: " << num2;
					throw std::logic_error("Failed swap");
				}
				break;
			}
			case operator_types_binary::comparison:
			{
				if ((num1 == num2) != (num1.calculate_efficient_cryptographic_hash() == num2.calculate_efficient_cryptographic_hash()) && (num1 >= 0) == (num2 >= 0) /*hash ignores the sign. operator== doesn't*/)
				{
					std::cout << "\nBasic comparison failed.";
					throw std::logic_error("Failed comparison or hashing.");
				}
				comparison_works<unsigned short int>((rand() % 2 == 0) ? num1 : num2);
				comparison_works<unsigned int>((rand() % 2 == 0) ? num1 : num2);
				comparison_works<unsigned long int>((rand() % 2 == 0) ? num1 : num2);
				comparison_works<unsigned long long int>((rand() % 2 == 0) ? num1 : num2);
				comparison_works<signed short int>((rand() % 2 == 0) ? num1 : num2);
				comparison_works<signed int>((rand() % 2 == 0) ? num1 : num2);
				comparison_works<signed long int>((rand() % 2 == 0) ? num1 : num2);
				comparison_works<signed long long int>((rand() % 2 == 0) ? num1 : num2);
				break;
			}
			default:
			{
				throw std::logic_error("It\'s not supposed to be possible to get here.");
			}
			}
		}
		else if (binary_or_unary_operator == "unary")
		{
			int num_type = generate_random_int(static_cast<int>(number_types::end_enum) - 1);
			unlimited_int num;
			assign_num(static_cast<number_types>(num_type), num);
			const int random_chooser = generate_random_int(400);
			if (random_chooser == 0)
				num = 0;
			else if (random_chooser == 1)
				num = 1;
			else if (random_chooser == 2)
				num = -1;
			else if (random_chooser == 3)
				num = 2;
			else if (random_chooser == 4)
				num = -2;
			operator_types_unary operator_type = static_cast<operator_types_unary>(generate_random_int(static_cast<int>(operator_types_unary::end_enum) - 1));
			switch (operator_type)
			{
			case operator_types_unary::bitwise_not_and_inversion:
			{
				if (num >= 0)
				{
					const unlimited_int result = ~num;
					unlimited_int num_cpy = num;
					auto are_all_bits_1 = [](const unlimited_int& num) -> bool
					{
						unlimited_int num_cpy = num;
						while (num_cpy != 0)
						{
							if (num_cpy.modulo_2() == 0)
								return false;
							num_cpy >>= 1;
						}
						return true;
					};
					{
						if (!are_all_bits_1(result ^ num) || !are_all_bits_1(result | num) || (result & num) != 0)
						{
							std::cout << "\nFailed bitwise not: num: " << num;
							throw std::logic_error("Failed bitwise not.");
						}
					}
					num_cpy.invert_bits();
					if (result != num_cpy)
					{
						std::cout << "\nFailed bitwise not: different results when using invert_bits() and operator~().\nnum: " << num << "\nresult: 0b" << result.to_string(2U) << "\nnum_cpy:0b" << num_cpy.to_string(2);
						throw std::logic_error("Failed bitwise not: different results when using invert_bits() and operator~()");
					}
				}
				break;
			}
			case operator_types_unary::unary_plus_plus_and_minus_minus:
			{
				unlimited_int num_cpy = num;
				num_cpy++;
				num_cpy--;
				--num_cpy;
				++num_cpy;
				if (num_cpy != num)
				{
					std::cout << "\nFailed self decrement or self increment.\nnum: " << num;
					throw std::logic_error("Failed unary decrement or increment.");
				}
				if (num_cpy-- != num)
				{
					std::cout << "\nFailed self post decrement.\nnum: " << num;
					throw std::logic_error("Failed unary post decrement.");
				}
				++num_cpy;
				if (num_cpy++ != num)
				{
					std::cout << "\nFailed self post increment.\nnum: " << num;
					throw std::logic_error("Failed unary post increment.");
				}
				break;
			}
			case operator_types_unary::change_or_flip_sign:
			{
				unlimited_int num_cpy = num;
				num_cpy.flip_sign();
				if (num_cpy != -num)
				{
					std::cout << "\nFailed flip_sign. num: " << num;
					throw std::logic_error("Failed flip sign");
				}
				num_cpy.flip_sign();
				if (num_cpy != num)
				{
					std::cout << "\nFailed flip_sign. num: " << num;
					throw std::logic_error("Failed flip sign");
				}
				const unlimited_int num_cpy_abs = num_cpy.abs();
				num_cpy.self_abs();
				if (num_cpy != num_cpy_abs)
				{
					std::cout << "\nFailed unary abs. num: " << num;
					throw std::logic_error("Failed abs");
				}
				num_cpy = num;
				unlimited_int num_cpy_negative = num_cpy;
				if (num_cpy_negative > 0)
					num_cpy_negative = -num_cpy_negative;
				num_cpy.self_negative();
				if (num_cpy != num_cpy_negative)
				{
					std::cout << "\nFailed unary negative. num: " << num;
					throw std::logic_error("Failed unary negative");
				}
				break;
			}
			case operator_types_unary::get_length_in_bits:
			{
				num.self_abs();
				const size_t len_in_bits = num.get_length_in_bits();
				size_t length_in_bits_counter = 0;
				for (unlimited_int num_cpy = num; num_cpy != 0; num_cpy >>= 1)
					++length_in_bits_counter;
				if (length_in_bits_counter != len_in_bits)
				{
					std::cout << "\nFailed get_length_in_bits or operator>>=. Length in bits according to operator >>=: "
						<< length_in_bits_counter << " and according to get_length_in_bits(): " << len_in_bits << "\nnum: " << num;
					throw std::logic_error("Failed get_length_in_bits");
				}
				break;
			}
			case operator_types_unary::modulo_2:
			{
				const unlimited_int num_plus_one = num + unlimited_int(1);
				if (num.modulo_2() == num_plus_one.modulo_2())
				{
					std::cout << "\nFailed modulo 2: num: " << num;
					throw std::logic_error("Failed modulo_2");
				}
				break;
			}
			case operator_types_unary::power2:
			{
				const unlimited_int num_original = num;
				const unlimited_int result = num.power2();
				const unlimited_int result_2 = num.power2_destroy_this();
				//Operator* will automatically use power2
				const unlimited_int result_3 = num_original * num_original;
				const unlimited_int twice_num_original = num_original * unlimited_int(2);
				//Force using regular multiply karatsuba (instead of the power2 version)
				const unlimited_int result_4 = (num_original * twice_num_original) / unlimited_int(2);
				if (result != result_2 || result != result_3 || result != result_4)
				{
					std::cout << "\nFailed power2.\nnum: " << num_original;
					throw std::logic_error("Failed power2");
				}
				break;
			}
			case operator_types_unary::bitwise_shift:
			{
				num.self_abs();
				const int amount_to_shift = generate_random_int(10000);
				const unlimited_int result_shift_left = num << amount_to_shift;
				unlimited_int num_cpy = num;
				num_cpy <<= amount_to_shift;
				if (result_shift_left != num_cpy)
				{
					std::cout << "\nFailed shift left.\nnum: " << num;
					throw std::logic_error("Failed shift left");
				}
				const int amount_to_shift_right = generate_random_int(static_cast<int>(result_shift_left.get_length_in_bits() + (size_t)5));
				const unlimited_int result_shift_right = result_shift_left >> amount_to_shift_right;
				num_cpy >>= amount_to_shift_right;
				if (num_cpy.calculate_efficient_cryptographic_hash() != result_shift_right.calculate_efficient_cryptographic_hash() || num_cpy != result_shift_right)
				{
					std::cout << "\nFailed shift right.\nnum: " << num;
					throw std::logic_error("Failed shift right");
				}
				break;
			}
			default:
			{
				throw std::logic_error("It\'s not supposed to be possible to get here.");
			}
			}
		}
	}
}
