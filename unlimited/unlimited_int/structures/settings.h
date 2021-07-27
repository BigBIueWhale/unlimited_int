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

//0 for no debugging at all, 1 for all debugging except verbose print, 2 for all debugging with verbose print, -2 for only memory counting (check for memory leaks)
#define DEBUG_MODE 1
#if (DEBUG_MODE <= -3) || (DEBUG_MODE == -1) || (DEBUG_MODE >= 3) //Invalid options
#error "FATAL COMPILATION ERROR of \"unlimited\" library. The DEBUG_MODE macro is set to an invalid option. Try manually editing the valud of DEBUG_MODE in file: \"unlimited/unlimited_int/structures/settings.h\""
#endif
#include <stdint.h> //for the typenames uint32_t, uint64_t, uint16_t
namespace unlimited //macros aren't inside the namespace, but it does affect the "typedef" declarations
{
#if IS_64_BIT_SYSTEM
	typedef uint32_t few_bits;
	typedef uint64_t many_bits;
#define NUM_OF_BITS_few_bits 32
#define NUM_OF_BITS_many_bits 64
	typedef int32_t few_bits_signed;
	typedef int64_t many_bits_signed;
#define MASK_LOW_BITS 0xffffffff
#define MAX_few_bits_NUM 0xffffffff
#define MAX_few_bits_NUM_PLUS_ONE 0x100000000
#define MAX_many_bits_NUM 0xffffffffffffffff
#else
	typedef uint16_t few_bits;
	typedef uint32_t many_bits;
#define NUM_OF_BITS_few_bits 16
#define NUM_OF_BITS_many_bits 32
	typedef int16_t few_bits_signed;
	typedef int32_t many_bits_signed;
#define MASK_LOW_BITS 0xffff
#define MAX_few_bits_NUM 0xffff
#define MAX_few_bits_NUM_PLUS_ONE 0x10000
#define MAX_many_bits_NUM 0xffffffff
#endif
}
//The maximum size of list_of_int_arrays::bank_storage per thread. Each thread can have the amount of RAM specified in this C++ macro
//in num of ints, or about 100 megabytes in 64bit mode or 50 megabytes in 32bit mode (each int is 32 bits in 64bit mode)
#define PIGGY_BANK_MAXIMUM ((many_bits)(26843545))
//#define PIGGY_BANK_MAXIMUM ((many_bits)(0)) //setting to 0 de facto disables piggy bank retention
	//#define PIGGY_BANK_MAXIMUM ((many_bits)(100000000000000000)) //in num of ints, or too much to matter
	//#define MAX_ALLOC 1048576 //2^20
#define MAX_ALLOC 16384 //per single int array.
#define MIN_ALLOC 4096 //per single int array (2048 32bits)^2 = (64 32bit integers)^2
//#define MIN_ALLOC 1
//#define MAX_ALLOC 20

#endif