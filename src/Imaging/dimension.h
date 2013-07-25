#if !defined(DIMENSION_H)
#define DIMENSION_H

#include <array>
#include <sstream>

#include "coordinates.h"
#include "utilities.h"

namespace Imaging
{
	/** TODO: This class covers only BIP format, i.e., channel -> column -> row.
	Should think about BSQ format, i.e., column -> row -> channel and BIL format, i.e.,
	column -> channel -> row if considering remote sensing applications. */

	/** Presents the dimension of an image by the number of elements.

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
		ImageSize(T width, T height, T depth = 1);

		////////////////////////////////////////////////////////////////////////////////////
		// Methods.
		/** Checks if the given (x, y, c) coordinate is within the range of
		(width, height, depth).
		@exception std::out_of_range	if (x, y) is outside of the current dimension, */
		void CheckRange(T x, T y, T c = 0) const;

		/** Checks if the given ROI is within the range. 
		@exception std::out_of_range	if the ROI is outside of the current dimension, */
		void CheckRange(const Point2D<T> &orgn, const Size2D<T> &sz) const;

		void CheckDepth(T depth) const;

		/** Gets the number of elements for the entire frame.
		total number of elements = depth x width x height */
		typename std::enable_if<std::is_integral<T>::value, T>::type
			GetNumElemPerFrame(void) const;

		/* This function makes sense for only BIP and BIL format. */
		/** Gets the number of elements per line.
		total number of elements / line = depth x width */
		typename std::enable_if<std::is_integral<T>::value, T>::type
			GetNumElemPerLine(void) const;

		// Need to be implemented depending on image format.
		/* This function is valid for only BIP format. */
		/** Gets the offset to the given position (x, y, c) as the number of elements. */
		typename std::enable_if<std::is_integral<T>::value, T>::type
			GetOffset(T x, T y, T c = 0) const;

		/** Data (defined in parent class, Size3D<T>)

		width: Number of pixels / line.
		height: Number of lines / frame.
		depth: Number of channels (elements) / pixel. */
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
		bool operator==(const Region<T, U> &rhs) const;
		bool operator!=(const Region<T, U> &rhs) const;

		/** Moves the origin by the given distance, and returns the result as a new
		Region<T, U> without changing this object. */
		Region<T, U> operator+(const Point2D<T> &dist) const;

		/** Zooms the size by the given zoom rate without moving the origin, and returns
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

	enum class ImageFormat {UNKNOWN, BIP, BSQ, BIL};

	template <typename T, typename ImageFormat F>
	class ImageSizeNew : public Size3D<T>
	{
	public:
		// Use constructors of base class instead of defining them here.
		void CheckDepth(T c) const;
		void CheckRange(const Point2D<T> &pt) const;
		void CheckRange(T x, T y) const;
		void CheckRange(const Region<T, T> &roi) const;
		void CheckRange(const Point2D<T> &orgn, const Size2D<T> &sz) const;
		T GetNumElem(void) const;
		T GetNumElemPerCh(void) const;
		T GetNumElemPerLine(void) const;
		T GetOffset(T x, T y, T c = 0) const;
	};

	//int ImageSizeNew<int, ImageFormat::BIP>::GetNumElemPerCh(void) const
	//{
	//	return 1;
	//}

	//template <typename T> template <>
	//T ImageSizeNew<T>::GetNumElemPerCh<ImageFormat::BSQ>(void) const
	//{
	//	return this->width * this->height;
	//}

	//template <typename T> template <>
	//T ImageSizeNew<T>::GetNumElemPerCh<ImageFormat::BIL>(void) const
	//{
	//	return 1;
	//}

	/** TODO: It makes sense to enable this class for only unsigned integer data types. */
	template <typename T>
	class ImageSizeBase : public Size3D<T>
	{
	public:
		// Use constructors of base class instead of defining them here.
		void CheckDepth(T c) const;
		void CheckRange(const Point2D<T> &pt) const;
		void CheckRange(T x, T y) const;
		void CheckRange(const Region<T, T> &roi) const;
		void CheckRange(const Point2D<T> &orgn, const Size2D<T> &sz) const;
		virtual T GetNumElem(void) const;
		virtual T GetOffset(T x, T y, T c = 0) const;
	};

	template <typename T>
	class ImageSizeBip : public ImageSizeBase<T>
	{
	public:
		// Use constructors of base class instead of defining them here.
		T GetNumElem(void) const;			// depth x width x height
		T GetNumElemPerLine(void) const;	// depth x width
		T GetOffset(T x, T y, T c = 0) const;
	};

	template <typename T>
	class ImageSizeBsq : public ImageSizeBase<T>
	{
	public:
		T GetNumElem(void) const;			// width x height x depth
		T GetNumElemPerCh(void) const;		// width x height
		T GetOffset(T x, T y, T c = 0) const;
	};

	template <typename T>
	class ImageSizeBil : public ImageSizeBase<T>
	{
	public:
		T GetNumElemPerLine(void) const;	// depth x width
		T GetOffset(T x, T y, T c = 0) const;
	};

}

#include "dimension_inl.h"

#endif
