#include "unlimited_int.hpp"
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
#include <future>
#include <condition_variable>
#endif
#include <mutex>
#include <vector>
#include <sstream>
#include <array>
using namespace unlimited;
void initialize_small_prime_numbers(std::vector<few_bits>* small_prime_numbers)
{
	constexpr size_t PRIME_NUMBERS_UNTIL = (size_t)200; //Until and including, although in this case 200 isn't actually a prime number so it'll only be until 199.
	static_assert(PRIME_NUMBERS_UNTIL < (size_t)MAX_few_bits_NUM, "static_assert failed in function \"initialize_small_prime_numbers\", can\'t calculate that many small prime numbers");
	if (small_prime_numbers->empty())
	{
		static std::mutex safety_against_race;
		std::lock_guard<std::mutex> locker(safety_against_race);
		if (small_prime_numbers->empty())
		{
			std::array<bool, PRIME_NUMBERS_UNTIL + (size_t)1> is_prime;
			is_prime[0] = false;
			is_prime[1] = false;
			for (size_t index = (size_t)2; index < is_prime.size(); ++index)
				is_prime[index] = true;
		//sieve of Eratosthenes
			size_t num_of_primes = (size_t)0;
			for (size_t index = (size_t)2; index < is_prime.size(); ++index)
			{
				if (is_prime[index])
				{
					small_prime_numbers->push_back((few_bits)index);
					++num_of_primes;
					for (size_t index_skipping = index * (size_t)2; index_skipping < is_prime.size(); index_skipping += index)
						is_prime[index_skipping] = false;
				}
			}
		}
	}
}
//Miller-Rabin primality test algorithm.
//If a number fails one of the iterations, the number is certainly composite. A composite number has a chance of 25% to pass every iteration. That's why 64 iterations ensures 1/(2^128) probability of mistake.
bool unlimited_int::is_prime(const int num_of_iterations, const volatile bool *const terminator) const
{
	initialize_small_prime_numbers(&unlimited_int::small_prime_numbers);
	const size_t size_small_prime_numbers = small_prime_numbers.size();
	for (size_t index_primes = (size_t)0; index_primes < size_small_prime_numbers; ++index_primes)
	{
		const few_bits current_small_prime = unlimited_int::small_prime_numbers[index_primes];
		if (this->compare_to_ignore_sign(current_small_prime) != 'L')
			return true;
		else if (((*this) % current_small_prime).is_zero())
			return false;
	}
	const bool terminator_is_nullptr = terminator == nullptr;
	if (!terminator_is_nullptr)
		if (*terminator)
			return false; //abort
	const char comparison_to_2 = this->compare_to_ignore_sign((few_bits)2);
	if (comparison_to_2 == 'E' || this->compare_to_ignore_sign((few_bits)3) == 'E')
		return true;
	if (comparison_to_2 == 'S' || this->modulo_2() == 0)
		return false;
	unlimited_int pMinusOne = *this;
	pMinusOne.self_abs();
	--pMinusOne;
	size_t _k = (size_t)0;
	unlimited_int _m = pMinusOne;
	while (_m.modulo_2() == 0)
	{
		_m >>= (size_t)1;
		++_k;
	}
	if (_k-- == (size_t)0)
		_k = (size_t)0;
	for (int iteration_counter = 0; iteration_counter < num_of_iterations; ++iteration_counter)
	{
		unlimited_int _a = unlimited_int::generate_random(unlimited_int((few_bits)2), pMinusOne);
		unlimited_int _x = unlimited_int::pow(_a, _m, *this, terminator);
		if (!terminator_is_nullptr)
			if (*terminator)
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
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
void generate_random_prime_single_thread(const unlimited_int& min, const unlimited_int& max, volatile bool *const result_was_set, unlimited_int *const result, std::mutex& result_lock, std::condition_variable& wakeup_main_thread, std::mutex& wakeup_main_thread_lock)
{
	try
	{
		//Just a syntax trick, to limit the scope of the local variables, thereby getting all of the local variable's destructors called
		//before calling unlimited_int::delete_all_thread_local_memory(). Without this syntax trick, someone editing this code might accidentally cause a memory leak.
		if (true)
		{
			unlimited_int current_try;
			do current_try = unlimited_int::generate_random(min, max);
			while (!current_try.is_prime(64, result_was_set) && !(*result_was_set));
			if (true)
			{
				std::lock_guard<std::mutex> locker_result(result_lock);
				if (!(*result_was_set)) //if it ended because it actually found a prime number, rather than because of is_ending
				{
					*result = current_try;
					*result_was_set = true;
					if (true)
					{
						std::lock_guard<std::mutex> locker(wakeup_main_thread_lock);
						wakeup_main_thread.notify_one();
					}
				}
			}
		}
		//To fix the bug / feature in MSVC++ that thread_local variables aren't destroyed when using std::async.
		//In most compilers the thread_local variables are destoyed when calling std::future::get()
		unlimited_int::delete_all_thread_local_memory();
	}
	catch (std::exception& e)
	{
		try
		{
			unlimited_int::delete_all_thread_local_memory();
		}
		catch (std::exception& e)
		{
			static_cast<void>(e);
			//Nothing I can really do to clear up MSVC++'s issue at this point.
		}
		static_cast<void>(e); //Suppress compiler warning of unreference local variable.
		if (true)
		{
			std::unique_lock<std::mutex> locker_result(result_lock);
			if (!(*result_was_set)) //if we're not too late to the party
			{
				*result_was_set = true;
				if (true)
				{
					std::lock_guard<std::mutex> locker(wakeup_main_thread_lock);
					wakeup_main_thread.notify_one();
				}
			}
		}
		//In any case we should rethrow the exception so that it'll be noticed
		std::rethrow_exception(std::current_exception());
	}
}
#define NUM_OF_BITS_IN_PRIME_NUMBER_TO_STOP_MULTITHREADING 256
//uses std::async for multithreading
unlimited_int unlimited_int::generate_random_prime_multithreaded(const unlimited_int& min, const unlimited_int& max, const unsigned num_threads)
{
	unlimited_int range_size = max - min;
	if (range_size._is_negative)
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
		return unlimited_int::generate_random_prime(min, max);
	else
	{
		unlimited_int result;
		std::vector<std::future<void>> parallel_threads;
		parallel_threads.reserve((size_t)num_threads_to_use);
		volatile bool found_prime = false;
		std::mutex result_was_set;
		std::condition_variable wakeup_me;
		std::mutex wakeup_me_lock;
		std::unique_lock<std::mutex> wakeup_me_unique_lock(wakeup_me_lock);
		for (unsigned thread_counter = 0U; thread_counter < num_threads_to_use; ++thread_counter)
			parallel_threads.push_back(std::async(std::launch::async, generate_random_prime_single_thread, std::ref(min), std::ref(max), &found_prime, &result, std::ref(result_was_set), std::ref(wakeup_me), std::ref(wakeup_me_lock)));
		//The condition must be specified in the lambda just in case of spurious wakeup 
		wakeup_me.wait(wakeup_me_unique_lock, [&found_prime] () { return found_prime; });
		std::vector<std::string> exceptions_thrown_by_threads;
		for (unsigned thread_counter = 0U; thread_counter < parallel_threads.size(); ++thread_counter)
		{
			try
			{
				//Now that "found_prime == true" it'll only take a few milliseconds for all the threads to abort what they were doing to join
				parallel_threads[thread_counter].get();
			}
			catch (std::exception& e)
			{
				exceptions_thrown_by_threads.push_back(std::string(e.what()));
			}
		}
		parallel_threads.clear();
		if (exceptions_thrown_by_threads.size() > (size_t)0)
		{
			std::stringstream message_of_exception;
			for (size_t index_thread = 0; index_thread < exceptions_thrown_by_threads.size(); ++index_thread)
			{
				const std::string& current_exception_message = exceptions_thrown_by_threads[index_thread];
				message_of_exception << "\nThread number " << index_thread + (size_t)1 << ": " << current_exception_message;
			}
			const std::string message_to_throw(message_of_exception.str());
			message_of_exception.str("");
			message_of_exception.clear();
			throw std::logic_error(message_to_throw);
		}
		exceptions_thrown_by_threads.clear();
		if (result.is_zero())
			throw std::logic_error("Error in function generate_random_prime_multithreaded:"
				" The generated prime number is 0."
				" That either means that there was a mistake in the calculations,"
				" or it\'s an issue with threads waking up too early.");
		return result;
	}
}
#endif
unlimited_int unlimited_int::generate_random_prime(const unlimited_int& min, const unlimited_int& max, const volatile bool *const terminator)
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
			if (*terminator)
				return unlimited_int();
	} while (!current_try.is_prime(64, terminator));
	return current_try;
}
