#if !defined(DIMENSION_H)
#define DIMENSION_H

#include <array>

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
		Dimension(const Dimension &src);
		Dimension &operator=(const Dimension &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Dimension(T w, T h, T d);

		////////////////////////////////////////////////////////////////////////////////////
		// Operators.
		bool operator==(const Dimension &src) const;
		bool operator!=(const Dimension &src) const;

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

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.

	template <typename T>
	Dimension<T>::Dimension(void) : width(0), height(0), depth(0) {}

	template <typename T>
	Dimension<T>::Dimension(const Dimension &src) :
		width(src.width), height(src.height), depth(src.depth) {}

	template <typename T>
	Dimension<T> &Dimension<T>::operator=(const Dimension &src)
	{
		this->width = src.width;
		this->height = src.height;
		this->depth = src.depth;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.

	template <typename T>
	Dimension<T>::Dimension(T w, T h, T d): width(w), height(h), depth(d) {}

	////////////////////////////////////////////////////////////////////////////////////////
	// Operators.

	template <typename T>
	bool Dimension<T>::operator==(const Dimension &src) const
	{
		return this->width == src.width && this->height == src.height &&
			this->depth == src.depth;
	}

	template <typename T>
	bool Dimension<T>::operator!=(const Dimension &src) const
	{
		return !(*this == src);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Methods.

	/** Throws an exception instead of returning false because you have to throw an
	exception at high level any way. */
	template <typename T>
	void Dimension<T>::CheckRange(T x, T y) const
	{
		if (x >= this->width || y >= this->height)
			throw std::out_of_range("(x,y) is out of range.");
	}

	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		Dimension<T>::GetNumElemPerFrame(void) const
	{
		return this->GetNumElemPerLine() * this->height;
	}

	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		Dimension<T>::GetNumElemPerLine(void) const
	{
		return this->depth * this->width;
	}

	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		Dimension<T>::GetOffset(T x, T y) const
	{
		return x * this->depth + y * this->width * this->depth;
	}

	template <typename T, typename U>
	class Region
	{
	public:
		// Data.
		//std::array<T, 2> x, y;
		//U width, height;
	};
}
#endif
