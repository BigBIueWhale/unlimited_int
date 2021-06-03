# unlimited_int
# Written by Ronen Zyroff in 2019-2021
Arbitrary-precision arithmetic library written in pure C++ (with some std for string manipulation) for C++. Has similar syntax to int in C++ with operator overloading.\
\
LOOK AT main.cpp for a complete example of how to use this library. READING main.cpp is the best tutorial of this library.\
\
You must tell your compiler to compile all .h and .cpp files that are included in this repository, as showed in the Makefile\
To use the library you must include "unlimited_int.h" and then use "unlimited_int" object as you would use "int".\
\
There are 3 main differences between unlimited_int and between regular int in terms of syntax:\
\
unlimited_int a = 1;\
unlimited_int b = a.copy(); //instead of b = a;\
\
The second difference is when returning an unlimited_int by a function. You must use an unlimited_int* and return a copy:\
\
unlimited_int* do_something()\
{\
  unlimited_int e;\
  return e.copy(); //and not return e; because the memory of e will be destroyed when the function ends.\
}\
int main()\
{\
  unlimited_int f = do_something();\
}\
\
The third difference is when assigning integers, it's best to specify what integer you're using:\
There are two options: "many_bits_signed" and "many_bits". It's dependent on the function (or operator overload) being used.\
You must check which option to use in the file "unlimited_int.h" but for small positive integers it doesn't matter.\
\
unlimited_int a = (many_bits_signed)3; //instead of a = 3;\
a += (many_bits)3; //instead of a += 3; and instead of a += (many_bits_signed)3;\
\
If you need to compile for a 32-bit system then change the file "int_array.h" in line 3 from:\
#define IS_64_BIT_SYSTEM true\
to:\
#define IS_64_BIT_SYSTEM false\
\
If you want to change the max amount of RAM that the program is allowed to use for storing int arrays in the "piggy bank"\
then change the file "int_array.h" in line 35 from:\
#define PIGGY_BANK_MAXIMUM ((many_bits)(805306368)) //3 GB of RAM\
to (for example)\
#define PIGGY_BANK_MAXIMUM ((many_bits)(13107200)) //50 MB of RAM
