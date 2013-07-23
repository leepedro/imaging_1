/** This file contains the test functions to test classes and functions defined in
image.h */
#include "../Imaging/image.h"

#include <stdexcept>
#include <iostream>

template <typename T>
void TestImage1(::size_t width, ::size_t height, ::size_t depth = 1)
{
	using namespace Imaging;

	Image<T> img1(width, height, depth), img2;	
	img2 = img1;
	Image<T> img3 = img1;

	if (img2.size != img1.size)
		throw std::logic_error("Image<T>");
	if (img3.size != img1.size)
		throw std::logic_error("Image<T>");

	std::cout << "Before change: I(0, 0) = " << img1(0, 0) << std::endl;
	img1(0, 0) = static_cast<T>(1);
	std::cout << "After change: I(0, 0) = " << img1(0, 0) << std::endl;

	std::cout << "Before change: width = " << img1.size.width << ", height = "
		<< img1.size.height << ", depth = " << img1.size.depth << std::endl;
	img1.resize(3, 2, 1);
	std::cout << "After change: width = " << img1.size.width << ", height = "
		<< img1.size.height << ", depth = " << img1.size.depth << std::endl;

	std::cout << "Before change: width = " << img2.size.width << ", height = "
		<< img2.size.height << ", depth = " << img2.size.depth << std::endl;
	img2.clear();
	std::cout << "After change: width = " << img2.size.width << ", height = " << img2.size.height << ", depth = "
		<< img2.size.depth << std::endl;

	Imaging::Image<T>::Iterator it = img1.GetIterator(1, 1);
}

template <typename T>
void TestCopy(void)
{
	using namespace Imaging;

	Image<T> img1(60, 40, 3), img2(60, 40, 3), img3(40, 20, 3);
	Region<Image<T>::SizeType, Image<T>::SizeType> roiSrc(10, 10, 40, 20);
	Copy(img1, roiSrc, img2, Point2D<Image<T>::SizeType>(10, 10));
	Copy(img1, roiSrc, img3, Point2D<Image<T>::SizeType>(0, 0));
}

void TestImage(void)
{
	std::cout << std::endl << "Test for image.h has started." << std::endl;

	TestImage1<unsigned char>(32, 16, 3);
	TestImage1<int>(32, 16);
	TestCopy<unsigned char>();
	TestCopy<int>();
	std::cout << std::endl << "Test for image.h has been completed." << std::endl;
}