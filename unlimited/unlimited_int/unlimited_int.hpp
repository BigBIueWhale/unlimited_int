#ifndef UNLIMITED_UNLIMITED_INT_H
#define UNLIMITED_UNLIMITED_INT_H
#include "structures/list_of_int_arrays.hpp"
#include <string>
#include <ostream>
#include <unordered_map>
#include <vector>
#include <memory>
namespace unlimited
{
	class unlimited_int;
	struct reciprocal_information
	{
	public:
		std::unique_ptr<unlimited_int> reciprocal; //the reciprocal_itself
		size_t amount_shifted; //the amount shifted left during the reciprocal calculation
		reciprocal_information(std::unique_ptr<unlimited_int>&& reciprocal, size_t amount_shifted);
		reciprocal_information(reciprocal_information&&) noexcept; //move constructor
		reciprocal_information() : amount_shifted((size_t)0) {}
	};
	struct reciprocal_information_for_database : public reciprocal_information
	{
	public:
		std::unique_ptr<unlimited_int> hash_of_dividend;
		//Only my custom doubly-linked list is guaranteed to support keeping a valid pointer to a node in the list without that pointer becoming invalidated after changing the list
		custom_linked_list_node<size_t>* link_to_list;
		reciprocal_information_for_database(reciprocal_information&&, std::unique_ptr<unlimited_int>&&, custom_linked_list_node<size_t>*);
		reciprocal_information_for_database(reciprocal_information_for_database&&) noexcept; //move constructor
		reciprocal_information_for_database() : link_to_list(nullptr) {}
	};
	struct reciprocals_database
	{
	public:
		std::unordered_map<size_t, reciprocal_information_for_database> reciprocals_map;
		//I need to do things with this list that aren't guaranteed to work with std::list, like for example: save a pointer to a node for later, without that pointer / iterator becoming invalidated.
		//We don't want to be dependent on the implementation of our specific compiler's standard library.
		custom_linked_list<size_t> most_recent; //fingerprints of the most recent.
		//clears the saved reciprocals in the database, they'll be destoyed anyways by the destructor
		void clear();
	};
	class unlimited_int
	{
	private:
		//Defined in unlimited/unlimited.cpp
		//Holds the current random number in the chain of random numbers. The chain is generated using the SHA256 or SHA512 hash function (depending on whether compiler is 64bits or 32bits).
		//thread_local insures that each thread generates its own version of current_random (otherwise corruption will occur, an std::mutex will slow down the program).
		static thread_local unlimited_int current_random;
		static thread_local reciprocals_database Newton_Raphson_lookup;
	protected:
		static std::vector<few_bits> small_prime_numbers;
//Member Variables
		size_t num_of_used_ints; //number of used ints int the entire list_of_int_arrays
		//A linked list of few_bits arrays. The unlimited_int is represented as follows:
		//The first Node in the linked list is the least-significant part of the integer. The first element of node "first" is the least significant part of the integer.
		list_of_int_arrays* intarrays = nullptr;
		size_t num_of_intarrays_used; //number of "Node"s used inside of the list_of_int_arrays function
		bool _is_negative; //By default the various constructors will set is_negative to false. Some functions (like SHA256 and the various bitwise operations) ignore is_negative.
		bool auto_destroy = true;
#if UNLIMITED_INT_LIBRARY_DEBUG_MODE > 0
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
		explicit unlimited_int(const uint32_t *const arr, const size_t len) { this->assign(arr, len); }
		//creates an unlimited_int from an array of uint64_t. Used heavily in the cryptographic secure random number generator, and specifically in the SHA-512 algorithm that hashes the value of an unlimited_int.
		explicit unlimited_int(const uint64_t* const arr, const size_t len) { this->assign(arr, len); };
//Assign
		//used by unlimited_int::unlimited_int(uint32_t*, size_t) but can also be used independently
		void assign(const uint32_t *const, const size_t);
		//used by unlimited_int::unlimited_int(uint64_t*, size_t) but can also be used independently
		void assign(const uint64_t *const, const size_t);
		void assign(const few_bits value_to_assign);
		void assign(const many_bits value_to_assign);
		void assign(const few_bits_signed value_to_assign);
		void assign(const many_bits_signed value_to_assign);
		//sets the most immediate variables in unlimited_int to thier default values. Doesn't touch the variables in this->intarrays
		void set_to_zero()
		{
			this->num_of_intarrays_used = (size_t)0;
			this->_is_negative = false;
			this->num_of_used_ints = (size_t)0;
		}
//Transfers
		//creates independent copy of this's values inside of num_to_paste_into using the already existent memory inside num_to_paste_into and increasing the size if necessary.
		void copy_to(unlimited_int& num_to_paste_into) const;
		//sets all variables to their default values without deleting anything that is dynamically allocated and without flushing into the piggy bank.
		void forget_memory();
		//copies the most significant part of an unlimited_int into another unlimited_int. Specifically the first num_of_ints_to_copy "few_bits"s. It pastes into the least significant portion of other.
		//Other doesn't need to be prepared. It causes other to be non-negative
		void copy_most_significant_to(unlimited_int& other, const size_t num_of_ints_to_copy) const;
//Compare
		//returns 'L', 'S' or 'E' based on the length of the numbers, and the negativity. Not accurate when both numbers are the same length and the same sign (except that when both numbers are 0 it is accurate).
		char estimate_compare_to(const unlimited_int& num_to_compare_to) const;
		//returns 'L', 'S' or 'E' based on the length of the numbers. Not accurate when both numbers are the same length.
		char estimate_compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const;
		//returns 'L' for larger, 'S' for smaller, 'E' for equal. This function is precise, and yet tries to increase efficiency by first using estimate_compare_to
		char compare_to(const unlimited_int& num_to_compare_to) const;
		//returns 'L' for larger, 'S' for smaller, 'E' for equal. This function is precise, and yet tries to increase efficiency by first using estimate_compare_to. Ignores minus/plus sign of this.
		char compare_to_ignore_sign(const unlimited_int& num_to_compare_to) const;
		char compare_to_ignore_sign(const few_bits other_num) const;
		char compare_to_ignore_sign(const many_bits other_num) const;
		//Respectes sign of number. Returns 'S', 'E' or 'L'. That stands for smaller, equal, larger
		char compare_to(const few_bits other_num) const;
		//Respectes sign of number. Returns 'S', 'E' or 'L'. That stands for smaller, equal, larger
		char compare_to(const many_bits other_num) const;
		//Respectes sign of both numbers. Returns 'S', 'E' or 'L'. That stands for smaller, equal, larger
		char compare_to(const few_bits_signed other_num) const;
		//Respectes sign of both numbers. Returns 'S', 'E' or 'L'. That stands for smaller, equal, larger
		char compare_to(const many_bits_signed other_num) const;
//Fill 0
		//Primes a number to be ready for filling-in its few_bits. The number must already have a intarrays that isn't nullptr.
		//If too many ints are requested to be zeroed, it throws an exception.
		//This function has no regard for all the various "used" specifiers,
		//instead it sets the num_of_used_ints in each int_array that it reaches, to intarr_len.
		void fill_0_until_num_of_ints_and_set_used_ints_to_maximum(size_t fill_0_until);
		//Primes a number to be ready for filling-in its few_bits. The number must already have a intarrays that isn't nullptr.
		//If too many ints are requested to be zeroed, it throws an exception.
		//This function has no regard for all the various "used" specifiers.
		void fill_0_until_num_of_ints(const size_t num_of_ints_to_zero);
		//more efficient by using original memory
//Cutoff
		//adjusts the num_of_used_ints variables in int_array and in unlimited_int to accurately describe where a number begins
		void cutoff_leading_zeros(custom_linked_list_node<int_array> *const int_array_to_start_at);
		//in case a number was big, and now only a tiny portion of it is used (for example),
		//the function flush_unused() can shrink the number to an appropriate size and put the unused "int_array"s into the piggy bank.
		void flush_unused()
		{
			const size_t difference = this->intarrays->size() - this->num_of_intarrays_used;
			if (difference > (size_t)0)
				this->intarrays->flush_significant_to_piggy_bank(difference);
		}
//Begin Iteration (helpers)
		//Useful function for iteration. Gets the most significant Node* that is also used.
		//This function decides whether an int_array is "used" solely based on this->num_of_intarrays_used
		custom_linked_list_node<int_array>* get_most_significant_used_int_array() const;
		//starts from the most significant used few_bits and then goes down num_int_to_find "few_bits"s
		//returns a __list_location__ with nullptr if num_int_to_find is too big.
		int_array_list::list_location find_num_used_int_from_significant(const size_t num_int_to_find) const;
//Shift
		//uses shift_left and bit manipulation to shift number left by a certain number of bits.
		void shift_left_by_bits(const size_t num_of_bits_to_shift_by);
		//uses shift_right and bit manipulation to shift number right by a certain number of bits.
		void shift_right_by_bits(const size_t num_of_bits_to_shift_by);
		//shifts right by number of few_bits. Keeps sign of number intact (a negative number will stay negative, unless it becomes 0).
		void shift_right(const size_t shift_by);
		//shifts right by number of few_bits. Method is to prepend int_arrays to the number.  Keeps sign of number intact (a negative number will stay negative, unless it becomes 0).
		void shift_left(const size_t shift_by);
//Karatsuba
		//grade school algorithm for multiplication. I need to design and write a squaring basecase multiplication algorithm to use in function "unlimited_int unlimited_int::power2() const" (for example) instead of using this function.
		void multiply_basecase(const unlimited_int* num_to_mult, unlimited_int* answer) const;
		//Karatsuba multiplication algorithm. Very efficient.
		unlimited_int multiply_karatsuba(const unlimited_int* num_to_mult) const;
		unlimited_int multiply_karatsuba_destroy_this_and_num_to_mult(unlimited_int* num_to_mult);
		//more efficient by using original memory
		unlimited_int multiply_karatsuba_destroy_this(const unlimited_int* num_to_mult);
		//splits this into two different unlimited_ints, significant bits and insignificant bits. Based on size_t that specifies the number of ints to put in unlimited_int low.
		void split_at(const size_t index, unlimited_int* high, unlimited_int* low) const;
		//same as split_at except that it it's more efficient because it uses the original memory of this
		void split_at_and_use_original(const size_t index, unlimited_int* high, unlimited_int* low);
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
		//Ignores sign, returns non-negative value.
		unlimited_int divide_by(const unlimited_int& num_to_divide_by) const;
		//Divides two numbers, ignores sign.
		//Fastest division method for small numbers
		//It's the grade-school algorithm.
		//Ignores sign, returns non-negative value.
		unlimited_int divide_by(const few_bits num_to_divide_by) const;
		unlimited_int divide_by_respect_sign(const few_bits num_to_divide_by) const;
		//Faster than divide_by for values up to (and including) 16 few_bits
		//Ignores sign, returns non-negative value.
		unlimited_int divide_by_repeated_addition(const unlimited_int& num_to_divide_by) const;
		//Accepts two unlimited_ints that the result of their division fits in "few_bits".
		//It uses binary search while continuously using compare_multiplication_to_num to get the comparisons for the binary search.
		few_bits binary_search_divide(const unlimited_int& num_to_divide_by) const;
		few_bits binary_search_divide(const few_bits num_to_divide_by) const;
		//function used by the long division binary search. Compares the result of multiplication of the multiplicand by the multiplier to the result target.
		//Returns 'L' if the multiplication result is greater than the result_target. Returns 'E' if it's equal or 'S' if it's smaller. Ignores sign of the parameters.
		static char compare_multiplication_to_num(const unlimited_int& multiplicand, const few_bits multiplier, const unlimited_int& result_target);
		//Used by divide_by
		//requires all functions to honor num_of_used_ints in every int_array. worst-case efficiency: O(n) where the coefficient n is MAX_ALLOC
		void push_to_insignificant(const few_bits num_to_push);
//Remainder of division
		void operator%=(const few_bits divisor);
		unlimited_int operator%(const few_bits divisor) const;
//Arithmetic (addition and subtraction)
		//adds using grade school algorithm.
		void add(const unlimited_int* num_to_add, unlimited_int* answer) const;
		//grade school subtraction algorithm.
		void subtract(const unlimited_int* num_to_subtract, unlimited_int* answer) const;
//Helpers
		//Uses known ASCII values. Base can be from 1 until 36 including. Works with both uppercase and lowercase English letters.
		static unsigned char_to_number(const char ch, const unsigned base = 10U);
		//Uses known ASCII values. Base can be from 1 until 36 including. Creates lowercase letters.
		static char number_to_char(const unsigned int num, const unsigned int base);
		//get the number of consecutive bits that are zero in the most significant part of few_bits
		static int num_of_zero_bits_preceding_number(const few_bits);
		static int num_of_zero_bits_succeeding_number(const few_bits);
		static int find_exact_log_2(few_bits num, bool* const is_power_2);
		//Checks whether a number is an exact power of 2 and if it is, the result is returned.
		//Receives bool* of is_power_2 and if the number isn't an exact power of 2, that bool is set to false.
		//Expected efficiency: O(1) where n is the number of bits.
		//Unlike other bitwise operators, this one works on negative unlimited_ints, and simply ignores the sign.
		size_t find_exact_log_2(bool *const is_power_2) const;
//Random
		//generates random number from the time in milliseconds and from the speed of the machine (the measured using time in milliseconds)
		//Cryptographically secure, needs improvement for speed, and for getting more truely random data at runtime.
		static unlimited_int generate_truly_random();
		//Starts with a seed from generate_truly_random() and then continues on, each time taking the sha256/sha512 hash (depending on x86/x64) and thereby creating a series of cryptographically secure random numbers.
		//Returns random number in the range of 0 until and including (2^256 - 1) or 0 until and including (2^512 - 1) with perfectly random distribution. 256 when compiling in x86 and 512 when compiling in x64.
		static unlimited_int generate_next_random();
		//stacks 256bit sha256 hashes or 512bit sha512 hashes until the number is long enough.
		static unlimited_int generate_random_that_is_at_least(const size_t num_of_bits);
//Memory allocation
		void increase_until_num_of_ints(size_t num_of_ints_to_increase_until)
		{
			if (this->intarrays == nullptr)
				this->intarrays = new list_of_int_arrays;
			this->intarrays->increase_until_num_of_ints(num_of_ints_to_increase_until);
		}
//Conversions
		bool fits_in_few_bits() const { return this->num_of_used_ints <= (size_t)1 && !this->is_negative(); }
		bool fits_in_many_bits() const { return this->num_of_used_ints <= (size_t)2 && !this->is_negative(); }
		//Throws std::overflow_error exception if the unlimited_int number can't be fully translated into few_bits (too big, or negative).
		few_bits to_few_bits() const;
		//Throws std::overflow_error exception if the unlimited_int number can't be fully translated into few_bits_signed (too big, or too negative).
		//This function has a guarantee that if the unlimited_int object is in range of the macros: MIN_few_bits_signed_NUM and MAX_few_bits_signed_NUM then it will succeed.
		few_bits_signed to_few_bits_signed() const;
		//Throws std::overflow_error exception if the unlimited_int number can't be fully translated into many_bits (too big, or negative).
		many_bits to_many_bits() const;
		//Throws std::overflow_error exception if the unlimited_int number can't be fully translated into many_bits_signed (too big, or too negative).
		//This function has a guarantee that if the unlimited_int object is in range of the macros: MIN_many_bits_signed_NUM and MAX_many_bits_signed_NUM then it will succeed.
		many_bits_signed to_many_bits_signed() const;
		//converts unlimited_int to few_bits (ignoring the sign of the unlimited_int object)
		//This function works no matter the size of this. It returns 0 of this is 0. It can work as a conversion
		//to few_bits when this unlimited_int is within the range of few_bits (meaning, it would have to be very small).
		few_bits get_least_significant_few_bits() const;
		//Just like get_least_significant_few_bits() except that it returns a many_bits, and therefore bases on the last 2 least significant few_bits.
		many_bits get_least_significant_many_bits() const;
//########################## End of Protected #######################################

	public:

//Constructors
		unlimited_int() { this->forget_memory(); }
		explicit unlimited_int(const signed short int num_to_assign);
		explicit unlimited_int(const signed int num_to_assign);
		explicit unlimited_int(const signed long int num_to_assign);
		explicit unlimited_int(const signed long long int num_to_assign);
		explicit unlimited_int(const unsigned short int num_to_assign);
		explicit unlimited_int(const unsigned int num_to_assign);
		explicit unlimited_int(const unsigned long int num_to_assign);
		explicit unlimited_int(const unsigned long long int num_to_assign);
		//The same thing as static unlimited_int::from_string()
		explicit unlimited_int(const std::string& str_num, unsigned base = 10U) : unlimited_int(unlimited_int::from_string(str_num, base)) {}
		//The same thing as static unlimited_int::from_string()
		explicit unlimited_int(const char* str_num, unsigned base = 10U) : unlimited_int(unlimited_int::from_string(str_num, base)) {}
		//copy constructor- creates full independent copy.
		unlimited_int(const unlimited_int& num_to_assign) { num_to_assign.copy_to(*this); }
		//move constructor- transfers the content of an unlimited_int prvalue, into "this"
		unlimited_int(unlimited_int&& source) noexcept;
//Assign
		//Use static unlimited_int::from_string instead, to specify the base.
		void operator=(const std::string& str_num) { (*this) = unlimited_int::from_string(str_num); }
		void operator=(const signed short int num_to_assign) { *this = unlimited_int(num_to_assign); }
		void operator=(const signed int num_to_assign) { *this = unlimited_int(num_to_assign); }
		void operator=(const signed long int num_to_assign) { *this = unlimited_int(num_to_assign); }
		void operator=(const signed long long int num_to_assign) { *this = unlimited_int(num_to_assign); }
		void operator=(const unsigned short int num_to_assign) { *this = unlimited_int(num_to_assign); }
		void operator=(const unsigned int num_to_assign) { *this = unlimited_int(num_to_assign); }
		void operator=(const unsigned long int num_to_assign) { *this = unlimited_int(num_to_assign); }
		void operator=(const unsigned long long int num_to_assign) { *this = unlimited_int(num_to_assign); }
//Transfers And Copies
		//Move assignment- transfers the content of an unlimited_int prvalue, into "this"
		void operator=(unlimited_int&& source) noexcept;
		//O(1) efficiency to swap two unlimited_int numbers. Also swaps the bool "auto_destroy" values
		void swap(unlimited_int& num_to_swap_with);
		//creates full independent copy.
		void operator=(const unlimited_int& num_to_assign) { num_to_assign.copy_to(*this); }
//Conversions
		explicit operator signed short int() const;
		explicit operator signed int() const;
		explicit operator signed long int() const;
		explicit operator signed long long int() const;
		explicit operator unsigned short int() const;
		explicit operator unsigned int() const;
		explicit operator unsigned long int() const;
		explicit operator unsigned long long int() const;
		//Better to use the actual unlimited_int::is_zero() function.
		explicit operator bool() const { return !this->is_zero(); }
		//Batter to use the actual unlimited_int::to_string function to specify the base.
		explicit operator std::string() const { return this->to_string(); }
//Information Gathering O(1)
		//Returns the length in bits (the used part).
		//It gives the precise number, and manipulates a copy of the most significant few_bits using bitwise operators to get the precise length in bits
		size_t get_length_in_bits() const;
		//checks if unlimited_int's value is equal to 0, based on num_of_used_ints
		bool is_zero() const { return this->num_of_used_ints == (size_t)0; }
		//super-efficient method to find modulo_2 of unlimited_int number.
		//This function returns either 1 or 0.
		//Confusingly it's actually modulo and not remainder of division (different approach with negative numbers).
		unsigned short int modulo_2() const { return static_cast<unsigned short int>(this->get_least_significant_few_bits() & (few_bits)1); }
//Karatsuba multiplication
		//The interface through which to use Karatsuba multiplication algorithm.
		//multiplies using Karatsuba and tries to optimize multiplication process by checking if one of the multiplicands is a power of 2. Also deals with negative numbers.
		unlimited_int operator*(const unlimited_int&) const;
		//The interface through which to use Karatsuba multiplication algorithm.
		//multiplies using Karatsuba and tries to optimize multiplication process by checking if one of the multiplicands is a power of 2. Also deals with negative numbers.
		//Uses multiply_karatsuba_destroy_this for extra efficiency over the normal multiply_karatsuba.
		void operator*=(const unlimited_int&);
//Karatsuba power2
		//more efficient Karatsube algorithm specifically for squaring numbers
		//I need to integrate a basecase power2 algorithm for even more efficiency.
		unlimited_int power2() const;
		//even more efficient than unlmimited_int::power2 because it utilises the original memory
		//I need to integrate a basecase power2 algorithm for even more efficiency.
		unlimited_int power2_destroy_this();
//Long Division
		//Interface through which to use "unlimited_int* divide_by(const unlimited_int& num_to_divide_by) const" when the number is big, and
		//"unlimited_int* divide_by(const unlimited_int& num_to_divide_by) const"
		unlimited_int operator/(const unlimited_int& denominator) const;
		void operator/=(const unlimited_int& denominator) { (*this) = (*this) / denominator; }
//Newton Raphson
		//Newton-Raphson method (kind of like binary search but faster) to find the reciprocal of an integer.
		//Received an integer telling it how precise the reciprocal needs to be calculated.
		//The precision is measured by how long a dividend can be (measured by number of few_bits) and still use the reciprocal to get an accurate division.
		//For example: if you're gonna use dividends with a maximum size of 90 few_bits then send this function 90
		//and it will automatically discern how much the reciprocal needs to be shifted. The amount shifted is returned as part of the reciprocal_information struct.
		//The reciprocal is will always be positive, and floored.
		reciprocal_information calculate_reciprocal_floor(const size_t length_dividend_to_support) const;
		//reciprocal must be with enough precision and also be positive (even if the dividend is negative). Otherwise an exception is thrown.
		//Returns the division result, and puts the remainder in "remainder" if it's given.
		//Calculates the correct sign for the results
		static unlimited_int divide_using_reciprocal(const unlimited_int& dividend, const reciprocal_information& reciprocal, const unlimited_int& divisor, unlimited_int* remainder = nullptr);
		//divides using Newton Raphson method and saves the reciprocal for later. More efficient when dividing by the same divisor again and again.
		//returns the division result, and puts the remainder in "remainder" if it's given as an unlimited_int*
		//Returns the correct sign of the results of division and remainder. Remember that remainder will always have the sign of the dividend, because we're talking about remainder, not modulo.
		static unlimited_int recurring_division(const unlimited_int& dividend, const unlimited_int& divisor, unlimited_int* remainder = nullptr);
//Remainder Of Division
		//Remainder (of division) operator. Not modulo. The sign of the result will always be the same as the left side of the operator.
		unlimited_int operator%(const unlimited_int& ui) const;
		//Remainder functions- remainder of division. NOT MUDULO (difference in treatment of negative values). Sign this won't change.
		void operator%=(const unlimited_int& ui);
		//Use this only when doing repeated division. Equivalend to operator%
		static unlimited_int remainder_recurring_divison(const unlimited_int& dividend, const unlimited_int& divisor);
//Arithmetic (addition and subtraction)
		//Slightly more efficient than operator+
		void operator+=(const unlimited_int& other) { this->add(&other, this); }
		//Adds two unlimited_int integers- respects sign
		unlimited_int operator+(const unlimited_int& other) const;
		void operator-=(const unlimited_int& other) { this->subtract(&other, this); }
		unlimited_int operator-(const unlimited_int& other) const;
//Unary Operations

		//Note about pre-increment and post-increment functions:
		//I decided that they should work best for this situation:
		//for (unlimited_int i(0); i < 2000; i++) {}
		//So that's why they all return void

		//Pre increment- use this function instead of post increment
		void operator++();
		//Pre decrement- use this function instead of post decrement
		void operator--();
		//Post increment- same as pre-increment
		void operator++(int);
		//Post decrement- same as pre-decrement
		void operator--(int);
		//Creates full copy with opposite sign, unless number is zero because then the sign stays positive.
		unlimited_int operator-() const;
		//O(1) operation to flip a numbers sign from positive to negative or from negative to positive. Doesn't do anything when the number is 0.
		void flip_sign() { if (!this->is_zero()) this->_is_negative = !this->_is_negative; }
		//Creates full copy with positive, unless number is zero because then the sign stays positive.
		unlimited_int abs() const;
		//O(1) operation to flip a number's sign to positive.
		void self_abs() { this->_is_negative = false; }
		//O(1) operation to flip a number's sign to negative. Doesn't do anything when the number is 0.
		void self_negative() { if (!this->is_zero()) this->_is_negative = true; }
//Comparison Operators
		bool operator<=(const unlimited_int& other) const { const char value = this->compare_to(other); return value == 'S' || value == 'E'; }
		bool operator>=(const unlimited_int& other) const { const char value = this->compare_to(other); return value == 'L' || value == 'E'; }
		bool operator==(const unlimited_int& other) const { const char value = this->compare_to(other); return value == 'E'; }
		bool operator<(const unlimited_int& other) const { const char value = this->compare_to(other); return value == 'S'; }
		bool operator>(const unlimited_int& other) const { const char value = this->compare_to(other); return value == 'L'; }
		bool operator!=(const unlimited_int& ui) const { return (this->compare_to(ui) != 'E'); }

		bool operator<=(const signed short int num) const;
		bool operator>=(const signed short int num) const;
		bool operator<(const signed short int num) const;
		bool operator>(const signed short int num) const;
		bool operator==(const signed short int num) const;
		bool operator!=(const signed short int num) const;
		friend bool operator<=(const signed short int num, const unlimited_int& ui);
		friend bool operator>=(const signed short int num, const unlimited_int& ui);
		friend bool operator<(const signed short int num, const unlimited_int& ui);
		friend bool operator>(const signed short int num, const unlimited_int& ui);
		friend bool operator==(const signed short int num, const unlimited_int& ui);
		friend bool operator!=(const signed short int num, const unlimited_int& ui);

		bool operator<=(const signed int num) const;
		bool operator>=(const signed int num) const;
		bool operator<(const signed int num) const;
		bool operator>(const signed int num) const;
		bool operator==(const signed int num) const;
		bool operator!=(const signed int num) const;
		friend bool operator<=(const signed int num, const unlimited_int& ui);
		friend bool operator>=(const signed int num, const unlimited_int& ui);
		friend bool operator<(const signed int num, const unlimited_int& ui);
		friend bool operator>(const signed int num, const unlimited_int& ui);
		friend bool operator==(const signed int num, const unlimited_int& ui);
		friend bool operator!=(const signed int num, const unlimited_int& ui);

		bool operator<=(const signed long int num) const;
		bool operator>=(const signed long int num) const;
		bool operator<(const signed long int num) const;
		bool operator>(const signed long int num) const;
		bool operator==(const signed long int num) const;
		bool operator!=(const signed long int num) const;
		friend bool operator<=(const signed long int num, const unlimited_int& ui);
		friend bool operator>=(const signed long int num, const unlimited_int& ui);
		friend bool operator<(const signed long int num, const unlimited_int& ui);
		friend bool operator>(const signed long int num, const unlimited_int& ui);
		friend bool operator==(const signed long int num, const unlimited_int& ui);
		friend bool operator!=(const signed long int num, const unlimited_int& ui);

		bool operator<=(const signed long long int num) const;
		bool operator>=(const signed long long int num) const;
		bool operator<(const signed long long int num) const;
		bool operator>(const signed long long int num) const;
		bool operator==(const signed long long int num) const;
		bool operator!=(const signed long long int num) const;
		friend bool operator<=(const signed long long int num, const unlimited_int& ui);
		friend bool operator>=(const signed long long int num, const unlimited_int& ui);
		friend bool operator<(const signed long long int num, const unlimited_int& ui);
		friend bool operator>(const signed long long int num, const unlimited_int& ui);
		friend bool operator==(const signed long long int num, const unlimited_int& ui);
		friend bool operator!=(const signed long long int num, const unlimited_int& ui);

		bool operator<=(const unsigned short int num) const;
		bool operator>=(const unsigned short int num) const;
		bool operator<(const unsigned short int num) const;
		bool operator>(const unsigned short int num) const;
		bool operator==(const unsigned short int num) const;
		bool operator!=(const unsigned short int num) const;
		friend bool operator<=(const unsigned short int num, const unlimited_int& ui);
		friend bool operator>=(const unsigned short int num, const unlimited_int& ui);
		friend bool operator<(const unsigned short int num, const unlimited_int& ui);
		friend bool operator>(const unsigned short int num, const unlimited_int& ui);
		friend bool operator==(const unsigned short int num, const unlimited_int& ui);
		friend bool operator!=(const unsigned short int num, const unlimited_int& ui);

		bool operator<=(const unsigned int num) const;
		bool operator>=(const unsigned int num) const;
		bool operator<(const unsigned int num) const;
		bool operator>(const unsigned int num) const;
		bool operator==(const unsigned int num) const;
		bool operator!=(const unsigned int num) const;
		friend bool operator<=(const unsigned int num, const unlimited_int& ui);
		friend bool operator>=(const unsigned int num, const unlimited_int& ui);
		friend bool operator<(const unsigned int num, const unlimited_int& ui);
		friend bool operator>(const unsigned int num, const unlimited_int& ui);
		friend bool operator==(const unsigned int num, const unlimited_int& ui);
		friend bool operator!=(const unsigned int num, const unlimited_int& ui);

		bool operator<=(const unsigned long int num) const;
		bool operator>=(const unsigned long int num) const;
		bool operator<(const unsigned long int num) const;
		bool operator>(const unsigned long int num) const;
		bool operator==(const unsigned long int num) const;
		bool operator!=(const unsigned long int num) const;
		friend bool operator<=(const unsigned long int num, const unlimited_int& ui);
		friend bool operator>=(const unsigned long int num, const unlimited_int& ui);
		friend bool operator<(const unsigned long int num, const unlimited_int& ui);
		friend bool operator>(const unsigned long int num, const unlimited_int& ui);
		friend bool operator==(const unsigned long int num, const unlimited_int& ui);
		friend bool operator!=(const unsigned long int num, const unlimited_int& ui);

		bool operator<=(const unsigned long long int num) const;
		bool operator>=(const unsigned long long int num) const;
		bool operator<(const unsigned long long int num) const;
		bool operator>(const unsigned long long int num) const;
		bool operator==(const unsigned long long int num) const;
		bool operator!=(const unsigned long long int num) const;
		friend bool operator<=(const unsigned long long int num, const unlimited_int& ui);
		friend bool operator>=(const unsigned long long int num, const unlimited_int& ui);
		friend bool operator<(const unsigned long long int num, const unlimited_int& ui);
		friend bool operator>(const unsigned long long int num, const unlimited_int& ui);
		friend bool operator==(const unsigned long long int num, const unlimited_int& ui);
		friend bool operator!=(const unsigned long long int num, const unlimited_int& ui);

//Bitwise Operators
		//Uses unlimited_int::to_string() and respects flags std::ios::hex, std::ios::dec, std::ios::oct
		friend std::ostream& operator<<(std::ostream& os, const unlimited_int& ui);
		//Bitwise shift left, Throws exception std::invalid_argument when used on a negative number
		void operator<<=(const size_t num_to_shift_by) { this->shift_left_by_bits(num_to_shift_by); }
		//Bitwise shift right, Throws exception std::invalid_argument when used on a negative number
		void operator>>=(const size_t num_to_shift_by) { this->shift_right_by_bits(num_to_shift_by); }
		//Bitwise operator AND, throws exception std::invalid_argument when used on negative number(s)
		unlimited_int operator&(const unlimited_int&) const;
		//Bitwise operator OR, the result will be as long as the longer number between the two. Throws exception std::invalid_argument when used on negative number(s)
		unlimited_int operator|(const unlimited_int&) const;
		//Bitwise operator XOR, the result will be as long as the longer number between the two. Throws exception std::invalid_argument when used on negative number(s)
		unlimited_int operator^(const unlimited_int&) const;
		//Bitwise operator NOT. Only inverses the used bits. For example:
		//unlimited_int num(0b00001101);
		//std::cout << unlimited_int(~num);
		//That code will output the number 0b10 because it's ignoring all bits that are before the most significant 1 of the original number.
		//Throws exception std::invalid_argument when used on a negative number
		unlimited_int operator~() const;
		//Just like operator~ except that it's more efficient because it directly changes the original number's bits. Just like &= compared to & (for example)
		//Throws exception std::invalid_argument when used on a negative number
		void invert_bits();
		//Bitwise operator AND. Throws exception std::invalid_argument when used on a negative number
		void operator&=(const unlimited_int& right) { *this = (*this) & right; }
		//Bitwise operator OR. Throws exception std::invalid_argument when used on a negative number
		void operator|=(const unlimited_int& right) { *this = (*this) | right; }
		//Bitwise operator XOR. Throws exception std::invalid_argument when used on a negative number
		void operator^=(const unlimited_int& right) { *this = (*this) ^ right; }
		//Bitwise shift right, Throws exception std::invalid_argument when used on a negative number
		unlimited_int operator>>(const size_t num_to_shift_by) const {
			unlimited_int answer = *this;
			answer >>= num_to_shift_by;
			return answer;
		}
		//Bitwise shift left, Throws exception std::invalid_argument when used on a negative number
		unlimited_int operator<<(const size_t num_to_shift_by) const {
			unlimited_int answer = *this;
			answer <<= num_to_shift_by;
			return answer;
		}
//String Conversions
		//from base 1 until base 36 (including). Can recognize prepended minus symbol (-). Supports both uppercase and lowercase. Receives null-terminated string.
		static unlimited_int from_string(const char* str, const unsigned int base = 10);
		//uses "static unlimited_int from_string(const char* str, const int base = 10)"
		static unlimited_int from_string(const std::string& str, const unsigned int base = 10) { return unlimited_int::from_string(str.c_str(), base); }
		//from base 1 until base 36 (including). Uses lowercase English letters. Faster in base 16.
		std::string to_string(const unsigned int base = 10) const;
//Power (^)
		//efficient method for power (math function) with remainder as well. Receives boolean pointer used for early termination of calculation.
		//With given negative numbers, it works the same as unlimited_int::operator% and the same as unlimited_int::pow(base, power).
		static unlimited_int pow(const unlimited_int& base, const unlimited_int& power, const unlimited_int& remainder, const volatile bool *const terminator = nullptr);
		//efficient method for power (math function)
		//Works with negative numbers as well. If power is negative it will return 0.
		static unlimited_int pow(const unlimited_int& base, const unlimited_int& power, const volatile bool *const terminator = nullptr);
//Random
		//Uses generate_random_that_is_at_least and then the modulo operator to generate a random number with perfectly random distribution in the given range.
		//Cryptographically secure.
		static unlimited_int generate_random(const unlimited_int& min, const unlimited_int& max);
//Hash
		//Calculates the sha-256 hash of an unlimited_int. Splits number into chunks of 32-bit integers, whether or not the program is in 64-bit mode.
		//The hash function produces the same output whether this library is in 32bit mode or in 64bit mode.
		//It directly iterates through the unlimited_int so there's no RAM risk of hashing huge unlimited_int numbers.
		unlimited_int calculate_sha256_hash() const;
		//Calculates the sha-512 hash of an unlimited_int. Splits number into chunks of 64-bit integers, whether or not the program is in 64-bit mode.
		//The hash function produces the same output whether this library is in 32bit mode or in 64bit mode.
		//It directly iterates through the unlimited_int so there's no RAM risk of hashing huge unlimited_int numbers.
		unlimited_int calculate_sha512_hash() const;
		//uses sha512 when compiling in 64bit mode, and sha256 when compiling in 32bit mode
		unlimited_int calculate_efficient_cryptographic_hash() const
		{
			if (UNLIMITED_INT_COMPILING_ON_64_BIT_SYSTEM)
				return this->calculate_sha512_hash();
			else
				return this->calculate_sha256_hash();
		}
		//Very quick not cryptographic hash function.
		//Ignores sign.
		size_t fingerprint() const;
//Prime
		//Miller-Rabin primality test: 1/(0.75^num_of_iterations) chance of mistake, exposes Carmichael numbers like 561 that satisfy the Fermat test.
		//Ignores that sign of a number: -2 is prime. Optional argument: num_of_iterations. 20 iterations will give you a 1 in a trillion chance of mistake.
		//Receives a pointer to a boolean that tells it to stop early and return false (in case of multithreading).
		bool is_prime(const int num_of_iterations = 64, const volatile bool *const terminator = nullptr) const;
#if UNLIMITED_INT_SUPPORT_MULTITHREADING
		//Receives optional argument: bool* terminator that when true, the function will terminate itself a.s.a.p and return unlimited_int that's equal to 0
		//Receives optional argument: int num_threads that specified how many threads are to be run concurrently to try to find the prime number.
		//If int num_threads is not specified then the number of threads used will be equal to the number of cores (logical processors) on the system.
		static unlimited_int generate_random_prime_multithreaded(const unlimited_int& min, const unlimited_int& max, const unsigned num_threads = 0U);
#endif
		static unlimited_int generate_random_prime(const unlimited_int& min, const unlimited_int& max, const volatile bool *const terminator = nullptr);
//Abstract Math
		//greatest common divisor- treats negative numbers as positive
		static unlimited_int gcd(const unlimited_int& a, const unlimited_int& b);
		//lowest common multiple- treats negative numbers as positive
		static unlimited_int lcm(const unlimited_int& a, const unlimited_int& b);
//Getters
		bool is_negative() const { return this->_is_negative; }
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
		static void clear_Newton_Raphson_lookup() { unlimited_int::Newton_Raphson_lookup.clear(); }
		//In MSVC++ there's a bug/feature that when using std::async, the thread_local variables aren't destroyed. That's why this function offers to manually destroy them.
		static void delete_all_thread_local_memory()
		{
			unlimited_int::flush_current_random();
			unlimited_int::clear_Newton_Raphson_lookup();
			list_of_int_arrays::destroy_piggy_bank();
		}
		//Don't erase small_prime_numbers array while it's being used. In a multithreaded environment it might be used by another thread, so be careful.
		//It's not required to call this function. The std::vector will clear out its own memory.
		static void clear_small_prime_numbers_array() { unlimited_int::small_prime_numbers.clear(); }
//Automatic Destructor
		//respects this->auto_destroy tag.
		~unlimited_int()
		{
			if (this->auto_destroy)
			{
				this->auto_destroy = false;
				this->flush();
			}
		}
	};
	//Uses unlimited_int::to_string() and respects flags std::ios::hex, std::ios::dec, std::ios::oct
	std::ostream& operator<<(std::ostream& os, const unlimited_int& ui);

	bool operator<=(const signed short int num, const unlimited_int& ui);
	bool operator>=(const signed short int num, const unlimited_int& ui);
	bool operator<(const signed short int num, const unlimited_int& ui);
	bool operator>(const signed short int num, const unlimited_int& ui);
	bool operator==(const signed short int num, const unlimited_int& ui);
	bool operator!=(const signed short int num, const unlimited_int& ui);

	bool operator<=(const signed int num, const unlimited_int& ui);
	bool operator>=(const signed int num, const unlimited_int& ui);
	bool operator<(const signed int num, const unlimited_int& ui);
	bool operator>(const signed int num, const unlimited_int& ui);
	bool operator==(const signed int num, const unlimited_int& ui);
	bool operator!=(const signed int num, const unlimited_int& ui);

	bool operator<=(const signed long int num, const unlimited_int& ui);
	bool operator>=(const signed long int num, const unlimited_int& ui);
	bool operator<(const signed long int num, const unlimited_int& ui);
	bool operator>(const signed long int num, const unlimited_int& ui);
	bool operator==(const signed long int num, const unlimited_int& ui);
	bool operator!=(const signed long int num, const unlimited_int& ui);

	bool operator<=(const signed long long int num, const unlimited_int& ui);
	bool operator>=(const signed long long int num, const unlimited_int& ui);
	bool operator<(const signed long long int num, const unlimited_int& ui);
	bool operator>(const signed long long int num, const unlimited_int& ui);
	bool operator==(const signed long long int num, const unlimited_int& ui);
	bool operator!=(const signed long long int num, const unlimited_int& ui);

	bool operator<=(const unsigned short int num, const unlimited_int& ui);
	bool operator>=(const unsigned short int num, const unlimited_int& ui);
	bool operator<(const unsigned short int num, const unlimited_int& ui);
	bool operator>(const unsigned short int num, const unlimited_int& ui);
	bool operator==(const unsigned short int num, const unlimited_int& ui);
	bool operator!=(const unsigned short int num, const unlimited_int& ui);

	bool operator<=(const unsigned int num, const unlimited_int& ui);
	bool operator>=(const unsigned int num, const unlimited_int& ui);
	bool operator<(const unsigned int num, const unlimited_int& ui);
	bool operator>(const unsigned int num, const unlimited_int& ui);
	bool operator==(const unsigned int num, const unlimited_int& ui);
	bool operator!=(const unsigned int num, const unlimited_int& ui);

	bool operator<=(const unsigned long int num, const unlimited_int& ui);
	bool operator>=(const unsigned long int num, const unlimited_int& ui);
	bool operator<(const unsigned long int num, const unlimited_int& ui);
	bool operator>(const unsigned long int num, const unlimited_int& ui);
	bool operator==(const unsigned long int num, const unlimited_int& ui);
	bool operator!=(const unsigned long int num, const unlimited_int& ui);

	bool operator<=(const unsigned long long int num, const unlimited_int& ui);
	bool operator>=(const unsigned long long int num, const unlimited_int& ui);
	bool operator<(const unsigned long long int num, const unlimited_int& ui);
	bool operator>(const unsigned long long int num, const unlimited_int& ui);
	bool operator==(const unsigned long long int num, const unlimited_int& ui);
	bool operator!=(const unsigned long long int num, const unlimited_int& ui);
}
#endif
