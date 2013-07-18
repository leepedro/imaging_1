/** This file contains the test functions to test classes and functions defined coordinates.h */
#include "../Imaging/coordinates.h"

#include <stdexcept>
#include <iostream>

void TestPoint2D(void)
{
	// Point2D<T>
	using namespace Imaging;	
	Point2D<int> pt1, pt2(1, 2);
	pt1 = pt2;
	Point2D<int> pt3 = pt2;

	if (pt3 != pt2)
		throw std::logic_error("Point2D<T>");
}

void TestPoint3D(void)
{
	// Point3D<T>
	using namespace Imaging;
	Point3D<int> pt1, pt2(1, 2, 3);
	pt1 = pt2;
	Point3D<int> pt3 = pt2;

	if (pt3 != pt2)
		throw std::logic_error("Point3D<T>");
}

void TestSize2D(void)
{
	// Size2D<T>
	using namespace Imaging;	
	Size2D<int> sz1, sz2(1, 2);
	sz1 = sz2;
	Size2D<int> sz3 = sz2;

	if (sz3 != sz2)
		throw std::logic_error("Size2D<T>");
}

void TestSize3D(void)
{
	// Size3D<T>
	using namespace Imaging;	
	Size3D<int> sz1, sz2(1, 2, 3);
	sz1 = sz2;
	Size3D<int> sz3 = sz2;

	if (sz3 != sz2)
		throw std::logic_error("Size3D<T>");
}

void TestCoordinates(void)
{
	TestPoint2D();
	TestPoint3D();
	TestSize2D();
	TestSize3D();
	std::cout << "Test for coordinates.h has been completed." << std::endl;
}
