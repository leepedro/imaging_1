/** This file contains the test functions to test classes and functions defined in
image.h */
#include "../Imaging/image.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

#include "opencv2/opencv.hpp"

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
	try
	{
		img1.CheckRange(0, 0);
		std::cout << "(0, 0) is within the range." << std::endl;
		img1.CheckRange(width, height);	// detected.
		// Following lines should NOT run.
		std::ostringstream msg;
		msg << "(" << width << ", " << height << ") is within the range." << std::endl;
		std::cout << msg.str() << std::endl;
		throw std::logic_error("Image<T>");
	}
	catch (const std::out_of_range &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		throw;
	}

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

void TestCopyWithImage(void)
{
	using namespace Imaging;

	cv::Mat imgSrc1 = cv::imread(std::string("Lenna.png"), CV_LOAD_IMAGE_COLOR);
	cv::namedWindow(std::string("Source 1"), CV_WINDOW_AUTOSIZE);
	cv::imshow(std::string("Source 1"), imgSrc1);
	cv::waitKey(0);
	Size3D<::size_t> sz(imgSrc1.cols, imgSrc1.rows, imgSrc1.channels());

	Image<unsigned char> img1;
	Copy(imgSrc1.ptr(), sz, img1);

	int width, height, depth;
	SafeCast(sz.height, height);
	SafeCast(sz.width, width);
	SafeCast(sz.depth, depth);
	cv::Mat imgDst1(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
	Copy(img1,
		Region<Image<unsigned char>::SizeType, Image<unsigned char>::SizeType>(0, 0, sz.width, sz.height),
		imgDst1.ptr());
	cv::namedWindow(std::string("Destination 1"), CV_WINDOW_AUTOSIZE);
	cv::imshow(std::string("Destination 1"), imgDst1);
	cv::waitKey(0);

	// Recommended way!
	auto it_src = img1.GetIterator(0, 0);
	cv::Mat imgDst2(height, width, CV_8UC3, &(*it_src));
	cv::namedWindow(std::string("Destination 2"), CV_WINDOW_AUTOSIZE);
	cv::imshow(std::string("Destination 2"), imgDst2);
	cv::waitKey(0);
}

void TestImage(void)
{
	std::cout << std::endl << "Test for image.h has started." << std::endl;

	TestImage1<unsigned char>(32, 16, 3);
	TestImage1<int>(32, 16);
	TestCopy<unsigned char>();
	TestCopy<int>();

	TestCopyWithImage();

	std::cout << std::endl << "Test for image.h has been completed." << std::endl;
}