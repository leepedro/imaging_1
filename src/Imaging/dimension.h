#if !defined(DIMENSION_H)
#define DIMENSION_H

#include <array>

#include "coordinates.h"

namespace Imaging
{
	/* Presents the dimension of an image by the number of elements.
	@NOTE The number of element is NOT the same as the number of bytes.
	It is identical to the number of bytes only if the image is 1 byte data type and the
	image does not have any padding bytes. */
	template <typename T>
	class Dimension
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Dimension(void);
		Dimension(const Dimension<T> &src);
		Dimension<T> &operator=(const Dimension<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Dimension(T w, T h, T d);

		////////////////////////////////////////////////////////////////////////////////////
		// Operators.
		bool operator==(const Dimension<T> &src) const;
		bool operator!=(const Dimension<T> &src) const;

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
		T width;	// Number of pixels / line.
		T height;	// Number of lines / frame.
		T depth;	// Number of channels (elements) / pixel.
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
		Region(const Cartesian2D<T> &origin, const Cartesian2D<U> &size);
		Region(T x, T y, U width, U height);

		////////////////////////////////////////////////////////////////////////////////////
		// Accessors.
		typename U &width, &height;

		////////////////////////////////////////////////////////////////////////////////////
		// Methods.
		Region<T, U> Move(const Cartesian2D<T> &change) const;
		Region<T, U> Zoom(const Cartesian2D<double> &zm) const;
		Region<T, U> Zoom(double zm) const;

		////////////////////////////////////////////////////////////////////////////////////
		// Data.
		Cartesian2D<T> origin;
		Cartesian2D<U> size;
	};

}

#include "dimension_inl.h"

#endif
