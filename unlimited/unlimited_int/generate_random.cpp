#include "unlimited_int.hpp"
//for the truly random number generator
#include <chrono> //for generating truly random numbers
#include <cmath> //Attempt at using floating point operation errors
using namespace unlimited;
many_bits ceiling_division(many_bits, many_bits);
#if DEBUG_MODE == 2
#include <iostream>
#endif
void insert_long_long_into_uint32_t(long long origin, uint32_t* destination_arr, size_t* counter_in_uint32_t)
{
#if IS_64_BIT_SYSTEM == false
	destination_arr[*counter_in_uint32_t] = (uint32_t)origin;
	++(*counter_in_uint32_t);
#else
	destination_arr[*counter_in_uint32_t] = (uint32_t)((many_bits)origin >> 32);
	++(*counter_in_uint32_t);
	destination_arr[*counter_in_uint32_t] = (uint32_t)((many_bits)origin & (many_bits)MASK_LOW_BITS);
	++(*counter_in_uint32_t);
#endif
}
//useful for clearing up the memory in the end of the program. Not actually necessary because unlimited_int::current_random will flush itself anyways at the end...
void unlimited_int::flush_current_random()
{
	unlimited_int::current_random.flush();
}
//Most methods here to increase randomness don't actually work, but it's just a failsafe. Anyways the entire "uint32_t* nums_to_generate_seed" ends up getting hashed, so it can only increase randomness.
//BTW this is not truly random, because it relies on time and memory address management, but for all intents and purposes it's truly random.
std::shared_ptr<unlimited_int> unlimited_int::generate_truly_random()
{
	//don't let this confuse you. There aren't actually 4096 * sizeof(int) bytes of randomness.
	//I don't have a camera pointing at a lava lamp in real time.
	const int size_of_seed_arr = 1024; //should be enough
	uint32_t* nums_to_generate_seed = new uint32_t[size_of_seed_arr];
	size_t uint32_t_index = 0;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	long long time_in_ms_passed_since_1970 = ms.count();
	insert_long_long_into_uint32_t(time_in_ms_passed_since_1970, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(&uint32_t_index), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)nums_to_generate_seed[(size_t)(&nums_to_generate_seed) % size_of_seed_arr], nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(&nums_to_generate_seed), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(void*)), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(short int)), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(int)), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(long int)), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(long long int)), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(float)), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(double)), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)(sizeof(long double)), nums_to_generate_seed, &uint32_t_index);
	std::shared_ptr<unlimited_int> num_to_return = unlimited_int(nums_to_generate_seed, size_of_seed_arr).calculate_efficient_cryptographic_hash();
	const long long extra_randomness_from_memory = (long long)(num_to_return.get());
	//using floating-point rounding errors to increase randomness
	for (int counter = 0; counter < 20; ++counter)
	{
		long double random_num = std::sqrt(
			std::abs(
				std::pow(
					(15.14298572336489365L * (double)(extra_randomness_from_memory)) / 5.13298579236489365L,
					(15.14298572336489365L * (double)(extra_randomness_from_memory)) / 5.13298579236489365L
				)
			)
		);
		unsigned char* c = reinterpret_cast<unsigned char*>(&random_num); //Interpreting a long double as an array of unsigned char
		long long int_from_double = 0;
		constexpr int min_bytes = sizeof(long long) < sizeof(long double) ? sizeof(long long) : sizeof(long double);
		for (int ch_num = 0; ch_num < min_bytes; ++ch_num)
			int_from_double |= (long long)c[ch_num] << (ch_num * 8);
		insert_long_long_into_uint32_t(int_from_double, nums_to_generate_seed, &uint32_t_index);
	}
	//using memory addresses and current structure of num_to_return to increase randomness
	insert_long_long_into_uint32_t((long long)num_to_return->get_length_in_bits(), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->get_least_significant(), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->num_of_intarrays_used, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.num_of_ints, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.intarrays.length, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.intarrays.first, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.intarrays.last, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.intarrays.first, nums_to_generate_seed, &uint32_t_index);
	num_to_return = unlimited_int(nums_to_generate_seed, size_of_seed_arr).calculate_efficient_cryptographic_hash();
	insert_long_long_into_uint32_t((long long)num_to_return->get_length_in_bits(), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->get_least_significant(), nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->num_of_intarrays_used, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.num_of_ints, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.intarrays.length, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.intarrays.first, nums_to_generate_seed, &uint32_t_index);
	insert_long_long_into_uint32_t((long long)num_to_return->intarrays.intarrays.last, nums_to_generate_seed, &uint32_t_index);
	//All the rest of these lines in this function are extremely important.
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	time_in_ms_passed_since_1970 = ms.count();
	insert_long_long_into_uint32_t(time_in_ms_passed_since_1970, nums_to_generate_seed, &uint32_t_index);
	num_to_return = unlimited_int(nums_to_generate_seed, size_of_seed_arr).calculate_efficient_cryptographic_hash();
	delete[] nums_to_generate_seed;
	return num_to_return;
}
std::shared_ptr<unlimited_int> unlimited_int::generate_next_random()
{
	if (unlimited_int::current_random.is_zero()) //if not initialized yet
		unlimited_int::current_random = unlimited_int::generate_truly_random(); //seeding random (only once in the entire program)
	std::shared_ptr<unlimited_int> next_in_chain = unlimited_int::current_random.calculate_efficient_cryptographic_hash();
	//Makes sure that the caller of this function can't predict what the next random number will be even if he knows the source code of this function.
	std::shared_ptr<unlimited_int> fork_of_main_chain = unlimited_int::current_random ^ (*next_in_chain); //xor produces a completely different random value that can't be traced back to this chain
	unlimited_int::current_random = next_in_chain;
	return fork_of_main_chain;
}
//returns the least significant few_bits as a many_bits because it also respects the sign of this unlimited_int
//if this is within the maximum range of few_bits then get_least_significant converts an unlimited_int to many_bits
many_bits_signed unlimited_int::get_least_significant() const
{
	if (this->num_of_used_ints == 0) { return 0; }
	const many_bits_signed least_significant = (many_bits_signed)(*this->intarrays.intarrays.first->value->intarr);
	if (this->is_negative) { return (-least_significant); }
	else { return least_significant; }
}
many_bits ceiling_division(many_bits numerator, many_bits denominator)
{
	many_bits result = numerator / denominator;
	if (result * denominator == numerator)
		return result;
	return result + (many_bits)1;
}
std::shared_ptr<unlimited_int> unlimited_int::generate_random_that_is_at_least(many_bits min_num_of_bits)
{
	unlimited_int* result_random = new unlimited_int;
#if IS_64_BIT_SYSTEM
	const many_bits num_of_sha512_hashes_needed = ceiling_division(min_num_of_bits, 512);
	many_bits num_hash;
	for (num_hash = 0; num_hash < num_of_sha512_hashes_needed; ++num_hash)
	{
		std::shared_ptr<unlimited_int> current_hash = generate_next_random();
		*current_hash <<= (num_hash * 512);
		*result_random |= *current_hash;
	}
	while (result_random->get_length_in_bits() < min_num_of_bits) //for the 1 in 1024 chance that result_random is still smaller than min_num_of_bits
	{
		std::shared_ptr<unlimited_int> current_hash = generate_next_random();
		*current_hash <<= (num_hash * 512);
		*result_random |= *current_hash;
		++num_hash;
	}
#else
	const many_bits num_of_sha256_hashes_needed = ceiling_division(min_num_of_bits, 256);
	many_bits num_hash;
	for (num_hash = 0; num_hash < num_of_sha256_hashes_needed; ++num_hash)
	{
		std::shared_ptr<unlimited_int> current_hash = generate_next_random();
		*current_hash <<= (num_hash * 256);
		*result_random |= *current_hash;
	}
	while (result_random->get_length_in_bits() < min_num_of_bits) //for the 1 in 512 chance that result_random is still smaller than min_num_of_bits
	{
		std::shared_ptr<unlimited_int> current_hash = generate_next_random();
		*current_hash <<= (num_hash * 256);
		*result_random |= *current_hash;
		++num_hash;
	}
#endif
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int* unlimited_int::generate_random_that_is_at_least(many_bits min_num_of_bits)\"";
#endif
#if DEBUG_MODE > 0
	if (result_random->find_inconsistencies())
		throw "\nThe inconsistency was found in start of function: \"unlimited_int* unlimited_int::generate_random_that_is_at_least(many_bits min_num_of_bits)\"";
#endif
	return std::shared_ptr<unlimited_int>(result_random);
}
std::shared_ptr<unlimited_int> unlimited_int::generate_random(const unlimited_int& min, const unlimited_int& max)
{
	char result_compare = min.compare_to(max);
	if (result_compare == 'L')
		throw "\nError found in function \"unlimited_int::generate_random(unlimited_int&, unlimited_int&)\": min > max";
	else if (result_compare == 'E')
		return std::shared_ptr<unlimited_int>(min.copy());
	std::shared_ptr<unlimited_int> result_of_sub = max - min;
	//Correction by 1 because max is also within range
	++(*result_of_sub);
	//No, the 256 isn't a mistake, even when using SHA512. It's just a big number.
	std::shared_ptr<unlimited_int> large_enough_random_num = unlimited_int::generate_random_that_is_at_least(result_of_sub->get_length_in_bits() + (many_bits)256);
	std::shared_ptr<unlimited_int> answer = *large_enough_random_num % *result_of_sub;
	*answer += min;
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int* unlimited_int::generate_random(const unlimited_int& min, const unlimited_int& max)\"";
#endif
#if DEBUG_MODE > 0
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function: \"unlimited_int* unlimited_int::generate_random(const unlimited_int& min, const unlimited_int& max)\"";
	if ((*answer < min) || (*answer > max))
		throw "Error in function \"unlimited_int* unlimited_int::generate_random(const unlimited_int& min, const unlimited_int& max)\". Result is out of range (an internal bug)";
#endif
	return answer;
}
