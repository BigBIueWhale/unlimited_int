# unlimited_int
# Written by Ronen Zyroff in 2019-2021
Arbitrary-precision arithmetic library written in pure C++ (with some std for string manipulation) for C++. Has similar syntax to int in C++ with operator overloading.\
\
LOOK AT main.cpp for a complete example of how to use this library. READING main.cpp is the best tutorial of this library.\
In summary: just include unlimited.h and write "using namespace unlimited;"
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
The third difference is when using a series of operations, for example:\
unlimited_int num = unlimited_int(unlimited_int::pow(5, 6, 7)); //the unlimited_int() constructor must be used to avoid the memory leak\
