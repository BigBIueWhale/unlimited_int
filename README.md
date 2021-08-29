# unlimited_int
# Written by Ronen Zyroff in 2019-2021
Arbitrary-precision arithmetic library written in pure C++ for C++. Has similar syntax to int in C++ with operator overloading.\
\
You must tell your compiler to compile all .hpp and .cpp files that are included in this repository, as showed in the Makefile\
To use the library you must write the following in your own cpp file:\
#include "unlimited/unlimited.h"\
#using namespace unlimited;\
\
The syntax is exactly the same as the regular built-in int except that the arithmetic functions return a smart pointer to an unlimited_int object.\
For example:\
std::cout << (unlimited_int(1) + unlimited_int(2)); //Wrong! This will print the pointer's address.\
std::cout << \*(unlimited_int(1) + unlimited_int(2)); //Correct! We need to dereference the smart pointer\
unlimited_int num(unlimited_int::pow(unlimited_int(2), unlimited_int(10))); //Correct! the smart pointer will automatically be converted to unlimited_int\
unlimited_int num(\*unlimited_int::pow(unlimited_int(2), unlimited_int(10))); //Wrong! this will still work but is O(n) instead of O(1)\
unlimited_int num = unlimited_int(5); //Wrong! The move constructor is explicit:\
unlimited_int num(unlimited_int(5)); //Correct!\
unlimited_int num(5); //Also correct!\
unlimited_int num = unlimited_int::pow(unlimited_int(2), unlimited_int(10)); //Wrong! The constructor is explicit:\
unlimited_int num(unlimited_int::pow(unlimited_int(2), unlimited_int(10))); //Correct!\
unlimited_int num;\
num = \*unlimited_int::pow(unlimited_int(2), unlimited_int(10)); //Wrong! This will still work but is O(n). What a waste. Instead do this:\
num = unlimited_int::pow(unlimited_int(2), unlimited_int(10)); //Correct! No need to dereference the smart pointer. The value will be "absorbed" in O(1).\
unlimited_int* cpy_num_ptr = num.copy(); //Option 1 to fully copy number\
delete cpy_num_ptr;\
unlimited_int cpy_num = num; //Option 2 to fully copy number. Preferred option.\
Funtions that return an unlimited_int should return: std::shared_ptr<unlimited_int>, because of the destructor of the unlimited_int object.\
For example:\
std::shared_ptr<unlimited_int> abs(const unlimited_int& num) {\
  unlimited_int* cpy = num.copy();\
  cpy.self_abs();\
  return std::shared_ptr<unlimited_int>(cpy);\
}\
