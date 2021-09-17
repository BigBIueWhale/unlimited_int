#ifndef SETTINGS_HPP_UNLIMITED_INT
#define SETTINGS_HPP_UNLIMITED_INT

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define UNLIMITED_INT_COMPILING_ON_64_BIT_SYSTEM true
#else
#define UNLIMITED_INT_COMPILING_ON_64_BIT_SYSTEM false
#endif

// Check GCC
#elif __GNUC__
#if __x86_64__ || __ppc64__
#define UNLIMITED_INT_COMPILING_ON_64_BIT_SYSTEM true
#else
#define UNLIMITED_INT_COMPILING_ON_64_BIT_SYSTEM false
#endif
#else
#error "FATAL COMPILATION ERROR of \"unlimited\" library. Compiler is not GCC and not Microsoft. Try manually editing \"unlimited/unlimited_int/structures/settings.h\" to specify whether the compiler is 64 bits or 32 bits"
#endif

//0 for no debugging at all, 1 for all debugging except verbose print, 2 for all debugging with (super-)verbose print, -2 for only memory counting (check for memory leaks)
#define UNLIMITED_INT_LIBRARY_DEBUG_MODE -2
#if (UNLIMITED_INT_LIBRARY_DEBUG_MODE <= -3) || (UNLIMITED_INT_LIBRARY_DEBUG_MODE == -1) || (UNLIMITED_INT_LIBRARY_DEBUG_MODE >= 3) //Invalid options
#error "FATAL COMPILATION ERROR of \"unlimited\" library. The DEBUG_MODE macro is set to an invalid option. Try manually editing the valud of DEBUG_MODE in file: \"unlimited/unlimited_int/structures/settings.h\""
#endif
#include <stdint.h> //For the typenames uint32_t, uint64_t, uint16_t
#include <exception> //For the declaration of std::exception
#include <stdexcept> //For the declarations of std::logic_error, std::out_of_range etc.
#include <limits> //For std::numeric_limits
namespace unlimited //Macros that are inside of the namespace, aren't affected by it, they're global anyways. But the typedef declarations will belong to the namespace "unlimited".
{
	typedef uint32_t few_bits;
	typedef uint64_t many_bits;
#define UNLIMITED_INT_NUM_OF_BITS_few_bits 32
#define UNLIMITED_INT_NUM_OF_BITS_many_bits 64
	typedef int32_t few_bits_signed;
	typedef int64_t many_bits_signed;
	static constexpr few_bits MAX_few_bits_NUM(std::numeric_limits<few_bits>::max());
	static constexpr few_bits MASK_LOW_BITS = MAX_few_bits_NUM;
	static constexpr many_bits MAX_few_bits_NUM_PLUS_ONE = ((many_bits)MAX_few_bits_NUM + (many_bits)1);
	static constexpr many_bits MAX_many_bits_NUM = (std::numeric_limits<many_bits>::max());
	static constexpr size_t MAX_size_t_NUM = (std::numeric_limits<size_t>::max());
	static constexpr few_bits_signed MAX_few_bits_signed_NUM = (std::numeric_limits<few_bits_signed>::max());
	static constexpr many_bits_signed MAX_many_bits_signed_NUM = (std::numeric_limits<many_bits_signed>::max());
	static constexpr few_bits_signed MIN_few_bits_signed_NUM = (std::numeric_limits<few_bits_signed>::min());
	static constexpr many_bits_signed MIN_many_bits_signed_NUM = (std::numeric_limits<many_bits_signed>::min());
	//list_of_int_arrays::bank_storage is a thread_local pool of memory for lower latency when quickly allocating and deallocating memory.
	//This macro specifies the maximum size of list_of_int_arrays::bank_storage per thread. Each thread can have the amount of RAM specified in this C++ macro
	//in num of ints, so 524288 about 2 megabytes in 64bit mode or 1 megabyte in 32bit mode (each int is 4 bytes in 64bit mode).
	//In general, the piggy bank offers minimal performance gains, so don't be afraid to set it to even 0.
	static constexpr size_t PIGGY_BANK_MAXIMUM = 524288;
	static_assert(unlimited::PIGGY_BANK_MAXIMUM >= 0, "Error in unlimited_int library in the file settings.h: PIGGY_BANK_MAXIMUM can\'t be a negative value.");
	static constexpr size_t MAX_ALLOC = 256;
	static constexpr size_t MIN_ALLOC = 128;
}
//set the multithreading macro to false if you want to compile for Linux using g++ with -static flag because that causes issues with multithreading
//Even if it's set to false, this library will still be thread-safe, it just won't open any threads of its own: unlimited::unlimited_int::generate_random_prime_multithreaded()
#define UNLIMITED_INT_SUPPORT_MULTITHREADING true
//#define MIN_ALLOC 1
//#define MAX_ALLOC 20
#endif
