#if !defined(DIMENSION_H)
#define DIMENSION_H

#include <array>

#include "coordinates.h"
#include "utilities.h"

namespace Imaging
{
	/* Presents the dimension of an image by the number of elements.
	@NOTE The number of element is NOT the same as the number of bytes.
	It is identical to the number of bytes only if the image is 1 byte data type and the
	image does not have any padding bytes. */
	template <typename T>
	class ImageSize : public Size3D<T>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		ImageSize(void);
		ImageSize(const ImageSize<T> &src);
		ImageSize<T> &operator=(const ImageSize<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		ImageSize(T width, T height, T depth);

		////////////////////////////////////////////////////////////////////////////////////
		// Operators.
		//bool operator==(const ImageSize<T> &src) const;
		//bool operator!=(const ImageSize<T> &src) const;

		////////////////////////////////////////////////////////////////////////////////////
		// Methods.
		/** Checks if the given (x, y) coordinate is within the range of (width, height).
		@exception std::out_of_range	if (x, y) is outside of the current dimension, */
		void CheckRange(T x, T y) const;

		/** Gets the number of elements for the entire frame.
		total number of elements = depth x width x height */
		typename std::enable_if<std::is_integral<T>::value, T>::type
			GetNumElemPerFrame(void) const;

		/** Gets the number of elements per line.
		total number of elements / line = depth x width */
		typename std::enable_if<std::is_integral<T>::value, T>::type
			GetNumElemPerLine(void) const;

		/** Gets the offset to the given position (x, y) as the number of elements. */
		typename std::enable_if<std::is_integral<T>::value, T>::type
			GetOffset(T x, T y) const;

		////////////////////////////////////////////////////////////////////////////////////
		// Data.
		//T width;	// Number of pixels / line.
		//T height;	// Number of lines / frame.
		//T depth;	// Number of channels (elements) / pixel.
	};


	/** Represents a region of interest or an area of interest.
	The dimension is defined as the number of pixels. */
	template <typename T, typename U>
	class Region
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Region(void);
		Region(const Region<T, U> &src);
		Region &operator=(const Region<T, U> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Region(const Point2D<T> &origin, const Size2D<U> &size);
		Region(T x, T y, U width, U height);

		////////////////////////////////////////////////////////////////////////////////////
		// Operators.

		/** Moves the origin by the given distance, and returns the result as a new
		Region<T, U> without changing this object. */
		Region<T, U> operator+(const Point2D<T> &dist) const;

		/** Zooms the size by the given zoom rate without moving the origin., and returns
		the result as a new Region<T, U> without changing this object. */
		Region<T, U> operator*(const Point2D<double> &zm) const;
		Region<T, U> operator*(double zm) const;

		////////////////////////////////////////////////////////////////////////////////////
		// Methods.

		/** Moves the origin by the given distance. */
		void Move(const Point2D<T> &dist);

		/** Zooms the size by the given zoom rate without moving the origin. */
		void Zoom(const Point2D<double> &zm);
		void Zoom(double zm);

		////////////////////////////////////////////////////////////////////////////////////
		// Data.
		Point2D<T> origin;
		Size2D<U> size;
	};

}

#include "dimension_inl.h"

#endif
