#include "../Imaging/image.h"

#include "tests.h"

#include <stdexcept>
#include <iostream>

template <typename T>
void TestImage1(void)
{
	Imaging::Image<T> img1;	
	Imaging::Image<T> img2(4, 4, 1);

	img2(0, 0) = static_cast<T>(1);
	T value = img2(0, 0);

	img2.clear();
	img1.resize(3, 2, 1);
	img2.resize(2, 3, 1);
	img1 = img2;

	Imaging::Image<T>::Iterator it = img1.GetIterator(1, 1);
}

int main(void)
{
	try
	{
		TestCoordinates();
		TestImage1<unsigned char>();
	}
	catch (const std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
}