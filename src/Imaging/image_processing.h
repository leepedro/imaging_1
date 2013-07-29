#if !defined(IMAGE_PROCESSING_H)
#define IMAGE_PROCESSING_H

#include "image.h"

namespace Imaging
{

	enum class Interpolation {UNKNOWN, LINEAR, NEAREST, CUBIC, LANCZO};
	/** Resizes image data from a source ROI, and copies the resized image data to
	destination ROI.

	@NOTE The destination image should have already allocated. */
	template <typename T>
	void Resize(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		const Point2D<double> &zm, Image<T> &imgDst,
		const Point2D<typename Image<T>::SizeType> &orgnDst,
		Interpolation interp = Interpolation::LINEAR);

	/** Resizes image data from a source ROI, and copies the resized image data to
	destination image.
	
	The destination image is resized to exactly fit the result. */
	template <typename T>
	void Resize(const Image<T> &imgSrc,
		const Region<typename Image<T>::SizeType, typename Image<T>::SizeType> &roiSrc,
		const Point2D<double> &zm, Image<T> &imgDst,
		Interpolation interp = Interpolation::LINEAR);

	/** Resizes the entire source image, and copies the resized image data to destination
	image. */
	template <typename T>
	void Resize(const Image<T> &imgSrc,	const Point2D<double> &zm, Image<T> &imgDst,
		Interpolation interp = Interpolation::LINEAR);
}
#endif
