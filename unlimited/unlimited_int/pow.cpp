#include "unlimited_int.hpp"
using namespace unlimited;
#if DEBUG_MODE == 2
#include <iostream>
#endif
//makes use if the identity (a ⋅ b) mod m = [(a mod m) ⋅ (b mod m)] mod m
std::shared_ptr<unlimited_int> unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
						, bool* terminator
#endif
							)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || mod.find_inconsistencies())
		throw "\nThe inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
#endif
	if (base.is_zero() && power.is_zero())
		throw "\nInvalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod)\" pow(0, 0) is mathematically undefined";
	if (mod.is_zero())
		throw "\nInvalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod)\" division by zero is undefined";
	unlimited_int* answer = new unlimited_int((many_bits)1);
	if (power.num_of_used_ints == 0)
		return std::shared_ptr<unlimited_int>(answer);
	if ((base.num_of_used_ints == 0) || (power.is_negative == true)) { answer->set_to_zero(); return std::shared_ptr<unlimited_int>(answer); }
	if (mod == unlimited_int((many_bits_signed)1)) { answer->set_to_zero(); return std::shared_ptr<unlimited_int>(answer); }
	std::shared_ptr<unlimited_int> current_power(base.copy());
	current_power = unlimited_int::remainder_recurring_divison(*current_power, mod);
	if (current_power->is_zero()) { answer->set_to_zero(); return std::shared_ptr<unlimited_int>(answer); }
	std::shared_ptr<unlimited_int> power_cpy(power.copy());
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
	if (terminator != nullptr)
		if (*terminator)
			return std::shared_ptr<unlimited_int>(answer);
#endif
	if (power_cpy->modulo_2() == 1)
	{
		*answer *= *current_power;
		*answer = unlimited_int::remainder_recurring_divison(*answer, mod);
	}
	*power_cpy >>= 1;
	while (!power_cpy->is_zero())
	{
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		if (terminator != nullptr)
			if (*terminator)
				return std::shared_ptr<unlimited_int>(answer);
#endif
		current_power = current_power->power2_destroy_this();
		*current_power = unlimited_int::remainder_recurring_divison(*current_power, mod);
		if (power_cpy->modulo_2() == 1)
		{
			*answer *= *current_power;
			*answer = unlimited_int::remainder_recurring_divison(*answer, mod);
		}
		*power_cpy >>= 1;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies() || mod.find_inconsistencies())
		throw "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\"";
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod)\" in answer";
#endif
	return std::shared_ptr<unlimited_int>(answer);
}
std::shared_ptr<unlimited_int> unlimited_int::pow(const unlimited_int& base, const unlimited_int& power)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
		throw "\nThe inconsistency was found in start of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
	if (base.is_zero() && power.is_zero())
		throw "\nInvalid arguments in function \"unlimited_int* unlimited_int::pow(const unlimited_int& base, const unlimited_int& power)\" pow(0, 0) is mathematically undefined";
	unlimited_int* answer = new unlimited_int(((many_bits_signed)1));
	if (power.num_of_used_ints == 0) { return std::shared_ptr<unlimited_int>(answer); }
	if ((base.num_of_used_ints == 0) || power.is_negative) { answer->set_to_zero(); return std::shared_ptr<unlimited_int>(answer); }
	std::shared_ptr<unlimited_int> current_power(base.copy());
	std::shared_ptr<unlimited_int> power_cpy(power.copy());
	if (power_cpy->modulo_2() == 1) { *answer *= *current_power; }
	*power_cpy >>= 1;
	while (!power_cpy->is_zero())
	{
		current_power = current_power->power2_destroy_this();
		if (power_cpy->modulo_2() == 1) { *answer *= *current_power; }
		*power_cpy >>= 1;
	}
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
#endif
#if DEBUG_MODE > 0
	if (base.find_inconsistencies() || power.find_inconsistencies())
		throw "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\"";
	if (answer->find_inconsistencies())
		throw "\nThe inconsistency was found in end of function: \"unlimited_int::pow(unlimited_int& base, unlimited_int& power)\" in answer";
#endif
	return std::shared_ptr<unlimited_int>(answer);
}
