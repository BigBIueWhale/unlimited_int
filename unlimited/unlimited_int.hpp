#ifndef UNLIMITED_UNLIMITED_INT_H
#define UNLIMITED_UNLIMITED_INT_H
#include "list_of_int_arrays.hpp"
#include <string>
#include <ostream>
#include <memory>
namespace unlimited
{
	class unlimited_int
	{
	private:
		//is defined in unlimited_int.cpp, just because otherwise it's not accessible from static member functions.
		static std::shared_ptr<unlimited_int> current_random;
	protected:
		many_bits num_of_used_ints;
		list_of_int_arrays intarrays;
		many_bits num_of_intarrays_used;
		bool is_negative;
		bool auto_destroy = true;
#if DEBUG_MODE > 0
		bool find_inconsistencies() const;
		void print_properties() const;
		void compact_print() const;
		void print() const;
#endif
		//primes a number to be ready for filling-in its few_bits. If too many ints are requested to be zeroed, it stops at the end of intarrays.
		void fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until);
		//fills-in zeros in a number in the least-significant ints. If too many ints are requested to be zeroed, it stops at the end of intarrays
		void fill_0_until_num_of_ints(const many_bits num_of_ints_to_zero);
		//more efficient by using original memory
		unlimited_int* multiply_karatsuba_destroy_this_and_num_to_mult(unlimited_int* num_to_mult);
		//more efficient by using original memory
		unlimited_int* multiply_karatsuba_destroy_this(const unlimited_int* num_to_mult);
		//adjusts the num_of_used_ints variables in int_array and in unlimited_int to accurately describe where a number begins
		void cutoff_leading_zeros(Node* int_array_to_start_at);
		//splits this into two different unlimited_ints, significant bits and insignificant bits. Based on many_bits that specifies the number of ints to put in unlimited_int low.
		void split_at(const many_bits index, unlimited_int* high, unlimited_int* low) const;
		//same as split_at except that it it's more efficient because it uses the original memory of this
		void split_at_and_use_original(const many_bits index, unlimited_int* high, unlimited_int* low);
		//grade school multiplication algorithm
		void multiply(const few_bits num_to_mult, unlimited_int* answer) const;
		//more efficient than multiply because it's specific-case, used by multiply.
		void multiply_both_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const;
		//more efficient than multiply because it's specific-case, used by multiply.
		void multiply_this_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const;
		//more efficient than multiply because it's specific-case, used by multiply.
		void multiply_answer_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const;
		//shifts right by number of few_bits
		void shift_right(const many_bits shift_by);
		//shifts right by number of few_bits
		void shift_left(const many_bits shift_by);
		//returns 'L', 'S' or 'E' based on the length of the numbers, and the negativity. Not accurate when both numbers are the same length and the same sign (except that when both numbers are 0 it is accurate).
		char estimate_compare_to(const unlimited_int& num_to_compare_to) const;
		//returns 'L', 'S' or 'E' based on the length of the numbers. Not accurate when both numbers are the same length.
		char estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const;
		//in case a number was big, and now only a tiny portion of it is used, the function flush_unused() can shrink the number to an appropriate size and put the unused "int_array"s into the piggy bank.
		void flush_unused()
		{
			many_bits difference = this->intarrays.intarrays.length - this->num_of_intarrays_used;
			if (difference > 0) { this->intarrays.flush_significant_to_piggy_bank(difference); }
		}
		//useful function for iteration. Gets the most significant Node* that is also used in the number.
		Node* get_most_significant_used_int_array() const;
		//uses shift_left and bit manipulation to shift number left by a certain number of bits.
		void shift_left_by_bits(const many_bits num_of_bits_to_shift_by);
		//uses shift_right and bit manipulation to shift number right by a certain number of bits.
		void shift_right_by_bits(const many_bits num_of_bits_to_shift_by);
		//accepts two unlimited_ints that the result of their division fits in "few_bits". It uses binary search while continuously using multiply(few_bits)
		few_bits binary_search_divide(const unlimited_int& num_to_divide_by) const;
		//copies the most significant part of an unlimited_int into another unlimited_int. Specifically the first num_of_ints_to_copy "few_bits"s. It pastes into the least significant portion of other.
		//Other doesn't need to be prepared.
		void copy_most_significant_to(unlimited_int& other, const many_bits num_of_ints_to_copy) const;
		//requires all functions to honor num_of_used_ints in every int_array. worst-case efficiency: O(n) where n is MAX_ALLOC
		void push_to_insignificant(const few_bits num_to_push);
		//starts from the most significant used few_bits and then goes down num_int_to_find "few_bits"s
		//returns a __list_location__ with nullptr if num_int_to_find is too big.
		__list_location__ find_num_used_int_from_significant(const many_bits num_int_to_find) const;
		//division algorithm - needs to be replaced. Splits the numerator so that its length is roughly equal to the denominator and then uses binary_search_divide. It's the grade-school algorithm.
		std::shared_ptr<unlimited_int> divide_by(const unlimited_int& num_to_divide_by) const;
		//Uses known ASCII values. Base can be from 1 until 36 including. Works with both uppercase and lowercase English letters.
		static int char_to_number(const char ch, const int base = 10);
		//Uses known ASCII values. Base can be from 1 until 36 including. Creates lowercase letters.
		static char number_to_char(const int num, const int base);
		//sets all variables to their default values without deleting anything that is dynamically allocated and without flushing into the piggy bank.
		void forget_memory();
		//creates an unlimited_int from an array of uint32_t. Used heavily in the cryptographic secure random number generator, and specifically in the SHA-256 algorithm that hashes the value of an unlimited_int.
		unlimited_int(uint32_t*, many_bits);
		//creates an unlimited_int from an array of uint64_t. Used heavily in the cryptographic secure random number generator, and specifically in the SHA-512 algorithm that hashes the value of an unlimited_int.
		unlimited_int(uint64_t*, many_bits);
		//used by unlimited_int::unlimited_int(uint32_t*, many_bits) but can also be used independently
		void assign(uint32_t*, many_bits);
		//used by unlimited_int::unlimited_int(uint64_t*, many_bits) but can also be used independently
		void assign(uint64_t*, many_bits);
		//adds using grade school algorithm.
		void add(const unlimited_int* num_to_add, unlimited_int* answer) const;
		void assign(const few_bits value_to_assign);
		void assign(const many_bits value_to_assign);
		void assign(const few_bits_signed value_to_assign) { this->assign((few_bits)value_to_assign); this->is_negative = value_to_assign < 0; }
		void assign(const many_bits_signed value_to_assign) { this->assign((many_bits)value_to_assign); this->is_negative = value_to_assign < 0; }
		//returns 'L' for larger, 'S' for smaller, 'E' for equal. This function is precise, and yet tries to increase efficiency by first using estimate_compare_to
		char compare_to(const unlimited_int& num_to_compare_to) const;
		//returns 'L' for larger, 'S' for smaller, 'E' for equal. This function is precise, and yet tries to increase efficiency by first using estimate_compare_to. Ignores minus/plus sign of this.
		char compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const;
		//grade school algorithm for multiplication. I need to design and write a squaring basecase multiplication algorithm to use in function "unlimited_int* unlimited_int::power2() const" (for example) instead of using this function.
		void multiply_basecase(const unlimited_int* num_to_mult, unlimited_int* answer) const;
		//Karatsuba multiplication algorithm. Very efficient.
		unlimited_int* multiply_karatsuba(const unlimited_int* num_to_mult) const;
		//grade school subtraction algorithm.
		void subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const;
		//returns the power of 2 that "this" is (for example it returns 10 if this is 1024) This function completely ignores the sign of this. Returns -1 if not exact power of 2. Efficiency: O(n) but probably closer to O(1) in most cases.
		many_bits_signed find_exact_log_2() const;
		//positive numbers and negative numbers get the same fingerprint.
		std::string fingerprint() const { return this->calculate_sha256_hash()->to_string(16); }
	public:
		//takes a shared_ptr and transfers it's content to an unlimited_int. It doesn't sets the auto_destroy tag of the shared_ptr to false.
		unlimited_int(std::shared_ptr<unlimited_int>);
		//takes a shared_ptr and transfers it's content to an unlimited_int. It doesn't sets the auto_destroy tag of the shared_ptr to false.
		void operator=(std::shared_ptr<unlimited_int>);
		//converts unlimited_int to few_bits, but actually returns many_bits_signed so that it can fit the few_bits number and the sign as well.
		//This function works no matter the size of this, but it only works as a conversion when this unlimited_int is within of few_bits (meaning, it would have to be very small).
		many_bits_signed get_least_significant() const;
		//returns the length of the current number in bits (the used part). It gives the precise number, and manipulates the most significant few_bits using bitwise operators to get the precise length in bits of this unlimited_int.
		many_bits get_length_in_bits() const;
		bool get_is_negative() const { return this->is_negative; }
		void set_auto_destroy(bool tof) { this->auto_destroy = tof; }
		bool get_auto_destroy() const { return this->auto_destroy; }
		unlimited_int() { this->forget_memory(); }
		unlimited_int(const few_bits num_to_assign) { this->assign(num_to_assign); }
		unlimited_int(const many_bits num_to_assign) { this->assign(num_to_assign); }
		unlimited_int(const few_bits_signed num_to_assign);
		unlimited_int(const many_bits_signed num_to_assign);
		//recieves an unlimited_int object to assign this, without copying, meaning that the original memory is used. The second parameter is bool and it decides whether to enable auto_destroy in this.
		//This entire function is basically a hack to gain access to a const unlimited_int
		unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this);
		//copy constructor- creates full independent copy.
		unlimited_int(const unlimited_int& num_to_assign) { num_to_assign.copy_to(*this); };
		//creates full independent copy.
		void operator=(const unlimited_int& num_to_assign) { num_to_assign.copy_to(*this); }
		//sets the most immediate variables in unlimited_int to thier default values. Doesn't touch the variables in this->intarrays
		void set_to_zero()
		{
			this->num_of_intarrays_used = 0;
			this->is_negative = false;
			this->num_of_used_ints = 0;
		}
		list_of_int_arrays* get_intarrays() { return &(this->intarrays); }
		void set_num_of_intarrays_used(many_bits set_to) { this->num_of_intarrays_used = set_to; }
		//destroy memory if intarrays using delete[] operator and then set all variables to their default values so that the number can keep being used.
		void destroy();
		//flush to piggy bank, but if there's no room left in the piggy bank because of PIGGY_BANK_MAXIMUM then delete[] operator is used.
		//The variables are then reset for future use.
		void flush();
		//more efficient Karatsube algorithm specifically for squaring numbers
		//I need to integrate a basecase power2 algorithm for even more efficiency.
		std::shared_ptr<unlimited_int> power2() const;
		//even more efficient than unlmimited_int::power2 because it utilises the original memory
		//I need to integrate a basecase power2 algorithm for even more efficiency.
		std::shared_ptr<unlimited_int> power2_destroy_this();
		//creates independent copy of this's values inside of num_to_paste_into using the already existent memory inside num_to_paste_into and increasing the size if necessary.
		void copy_to(unlimited_int& num_to_paste_into) const;
		//O(1) efficiency to swap two unlimited_int numbers. Also swaps the bool "auto_destroy" values
		void swap(unlimited_int& num_to_swap_with);
		//The interface through which to use Karatsuba multiplication algorithm.
		//multiplies using Karatsuba and tries to optimize multiplication process by checking if one of the multiplicands is a power of 2. Also deals with negative numbers.
		std::shared_ptr<unlimited_int> operator*(const unlimited_int&) const;
		//Interface through which to use "unlimited_int* divide_by(const unlimited_int& num_to_divide_by) const" (which is quite inefficient, uses binary search). Also this function deals with the sign of the numbers.
		std::shared_ptr<unlimited_int> operator/(const unlimited_int& denominator) const;
		void operator/=(const unlimited_int& denominator) { (*this) = (*this) / denominator; }
		//Remainder (of division) operator. Not modulo. The sign of the result will always be the same as the left side of the operator.
		std::shared_ptr<unlimited_int> operator%(const unlimited_int& ui) const;
		//Remainder functions- remainder of division. NOT MUDULO (difference in treatment of negative values). Sign this won't change.
		void operator%=(const unlimited_int& ui);
		//interface to access multiply functions. It multiplies an unlimited_int by few_bits. Efficiency: O(n) where n is the number of digits in the unlimited_int number.
		std::shared_ptr<unlimited_int> operator*(const few_bits num) const;
		friend std::shared_ptr<unlimited_int> operator*(const few_bits, const unlimited_int&);
		//A little bit more efficient than using the regular multiplication function.
		void operator*=(const few_bits num) { this->multiply(num, this); }
		//The interface through which to use Karatsuba multiplication algorithm.
		//multiplies using Karatsuba and tries to optimize multiplication process by checking if one of the multiplicands is a power of 2. Also deals with negative numbers.
		//Uses multiply_karatsuba_destroy_this for extra efficiency over the normal multiply_karatsuba.
		void operator*=(const unlimited_int&);
		//Slightly more efficient.
		void operator+=(const unlimited_int& other) { this->add(&other, this); }
		//Uses void "add(const unlimited_int* num_to_add, unlimited_int* answer) const"
		std::shared_ptr<unlimited_int> operator+(const unlimited_int& other) const;
		void operator-=(const unlimited_int& other) { this->subtract(&other, this); }
		std::shared_ptr<unlimited_int> operator-(const unlimited_int& other) const;
		bool operator<=(const unlimited_int& other) const { char value = this->compare_to(other); return ((value == 'S') || (value == 'E')); }
		bool operator>=(const unlimited_int& other) const { char value = this->compare_to(other); return ((value == 'L') || (value == 'E')); }
		bool operator==(const unlimited_int& other) const { char value = this->compare_to(other); return (value == 'E'); }
		bool operator<(const unlimited_int& other) const { char value = this->compare_to(other); return (value == 'S'); }
		bool operator>(const unlimited_int& other) const { char value = this->compare_to(other); return (value == 'L'); }
		bool operator!=(const unlimited_int& ui) const { return (this->compare_to(ui) != 'E'); }
		//expected Efficiency O(1)
		void operator++();
		//expected Efficiency O(1)
		void operator--();
		//Creates full copy with opposite sign, unless number is zero because then the sign stays positive.
		std::shared_ptr<unlimited_int> operator-();
		//O(1) operation to flip a numbers sign from positive to negative or from negative to positive. Doesn't do anything when the number is 0.
		void flip_sign() { if (!this->is_zero()) this->is_negative = !this->is_negative; }
		//Creates full copy with positive, unless number is zero because then the sign stays positive.
		std::shared_ptr<unlimited_int> abs();
		//O(1) operation to flip a number's sign to positive.
		void self_abs() { this->is_negative = false; }
		//O(1) operation to flip a number's sign to negative. Doesn't do anything when the number is 0.
		void make_negative() { if (!this->is_zero()) this->is_negative = false; }
		void operator=(const many_bits_signed num) { this->assign(num); }
		friend std::ostream& operator<<(std::ostream& os, const unlimited_int& ui);
		void operator<<=(const many_bits num_to_shift_by) { this->shift_left_by_bits(num_to_shift_by); }
		void operator>>=(const many_bits num_to_shift_by) { this->shift_right_by_bits(num_to_shift_by); }
		//Bitwise operator AND, returns positive number, ignores the sign
		std::shared_ptr<unlimited_int> operator&(const unlimited_int&) const;
		//Bitwise operator OR, the result will be as long as the longer number between the two. Returns positive number, ignores the sign
		std::shared_ptr<unlimited_int> operator|(const unlimited_int&) const;
		//Bitwise operator XOR, the result will be as long as the longer number between the two. Returns positive number, ignores the sign.
		std::shared_ptr<unlimited_int> operator^(const unlimited_int&) const;
		//Bitwise operator NOT. Only inverses the used bits. For example:
		//unlimited_int num = 0b00001101
		//std::cout << unlimited_int(~num);
		//That code will output 0b00000010 because it's ignoring all bits that are before the most significant 1.
		std::shared_ptr<unlimited_int> operator~() const;
		//Just like operator~ except that it's more efficient because it directly changes the original number's bits. Just like &= compared to & (for example)
		void invert_bits();
		void operator&=(const unlimited_int& right) { *this = (*this) & right; }
		void operator|=(const unlimited_int& right) { *this = (*this) | right; }
		void operator^=(const unlimited_int& right) { *this = (*this) ^ right; }
		std::shared_ptr<unlimited_int> operator>>(const many_bits num_to_shift_by) const {
			unlimited_int* answer = new unlimited_int; this->copy_to(*answer); *answer >>= num_to_shift_by;
			return std::shared_ptr<unlimited_int>(answer);
		}
		std::shared_ptr<unlimited_int> operator<<(const many_bits num_to_shift_by) const {
			unlimited_int* answer = new unlimited_int; this->copy_to(*answer); *answer <<= num_to_shift_by;
			return std::shared_ptr<unlimited_int>(answer);
		}
		//returns pointer to clone of this unlimited_int. Correct syntax is:
		//unlimited_int original = 7879;
		//unlimited_int clone_a = original.copy();
		unlimited_int* copy() const {
			unlimited_int* copy_ui = new unlimited_int;
			this->copy_to(*copy_ui);
			return copy_ui;
		}
		//super-efficient method to find modulo_2 of unlimited_int number.
		//This function returns either 1 or 0. Now, confusingly it's actually modulo and not remainder of division (different approach with negative numbers).
		int modulo_2() const
		{
			if (this->num_of_used_ints == 0) { return 0; }
			if (this->intarrays.intarrays.first->value->intarr[0] & 0x1) { return 1; }
			return 0;
		}
		//from base 1 until base 36 (including). Uses lowercase English letters. Returns null-terminated string.
		char* to_c_string(const int base = 16) const;
		//from base 1 until base 36 (including). Uses lowercase English letters.
		std::string to_string(const int base = 16) const;
		//checks if unlimited_int's value is equal to 0, based on num_of_used_ints
		bool is_zero() const { return (this->num_of_used_ints == 0); }
		//efficient method for power (math function) with remainder as well.
		static std::shared_ptr<unlimited_int> pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod);
		//efficient method for power (math function)
		static std::shared_ptr<unlimited_int> pow(const unlimited_int& base, const unlimited_int& power);
		//greatest common divisor
		static std::shared_ptr<unlimited_int> gcd(const unlimited_int& a, const unlimited_int& b);
		//lowest common multiple
		static std::shared_ptr<unlimited_int> lcm(const unlimited_int& a, const unlimited_int& b);
		//from base 1 until base 36 (including). Can recognize prepended minus symbol (-). Supports both uppercase and lowercase. Receives null-terminated string.
		static std::shared_ptr<unlimited_int> from_string(const char* str, const int base = 10);
		//uses "static unlimited_int* from_string(const char* str, const int base = 10)"
		static std::shared_ptr<unlimited_int> from_string(const std::string& str, const int base = 10) { return unlimited_int::from_string(str.c_str(), base); }
	protected:
		//generates random number from the time in milliseconds and from the speed of the machine (the measured using time in milliseconds)
		//Cryptographically secure, needs improvement for speed, and for getting more truely random data at runtime.
		static std::shared_ptr<unlimited_int> generate_truly_random();
		//Starts with a seed from generate_truly_random() and then continues on, each time taking the sha256/sha512 hash (depending on x86/x64) and thereby creating a series of cryptographically secure random numbers.
		//Returns random number in the range of 0 until and including (2^256 - 1) or 0 until and including (2^512 - 1) with perfectly random distribution. 256 when compiling in x86 and 512 when compiling in x64.
		static std::shared_ptr<unlimited_int> generate_next_random();
		//stacks 256bit sha256 hashes or 512bit sha512 hashes until the number is long enough.
		static std::shared_ptr<unlimited_int> generate_random_that_is_at_least(many_bits num_of_bits);
	public:
		//only use this when exiting the program. It helps to figure out memory leaks by making sure that the statically allocated memory attached to unlimited::unlimited_int::current_random is flushed.
		static void flush_current_random();
		//Uses generate_random_that_is_at_least and then the modulo operator to generate a random number with perfectly random distribution in the given range.
		//Cryptographically secure.
		static std::shared_ptr<unlimited_int> generate_random(const unlimited_int& min, const unlimited_int& max);
		//Calculates the sha-256 hash of an unlimited_int. Splits number into chunks of 32-bit integers, whether or not the program is in 64-bit mode.
		//The hash function produces the same output whether this library is in 32bit mode or in 64bit mode.
		//It directly iterates through the unlimited_int so there's no RAM risk of hashing huge unlimited_int numbers.
		std::shared_ptr<unlimited_int> calculate_sha256_hash() const;
		//Calculates the sha-512 hash of an unlimited_int. Splits number into chunks of 64-bit integers, whether or not the program is in 64-bit mode.
		//The hash function produces the same output whether this library is in 32bit mode or in 64bit mode.
		//It directly iterates through the unlimited_int so there's no RAM risk of hashing huge unlimited_int numbers.
		std::shared_ptr<unlimited_int> calculate_sha512_hash() const;
		//Miller-Rabin primality test: 1/2^128 chance of mistake, no "special cases" like 561 that satisfies Fermat test.
		//Ignores that sign of a number: -2 is prime.
		bool is_prime() const;
		static std::shared_ptr<unlimited_int> generate_random_prime(const unlimited_int& min, const unlimited_int& max);
		//respects this->auto_destroy tag.
		~unlimited_int() { if (this->auto_destroy) { this->flush(); this->auto_destroy = false; } }
	};
	//respects flags: std::ios::dec, std::ios::oct, std::ios::hex.
	//Super slow when printing in a base that's not a power of 2, because the division algorithm is still binary search (really slow).
	//Really fast when printing in base 8, 16.
	std::ostream& operator<<(std::ostream& os, const unlimited_int& ui);
	std::shared_ptr<unlimited_int> operator*(const few_bits, const unlimited_int&);
}
#endif
