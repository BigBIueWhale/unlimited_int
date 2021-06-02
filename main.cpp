// Example "Hello World"-like main file for this library
//The next 5 lines of code are necessary for the library to function:
#include "unlimited_int.h"
#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
many_bits int_array::num_of_ints_created = 0;
many_bits int_array::num_of_ints_destroyed = 0;
#endif

unlimited_int* do_stuff();
unlimited_int* calc_fibonacci(int);
void use_fibonacci_function();
void use_do_stuff_function();
int main(int argc, char* argv[])
{
	use_do_stuff_function();
	std::cout << std::endl;
	use_fibonacci_function();

#if (DEBUG_MODE > 0) || (DEBUG_MODE == -2)
	list_of_int_arrays::flush_piggy_bank();
	std::cout << "\n\nDEBUG INFORMATION:\n";
	std::cout << "\n$$num_of_ints_created: " << int_array::num_of_ints_created;
	std::cout << "\nnum_of_ints_destroyed: " << int_array::num_of_ints_destroyed;
#endif
	return 0;
}
void use_fibonacci_function()
{
	int fibonacci_index = 0;
	std::cout << "\nEnter Fibonacci index to find (where 0 is 0, 1 is 1, 2 is 1, 3 is 2, 4 is 3): ";
	std::cin >> fibonacci_index;
	unlimited_int result = calc_fibonacci(fibonacci_index);

	unlimited_int power_of_10 = (many_bits_signed)10, max_number_of_digits = (many_bits_signed)1000;
	power_of_10 = unlimited_int::pow(power_of_10, max_number_of_digits); //an effient ^ function that comes with unlimited_int
	result %= power_of_10; //cutoff the result to only the 1,000 least-significant digits

	std::cout << "\nFibonacci index: " << fibonacci_index << " is:\n" << result << "\nBut that\'s only the last " << max_number_of_digits << " digits.";
}
void use_do_stuff_function()
{
	unlimited_int result = do_stuff(); //if we don't catch the unlimited_int* there will be a memory leak, because
								   //the function "unlimited_int::operator=(unlimited_int*)" destroys the pointer, which is necessary.
	std::cout << "\nResult is: " << result;
	//The result is supposed to be:
	//-650607393988744230398605480667353124279313364516927280270664217534600881455676511185440375579353552953786046628759562238286700744489064687808027768494436805055069938490102920003903081840026920938097252943693088720429154293996200729034796480895976005835128313829586926584019567765198766014038263924498551860950290484245264865080150413911513000438630400169097682061337546372958046082672260653313851338797504060973913036053123419197622905572501371758731706792111854216478094107008141555240464424227573302280489078307780754660643123043448308883853229442001542660801329871329442367968470904652380823177833124
}
unlimited_int* do_stuff()
{
	unlimited_int a = ((many_bits_signed)10), b, sum((many_bits_signed)1); //different ways of initializing unlimited_int objects
	b = (many_bits_signed)4;
	//never initialize like this: b = a;
	//instead, use this: b = a.copy();

	for (int counter = 0; counter < 1000; ++counter)
	{
		sum *= b;
		sum -= a;
		sum = sum - a; //the same as the above line.
		//The next two lines cancel each-other out.
		sum += (many_bits)1;
		sum -= (many_bits)1;
		//The next two lines cancel each-other out.
		sum <<= (many_bits)5;
		sum >>= (many_bits)5;
		//The next two swapping methods will cancel each-other out:
		//Two fast methods to swap between "unlimited_int"s:
		//method 1 (1 line of code):
		a.swap(b); //This swaps between two unlimited_int numbers efficiently. a becomes b, and b becomes a.
		//method 2 (the next 9 lines of code) NEVER USE METHOD 2!!!!!:
		a.set_auto_destroy(false);
		b.set_auto_destroy(false);
		unlimited_int temp_ui = a; //Using the copy-constructor the thing I said never to do. But I'm doing it now for efficiency.
		temp_ui = a; //same as the above line except it doesn't use the copy-constructor so it sets "temp_ui.auto_destroy == true" and "a.auto_destroy == false"
		temp_ui.set_auto_destroy(false); //to fix the issue that the above line created.
		a = b; //Since a's "auto_destroy" is false, "a" isn't flushed now, that's why I said to never use assignment like this. You should use a = b.copy(); in general.
		b = temp_ui; //again, b's "auto_destroy" is false, so "b" isn't flushed now.
		b.set_auto_destroy(true);
		a.set_auto_destroy(true);
		/*
		Slow method to swap:
		unlimited_int temp = a.copy();
		a = b.copy();
		b = temp.copy();
		temp.flush() //not essential.
		*/
	}

	//print "sum" in base 10
	std::cout << "\nCalculation result in base 10: " << sum;

	//print "sum" in base 2
	char* c_string = sum.to_string(2);
	std::cout << "\nCalculation result in base 2: " << c_string;
	delete[] c_string;

	//convert "unlimited_int a" into an integer (assuming it's small enough)
	int a_int = (many_bits_signed)a;
	std::cout << "\na is: " << a_int;

	//clone "unlimited_int sum"
	unlimited_int copy_of_the_result = sum.copy();
	//unlimited_int copy_of_the_result = sum; without .copy() will create a problem because when they're both
	//destroyed at the end of this function the same place in memory will be cleared, which will cause an error.
	std::cout << "\nThe result is negative (1/0): " << copy_of_the_result.get_is_negative(); //efficient method
	std::cout << "\nThe result is negative (1/0): " << (copy_of_the_result < (many_bits_signed)0); //inefficient method
	
	return copy_of_the_result.copy(); //we can't return the original "copy_of_the_result" because it'll be destroyed
									  //immediately at the end of this function
}
unlimited_int* calc_fibonacci(int number_index) //0th index is 0, 1st index is 1, 2nd index is 1, 3rd index is 2
{
	unlimited_int a = (many_bits_signed)0, b = (many_bits_signed)1;
	if (number_index <= 0)
		return a.copy();
	else if (number_index == 1)
		return b.copy();
	bool b_contains_higher_number = true;
	for (int counter = 1; counter < number_index; ++counter)
	{
		if (counter % 100000 == 0)
		{
			std::cout << "\nCounter: " << counter;
		}
		if (b_contains_higher_number)
		{
			a = a + b;
			b_contains_higher_number = false;
		}
		else
		{
			b = a + b;
			b_contains_higher_number = true;
		}
	}
	if (b_contains_higher_number)
		return b.copy();
	return a.copy();
}
