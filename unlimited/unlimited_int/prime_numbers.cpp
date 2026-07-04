#include "unlimited_int.hpp"
using namespace unlimited;
//Miller-Rabin primality test algorithm.
//If a number fails one of the iterations, the number is certainly composite. A composite number has a chance of 25% to pass every iteration. That's why 64 iterations ensures 1/(2^128) probability of mistake.
bool unlimited_int::is_prime(const int num_of_iterations, const std::atomic<bool> *const terminator) const
{
	if (num_of_iterations <= 0)
		throw std::invalid_argument("Error in function \"bool unlimited_int::is_prime(int, const std::atomic<bool>*) const\": num_of_iterations must be positive.");
	const bool terminator_is_nullptr = terminator == nullptr;
	if (!terminator_is_nullptr)
		if (terminator->load(std::memory_order_relaxed))
			return false; //abort
	//0 and 1 are not prime. The small-primes loop below assumes |this| >= 2;
	//without this guard it would incorrectly return true for values less than the smallest prime (2).
	if (this->compare_to_ignore_sign((few_bits)2) == 'S')
		return false;
	const size_t size_small_prime_numbers = small_prime_numbers.size();
	for (size_t index_primes = (size_t)0; index_primes < size_small_prime_numbers; ++index_primes)
	{
		const few_bits current_small_prime = unlimited_int::small_prime_numbers[index_primes];
		if (this->compare_to_ignore_sign(current_small_prime) != 'L')
			return true;
		else if (((*this) % current_small_prime).is_zero())
			return false;
	}
	if (!terminator_is_nullptr)
		if (terminator->load(std::memory_order_relaxed))
			return false; //abort
	const char comparison_to_2 = this->compare_to_ignore_sign((few_bits)2);
	if (comparison_to_2 == 'E' || this->compare_to_ignore_sign((few_bits)3) == 'E')
		return true;
	if (comparison_to_2 == 'S' || this->modulo_2() == static_cast<unsigned short>(0))
		return false;
	unlimited_int pMinusOne = *this;
	pMinusOne.self_abs();
	--pMinusOne;
	size_t _k = (size_t)0;
	unlimited_int _m = pMinusOne;
	while (_m.modulo_2() == static_cast<unsigned short>(0))
	{
		_m >>= (size_t)1;
		++_k;
	}
	if (_k-- == (size_t)0)
		_k = (size_t)0;
	for (int iteration_counter = 0; iteration_counter < num_of_iterations; ++iteration_counter)
	{
		unlimited_int pMinusTwo = pMinusOne - unlimited_int(1);
		unlimited_int _a = unlimited_int::generate_random(unlimited_int(2), pMinusTwo);
		unlimited_int _x = unlimited_int::pow(_a, _m, *this, terminator);
		if (!terminator_is_nullptr)
			if (terminator->load(std::memory_order_relaxed))
				return false; //abort
		if (_x.compare_to_ignore_sign((few_bits)1) == 'E' || _x == pMinusOne)
			continue;
		bool to_return_false = true;
		for (size_t counter2 = (size_t)0; counter2 < _k; ++counter2)
		{
			_x = _x.power2();
			_x %= *this;
			if (_x.compare_to_ignore_sign((few_bits)1) == 'E')
				return false;
			if (_x == pMinusOne)
			{
				to_return_false = false;
				break;
			}
		}
		if (to_return_false)
			return false;
	}
	return true;
}
unlimited_int unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max, const std::atomic<bool> *const terminator)
{
	const bool terminator_is_nullptr = terminator == nullptr;
	unlimited_int range_size(max - min);
	if (range_size._is_negative)
		throw std::invalid_argument("Error in function \"std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max)\". Invalid arguments: max is smaller than min.");
	unlimited_int current_try;
	do
	{
		current_try = unlimited_int::generate_random(min, max);
		if (!terminator_is_nullptr)
			if (terminator->load(std::memory_order_relaxed))
				return unlimited_int();
	} while (!current_try.is_prime(64, terminator));
	return current_try;
}
