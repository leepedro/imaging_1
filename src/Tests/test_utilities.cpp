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

template <typename T>
void TestSafeAdd(T a, T b, T &c)
{
	try
	{
		Imaging::SafeAdd(a, b, c);
		c = Imaging::SafeAdd(a, b);
		std::cout << typeid(T).name() << " " << a << " + " <<
		typeid(T).name() << " " << b << " = " << typeid(T).name() << " " << c << std::endl;
	}
	catch (const std::overflow_error &)
	{
		std::cout << "Overflow detected while adding " << typeid(T).name() << " " << a <<
		" and " << typeid(T).name() << " " << b << std::endl;
	}
	catch (...)
	{
		throw;
	}
}

void TestSafeArithmetic(void)
{
	std::cout << "Test for safe arithmetic operation started." << std::endl;

	int i_max(std::numeric_limits<int>::max()), i_neg(-1), i_small(1), i_dst;
	int i_min(std::numeric_limits<int>::min());
	unsigned int ui_max(std::numeric_limits<unsigned int>::max()), ui_small(1), ui_dst;
	long long ll_max(std::numeric_limits<long long>::max()), ll_neg(-1), ll_small(1);
	long long ll_min(std::numeric_limits<long long>::min()), ll_dst;

	// negative integer overflow risk.
	std::cout << std::endl;
	std::cout << "negative integer overflow risk" << std::endl;
	TestSafeAdd(i_min, i_neg, i_dst);		// overflow detected.
	TestSafeAdd(i_neg, i_small, i_dst);
	TestSafeAdd(ll_min, ll_neg, ll_dst);	// overflow detected.
	TestSafeAdd(ll_neg, ll_small, ll_dst);

	// positive integer overflow risk.
	std::cout << std::endl;
	std::cout << "positive integer overflow risk" << std::endl;
	TestSafeAdd(i_max, i_small, i_dst);	// overflow detected.
	TestSafeAdd(i_max, i_neg, i_dst);
	TestSafeAdd(ui_max, ui_small, ui_dst);	// overflow detected.
	TestSafeAdd(ll_max, ll_small, ll_dst);	// overflow detected.
	TestSafeAdd(ll_max, ll_neg, ll_dst);

	std::cout << "Test for safe arithmetic operation completed." << std::endl;
}

template <typename T, ::size_t N>
void TestStdArrayAdd(const std::array<T, N> &a, const std::array<T, N> &b,
					 std::array<T, N> &c)
{
	try
	{
		// Following line does not work without "using namespace Imaging".
		//c = a + b;

		// Following line works.
		c = Imaging::operator+(a, b);
		std::cout << "Added two std::array<" << typeid(T).name() << ">." << std::endl;
	}
	catch (const std::overflow_error &)
	{
		std::cout << "Overflow detected while adding two std::array<" << typeid(T).name()
			<< ">." << std::endl;
	}
	catch (...)
	{
		throw;
	}
}

void TestStdArray(void)
{
	std::cout << "Test for safe arithmetic operation for std::array<T, N> started."
		<< std::endl;

	int i_max = std::numeric_limits<int>::max();
	std::array<int, 3> i1 = {1, 2, 3}, i2 = {i_max, i_max, i_max}, i3;
	//std::array<long long, 3> l1;

	// negative integer overflow risk.
	std::cout << std::endl;
	std::cout << "negative integer overflow risk" << std::endl;
	TestStdArrayAdd(i1, i2, i3);

	// positive integer overflow risk.
	std::cout << std::endl;
	std::cout << "positive integer overflow risk" << std::endl;

	// Following line does not work without "using namespace Imaging".
	//i3 = i1 + i2;

	// Following line works.
	//i3 = Imaging::operator+(i1, i2);	// Overflowed!

	// Lesson: Do NOT define global operators inside of a namespace.

	//using namespace Imaging;
	//Add(i1, i2, i3);
	//Add(i1, i2, l1);

	std::cout << "Test for safe arithmetic operation for std::array<T, N> has been completed."
		<< std::endl;
}

void TestUtilities(void)
{
	TestsSafeCast();
	TestSafeArithmetic();
	TestStdArray();
	std::cout << "Test for utilities.h has been completed." << std::endl;
}