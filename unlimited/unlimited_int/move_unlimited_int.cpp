#include "unlimited_int.hpp"
using namespace unlimited;
//Move assignment- transfers the content of an unlimited_int prvalue, into "this"
void unlimited_int::operator=(unlimited_int&& source) noexcept
{
	if (this->auto_destroy)
		this->flush();
	this->auto_destroy = source.auto_destroy;
	this->is_negative = source.is_negative;
	this->num_of_intarrays_used = source.num_of_intarrays_used;
	this->num_of_used_ints = source.num_of_used_ints;
	this->intarrays = source.intarrays;
	source.forget_memory();
}
//move constructor
unlimited_int::unlimited_int(unlimited_int&& source) noexcept
{
	this->auto_destroy = source.auto_destroy;
	this->is_negative = source.is_negative;
	this->num_of_intarrays_used = source.num_of_intarrays_used;
	this->num_of_used_ints = source.num_of_used_ints;
	this->intarrays = source.intarrays;
	source.forget_memory();
}
