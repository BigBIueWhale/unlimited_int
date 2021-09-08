#include "unlimited_int.hpp"
using namespace unlimited;
unlimited_int unlimited_int::gcd(const unlimited_int& a, const unlimited_int& b)
{
	unlimited_int a_editable = a;
	unlimited_int b_editable = b;
	a_editable.self_abs();
	b_editable.self_abs();
	while (!b_editable.is_zero())
	{
		unlimited_int remainder_result = a_editable % b_editable;
		a_editable = std::move(b_editable);
		b_editable = std::move(remainder_result);
	}
	return a_editable;
}
//lcm(a, b) = abs(a * b) / gcd(a, b)
unlimited_int unlimited_int::lcm(const unlimited_int& a, const unlimited_int& b)
{
	unlimited_int result = b * (a / unlimited::unlimited_int::gcd(a, b));
	result.self_abs();
	return result;
}
