#ifndef UNLIMITED_INT_CLASS_WAS_DECLARED
#define UNLIMITED_INT_CLASS_WAS_DECLARED true
#include "list_of_int_arrays.h"
class unlimited_int
{
protected:
	many_bits num_of_used_ints;
	list_of_int_arrays intarrays;
	many_bits num_of_intarrays_used;
	bool is_negative;
	bool auto_destroy = true;
#if DEBUG_MODE > 0
	bool find_inconsistencies();
#endif
	void fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until);
	unlimited_int* multiply_karatsuba_destroy_this_and_num_to_mult(unlimited_int* num_to_mult);
	unlimited_int* multiply_karatsuba_destroy_this(unlimited_int* num_to_mult);
	void cutoff_leading_zeros(Node* int_array_to_start_at);
	static few_bits from_hex_char_to_int(char c);
	void print_properties();
	void fill_0_until_num_of_ints(many_bits num_of_ints_to_zero);
	void compact_print();
	void print();
	void split_at(many_bits index, unlimited_int* high, unlimited_int* low);
	void split_at_and_use_original(many_bits index, unlimited_int* high, unlimited_int* low);
	void multiply(few_bits num_to_mult, unlimited_int* answer);
	void multiply_both_only_one_array(few_bits num_to_mult, unlimited_int* answer);
	void multiply_this_only_one_array(few_bits num_to_mult, unlimited_int* answer);
	void multiply_answer_only_one_array(few_bits num_to_mult, unlimited_int* answer);
	void shift_right_one_by_one(many_bits shift_by);
	void shift_right(many_bits shift_by);
	void shift_left(many_bits shift_by);
	char estimate_compare_to(unlimited_int& num_to_compare_to); //returns 'L', 'S' or 'E' based on the length of the numbers, and the negativity. Not accurate when both numbers are the same length and the same sign.
	void flush_unused()
	{
		many_bits difference = this->intarrays.intarrays.length - this->num_of_intarrays_used;
		if (difference > 0) { this->intarrays.flush_significant_to_piggy_bank(difference); }
	}
	void shift_left_by_bits(many_bits num_of_bits_to_shift_by);
	void shift_right_by_bits(many_bits num_of_bits_to_shift_by);
	//accepts two unlimited_ints that the result of their division fits in "few_bits"
	few_bits binary_search_divide(unlimited_int& num_to_divide_by);
	unlimited_int* divide_using_binary_search(unlimited_int& num_to_divide_by);
	void copy_most_significant_to(unlimited_int& other, many_bits num_of_ints_to_copy);
	void push_to_insignificant(few_bits num_to_push);
	__list_location__ find_num_used_int_from_significant(many_bits num_int_to_find);
	unlimited_int* divide_by(unlimited_int& num_to_divide_by);
	static int char_to_number(char ch, const int base);
	static char number_to_char(int num, const int base);
	static unlimited_int* generate_random(unlimited_int& max);
public:
	void forget_memory();
	bool get_is_negative() { return this->is_negative; }
	void set_auto_destroy(bool tof) { this->auto_destroy = tof; }
	void assign_hex(std::string num);
	unlimited_int() { this->set_to_zero(); }
	unlimited_int(few_bits num_to_assign) { this->assign(num_to_assign); }
	unlimited_int(many_bits num_to_assign) { this->assign(num_to_assign); }
	unlimited_int(few_bits_signed num_to_assign);
	unlimited_int(many_bits_signed num_to_assign);
	unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this);
	unlimited_int(unlimited_int* num_to_assign) { (*this) = num_to_assign; }
	void operator=(unlimited_int& num_to_assign);
	//The following function is THE most important function in this whole library in terms of syntax.
	void operator=(unlimited_int* num_to_assign) {
		if (num_to_assign == nullptr) { this->set_to_zero(); return; }
		(*this) = (*num_to_assign); delete num_to_assign; }
	void set_to_zero()
	{
		this->num_of_intarrays_used = 0;
		this->is_negative = false;
		this->num_of_used_ints = 0;
	}
	list_of_int_arrays* get_intarrays() { return &(this->intarrays); }
	void set_num_of_intarrays_used(many_bits set_to) { this->num_of_intarrays_used = set_to; }
	void destroy();
	void flush();
	void add(unlimited_int* num_to_add, unlimited_int* answer);
	void multiply_basecase(unlimited_int* num_to_mult, unlimited_int* answer);
	unlimited_int* multiply_karatsuba(unlimited_int* num_to_mult);
	unlimited_int* power2();
	unlimited_int* power2_destroy_this();
	void subtract(unlimited_int* num_to_add, unlimited_int* answer);
	void copy_to(unlimited_int& num_to_paste_into);
	void swap(unlimited_int& num_to_swap_with);
	char compare_to(unlimited_int& num_to_compare_to); //returns 'L' for larger, 'S' for smaller, 'E' for equal
	void assign(few_bits value_to_assign);
	void assign(many_bits value_to_assign);
	void assign(few_bits_signed value_to_assign) { this->assign((few_bits)value_to_assign); this->is_negative = value_to_assign < 0; }
	void assign(many_bits_signed value_to_assign) { this->assign((many_bits)value_to_assign); this->is_negative = value_to_assign < 0; }
	void randomize(many_bits num_of_digits);
	unlimited_int* operator*(unlimited_int&);
	unlimited_int* operator/(unlimited_int& denominator);
	unlimited_int* operator/(many_bits_signed num) { unlimited_int den(num); return ((*this) / den); }
	void operator/=(unlimited_int& denominator) { (*this) = (*this) / denominator; }
	void operator/=(many_bits_signed num) { unlimited_int ui(num); return ((*this) /= ui); }
	unlimited_int* operator%(unlimited_int& ui);
	void operator%=(unlimited_int& ui);
	void operator%=(many_bits_signed num) { unlimited_int ui(num); (*this) %= ui; }
	unlimited_int* operator%(many_bits_signed num) { unlimited_int ui(num); return ((*this) % ui); }
	unlimited_int* operator%(many_bits num) { return ((*this) % ((many_bits_signed)num)); }
	unlimited_int* operator*(few_bits num);
	operator many_bits_signed() const {
		if (this->num_of_used_ints == 0) { return 0; }
		if (this->is_negative) { return (-((many_bits_signed)(*this->intarrays.intarrays.first->value->intarr))); }
		else { return (*this->intarrays.intarrays.first->value->intarr); }
	}
	friend unlimited_int* operator*(few_bits, unlimited_int&);
	void operator*=(few_bits num) { this->multiply(num, this); }
	void operator*=(unlimited_int&);
	void operator+=(unlimited_int& other) { this->add(&other, this); }
	unlimited_int* operator+(unlimited_int& other);
	void operator-=(unlimited_int& other) { this->subtract(&other, this); }
	unlimited_int* operator-(unlimited_int& other);
	bool operator<=(unlimited_int& other) { char value = this->compare_to(other); return ((value == 'S') || (value == 'E')); }
	bool operator>=(unlimited_int& other) { char value = this->compare_to(other); return ((value == 'L') || (value == 'E')); }
	bool operator==(unlimited_int& other) { char value = this->compare_to(other); return (value == 'E'); }
	bool operator<(unlimited_int& other) { char value = this->compare_to(other); return (value == 'S'); }
	bool operator>(unlimited_int& other) { char value = this->compare_to(other); return (value == 'L'); }
	bool operator==(many_bits_signed num) { unlimited_int num_ui(num); return (this->compare_to(num_ui) == 'E'); }
	friend bool operator==(many_bits_signed num, unlimited_int& ui) { unlimited_int num_ui(num); return (ui.compare_to(num_ui) == 'E'); }
	bool operator<(many_bits_signed num) { unlimited_int num_ui(num); return (this->compare_to(num_ui) == 'S'); }
	friend bool operator<(many_bits_signed num, unlimited_int& ui) { unlimited_int num_ui(num); return (ui.compare_to(num_ui) == 'L'); }
	bool operator>(many_bits_signed num) { unlimited_int num_ui(num); return (this->compare_to(num_ui) == 'L'); }
	friend bool operator>(many_bits_signed num, unlimited_int& ui) { unlimited_int num_ui(num); return (ui.compare_to(num_ui) == 'S'); }
	bool operator<=(many_bits_signed num) {
		unlimited_int num_ui(num); char value = this->compare_to(num_ui);
		return ((value == 'E') || (value == 'S')); }
	friend bool operator<=(many_bits_signed num, unlimited_int& ui) {
		unlimited_int num_ui(num); char value = ui.compare_to(num_ui);
		return ((value == 'E') || (value == 'L')); }
	bool operator>=(many_bits_signed num) {
		unlimited_int num_ui(num); char value = this->compare_to(num_ui);
		return ((value == 'E') || (value == 'L'));
	}
	friend bool operator>=(many_bits_signed num, unlimited_int& ui) {
		unlimited_int num_ui(num); char value = ui.compare_to(num_ui);
		return ((value == 'E') || (value == 'S'));
	}
	bool operator!=(unlimited_int& ui) { return (this->compare_to(ui) != 'E'); }
	bool operator!=(many_bits_signed num) { unlimited_int num_ui(num); return (this->compare_to(num_ui) != 'E'); }
	friend bool operator!=(many_bits_signed num, unlimited_int& ui) { unlimited_int num_ui(num); return (ui.compare_to(num_ui) != 'E'); }
	void operator+=(many_bits num) { unlimited_int num_ui(num); this->add(&num_ui, this); }
	void operator-=(many_bits num) { unlimited_int num_ui(num); this->subtract(&num_ui, this); }
	unlimited_int* operator+(many_bits);
	friend unlimited_int* operator+(many_bits, unlimited_int&);
	friend unlimited_int* operator-(many_bits, unlimited_int&);
	void operator+=(many_bits_signed num) { unlimited_int num_ui(num); this->add(&num_ui, this); }
	void operator-=(many_bits_signed num) { unlimited_int num_ui(num); this->subtract(&num_ui, this); }
	unlimited_int* operator+(many_bits_signed num)
	{
		if (num < 0) { return this->operator-((many_bits)(-num)); }
		return this->operator+((many_bits)num);
	}
	friend unlimited_int* operator+(many_bits_signed num, unlimited_int& ui)
	{
		if (num < 0) { return ui.operator-((many_bits)(-num)); }
		return ui.operator+((many_bits)num);
	}
	unlimited_int* operator-(many_bits_signed num)
	{
		if (num < 0) { return this->operator+((many_bits)(-num)); }
		return this->operator-((many_bits)num);
	}
	friend unlimited_int* operator-(many_bits_signed num, unlimited_int& ui)
	{
		if (num < 0) { return ui.operator+((many_bits)(-num)); }
		return ui.operator-((many_bits)num);
	}
	void operator++();
	void operator--();
	void operator=(many_bits_signed num) { this->assign(num); }
	friend std::ostream& operator<<(std::ostream& os, unlimited_int& ui);
	void operator<<=(many_bits num_to_shift_by) { this->shift_left_by_bits(num_to_shift_by); }
	void operator>>=(many_bits num_to_shift_by) { this->shift_right_by_bits(num_to_shift_by); }
	unlimited_int* operator>>(many_bits num_to_shift_by) {
		unlimited_int* answer = new unlimited_int; this->copy_to(*answer); *answer >>= num_to_shift_by;
		return answer; }
	unlimited_int* operator<<(many_bits num_to_shift_by) {
		unlimited_int* answer = new unlimited_int; this->copy_to(*answer); *answer <<= num_to_shift_by;
		return answer; }
	unlimited_int* copy() {
		unlimited_int* copy_ui = new unlimited_int;
		this->copy_to(*copy_ui);
		return copy_ui;
	}
	int modulo_2()
	{
		if (this->num_of_used_ints == 0) { return 0; }
		if (this->intarrays.intarrays.first->value->intarr[0] & 0x1) { return 1; }
		return 0;
	}
	char* to_string(int base = 10);
	bool is_zero() { return (this->num_of_used_ints == 0); }
	static unlimited_int* pow(unlimited_int& base, unlimited_int& power, unlimited_int& mod);
	static unlimited_int* pow(unlimited_int& base, unlimited_int& power);
	static unlimited_int* from_string(char* str, const int base);
	static unlimited_int* generate_random(unlimited_int& min, unlimited_int& max);
	static few_bits generate_random_few_bits();
	static many_bits generate_random_many_bits();
	~unlimited_int() { if (this->auto_destroy) { this->flush(); } }
};
#endif