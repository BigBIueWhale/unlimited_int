# unlimited_int
# Written by Ronen Zyroff in 2019-2021
Arbitrary-precision arithmetic library written in pure C++ for C++. Has similar syntax to int in C++ with operator overloading.
# Basic use
unlimited_int small_num(5);\
unlimited_int big_num = unlimited_int::from_string("129312981629387124981620398712489112387461298346198234");\
unlimited_int big_num2 = unlimited_int::from_string("129012873823723432423");\
unlimited_int result_pow_with_remainder = unlimited_int::pow(small_num, big_num, big_num2);\
std::cout << "\nResult is: " << result_pow_with_remainder << "\nResult in hex: " << std::hex << result_pow_with_remainder << std::dec;
