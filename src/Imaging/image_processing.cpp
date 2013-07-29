#include "image_processing.h"

namespace Imaging
{
	template <>
	void Resize(const Image<unsigned char> &imgSrc,
		const Region<typename Image<unsigned char>::SizeType, typename Image<unsigned char>::SizeType> &roiSrc,
		const Point2D<double> &zm, Image<unsigned char> &imgDst,
		const Point2D<typename Image<unsigned char>::SizeType> &orgnDst, Interpolation interp)
	{

	}

	template <>
	void Resize(const Image<unsigned char> &imgSrc,
		const Region<typename Image<unsigned char>::SizeType, typename Image<unsigned char>::SizeType> &roiSrc,
		const Point2D<double> &zm, Image<unsigned char> &imgDst, Interpolation interp)
	{
	}

	template <>
	template <typename unsigned char>
	void Resize(const Image<unsigned char> &imgSrc,	const Point2D<double> &zm,
		Image<unsigned char> &imgDst, Interpolation interp)
	{
	}
}