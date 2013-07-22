/** This file contains the test functions to test classes and functions defined in
dimension.h */
#include "../Imaging/dimension.h"

#include <stdexcept>
#include <iostream>

template <typename T>
void TestImageSize(T width, T height, T depth)
{
	using namespace Imaging;

	ImageSize<T> sz1(width, height, depth), sz2;
	sz2 = sz1;
	ImageSize<T> sz3 = sz1;

	if (sz2 != sz1)
		throw std::logic_error("ImageSize<T>");
	if (sz3 != sz1)
		throw std::logic_error("ImageSize<T>");
	std::cout << "width = " << sz1.width << ", height = " << sz1.height << ", depth = "
		<< sz1.depth << std::endl;

	try
	{
		sz1.CheckRange(0, 0);
		std::cout << "(0, 0) is within the range." << std::endl;
		sz1.CheckRange(width, height);
		// Following lines should NOT run.
		std::ostringstream msg;
		msg << "(" << width << ", " << height << ") is within the range." << std::endl;
		std::cout << msg.str() << std::endl;
		throw std::logic_error("ImageSize<T>");
	}
	catch (const std::out_of_range &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		throw;
	}

	T nElemPerFrame = sz1.GetNumElemPerFrame();
	T nElemPerLine = sz1.GetNumElemPerLine();
	T offset = sz1.GetOffset(1, 1);
	std::cout << nElemPerFrame << " elements / frame" << std::endl;
	std::cout << nElemPerLine << " elements / line" << std::endl;
	std::cout << "Offset(1, 1) = " << offset << std::endl;
}

template <typename T, typename U>
void TestRegion(T x, T y, U width, U height)
{
	using namespace Imaging;

	Region<T, U> roi1(x, y, width, height), roi2;
	roi2 = roi1;
	Region<T, U> roi3 = roi1;

	if (roi2 != roi1)
		throw std::logic_error("Region<T, U>");
	if (roi3 != roi1)
		throw std::logic_error("Region<T, U>");
	std::cout << "{(" << roi1.origin.x << ", " << roi1.origin.y << "), [" << roi1.size.width
		<< ", " << roi1.size.height << "]}" << std::endl;

	roi2 = roi1 + Point2D<T>(1, 1);
	std::cout << "Before moving: {(" << roi1.origin.x << ", " << roi1.origin.y << "), [" 
		<< roi1.size.width << ", " << roi1.size.height << "]}" << std::endl;
	std::cout << "After moving: {(" << roi2.origin.x << ", " << roi2.origin.y << "), ["
		<< roi2.size.width << ", " << roi2.size.height << "]}" << std::endl;

	roi3 = roi1 * 2.0;
	std::cout << "Before zooming: {(" << roi1.origin.x << ", " << roi1.origin.y << "), ["
		<< roi1.size.width << ", " << roi1.size.height << "]}" << std::endl;
	std::cout << "After zooming: {(" << roi3.origin.x << ", " << roi3.origin.y << "), ["
		<< roi3.size.width << ", " << roi3.size.height << "]}" << std::endl;

	roi3 = roi1 * Point2D<double>(2.0, 1.0);
	std::cout << "Before zooming: {(" << roi1.origin.x << ", " << roi1.origin.y << "), ["
		<< roi1.size.width << ", " << roi1.size.height << "]}" << std::endl;
	std::cout << "After zooming: {(" << roi3.origin.x << ", " << roi3.origin.y << "), ["
		<< roi3.size.width << ", " << roi3.size.height << "]}" << std::endl;
}

void TestDimension(void)
{
	try
	{
		std::cout << std::endl << "Test for dimension.h has started." << std::endl;
		
		TestImageSize<int>(32, 16, 3);
		TestImageSize<unsigned int>(32, 16, 3);

		TestRegion<int, unsigned int>(0, 0, 4, 8);
		TestRegion<int, int>(-1, -1, 4, 8);

		std::cout << "Test for dimension.h has been completed." << std::endl;
	}
	catch (const std::logic_error &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		throw;
	}
}
