#if !defined(DIMENSION_INL_H)
#define DIMENSION_INL_H

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.

	template <typename T>
	Dimension<T>::Dimension(void) : width(0), height(0), depth(0) {}

	template <typename T>
	Dimension<T>::Dimension(const Dimension<T> &src) :
		width(src.width), height(src.height), depth(src.depth) {}

	template <typename T>
	Dimension<T> &Dimension<T>::operator=(const Dimension<T> &src)
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


	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T, typename U>
	Region<T, U>::Region(void) : width(size.x), height(size.y) {}

	template <typename T, typename U>
	Region<T, U>::Region(const Region<T, U> &src) :
		width(size.x), height(size.y), origin(src.origin), size(src.size) {}

	template <typename T, typename U>
	Region<T, U> &Region<T, U>::operator=(const Region<T, U> &src)
	{
		this->origin = src.origin;
		this->size = src.size;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T, typename U>
	Region<T, U>::Region(const Cartesian2D<T> &origin, const Cartesian2D<U> &size) :
		width(size.x), height(size.y), origin(origin), size(size) {}

	// Delegation constructors are possible only from VS2013, so this won't work for now.
	template <typename T, typename U>
	Region<T, U>::Region(T x, T y, U width, U height) :
		Region<T, U>(Cartesian2D<T>(x, y), Cartesian2D<U>(width, height)) {}
}
#endif
