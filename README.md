# unlimited_int
# Written by Ronen Zyroff in 2019-2021
Arbitrary-precision arithmetic library written in pure C++ (with some std for string manipulation) for C++. Has similar syntax to int in C++ with operator overloading.\
\
LOOK AT main.cpp for a complete example of how to use this library. READING main.cpp is the best tutorial of this library.\
In summary: just include unlimited/unlimited.h and write: "using namespace unlimited;"\
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
unlimited_int num = unlimited_int::pow(unlimited_int(2), unlimited_int(10)); //Correct! the smart pointer will automatically be converted to unlimited_int\
unlimited_int num = \*unlimited_int::pow(unlimited_int(2), unlimited_int(10)); //Wrong! this will still work but is O(n) instead of O(1)\
unlimited_int* cpy_num_ptr = num.copy(); //Option 1 to fully copy number\
delete cpy_num_ptr;\
unlimited_int cpy_num = num; //Option 2 to fully copy number. Preferred option.\
Funtions that return an unlimited_int should return: std::shared_ptr<unlimited_int>\
For example:\
std::shared_ptr<unlimited_int> abs(const unlimited_int& num) {\
  unlimited_int* cpy = num.copy();\
  cpy.self_abs();\
  return std::shared_ptr<unlimited_int>(cpy);\
}
