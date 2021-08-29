#include "unlimited_int.hpp"
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
#include <thread>
#include <vector>
#include <mutex>
#endif
using namespace unlimited;
//Miller-Rabin primality test algorithm.
//If a number fails one of the iterations, the number is certainly composite. A composite number has a chance of 25% to pass every iteration. That's why 64 iterations ensures 1/(2^128) probability of mistake.
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
bool unlimited_int::is_prime(const int num_of_iterations, volatile bool *const terminator) const
#else
bool unlimited_int::is_prime(const int num_of_iterations) const
#endif
{
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
	if (terminator != nullptr)
		if (*terminator)
			return false; //abort
#endif
	const char comparison_to_2 = this->compare_to_ignore_sign(unlimited_int((few_bits)2));
	if (comparison_to_2 == 'E' || this->compare_to_ignore_sign(unlimited_int((few_bits)3)) == 'E')
		return true;
	if (comparison_to_2 == 'S' || this->modulo_2() == 0)
		return false;
	std::unique_ptr<unlimited_int> pMinusOne(this->copy());
	pMinusOne->is_negative = false;
	--(*pMinusOne);
	size_t _k = (size_t)0;
	std::unique_ptr<unlimited_int> _m(pMinusOne->copy());
	while (_m->modulo_2() == 0)
	{
		(*_m) >>= 1;
		++_k;
	}
	if (_k-- == (size_t)0)
		_k = (size_t)0;
	for (int iteration_counter = 0; iteration_counter < num_of_iterations; ++iteration_counter)
	{
		std::shared_ptr<unlimited_int> _a = unlimited_int::generate_random(unlimited_int((few_bits)2), *pMinusOne);
		std::shared_ptr<unlimited_int> _x = unlimited_int::pow(*_a, *_m, *this
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
								,terminator
#endif
								);
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		if (terminator != nullptr)
			if (*terminator)
				return false; //abort
#endif
		if (*_x == unlimited_int(1) || *_x == *pMinusOne)
			continue;
		bool to_return_false = true;
		for (size_t counter2 = (size_t)0; counter2 < _k; ++counter2)
		{
			_x = _x->power2();
			*_x %= *this;
			if (*_x == unlimited_int((few_bits)1))
				return false;
			if (*_x == *pMinusOne)
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
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
void generate_random_prime_single_thread(const unlimited_int *const min, const unlimited_int *const max, volatile bool *const is_ending, unlimited_int *const result, std::mutex *const result_lock)
{
	std::shared_ptr<unlimited_int> current_try;
	do current_try = unlimited_int::generate_random(*min, *max);
	while (!current_try->is_prime(64, is_ending) && !(*is_ending));
	result_lock->lock();
	if (!(*is_ending)) //if it ended because it actually found a prime number, rather than because of is_ending
	{
		*result = current_try;
		*is_ending = true;
	}
	result_lock->unlock();
}
#define NUM_OF_BITS_IN_PRIME_NUMBER_TO_STOP_MULTITHREADING 256
//uses multithreading
std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max, bool* terminator, int num_threads)
{
	unlimited_int range_size(max - min);
	if (range_size.is_negative)
		throw std::invalid_argument("Error in function \"std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max)\". Invalid arguments: max is smaller than min.");
	++range_size;
	unsigned max_cores = std::thread::hardware_concurrency() / 2;
	if (max_cores == 0U)
		max_cores = 1U;
	unsigned num_threads_to_use;
	if (num_threads <= 0)
		num_threads_to_use = max_cores; //use all cores
	else
	{
		num_threads_to_use = (unsigned)num_threads; //use user-specified number of cores
		if (num_threads_to_use > max_cores) //Not efficient to have more threads than CPU Logical processors
			num_threads_to_use = max_cores;
	}
	if (range_size.get_length_in_bits() <= (size_t)NUM_OF_BITS_IN_PRIME_NUMBER_TO_STOP_MULTITHREADING || num_threads_to_use == 1U) //number too small for threads to be worth it.
	{
		unlimited_int* result = new unlimited_int;
		std::mutex placeholder_mutex;
		bool is_ending = false;
		if (terminator == nullptr)
			generate_random_prime_single_thread(&min, &max, &is_ending, result, &placeholder_mutex);
		else
			generate_random_prime_single_thread(&min, &max, terminator, result, &placeholder_mutex);
		return std::shared_ptr<unlimited_int>(result);
	}
	else
	{
		std::vector<std::thread*> parallel_threads;
		parallel_threads.reserve((size_t)num_threads_to_use);
		bool found_prime = false;
		unlimited_int* result = new unlimited_int;
		std::mutex result_was_set;
		for (unsigned thread_counter = 0U; thread_counter < num_threads_to_use; ++thread_counter)
		{
			std::thread* current_thread = new std::thread(generate_random_prime_single_thread, &min, &max, &found_prime, result, &result_was_set);
			parallel_threads.push_back(current_thread);
		}
		while (!found_prime)
		{
			if (terminator != nullptr)
				if (*terminator)
				{
					found_prime = true; //yes, we're lying to our poor minions, to get this thing to terminate already.
					break;
				}
			//sleep for 1 seventh of a second. We don't want this loop to take all the processing power away from the threads that are trying to find prime numbers.
			std::this_thread::sleep_for(std::chrono::milliseconds(143LL));
		}
		for (unsigned thread_counter = 0U; thread_counter < num_threads_to_use; ++thread_counter)
		{
			//Now that "found_prime == true" it'll only take a few milliseconds for all the threads to abort what they were doing the join
			parallel_threads[thread_counter]->join();
			delete parallel_threads[thread_counter];
		}
		return std::shared_ptr<unlimited_int>(result);
	}
}
#else
std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max)
{
	unlimited_int range_size(max - min);
	if (range_size.is_negative)
		throw std::invalid_argument("Error in function \"std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max)\". Invalid arguments: max is smaller than min.");
	std::shared_ptr<unlimited_int> current_try;
	do current_try = unlimited_int::generate_random(min, max);
	while (!current_try->is_prime(64));
	return current_try;
}
#endif
