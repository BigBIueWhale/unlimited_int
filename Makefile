example: unlimited/*.cpp unlimited/*.hpp unlimited/unlimited_int/*.hpp unlimited/unlimited_int/*.cpp unlimited/unlimited_int/structures/*.hpp unlimited/unlimited_int/structures/*.cpp main.cpp
	g++ --std=c++20 -m64 unlimited/*.cpp unlimited/*.hpp unlimited/unlimited_int/*.hpp unlimited/unlimited_int/*.cpp unlimited/unlimited_int/structures/*.hpp unlimited/unlimited_int/structures/*.cpp main.cpp -o output.exe -Wall -Wpedantic -Wextra -O3 -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	
