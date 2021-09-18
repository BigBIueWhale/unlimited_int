#include "unlimited_int.hpp"
using namespace unlimited;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
#include <iostream>
#endif
//makes use if the identity (a ⋅ b) mod m = [(a mod m) ⋅ (b mod m)] mod m
unlimited_int unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& remainder, const volatile bool *const terminator)
{
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"";
#endif
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || mod.find_inconsistencies())
		throw std::logic_error("The inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& remainder)\"");
#endif
	const bool terminator_is_nullptr = terminator == nullptr;
	if (base.is_zero() && power.is_zero())
		throw std::invalid_argument("Invalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& remainder)\" pow(0, 0) is mathematically undefined");
	if (remainder.is_zero())
		throw std::invalid_argument("Invalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& remainder)\" division by zero is undefined");
	if (power.is_zero())
		return unlimited_int(1);
	if (base.is_zero() || power.is_negative())
		return unlimited_int();
	if (remainder == 1)
		return unlimited_int();
	unlimited_int current_power = base;
	current_power = unlimited_int::remainder_recurring_divison(current_power, remainder);
	if (current_power.is_zero())
		return unlimited_int();
	if (!terminator_is_nullptr)
		if (*terminator)
			return unlimited_int();
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
		if (power_cpy.modulo_2() == static_cast <unsigned short int>(1))
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
