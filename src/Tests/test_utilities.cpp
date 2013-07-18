/** This file contains the test functions to test classes and functions defined utilities.h */
#include "../Imaging/utilities.h"

#include <stdexcept>
#include <iostream>

void TestStdArray(void)
{
	std::array<int, 3> i1 = {1, 2, 3}, i2 = {11, 12, 13}, i3;

	// Following line does not work without "using namespace Imaging" because of the namespace.
	//i3 = i1 + i2;

	// Following line works.
	i3 = Imaging::operator+(i1, i2);

	// Lesson: Do NOT define global operators inside of a namespace.
}

void TestUtilities(void)
{
	TestStdArray();
	std::cout << "Test for utilities.h has been completed." << std::endl;
}