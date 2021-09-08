#ifndef SETTINGS_H_UNLIMITED_INT
#define SETTINGS_H_UNLIMITED_INT
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define IS_64_BIT_SYSTEM true
#else
#define IS_64_BIT_SYSTEM false
#endif

// Check GCC
#elif __GNUC__
#if __x86_64__ || __ppc64__
#define IS_64_BIT_SYSTEM true
#else
#define IS_64_BIT_SYSTEM false
#endif
#else
#error "FATAL COMPILATION ERROR of \"unlimited\" library. Compiler is not GCC and not Microsoft. Try manually editing \"unlimited/unlimited_int/structures/settings.h\" to specify whether the compiler is 64 bits or 32 bits"
#endif

//0 for no debugging at all, 1 for all debugging except verbose print, 2 for all debugging with (super-)verbose print, -2 for only memory counting (check for memory leaks)
#define DEBUG_MODE -2
#if (DEBUG_MODE <= -3) || (DEBUG_MODE == -1) || (DEBUG_MODE >= 3) //Invalid options
#error "FATAL COMPILATION ERROR of \"unlimited\" library. The DEBUG_MODE macro is set to an invalid option. Try manually editing the valud of DEBUG_MODE in file: \"unlimited/unlimited_int/structures/settings.h\""
#endif
#include <stdint.h> //for the typenames uint32_t, uint64_t, uint16_t
#include <exception> //For the declaration of std::exception
#include <stdexcept> //For the declarations of std::logic_error, std::out_of_range etc.
namespace unlimited //macros that are inside of the namespace, aren't affected by it, they're global anyways. But the typedef declarations will belong to the namespace "unlimited".
{
#if IS_64_BIT_SYSTEM
	typedef uint32_t few_bits;
	typedef uint64_t many_bits;
	typedef int32_t few_bits_signed;
	typedef int64_t many_bits_signed;
#define NUM_OF_BITS_few_bits 32
#else
	typedef uint16_t few_bits;
	typedef uint32_t many_bits;
	typedef int16_t few_bits_signed;
	typedef int32_t many_bits_signed;
#define NUM_OF_BITS_few_bits 16
#endif
#define NUM_OF_BITS_many_bits (sizeof(many_bits) * 8)
#define MAX_few_bits_NUM ((few_bits)(~((few_bits)0)))
#define MASK_LOW_BITS MAX_few_bits_NUM
#define MAX_few_bits_NUM_PLUS_ONE ((many_bits)MAX_few_bits_NUM + (many_bits)1)
#define MAX_many_bits_NUM ((many_bits)(~((many_bits)0)))
#define MAX_size_t_NUM ((size_t)(~((size_t)0)))
}
//list_of_int_arrays::bank_storage is a thread_local pool of memory for lower latency when quickly allocating and deallocating memory.
//This macro specifies the maximum size of list_of_int_arrays::bank_storage per thread. Each thread can have the amount of RAM specified in this C++ macro
//in num of ints, so 524288 about 2 megabytes in 64bit mode or 1 megabyte in 32bit mode (each int is 4 bytes in 64bit mode).
//In general, the piggy bank offers minimal performance gains, so don't be afraid to set it to even 0.
#define PIGGY_BANK_MAXIMUM 524288
//#define PIGGY_BANK_MAXIMUM 0
static_assert(PIGGY_BANK_MAXIMUM >= 0, "Error in unlimited_int library in the file settings.h: PIGGY_BANK_MAXIMUM can\'t be a negative value.");
//#define PIGGY_BANK_MAXIMUM ((many_bits)(0)) //setting to 0 de facto disables piggy bank retention
	//#define PIGGY_BANK_MAXIMUM ((many_bits)(100000000000000000)) //in num of ints, or too much to matter
	//#define MAX_ALLOC 1048576 //2^20
//#define MAX_ALLOC 16384 //per single int array.
//#define MIN_ALLOC 4096 //per single int array (2048 32bits)^2 = (64 32bit integers)^2
#define MAX_ALLOC 256
#define MIN_ALLOC 128
//#define MAX_ALLOC 256000
//#define MIN_ALLOC 128000
static_assert(MAX_ALLOC >= MIN_ALLOC || MIN_ALLOC <= 0, "Error in unlimited_int library in the file settings.h: MAX_ALLOC and/or MIN_ALLOC have invalid values.");
//set the multithreading macro to false if you want to compile for Linux using g++ with -static flag because that causes issues with multithreading
//Even if it's set to false, this library will still be thread-safe, it just won't open any threads of its own: unlimited::unlimited_int::generate_random_prime_multithreaded()
#define UNLIMITED_INT_SUPPORT_MULTITHREADING true
//#define MIN_ALLOC 1
//#define MAX_ALLOC 20
#endif
