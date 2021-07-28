#ifndef UNLIMITED_UNLIMITED_INT_H
#define UNLIMITED_UNLIMITED_INT_H
#include "structures/list_of_int_arrays.hpp"
#include <string>
#include <ostream>
#include <memory> //for std::shared_ptr
namespace unlimited
{
	class unlimited_int
	{
	private:
		//Defined in unlimited/unlimited.cpp
		//Holds the current random number in the chain of random numbers. The chain is generated using the SHA256 or SHA512 hash function (depending on whether compiler is 64bits or 32bits).
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		//thread_local insures that each thread generates its own version of current_random (otherwise corruption will occur, an std::mutex will slow down the program).
		static thread_local unlimited_int current_random;
#else
		static unlimited_int current_random;
#endif
	protected:
//Member Variables
		many_bits num_of_used_ints; //number of used ints int the entire list_of_int_arrays
		//A linked list of few_bits arrays. The unlimited_int is represented as follows:
		//The first Node in the linked list is the least-significant part of the integer. The first element of node "first" is the least significant part of the integer.
		list_of_int_arrays intarrays;
		many_bits num_of_intarrays_used; //number of "Node"s used inside of the list_of_int_arrays function
		bool is_negative; //By default the various constructors will set is_negative to false. Some functions (like SHA256 and the various bitwise operations) ignore is_negative.
		bool auto_destroy = true;
#if DEBUG_MODE > 0
//Debugging Functions
		bool find_inconsistencies() const;
		void print_properties() const;
		void compact_print() const;
		void print() const;
#endif
//Constructors
		//recieves an unlimited_int object to assign this, without copying, meaning that the original memory is used. The second parameter is bool and it decides whether to enable auto_destroy in this.
		//This entire function is basically a hack to gain access to a const unlimited_int
		explicit unlimited_int(const unlimited_int& num_to_assign, bool auto_destroy_this);
		//creates an unlimited_int from an array of uint32_t. Used heavily in the cryptographic secure random number generator, and specifically in the SHA-256 algorithm that hashes the value of an unlimited_int.
		explicit unlimited_int(uint32_t*, many_bits);
		//creates an unlimited_int from an array of uint64_t. Used heavily in the cryptographic secure random number generator, and specifically in the SHA-512 algorithm that hashes the value of an unlimited_int.
		explicit unlimited_int(uint64_t*, many_bits);
//Assign
		//used by unlimited_int::unlimited_int(uint32_t*, many_bits) but can also be used independently
		void assign(uint32_t*, many_bits);
		//used by unlimited_int::unlimited_int(uint64_t*, many_bits) but can also be used independently
		void assign(uint64_t*, many_bits);
		void assign(const few_bits value_to_assign);
		void assign(const many_bits value_to_assign);
		void assign(const few_bits_signed value_to_assign) { this->assign((few_bits)value_to_assign); this->is_negative = value_to_assign < 0; }
		void assign(const many_bits_signed value_to_assign) { this->assign((many_bits)value_to_assign); this->is_negative = value_to_assign < 0; }
		//sets the most immediate variables in unlimited_int to thier default values. Doesn't touch the variables in this->intarrays
		void set_to_zero()
		{
			this->num_of_intarrays_used = 0;
			this->is_negative = false;
			this->num_of_used_ints = 0;
		}
//Transfers
		//destoys this according while respecting this->auto_destroy. Then it "absorbs" other by "copying" it into this in O(1) time. Meanwhile, also causing other to reset to zero and forget its previous state.
		void absorb(unlimited_int& other);
		//creates independent copy of this's values inside of num_to_paste_into using the already existent memory inside num_to_paste_into and increasing the size if necessary.
		void copy_to(unlimited_int& num_to_paste_into) const;
		//sets all variables to their default values without deleting anything that is dynamically allocated and without flushing into the piggy bank.
		void forget_memory();
		//copies the most significant part of an unlimited_int into another unlimited_int. Specifically the first num_of_ints_to_copy "few_bits"s. It pastes into the least significant portion of other.
		//Other doesn't need to be prepared.
		void copy_most_significant_to(unlimited_int& other, const many_bits num_of_ints_to_copy) const;
//Compare
		//returns 'L', 'S' or 'E' based on the length of the numbers, and the negativity. Not accurate when both numbers are the same length and the same sign (except that when both numbers are 0 it is accurate).
		char estimate_compare_to(const unlimited_int& num_to_compare_to) const;
		//returns 'L', 'S' or 'E' based on the length of the numbers. Not accurate when both numbers are the same length.
		char estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const;
		//returns 'L' for larger, 'S' for smaller, 'E' for equal. This function is precise, and yet tries to increase efficiency by first using estimate_compare_to
		char compare_to(const unlimited_int& num_to_compare_to) const;
		//returns 'L' for larger, 'S' for smaller, 'E' for equal. This function is precise, and yet tries to increase efficiency by first using estimate_compare_to. Ignores minus/plus sign of this.
		char compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const;
//Fill 0
		//in case a number was big, and now only a tiny portion of it is used, the function flush_unused() can shrink the number to an appropriate size and put the unused "int_array"s into the piggy bank.
		//primes a number to be ready for filling-in its few_bits. If too many ints are requested to be zeroed, it stops at the end of intarrays.
		void fill_0_until_num_of_ints_and_set_used_ints_to_maximum(many_bits fill_0_until);
		//fills-in zeros in a number in the least-significant ints. If too many ints are requested to be zeroed, it stops at the end of intarrays
		void fill_0_until_num_of_ints(const many_bits num_of_ints_to_zero);
		//more efficient by using original memory
//Cutoff
		//adjusts the num_of_used_ints variables in int_array and in unlimited_int to accurately describe where a number begins
		void cutoff_leading_zeros(Node* int_array_to_start_at);
		void flush_unused()
		{
			many_bits difference = this->intarrays.intarrays.length - this->num_of_intarrays_used;
			if (difference > 0) { this->intarrays.flush_significant_to_piggy_bank(difference); }
		}
//Begin Iteration (helpers)
		//useful function for iteration. Gets the most significant Node* that is also used in the number.
		Node* get_most_significant_used_int_array() const;
		//starts from the most significant used few_bits and then goes down num_int_to_find "few_bits"s
		//returns a __list_location__ with nullptr if num_int_to_find is too big.
		__list_location__ find_num_used_int_from_significant(const many_bits num_int_to_find) const;
//Shift
		//uses shift_left and bit manipulation to shift number left by a certain number of bits.
		void shift_left_by_bits(const many_bits num_of_bits_to_shift_by);
		//uses shift_right and bit manipulation to shift number right by a certain number of bits.
		void shift_right_by_bits(const many_bits num_of_bits_to_shift_by);
		//shifts right by number of few_bits
		void shift_right(const many_bits shift_by);
		//shifts right by number of few_bits. Method is to prepend int_arrays to the number.
		void shift_left(const many_bits shift_by);
//Karatsuba
		//grade school algorithm for multiplication. I need to design and write a squaring basecase multiplication algorithm to use in function "unlimited_int* unlimited_int::power2() const" (for example) instead of using this function.
		void multiply_basecase(const unlimited_int* num_to_mult, unlimited_int* answer) const;
		//Karatsuba multiplication algorithm. Very efficient.
		unlimited_int* multiply_karatsuba(const unlimited_int* num_to_mult) const;
		unlimited_int* multiply_karatsuba_destroy_this_and_num_to_mult(unlimited_int* num_to_mult);
		//more efficient by using original memory
		unlimited_int* multiply_karatsuba_destroy_this(const unlimited_int* num_to_mult);
		//splits this into two different unlimited_ints, significant bits and insignificant bits. Based on many_bits that specifies the number of ints to put in unlimited_int low.
		void split_at(const many_bits index, unlimited_int* high, unlimited_int* low) const;
		//same as split_at except that it it's more efficient because it uses the original memory of this
		void split_at_and_use_original(const many_bits index, unlimited_int* high, unlimited_int* low);
//Multiplication By "few_bits"
		//grade school multiplication algorithm
		void multiply(const few_bits num_to_mult, unlimited_int* answer) const;
		//more efficient than multiply because it's specific-case, used by multiply.
		void multiply_both_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const;
		//more efficient than multiply because it's specific-case, used by multiply.
		void multiply_this_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const;
		//more efficient than multiply because it's specific-case, used by multiply.
		void multiply_answer_only_one_array(const few_bits num_to_mult, unlimited_int* answer) const;
//Division
		//Divides two numbers, ignores sign.
		//Splits the numerator so that its length is roughly equal to the denominator and then uses binary_search_divide.
		//It's the grade-school algorithm.
		std::shared_ptr<unlimited_int> divide_by(const unlimited_int& num_to_divide_by) const;
		//Accepts two unlimited_ints that the result of their division fits in "few_bits".
		//It uses binary search while continuously using compare_multiplication_to_num to get the comparisons for the binary search.
		few_bits binary_search_divide(const unlimited_int& num_to_divide_by) const;
		//function used by the long division binary search. Compares the result of multiplication of the multiplicand by the multiplier to the result target.
		//Returns 'L' if the multiplication result is greater than the result_target. Returns 'E' if it's equal or 'S' if it's smaller. Ignores sign of the parameters.
		static char compare_multiplication_to_num(const unlimited_int& multiplicand, const few_bits multiplier, const unlimited_int& result_target);
		//Used by divide_by
		//requires all functions to honor num_of_used_ints in every int_array. worst-case efficiency: O(n) where the coefficient n is MAX_ALLOC
		void push_to_insignificant(const few_bits num_to_push);
//Arithmetic (addition and subtraction)
		//adds using grade school algorithm.
		void add(const unlimited_int* num_to_add, unlimited_int* answer) const;
		//grade school subtraction algorithm.
		void subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const;
//Helpers
		//Uses known ASCII values. Base can be from 1 until 36 including. Works with both uppercase and lowercase English letters.
		static int char_to_number(const char ch, const int base = 10);
		//Uses known ASCII values. Base can be from 1 until 36 including. Creates lowercase letters.
		static char number_to_char(const int num, const int base);
		//get the number of consecutive bits that are zero in the most significant part of few_bits
		static int num_of_zero_bits_preceding_number(const few_bits);
		//Checks whether a number is an exact power of 2 and if it is, the result is returned.
		//Returns -1 if not exact power of 2. Expected efficiency: O(1) where n is the number of bits.
		many_bits_signed find_exact_log_2() const;
//Random
		//generates random number from the time in milliseconds and from the speed of the machine (the measured using time in milliseconds)
		//Cryptographically secure, needs improvement for speed, and for getting more truely random data at runtime.
		static std::shared_ptr<unlimited_int> generate_truly_random();
		//Starts with a seed from generate_truly_random() and then continues on, each time taking the sha256/sha512 hash (depending on x86/x64) and thereby creating a series of cryptographically secure random numbers.
		//Returns random number in the range of 0 until and including (2^256 - 1) or 0 until and including (2^512 - 1) with perfectly random distribution. 256 when compiling in x86 and 512 when compiling in x64.
		static std::shared_ptr<unlimited_int> generate_next_random();
		//stacks 256bit sha256 hashes or 512bit sha512 hashes until the number is long enough.
		static std::shared_ptr<unlimited_int> generate_random_that_is_at_least(many_bits num_of_bits);

//########################## End Of Protected #######################################

	public:

//Constructors
		unlimited_int() { this->forget_memory(); }
		explicit unlimited_int(const few_bits num_to_assign) { this->assign(num_to_assign); }
		explicit unlimited_int(const many_bits num_to_assign) { this->assign(num_to_assign); }
		explicit unlimited_int(const few_bits_signed num_to_assign);
		explicit unlimited_int(const many_bits_signed num_to_assign);
		//copy constructor- creates full independent copy.
		unlimited_int(const unlimited_int& num_to_assign) { num_to_assign.copy_to(*this); };
		//Assign Integer
		void operator=(const many_bits_signed num) { this->assign(num); }
//Transfers And Copies
		//just like copy() except that this is O(1) time because it uses the original to construct unlimited_int. this will be 0 after the function runs.
		unlimited_int* to_dynamic();
		//takes a shared_ptr and transfers it's content to an unlimited_int in O(1) time. It sets the auto_destroy tag of the shared_ptr to false and resets it to zero.
		unlimited_int(std::shared_ptr<unlimited_int>);
		//takes a shared_ptr and transfers its contents to an unlimited_int in O(1) time. It sets the auto_destroy tag of the shared_ptr to false and resets it to zero.
		void operator=(std::shared_ptr<unlimited_int>);
		//O(1) efficiency to swap two unlimited_int numbers. Also swaps the bool "auto_destroy" values
		void swap(unlimited_int& num_to_swap_with);
		//O(n) function. Returns pointer to independent clone of this unlimited_int. Correct syntax is:
		//unlimited_int original = 7879;
		//unlimited_int* clone_a = original.copy();
		//delete clone_a;
		unlimited_int* copy() const {
			unlimited_int* copy_ui = new unlimited_int;
			this->copy_to(*copy_ui);
			return copy_ui;
		}
		//creates full independent copy.
		void operator=(const unlimited_int& num_to_assign) { num_to_assign.copy_to(*this); }
//Information Gathering O(1)
		//converts unlimited_int to few_bits, but actually returns many_bits_signed so that it can fit the few_bits number and the sign as well.
		//This function works no matter the size of this, but it only works as a conversion when this unlimited_int is within of few_bits (meaning, it would have to be very small).
		many_bits_signed get_least_significant() const;
		//returns the length of the current number in bits (the used part). It gives the precise number, and manipulates the most significant few_bits using bitwise operators to get the precise length in bits of this unlimited_int.
		many_bits get_length_in_bits() const;
		//checks if unlimited_int's value is equal to 0, based on num_of_used_ints
		bool is_zero() const { return (this->num_of_used_ints == 0); }
		//super-efficient method to find modulo_2 of unlimited_int number.
		//This function returns either 1 or 0. Now, confusingly it's actually modulo and not remainder of division (different approach with negative numbers).
		int modulo_2() const
		{
			if (this->num_of_used_ints == 0) { return 0; }
			if (this->intarrays.intarrays.first->value->intarr[0] & 0x1) { return 1; }
			return 0;
		}
//Karatsuba multiplication
		//The interface through which to use Karatsuba multiplication algorithm.
		//multiplies using Karatsuba and tries to optimize multiplication process by checking if one of the multiplicands is a power of 2. Also deals with negative numbers.
		std::shared_ptr<unlimited_int> operator*(const unlimited_int&) const;
		//The interface through which to use Karatsuba multiplication algorithm.
		//multiplies using Karatsuba and tries to optimize multiplication process by checking if one of the multiplicands is a power of 2. Also deals with negative numbers.
		//Uses multiply_karatsuba_destroy_this for extra efficiency over the normal multiply_karatsuba.
		void operator*=(const unlimited_int&);
//Multiplication by few_bits
		friend std::shared_ptr<unlimited_int> operator*(const few_bits, const unlimited_int&);
		//A little bit more efficient than using the regular multiplication by few_bits function.
		void operator*=(const few_bits num) { this->multiply(num, this); }
		std::shared_ptr<unlimited_int> operator*(const few_bits num) const;
//Karatsuba power2
		//more efficient Karatsube algorithm specifically for squaring numbers
		//I need to integrate a basecase power2 algorithm for even more efficiency.
		std::shared_ptr<unlimited_int> power2() const;
		//even more efficient than unlmimited_int::power2 because it utilises the original memory
		//I need to integrate a basecase power2 algorithm for even more efficiency.
		std::shared_ptr<unlimited_int> power2_destroy_this();
//Long Division
		//Interface through which to use "unlimited_int* divide_by(const unlimited_int& num_to_divide_by) const" (which is quite inefficient, uses binary search). Also this function deals with the sign of the numbers.
		std::shared_ptr<unlimited_int> operator/(const unlimited_int& denominator) const;
		void operator/=(const unlimited_int& denominator) { (*this) = (*this) / denominator; }
//Remainder Of Division
		//Remainder (of division) operator. Not modulo. The sign of the result will always be the same as the left side of the operator.
		std::shared_ptr<unlimited_int> operator%(const unlimited_int& ui) const;
		//Remainder functions- remainder of division. NOT MUDULO (difference in treatment of negative values). Sign this won't change.
		void operator%=(const unlimited_int& ui);
		//interface to access multiply functions. It multiplies an unlimited_int by few_bits. Efficiency: O(n) where n is the number of digits in the unlimited_int number.
//Arithmetic (addition and subtraction)
		//Slightly more efficient than operator+
		void operator+=(const unlimited_int& other) { this->add(&other, this); }
		//Adds two unlimited_int integers- respects sign
		std::shared_ptr<unlimited_int> operator+(const unlimited_int& other) const;
		void operator-=(const unlimited_int& other) { this->subtract(&other, this); }
		std::shared_ptr<unlimited_int> operator-(const unlimited_int& other) const;
//Onary Operations
		//expected Efficiency O(1)
		void operator++();
		//expected Efficiency O(1)
		void operator--();
		//Creates full copy with opposite sign, unless number is zero because then the sign stays positive.
		std::shared_ptr<unlimited_int> operator-() const;
		//O(1) operation to flip a numbers sign from positive to negative or from negative to positive. Doesn't do anything when the number is 0.
		void flip_sign() { if (!this->is_zero()) this->is_negative = !this->is_negative; }
		//Creates full copy with positive, unless number is zero because then the sign stays positive.
		std::shared_ptr<unlimited_int> abs() const;
		//O(1) operation to flip a number's sign to positive.
		void self_abs() { this->is_negative = false; }
		//O(1) operation to flip a number's sign to negative. Doesn't do anything when the number is 0.
		void make_negative() { if (!this->is_zero()) this->is_negative = false; }
//Comparison Operators
		bool operator<=(const unlimited_int& other) const { char value = this->compare_to(other); return ((value == 'S') || (value == 'E')); }
		bool operator>=(const unlimited_int& other) const { char value = this->compare_to(other); return ((value == 'L') || (value == 'E')); }
		bool operator==(const unlimited_int& other) const { char value = this->compare_to(other); return (value == 'E'); }
		bool operator<(const unlimited_int& other) const { char value = this->compare_to(other); return (value == 'S'); }
		bool operator>(const unlimited_int& other) const { char value = this->compare_to(other); return (value == 'L'); }
		bool operator!=(const unlimited_int& ui) const { return (this->compare_to(ui) != 'E'); }
//Bitwise Operators
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
//String Conversions
		//from base 1 until base 36 (including). Can recognize prepended minus symbol (-). Supports both uppercase and lowercase. Receives null-terminated string.
		static std::shared_ptr<unlimited_int> from_string(const char* str, const int base = 10);
		//uses "static unlimited_int* from_string(const char* str, const int base = 10)"
		static std::shared_ptr<unlimited_int> from_string(const std::string& str, const int base = 10) { return unlimited_int::from_string(str.c_str(), base); }
		//from base 1 until base 36 (including). Uses lowercase English letters. Returns null-terminated string.
		char* to_c_string(const int base = 16) const;
		//from base 1 until base 36 (including). Uses lowercase English letters.
		std::string to_string(const int base = 16) const;
//Power (^)
		//efficient method for power (math function) with remainder as well. Receives boolean pointer used for early termination of calculation.
		static std::shared_ptr<unlimited_int> pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& mod
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
								, bool* terminator = nullptr
#endif
								);
		//efficient method for power (math function)
		static std::shared_ptr<unlimited_int> pow(const unlimited_int& base, const unlimited_int& power);
//Random
		//Uses generate_random_that_is_at_least and then the modulo operator to generate a random number with perfectly random distribution in the given range.
		//Cryptographically secure.
		static std::shared_ptr<unlimited_int> generate_random(const unlimited_int& min, const unlimited_int& max);
//Hash
		//Calculates the sha-256 hash of an unlimited_int. Splits number into chunks of 32-bit integers, whether or not the program is in 64-bit mode.
		//The hash function produces the same output whether this library is in 32bit mode or in 64bit mode.
		//It directly iterates through the unlimited_int so there's no RAM risk of hashing huge unlimited_int numbers.
		std::shared_ptr<unlimited_int> calculate_sha256_hash() const;
		//Calculates the sha-512 hash of an unlimited_int. Splits number into chunks of 64-bit integers, whether or not the program is in 64-bit mode.
		//The hash function produces the same output whether this library is in 32bit mode or in 64bit mode.
		//It directly iterates through the unlimited_int so there's no RAM risk of hashing huge unlimited_int numbers.
		std::shared_ptr<unlimited_int> calculate_sha512_hash() const;
//Prime
		//Miller-Rabin primality test: 1/2^128 chance of mistake, no "special cases" like 561 that satisfies Fermat test.
		//Ignores that sign of a number: -2 is prime. Receives a pointer to a boolean that tells it to stop early.
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		bool is_prime(bool* terminator = nullptr) const;
#else
		bool is_prime() const;
#endif
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		//Receives optional argument: bool* terminator that when true, the function will terminate itself a.s.a.p and return unlimited_int that's equal to 0
		//Receives optional argument: int num_threads that specified how many threads are to be run concurrently to try to find the prime number.
		//If int num_threads is not specified then the number of threads used will be equal to the number of cores (logical processors) on the system.
		static std::shared_ptr<unlimited_int> generate_random_prime(const unlimited_int& min, const unlimited_int& max, bool* terminator = nullptr, int num_threads = -1);
#else
		static std::shared_ptr<unlimited_int> generate_random_prime(const unlimited_int& min, const unlimited_int& max);
#endif
//Abstract Math
		//greatest common divisor- treats negative numbers as positive
		static std::shared_ptr<unlimited_int> gcd(const unlimited_int& a, const unlimited_int& b);
		//lowest common multiple- treats negative numbers as positive
		static std::shared_ptr<unlimited_int> lcm(const unlimited_int& a, const unlimited_int& b);
//Getters And Setters- There's no reason to need to use these functions
		bool get_is_negative() const { return this->is_negative; }
		bool get_auto_destroy() const { return this->auto_destroy; }
		list_of_int_arrays& get_intarrays() { return this->intarrays; }

		void set_auto_destroy(bool tof) { this->auto_destroy = tof; }
		void set_num_of_intarrays_used(many_bits set_to) { this->num_of_intarrays_used = set_to; }
//Manual Destructors
		//Manual method to destroy the dynamically allocated memory that's used by this object
		//The variables (pointers etc.) are then reset for future use.
		//No need to call this function, because the destructor will do its job automatically.
		void destroy();
		//Manual method to flush to piggy bank, but if there's no room left in the piggy bank because of PIGGY_BANK_MAXIMUM then delete[] operator is automatically utilized.
		//The variables (pointers etc.) are then reset for future use.
		//No need to call this function, because the destructor will do its job automatically.
		void flush();
		//Resets the random number generator's chain. If the random number function is called again after calling this function, is will still work.
		//No need to call this function, because the destructor of the static variable will do its job automatically
		static void flush_current_random();
//Automatic Destructor
		//respects this->auto_destroy tag.
		~unlimited_int() { if (this->auto_destroy) { this->flush(); this->auto_destroy = false; } }
	};
	//respects flags: std::ios::dec, std::ios::oct, std::ios::hex.
	//Super slow when printing in a base that's not a power of 2, because the division algorithm is still binary search (really slow).
	//Really fast when printing in base 8, 16.
	std::ostream& operator<<(std::ostream& os, const unlimited_int& ui);
	//Optimized method for multiplying unlimited_int by few_bits
	std::shared_ptr<unlimited_int> operator*(const few_bits, const unlimited_int&);
}
#endif
