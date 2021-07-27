#include "unlimited_int.hpp"
using namespace unlimited;
std::shared_ptr<unlimited_int> unlimited_int::gcd(const unlimited_int& a, const unlimited_int& b)
{
	std::shared_ptr<unlimited_int> a_editable(a.copy());
	std::shared_ptr<unlimited_int> b_editable(b.copy());
	a_editable->is_negative = false;
	b_editable->is_negative = false;
	while (!b_editable->is_zero())
	{
		std::shared_ptr<unlimited_int> remainder_result = (*a_editable) % (*b_editable);
		a_editable = b_editable;
		b_editable = remainder_result;
	}
	return a_editable;
}
//lcm(a, b) = abs(a * b) / gcd(a, b)
std::shared_ptr<unlimited_int> unlimited_int::lcm(const unlimited_int& a, const unlimited_int& b)
{
	std::shared_ptr<unlimited_int> gcd = unlimited::unlimited_int::gcd(a, b);
	std::shared_ptr<unlimited_int> result_of_division = a / *gcd;
	std::shared_ptr<unlimited_int> result = b * (*result_of_division);
	result->is_negative = false;
	return result;
}
