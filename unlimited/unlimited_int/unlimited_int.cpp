#include "unlimited_int.hpp"
#if DEBUG_MODE == 2
#include <iostream>
#endif
using namespace unlimited;
//I need to design and write a squaring basecase multiplication algorithm
//The most important thing, is to make the division algorithm more efficient, and at least to make the binary search faster.
void unlimited_int::forget_memory()
{
	this->intarrays.num_of_ints = 0;
	this->intarrays.intarrays.length = 0;
	this->intarrays.intarrays.first = nullptr;
	this->intarrays.intarrays.last = nullptr;
	this->set_to_zero();
	this->auto_destroy = true;
}
void unlimited_int::operator=(std::shared_ptr<unlimited_int> shared_ptr_ui)
{
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in start of function \"void unlimited_int::operator=(std::shared_ptr<unlimited_int> shared_ptr_ui)\":";
#endif
#if DEBUG_MODE > 0
	if (shared_ptr_ui->find_inconsistencies())
		throw "Inconsistency found in start of function \"void unlimited_int::operator=(std::shared_ptr<unlimited_int> shared_ptr_ui)\"";
#endif
	this->absorb(*shared_ptr_ui);
#if DEBUG_MODE == 2
	std::cout << "\nFinding inconsistencies in end of function \"void unlimited_int::operator=(std::shared_ptr<unlimited_int> shared_ptr_ui)\":";
#endif
#if DEBUG_MODE > 0
	if (this->find_inconsistencies())
		throw "Inconsistency found in end of function \"void unlimited_int::operator=(std::shared_ptr<unlimited_int> shared_ptr_ui)\"";
#endif
}
void unlimited_int::flush()
{
	this->auto_destroy = true;
	this->intarrays.flush_to_piggy_bank(); //bank_storage is the pool. If there's no room in the pool then delete[]/delete is used
	this->set_to_zero();
}
void unlimited_int::destroy()
{
	this->auto_destroy = true;
	this->intarrays.destroy_and_reset(); //using delete[] operator and delete operator
	this->set_to_zero();
}
