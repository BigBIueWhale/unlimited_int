#include "unlimited_int.hpp"
#include <array>
#include <vector>
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
//Sliding window exponentiation parameters. WINDOW_BITS is the maximum window width measured in bits of the power. WINDOW_TABLE_ENTRIES is the number of precomputed odd powers needed to cover every window that ends on a 1 bit, which is 2^(WINDOW_BITS - 1) (the count of odd numbers in [1, 2^WINDOW_BITS - 1]).
static constexpr size_t WINDOW_BITS = (size_t)4;
static constexpr size_t WINDOW_TABLE_ENTRIES = ((size_t)1) << (WINDOW_BITS - (size_t)1);
//Threshold below which the plain square-and-multiply loop beats windowing. Break-even is around 24 bits (below which the 1 squaring + 7 multiplications spent building odd_powers_table outweigh the per-bit savings); 32 bits gives a bit of margin and lets all native-sized powers take the plain path.
static constexpr size_t SMALL_POWER_THRESHOLD_BITS = (size_t)32;
//makes use if the identity (a ⋅ b) mod m = [(a mod m) ⋅ (b mod m)] mod m
unlimited_int unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& remainder, const volatile bool *const terminator)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || remainder.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"");
#endif
	const bool terminator_is_nullptr = terminator == nullptr;
	if (base.is_zero() && power.is_zero())
		throw std::invalid_argument("Invalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& remainder)\" pow(0, 0) is mathematically undefined");
	if (remainder.is_zero())
		throw std::invalid_argument("Invalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& remainder)\" division by zero is undefined");
	if (remainder.compare_to_ignore_sign((few_bits)1) == 'E')
		return unlimited_int();
	if (power.is_zero())
		return unlimited_int(1);
	if (base.is_zero() || power.is_negative())
		return unlimited_int();
	//TODO performance optimization opportunity: every remainder_recurring_divison call in this function goes through a full Newton Raphson reciprocal lookup and correction loop, even though the modulus (remainder) is constant across all reductions inside a single pow(). For modular exponentiation specifically, converting to Montgomery form (represent x as x*R mod remainder with R = B^k chosen so that the reduction is a multiply + shift + at-most-one conditional subtract instead of a division) replaces each modular reduction's divide-shaped cost with a multiply-shaped cost. For RSA, Diffie-Hellman, and Miller-Rabin workloads - which are the primary consumers of this function - Montgomery multiplication yields a 2-3x speedup. The setup and teardown for the form change is paid once per pow() call and spread over all log2(power) iterations, so it pays off whenever power has more than a handful of bits.
	unlimited_int current_power = base;
	current_power = unlimited_int::remainder_recurring_divison(current_power, remainder);
	if (current_power.is_zero())
		return unlimited_int();
	if (!terminator_is_nullptr)
		if (*terminator)
			return unlimited_int();
	const size_t power_bit_length = power.get_length_in_bits();
	//For small powers the precomputation cost (1 squaring + 7 multiplications, each followed by a modular reduction, to fill odd_powers_table) exceeds the savings from windowing, so a plain square-and-multiply loop is used instead. The plain loop handles the LSB of the power before entering the while-loop so that the first "square current_power" step inside the while-loop produces current_power^2, not current_power^1.
	if (power_bit_length <= SMALL_POWER_THRESHOLD_BITS)
	{
		unlimited_int power_cpy = power;
		unlimited_int answer((few_bits)1);
		if (power_cpy.modulo_2() == static_cast<unsigned short>(1))
		{
			answer *= current_power;
			answer = unlimited_int::remainder_recurring_divison(answer, remainder);
		}
		power_cpy >>= (size_t)1;
		while (!power_cpy.is_zero())
		{
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			current_power = current_power.power2_destroy_this();
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			current_power = unlimited_int::remainder_recurring_divison(current_power, remainder);
			if (power_cpy.modulo_2() == static_cast<unsigned short int>(1))
			{
				if (!terminator_is_nullptr)
					if (*terminator)
						return unlimited_int();
				answer *= current_power;
				if (!terminator_is_nullptr)
					if (*terminator)
						return unlimited_int();
				answer = unlimited_int::remainder_recurring_divison(answer, remainder);
			}
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			power_cpy >>= (size_t)1;
		}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
		if (base.find_inconsistencies() || power.find_inconsistencies() || remainder.find_inconsistencies())
			throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"");
		if (answer.find_inconsistencies())
			throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\" in answer");
#endif
		return answer;
	}
	//Sliding window exponentiation, window size = WINDOW_BITS. The main loop walks the bits of the power from most significant to least significant. Every bit of the power costs one squaring of answer. On each 1 bit the loop opens a window of up to WINDOW_BITS bits, extended downward to the next 1 bit so that the window always ends on a 1 bit (trailing zero bits slide past inside the next iteration as plain squarings), and then multiplies answer by the precomputed odd power of current_power matching the window value.
	//Because every window ends on a 1 bit, the window value is always one of the 8 odd numbers 1, 3, 5, ..., 15, so only WINDOW_TABLE_ENTRIES precomputed entries are needed. odd_powers_table[k] = current_power^(2k+1) mod remainder, built with 1 squaring + 7 multiplications up front, each followed by a modular reduction.
	//Compared to plain square-and-multiply - which multiplies answer by current_power once for every 1 bit in the power, averaging n/2 multiplications for an n-bit power - sliding window does at most 1 multiplication per WINDOW_BITS bits, averaging about n/(WINDOW_BITS + 1) because runs of zero bits inside a would-be window slide past the window on the next 0 bit with only squarings. Counting the 8 precomputation multiplications the total is about n/5 + 8, roughly a 2.5x reduction in multiplications for n around 1000. The squaring count is unchanged at about n.
	std::array<unlimited_int, WINDOW_TABLE_ENTRIES> odd_powers_table;
	//Compute current_power^2 first (consumes a single copy of current_power via the copy-then-destroy pattern), then move current_power into odd_powers_table[0] so that the storage for base^1 is transferred rather than re-allocated.
	unlimited_int current_power_squared = current_power;
	current_power_squared = current_power_squared.power2_destroy_this();
	current_power_squared = unlimited_int::remainder_recurring_divison(current_power_squared, remainder);
	odd_powers_table[(size_t)0] = std::move(current_power);
	if (!terminator_is_nullptr)
		if (*terminator)
			return unlimited_int();
	for (size_t table_index = (size_t)1; table_index < WINDOW_TABLE_ENTRIES; ++table_index)
	{
		odd_powers_table[table_index] = odd_powers_table[table_index - (size_t)1] * current_power_squared;
		odd_powers_table[table_index] = unlimited_int::remainder_recurring_divison(odd_powers_table[table_index], remainder);
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
	}
	//Extract the bits of |power| into a flat buffer, most significant bit first. power_bits_most_significant_first[0] is the top bit of |power| (guaranteed to be 1) and power_bits_most_significant_first[power_bit_length - 1] is the bottom bit. Done once up front so the main loop can index into the buffer directly instead of threading a custom_linked_list_node + int_array + bit-within-int cursor through the window-formation logic.
	std::vector<bool> power_bits_most_significant_first;
	power_bits_most_significant_first.reserve(power_bit_length);
	{
		custom_linked_list_node<int_array>* current_int_array_node = power.get_most_significant_used_int_array();
		int_array current_int_array = *current_int_array_node->value;
		size_t index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
		//In the most significant int, skip the leading zero bits so the first emitted bit is the actual top bit of |power|. In every less-significant int the entire int is used, so the starting bit is NUM_OF_BITS_few_bits - 1.
		const few_bits most_significant_int_value = current_int_array.intarr[index_in_int_array];
		int starting_bit_position_in_int = NUM_OF_BITS_few_bits - 1 - unlimited_int::num_of_zero_bits_preceding_number(most_significant_int_value);
		while (true)
		{
			const few_bits current_int_value = current_int_array.intarr[index_in_int_array];
			for (int bit_position_in_int = starting_bit_position_in_int; bit_position_in_int >= 0; --bit_position_in_int)
				power_bits_most_significant_first.push_back(((current_int_value >> bit_position_in_int) & (few_bits)1) != (few_bits)0);
			starting_bit_position_in_int = NUM_OF_BITS_few_bits - 1;
			if (index_in_int_array != (size_t)0)
			{
				--index_in_int_array;
				continue;
			}
			//Move to the previous int_array in the linked list, skipping any that happen to have 0 used ints before hitting the begin-sentinel.
			bool reached_begin_sentinel = false;
			while (true)
			{
				current_int_array_node = current_int_array_node->previous;
				if (current_int_array_node == power.intarrays->begin())
				{
					reached_begin_sentinel = true;
					break;
				}
				current_int_array = *current_int_array_node->value;
				if (current_int_array.num_of_used_ints != (size_t)0)
					break;
			}
			if (reached_begin_sentinel)
				break;
			index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
		}
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (power_bits_most_significant_first.size() != power_bit_length)
		throw std::logic_error("Inconsistency in unlimited_int::pow windowed path: MSB-first bit extraction of |power| did not produce get_length_in_bits() bits");
#endif
	const size_t total_bits = power_bits_most_significant_first.size();
	//First window: answer starts at 1, so the window_width squarings that would be performed on it would all leave it at 1, and the first multiplication by a table entry would leave answer equal to that table entry. Skip straight to a direct assignment to avoid those wasted squarings.
	unlimited_int answer;
	size_t current_bit = (size_t)0;
	{
		size_t window_end = current_bit + WINDOW_BITS - (size_t)1;
		if (window_end >= total_bits)
			window_end = total_bits - (size_t)1;
		while (!power_bits_most_significant_first[window_end])
			--window_end;
		size_t window_value = (size_t)0;
		for (size_t bit_index = current_bit; bit_index <= window_end; ++bit_index)
		{
			window_value <<= 1;
			if (power_bits_most_significant_first[bit_index])
				window_value |= (size_t)1;
		}
		const size_t table_index = (window_value - (size_t)1) >> 1;
		answer = odd_powers_table[table_index];
		current_bit = window_end + (size_t)1;
	}
	while (current_bit < total_bits)
	{
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
		if (!power_bits_most_significant_first[current_bit])
		{
			answer = answer.power2_destroy_this();
			answer = unlimited_int::remainder_recurring_divison(answer, remainder);
			++current_bit;
			continue;
		}
		size_t window_end = current_bit + WINDOW_BITS - (size_t)1;
		if (window_end >= total_bits)
			window_end = total_bits - (size_t)1;
		while (!power_bits_most_significant_first[window_end])
			--window_end;
		const size_t window_width = window_end - current_bit + (size_t)1;
		size_t window_value = (size_t)0;
		for (size_t bit_index = current_bit; bit_index <= window_end; ++bit_index)
		{
			window_value <<= 1;
			if (power_bits_most_significant_first[bit_index])
				window_value |= (size_t)1;
		}
		for (size_t squaring_counter = (size_t)0; squaring_counter < window_width; ++squaring_counter)
		{
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			answer = answer.power2_destroy_this();
			answer = unlimited_int::remainder_recurring_divison(answer, remainder);
		}
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
		const size_t table_index = (window_value - (size_t)1) >> 1;
		answer *= odd_powers_table[table_index];
		answer = unlimited_int::remainder_recurring_divison(answer, remainder);
		current_bit = window_end + (size_t)1;
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || remainder.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"");
	if (answer.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\" in answer");
#endif
	return answer;
}
unlimited_int unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const volatile bool *const terminator)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"");
#endif
	const bool terminator_is_nullptr = terminator == nullptr;
	if (base.is_zero() && power.is_zero())
		throw std::invalid_argument("Invalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power)\" pow(0, 0) is mathematically undefined");
	if (power.is_zero())
		return unlimited_int(1);
	if (base.is_zero() || power._is_negative)
		return unlimited_int();
	if (!terminator_is_nullptr)
		if (*terminator)
			return unlimited_int();
	const size_t power_bit_length = power.get_length_in_bits();
	//Same algorithm structure as the modular overload above, without the modular reduction after each multiplication or squaring. See the commentary up there for the details of both the plain-loop fallback and the sliding-window main path.
	if (power_bit_length <= SMALL_POWER_THRESHOLD_BITS)
	{
		unlimited_int current_power = base;
		unlimited_int power_cpy = power;
		power_cpy._is_negative = false;
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
		unlimited_int answer((few_bits)1);
		if (power_cpy.modulo_2() == static_cast<unsigned short>(1))
			answer *= current_power;
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
		power_cpy >>= (size_t)1;
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
		while (!power_cpy.is_zero())
		{
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			current_power = current_power.power2_destroy_this();
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			if (power_cpy.modulo_2() == static_cast<unsigned short>(1))
				answer *= current_power;
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			power_cpy >>= (size_t)1;
		}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
		std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
		if (base.find_inconsistencies() || power.find_inconsistencies())
			throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"");
		if (answer.find_inconsistencies())
			throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\" in answer");
#endif
		return answer;
	}
	std::array<unlimited_int, WINDOW_TABLE_ENTRIES> odd_powers_table;
	odd_powers_table[(size_t)0] = base;
	unlimited_int base_squared = base;
	base_squared = base_squared.power2_destroy_this();
	if (!terminator_is_nullptr)
		if (*terminator)
			return unlimited_int();
	for (size_t table_index = (size_t)1; table_index < WINDOW_TABLE_ENTRIES; ++table_index)
	{
		odd_powers_table[table_index] = odd_powers_table[table_index - (size_t)1] * base_squared;
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
	}
	std::vector<bool> power_bits_most_significant_first;
	power_bits_most_significant_first.reserve(power_bit_length);
	{
		custom_linked_list_node<int_array>* current_int_array_node = power.get_most_significant_used_int_array();
		int_array current_int_array = *current_int_array_node->value;
		size_t index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
		const few_bits most_significant_int_value = current_int_array.intarr[index_in_int_array];
		int starting_bit_position_in_int = NUM_OF_BITS_few_bits - 1 - unlimited_int::num_of_zero_bits_preceding_number(most_significant_int_value);
		while (true)
		{
			const few_bits current_int_value = current_int_array.intarr[index_in_int_array];
			for (int bit_position_in_int = starting_bit_position_in_int; bit_position_in_int >= 0; --bit_position_in_int)
				power_bits_most_significant_first.push_back(((current_int_value >> bit_position_in_int) & (few_bits)1) != (few_bits)0);
			starting_bit_position_in_int = NUM_OF_BITS_few_bits - 1;
			if (index_in_int_array != (size_t)0)
			{
				--index_in_int_array;
				continue;
			}
			bool reached_begin_sentinel = false;
			while (true)
			{
				current_int_array_node = current_int_array_node->previous;
				if (current_int_array_node == power.intarrays->begin())
				{
					reached_begin_sentinel = true;
					break;
				}
				current_int_array = *current_int_array_node->value;
				if (current_int_array.num_of_used_ints != (size_t)0)
					break;
			}
			if (reached_begin_sentinel)
				break;
			index_in_int_array = current_int_array.num_of_used_ints - (size_t)1;
		}
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (power_bits_most_significant_first.size() != power_bit_length)
		throw std::logic_error("Inconsistency in unlimited_int::pow windowed path: MSB-first bit extraction of |power| did not produce get_length_in_bits() bits");
#endif
	const size_t total_bits = power_bits_most_significant_first.size();
	unlimited_int answer;
	size_t current_bit = (size_t)0;
	{
		size_t window_end = current_bit + WINDOW_BITS - (size_t)1;
		if (window_end >= total_bits)
			window_end = total_bits - (size_t)1;
		while (!power_bits_most_significant_first[window_end])
			--window_end;
		size_t window_value = (size_t)0;
		for (size_t bit_index = current_bit; bit_index <= window_end; ++bit_index)
		{
			window_value <<= 1;
			if (power_bits_most_significant_first[bit_index])
				window_value |= (size_t)1;
		}
		const size_t table_index = (window_value - (size_t)1) >> 1;
		answer = odd_powers_table[table_index];
		current_bit = window_end + (size_t)1;
	}
	while (current_bit < total_bits)
	{
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
		if (!power_bits_most_significant_first[current_bit])
		{
			answer = answer.power2_destroy_this();
			++current_bit;
			continue;
		}
		size_t window_end = current_bit + WINDOW_BITS - (size_t)1;
		if (window_end >= total_bits)
			window_end = total_bits - (size_t)1;
		while (!power_bits_most_significant_first[window_end])
			--window_end;
		const size_t window_width = window_end - current_bit + (size_t)1;
		size_t window_value = (size_t)0;
		for (size_t bit_index = current_bit; bit_index <= window_end; ++bit_index)
		{
			window_value <<= 1;
			if (power_bits_most_significant_first[bit_index])
				window_value |= (size_t)1;
		}
		for (size_t squaring_counter = (size_t)0; squaring_counter < window_width; ++squaring_counter)
		{
			if (!terminator_is_nullptr)
				if (*terminator)
					return unlimited_int();
			answer = answer.power2_destroy_this();
		}
		if (!terminator_is_nullptr)
			if (*terminator)
				return unlimited_int();
		const size_t table_index = (window_value - (size_t)1) >> 1;
		answer *= odd_powers_table[table_index];
		current_bit = window_end + (size_t)1;
	}
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"");
	if (answer.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\" in answer");
#endif
	return answer;
}
