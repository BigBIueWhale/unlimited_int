# unlimited_int
Arbitrary-precision arithmetic library written in C++ for C++ that works like regular int and dynamically allocates memory.

You must tell your compiler to compile all .h and .cpp files that are included in this repository.
To use the library you must include "unlimited_int.h" and then use "unlimited_int" object as you would use "int".

If you need to compile for a 32-bit system then change the file "int_array.h" in line 3 from:
#define IS_64_BIT_SYSTEM true
to:
#define IS_64_BIT_SYSTEM false
