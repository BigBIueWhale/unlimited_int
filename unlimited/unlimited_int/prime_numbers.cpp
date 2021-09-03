#include "unlimited_int.hpp"
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
#include <future>
#endif
#include <vector>
#include <sstream>
#include <mutex>
using namespace unlimited;
void initialize_small_prime_numbers(unlimited_int::array_container<few_bits>* small_prime_numbers)
{
	few_bits* volatile* const arr_ptr = &small_prime_numbers->arr;
	constexpr size_t PRIME_NUMBERS_UNTIL = (size_t)MAX_few_bits_NUM < (size_t)200 ? (size_t)MAX_few_bits_NUM : (size_t)200;
	if (*arr_ptr == nullptr)
	{
		static std::mutex safety_against_race;
		std::lock_guard<std::mutex> locker(safety_against_race);
		if (*arr_ptr == nullptr)
		{
			constexpr size_t SIZE_OF_BOOLEAN_ARRAY = PRIME_NUMBERS_UNTIL + (size_t)1;
			bool* const is_prime = new bool[SIZE_OF_BOOLEAN_ARRAY];
			is_prime[0] = false;
			is_prime[1] = false;
			for (size_t index = (size_t)2; index < SIZE_OF_BOOLEAN_ARRAY; ++index)
				is_prime[index] = true;
			few_bits *const arr = *arr_ptr;
		//sieve of Eratosthenes
			size_t num_of_primes = (size_t)0;
			for (size_t index = (size_t)2; index < SIZE_OF_BOOLEAN_ARRAY; ++index)
			{
				if (is_prime[index])
				{
					++num_of_primes;
					for (size_t index_skipping = index * (size_t)2; index_skipping < SIZE_OF_BOOLEAN_ARRAY; index_skipping += index)
						is_prime[index_skipping] = false;
				}
			}
			few_bits* const arr_primes = new few_bits[num_of_primes];
			small_prime_numbers->size = num_of_primes;
			for (size_t index_arr_primes = (size_t)0, index_booleans = (size_t)2; index_booleans < SIZE_OF_BOOLEAN_ARRAY; ++index_booleans)
			{
				if (is_prime[index_booleans])
				{
					arr_primes[index_arr_primes] = (unsigned long long)index_booleans;
					++index_arr_primes;
				}
			}
			*arr_ptr = arr_primes;
		}
	}
}
//Miller-Rabin primality test algorithm.
//If a number fails one of the iterations, the number is certainly composite. A composite number has a chance of 25% to pass every iteration. That's why 64 iterations ensures 1/(2^128) probability of mistake.
bool unlimited_int::is_prime(const int num_of_iterations, volatile bool *const terminator) const
{
	initialize_small_prime_numbers(&unlimited_int::small_prime_numbers);
	const size_t num_of_small_primes = small_prime_numbers.size;
	const few_bits *const arr_small_primes = small_prime_numbers.arr;
	for (size_t index_primes = (size_t)0; index_primes < num_of_small_primes; ++index_primes)
	{
		const few_bits current_small_prime = arr_small_primes[index_primes];
		if (this->compare_to_ignore_sign(current_small_prime) != 'L')
			return true;
		else if (((*this) % current_small_prime)->is_zero())
			return false;
	}
	const bool terminator_is_nullptr = terminator == nullptr;
	if (!terminator_is_nullptr)
		if (*terminator)
			return false; //abort
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
		std::shared_ptr<unlimited_int> _x = unlimited_int::pow(*_a, *_m, *this, terminator);
		if (!terminator_is_nullptr)
			if (*terminator)
				return false; //abort
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
	try
	{
		//Just a syntax trick, to limit the scope of the local variables, thereby getting all of the local variable's destructors called
		//before calling unlimited_int::delete_all_thread_local_memory(). Without this syntax trick, someone editing this code might accidentally cause a memory leak.
		if (true)
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
		//To fix the bug / feature in MSVC++ that thread_local variables aren't destroyed when using std::async.
		//In most compilers the thread_local variables are destoyed when calling std::future::get()
		unlimited_int::delete_all_thread_local_memory();
	}
	catch (std::exception& e)
	{
		result_lock->lock();
		if (!(*is_ending))
			*is_ending = true;
		result_lock->unlock();
		throw e;
	}
}
#define NUM_OF_BITS_IN_PRIME_NUMBER_TO_STOP_MULTITHREADING 256
//uses multithreading
std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime_multithreaded(const unlimited_int& min, const unlimited_int& max, volatile bool *const terminator, int num_threads)
{
	const bool terminator_is_nullptr = terminator == nullptr;
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
		bool is_ending = false;
		return unlimited_int::generate_random_prime(min, max, terminator);
	}
	else
	{
		unlimited_int* result = new unlimited_int;
		std::vector<std::future<void>> parallel_threads;
		parallel_threads.reserve((size_t)num_threads_to_use);
		bool found_prime = false;
		std::mutex result_was_set;
		for (unsigned thread_counter = 0U; thread_counter < num_threads_to_use; ++thread_counter)
			parallel_threads.push_back(std::move(std::async(std::launch::async, generate_random_prime_single_thread, &min, &max, &found_prime, result, &result_was_set)));
		while (!found_prime)
		{
			if (!terminator_is_nullptr)
				if (*terminator)
				{
					found_prime = true; //yes, we're lying to our poor minions, to get this thing to terminate already.
					break;
				}
			//sleep for 1 seventh of a second. We don't want this loop to take all the processing power away from the threads that are trying to find prime numbers.
			std::this_thread::sleep_for(std::chrono::milliseconds(143LL));
		}
		std::vector<std::string> exceptions_thrown_by_threads;
		for (unsigned thread_counter = 0U; thread_counter < num_threads_to_use; ++thread_counter)
		{
			try
			{
				//Now that "found_prime == true" it'll only take a few milliseconds for all the threads to abort what they were doing to join
				parallel_threads[thread_counter].get();
			}
			catch (std::exception& e)
			{
				exceptions_thrown_by_threads.push_back(std::move(std::string(e.what())));
			}
		}
		if (exceptions_thrown_by_threads.size() > (size_t)0)
		{
			std::stringstream message_of_exception;
			message_of_exception << "While generating a prime number, " << exceptions_thrown_by_threads.size() << " out of " << num_threads_to_use << " threads threw an exception: ";
			for (size_t index_thread = 0; index_thread < exceptions_thrown_by_threads.size(); ++index_thread)
			{
				const std::string& current_exception_message = exceptions_thrown_by_threads[index_thread];
				message_of_exception << " Exception from thread number " << index_thread + (size_t)1 << ": " << current_exception_message;
			}
			const std::string message_to_throw(std::move(message_of_exception.str()));
			message_of_exception.str("");
			message_of_exception.clear();
			throw std::logic_error(message_to_throw);
		}
		return std::shared_ptr<unlimited_int>(result);
	}
}
#endif
std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max, volatile bool *const terminator)
{
	const bool terminator_is_nullptr = terminator == nullptr;
	unlimited_int range_size(max - min);
	if (range_size.is_negative)
		throw std::invalid_argument("Error in function \"std::shared_ptr<unlimited_int> unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max)\". Invalid arguments: max is smaller than min.");
	std::shared_ptr<unlimited_int> current_try;
	do
	{
		current_try = unlimited_int::generate_random(min, max);
		if (!terminator_is_nullptr)
			if (*terminator)
				break;
	} while (!current_try->is_prime(64, terminator));
	return current_try;
}
