/** This file contains the test functions to test classes and functions defined in
image2.h */

#include "../Imaging/image2.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

template <typename T>
void TestImageFrame1(::size_t width, ::size_t height, ::size_t depth = 1)
{
	using namespace Imaging;

	ImageFrame<T> img1(width, height, depth), img2;	
	img2 = img1;
	ImageFrame<T> img3 = img1;

	if (img2.size != img1.size)
		throw std::logic_error("ImageFrame<T>");
	if (img3.size != img1.size)
		throw std::logic_error("ImageFrame<T>");

	ImageFrame<T> img4 = std::move(img2);

	//try
	//{
	//	img1.CheckRange(0, 0);
	//	std::cout << "(0, 0) is within the range." << std::endl;
	//	img1.CheckRange(width, height);	// detected.
	//	// Following lines should NOT run.
	//	std::ostringstream msg;
	//	msg << "(" << width << ", " << height << ") is within the range." << std::endl;
	//	std::cout << msg.str() << std::endl;
	//	throw std::logic_error("Image<T>");
	//}
	//catch (const std::out_of_range &ex)
	//{
	//	std::cout << ex.what() << std::endl;
	//}
	//catch (...)
	//{
	//	throw;
	//}

	//std::cout << "Before change: I(0, 0) = " << img1(0, 0) << std::endl;
	//img1(0, 0) = static_cast<T>(1);
	//std::cout << "After change: I(0, 0) = " << img1(0, 0) << std::endl;

	std::cout << "Before change: width = " << img1.size.width << ", height = "
		<< img1.size.height << ", depth = " << img1.size.depth << std::endl;
	//img1.Resize(3, 2, 1);
	//std::cout << "After change: width = " << img1.size.width << ", height = "
	//	<< img1.size.height << ", depth = " << img1.size.depth << std::endl;

	std::cout << "Before change: width = " << img2.size.width << ", height = "
		<< img2.size.height << ", depth = " << img2.size.depth << std::endl;
	//img2.Clear();
	//std::cout << "After change: width = " << img2.size.width << ", height = " << img2.size.height << ", depth = "
	//	<< img2.size.depth << std::endl;

	//Imaging::ImageFrame<T>::Iterator it = img1.GetIterator(1, 1);
}

void TestImageFrame(void)
{
	using namespace Imaging;

	std::cout << std::endl << "Test for image2.h has started." << std::endl;

	TestImageFrame1<unsigned char>(32, 16, 3);
	TestImageFrame1<int>(32, 16);

	std::cout << std::endl << "Test for image2.h has been completed." << std::endl;
}