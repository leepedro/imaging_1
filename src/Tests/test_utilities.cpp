/** This file contains the test functions to test classes and functions defined utilities.h */
#include "../Imaging/utilities.h"

#include <stdexcept>
#include <iostream>

template <typename T, typename U>
void TestSafeCast(const T src, U &dst)
{	
	try
	{
		Imaging::SafeCast(src, dst);
		std::cout << "Casted " << typeid(T).name() << " " << src << " to " <<
		typeid(U).name() << " " << dst << std::endl;
	}
	catch (const std::overflow_error &)
	{
		std::cout << "Overflow detected while casting " << typeid(T).name() << " " << src <<
		" to " << typeid(U).name() << std::endl;
	}
	catch (...)
	{
		throw;
	}
}

void TestsSafeCast(void)
{
	std::cout << "Test for safe casting started." << std::endl;

	int i_max(std::numeric_limits<int>::max()), i_neg(-1), i_small(1), i_dst;
	unsigned int ui_max(std::numeric_limits<unsigned int>::max()), ui_small(1), ui_dst;
	long long ll_max(std::numeric_limits<long long>::max()), ll_neg(-1), ll_small(1);
	unsigned long long ull_dst;
	float f_small(1.0), f_dst;
	double d_neg(-1.0), d_small(1.0), d_dst;

	// negative integer overflow risk.
	std::cout << std::endl;
	std::cout << "negative integer overflow risk" << std::endl;
	TestSafeCast(d_neg, ui_dst);		// Warning C4244, overflow detected.
	TestSafeCast(d_small, ui_dst);		// Warning C4244
	TestSafeCast(i_neg, ui_dst);		// overflow detected.
	TestSafeCast(i_small, ui_dst);

	// positive integer overflow risk.
	std::cout << std::endl;
	std::cout << "positive integer overflow risk" << std::endl;
	TestSafeCast(ll_max, i_dst);		// overflow detected.
	TestSafeCast(ll_small, i_dst);
	TestSafeCast(ui_max, i_dst);		// overflow detected.
	TestSafeCast(ui_small, i_dst);

	// both negative and positive
	std::cout << std::endl;
	std::cout << "both negative and positive integer overflow risk" << std::endl;
	TestSafeCast(ll_max, ui_dst);		// overflow detected.
	TestSafeCast(ll_neg, ui_dst);		// overflow detected.
	TestSafeCast(ll_small, ui_dst);

	// no risk
	std::cout << std::endl;
	std::cout << "no risk" << std::endl;
	TestSafeCast(ui_max, ull_dst);
	TestSafeCast(f_small, d_dst);
	TestSafeCast(d_small, f_dst);

	std::cout << "Test for safe casting completed." << std::endl;
}

void TestStdArray(void)
{
	int i_max = std::numeric_limits<int>::max();
	std::array<int, 3> i1 = {1, 2, 3}, i2 = {i_max, i_max, i_max}, i3;
	std::array<long long, 3> l1;

	// Following line does not work without "using namespace Imaging".
	//i3 = i1 + i2;

	// Following line works.
	i3 = Imaging::operator+(i1, i2);	// Overflowed!

	// Lesson: Do NOT define global operators inside of a namespace.

	using namespace Imaging;
	Add(i1, i2, i3);
	Add(i1, i2, l1);
}

void TestUtilities(void)
{
	TestsSafeCast();
	TestStdArray();
	std::cout << "Test for utilities.h has been completed." << std::endl;
}