//This is just an example file. "The best way to learn is through example"
#include "unlimited/unlimited.h"
#include <iostream>
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
unlimited::many_bits unlimited::int_array::num_of_ints_created = 0;
unlimited::many_bits unlimited::int_array::num_of_ints_destroyed = 0;
#endif
void helper();
int main(int argc, char* argv[])
{
	helper();
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
	unlimited::delete_all_static_memory();
	std::cout << "\n\nDEBUG INFORMATION:\n" << std::dec;
	std::cout << "\n$$num_of_ints_created: " << unlimited::int_array::num_of_ints_created;
	std::cout << "\nnum_of_ints_destroyed: " << unlimited::int_array::num_of_ints_destroyed;
#endif
	return 0;
}
using namespace unlimited;
unlimited_int* Fibonacci(unsigned long long index);
void helper()
{
	std::cout << "\nEnter the number of bits for prime number: ";
	int bits;
	std::cin >> bits;
	unlimited_int prime = unlimited_int::generate_random_prime(unlimited_int::pow(2, bits - 1), unlimited_int(unlimited_int::pow(2, bits)) - 1);
	std::cout << "\nprime number calculated.\nIn hexadecimal: " << std::hex << prime << "\nIn decimal: " << std::dec << prime;
	std::cout << "\nSHA256 of prime number: " << std::hex << unlimited_int(prime.calculate_sha256_hash()) << std::dec;
	std::cout << "\nSHA512 of prime number: " << std::hex << unlimited_int(prime.calculate_sha512_hash()) << std::dec;
	std::cout << "\nRandom number between 0 and the prime number (including): " << unlimited_int(prime.generate_random(0, prime));
	std::cout << "\nThe random number in base 36: " << prime.to_string(36);
	std::cout << "\nThe greatest common divisor of two random numbers: " << unlimited_int(unlimited_int::gcd(unlimited_int::generate_random(0, 1000), unlimited_int::generate_random(0, 1000)));
	std::cout << "\nThe lowest common multiple of two random numbers: " << unlimited_int(unlimited_int::lcm(unlimited_int::generate_random(0, 1000), unlimited_int::generate_random(0, 1000)));
	unsigned long long fib_index;
	std::cout << "\n\nEnter the index to find in Fibonacci sequence (where index 1 is 0 and index 2 is 1 etc.): ";
	std::cin >> fib_index;
	std::cout << "\nFibonacci num in hex: " << std::hex << unlimited_int(Fibonacci(fib_index));
	std::string line;
	std::cout << "\nEnter positive or negative integer in base 10: ";
	std::cin.ignore();
	std::getline(std::cin, line);
	std::cout << "\nThat same number squared: " << std::dec << unlimited_int(unlimited_int(unlimited_int::from_string(line)).power2());
}
unlimited_int* Fibonacci(unsigned long long index)
{
	if (index == 0)
		throw "index 0 is invalid";
	else if (index == 1)
		return unlimited_int(0).copy();
	else if (index == 2)
		return unlimited_int(1).copy();
	unlimited_int a = 1, b = 0;
	bool a_is_bigger = true;
	for (unsigned long long counter = 3; counter <= index; ++counter)
	{
		if (counter % 10000 == 0)
		{
			std::cout << "\nProgress Fibonacci: " << (((double)counter / (double)index) * 100) << "%";
		}
		if (a_is_bigger)
		{
			b = a + b;
			a_is_bigger = false;
		}
		else
		{
			a = a + b;
			a_is_bigger = true;
		}
	}
	std::cout << "\n";
	if (a_is_bigger)
		return a.copy();
	else
		return b.copy();
}
