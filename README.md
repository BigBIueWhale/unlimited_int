# unlimited_int
# Written by Ronen Zyroff in 2019-2021
Arbitrary-precision arithmetic library written in pure C++ for C++. Has similar syntax to int in C++ with operator overloading.
# Basic use
#include "unlimited.hpp"\
using namespace unlimited;\
int main()\
{\
  unlimited_int small_num(5);\
  unlimited_int big_num = unlimited_int::from_string("129312981629387124981620398712489112387461298346198234");\
  unlimited_int big_num2 = unlimited_int::from_string("129012873823723432423");\
  unlimited_int result_pow_with_remainder = unlimited_int::pow(small_num, big_num, big_num2);\
  std::cout << "\nResult is: " << result_pow_with_remainder << "\nResult in hex: " << std::hex << result_pow_with_remainder << std::dec;\
  return 0;\
}\
The output of that program will be:\
68787405743989542317
# Fibonacci example:
#include "unlimited.hpp"\
using namespace unlimited;\
#include \<utility\> //For std::move\
int main()\
{\
  unlimited_int a(1);\
  unlimited_int b(1);\
  bool turn_a = true;\
  for (int c = 0; c < 20000; ++c)\
  {\
    if (turn_a)\
    {\
      turn_a = false;\
      a += b;\
    }\
    else\
    {\
      turn_a = true;\
      b += a;\
    }\
  }\
  unlimited_int result;\
  if (turn_a)\
    result = std::move(b);\
  else\
    result = std::move(a);\
  std::cout << "\nThe result is: " << result;\
}\
The output of that program will be:\
68787405743989542317
