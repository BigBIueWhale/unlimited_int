#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//makes use if the identity (a ⋅ b) mod m = [(a mod m) ⋅ (b mod m)] mod m
std::shared_ptr<unlimited_int> unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod, volatile bool *const terminator)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || mod.find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"");
#endif
	const bool terminator_is_nullptr = terminator == nullptr;
	if (base.is_zero() && power.is_zero())
		throw std::invalid_argument("\nInvalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod)\" pow(0, 0) is mathematically undefined");
	if (mod.is_zero())
		throw std::invalid_argument("\nInvalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod)\" division by zero is undefined");
	unlimited_int* answer = new unlimited_int((few_bits)1);
	if (power.num_of_used_ints == (size_t)0)
		return std::shared_ptr<unlimited_int>(answer);
	if (base.num_of_used_ints == (size_t)0 || power.is_negative == true)
	{
		answer->set_to_zero();
		return std::shared_ptr<unlimited_int>(answer);
	}
	if (mod == unlimited_int((few_bits)1))
	{
		answer->set_to_zero();
		return std::shared_ptr<unlimited_int>(answer);
	}
	std::shared_ptr<unlimited_int> current_power(base.copy());
	current_power = unlimited_int::remainder_recurring_divison(*current_power, mod);
	if (current_power->is_zero())
	{
		answer->set_to_zero();
		return std::shared_ptr<unlimited_int>(answer);
	}
	std::shared_ptr<unlimited_int> power_cpy(power.copy());
	if (!terminator_is_nullptr)
		if (*terminator)
			return std::shared_ptr<unlimited_int>(answer);
	if (power_cpy->modulo_2() == 1)
	{
		*answer *= *current_power;
		*answer = unlimited_int::remainder_recurring_divison(*answer, mod);
	}
	*power_cpy >>= (size_t)1;
	while (!power_cpy->is_zero())
	{
		if (!terminator_is_nullptr)
			if (*terminator)
				return std::shared_ptr<unlimited_int>(answer);
		current_power = current_power->power2_destroy_this();
		if (!terminator_is_nullptr)
			if (*terminator)
				return std::shared_ptr<unlimited_int>(answer);
		*current_power = unlimited_int::remainder_recurring_divison(*current_power, mod);
		if (power_cpy->modulo_2() == 1)
		{
			if (!terminator_is_nullptr)
				if (*terminator)
					return std::shared_ptr<unlimited_int>(answer);
			*answer *= *current_power;
			if (!terminator_is_nullptr)
				if (*terminator)
					return std::shared_ptr<unlimited_int>(answer);
			*answer = unlimited_int::remainder_recurring_divison(*answer, mod);
		}
		if (!terminator_is_nullptr)
			if (*terminator)
				return std::shared_ptr<unlimited_int>(answer);
		*power_cpy >>= (size_t)1;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || mod.find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"");
	if (answer->find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\" in answer");
#endif
	return std::shared_ptr<unlimited_int>(answer);
}
std::shared_ptr<unlimited_int> unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, volatile bool *const terminator)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"");
#endif
	const bool terminator_is_nullptr = terminator == nullptr;
	if (base.is_zero() && power.is_zero())
		throw std::invalid_argument("\nInvalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power)\" pow(0, 0) is mathematically undefined");
	unlimited_int* answer = new unlimited_int(((few_bits)1));
	if (power.num_of_used_ints == (size_t)0)
		return std::shared_ptr<unlimited_int>(answer);
	if (base.num_of_used_ints == (size_t)0 || power.is_negative)
	{
		answer->set_to_zero();
		return std::shared_ptr<unlimited_int>(answer);
	}
	if (!terminator_is_nullptr)
		if (*terminator)
			return std::shared_ptr<unlimited_int>(answer);
	std::shared_ptr<unlimited_int> current_power(base.copy());
	std::shared_ptr<unlimited_int> power_cpy(power.copy());
	if (!terminator_is_nullptr)
		if (*terminator)
			return std::shared_ptr<unlimited_int>(answer);
	if (power_cpy->modulo_2() == 1)
		*answer *= *current_power;
	if (!terminator_is_nullptr)
		if (*terminator)
			return std::shared_ptr<unlimited_int>(answer);
	*power_cpy >>= (size_t)1;
	if (!terminator_is_nullptr)
		if (*terminator)
			return std::shared_ptr<unlimited_int>(answer);
	while (!power_cpy->is_zero())
	{
		if (!terminator_is_nullptr)
			if (*terminator)
				return std::shared_ptr<unlimited_int>(answer);
		current_power = current_power->power2_destroy_this();
		if (!terminator_is_nullptr)
			if (*terminator)
				return std::shared_ptr<unlimited_int>(answer);
		if (power_cpy->modulo_2() == 1)
			*answer *= *current_power;
		if (!terminator_is_nullptr)
			if (*terminator)
				return std::shared_ptr<unlimited_int>(answer);
		*power_cpy >>= (size_t)1;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"");
	if (answer->find_inconsistencies())
		throw std::logic_error("\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\" in answer");
#endif
	return std::shared_ptr<unlimited_int>(answer);
}
