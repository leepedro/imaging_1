#if !defined(DIMENSION_INL_H)
#define DIMENSION_INL_H

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// ImageSize<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.

	template <typename T>
	ImageSize<T>::ImageSize(void) : Size3D<T>(0, 0, 0) {}

	template <typename T>
	ImageSize<T>::ImageSize(const ImageSize<T> &src) : Size3D<T>(src) {}
		//width(src.width), height(src.height), depth(src.depth) {}

	template <typename T>
	ImageSize<T> &ImageSize<T>::operator=(const ImageSize<T> &src)
	{
		this->width = src.width;
		this->height = src.height;
		this->depth = src.depth;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.

	template <typename T>
	ImageSize<T>::ImageSize(T width, T height, T depth): Size3D<T>(width, height, depth) {}

	////////////////////////////////////////////////////////////////////////////////////////
	// Operators.

	//template <typename T>
	//bool ImageSize<T>::operator==(const ImageSize &src) const
	//{
	//	return this->width == src.width && this->height == src.height &&
	//		this->depth == src.depth;
	//}

	//template <typename T>
	//bool ImageSize<T>::operator!=(const ImageSize &src) const
	//{
	//	return !(*this == src);
	//}

	////////////////////////////////////////////////////////////////////////////////////////
	// Methods.

	/** Throws an exception instead of returning false because you have to throw an
	exception at high level any way. */
	template <typename T>
	void ImageSize<T>::CheckRange(T x, T y) const
	{
		if (x >= this->width || y >= this->height)
			throw std::out_of_range("(x,y) is out of range.");
	}

	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		ImageSize<T>::GetNumElemPerFrame(void) const
	{
		return this->GetNumElemPerLine() * this->height;
	}

	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		ImageSize<T>::GetNumElemPerLine(void) const
	{
		return this->depth * this->width;
	}

	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		ImageSize<T>::GetOffset(T x, T y) const
	{
		return x * this->depth + y * this->width * this->depth;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.

	template <typename T, typename U>
	Region<T, U>::Region(void) {}

	template <typename T, typename U>
	Region<T, U>::Region(const Region<T, U> &src) :
		origin(src.origin), size(src.size) {}

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
	Region<T, U>::Region(const Point2D<T> &origin, const Size2D<U> &size) :
		origin(origin), size(size) {}

	// Delegation constructors are possible only from VS2013, so this won't work for now.
	template <typename T, typename U>
	Region<T, U>::Region(T x, T y, U width, U height) :
#if _MSC_VER > 1700	// from VS2013
		Region<T, U>(Point2D<T>(x, y), Size2D<U>(width, height)) {}	// for VS2013
#else				// up to VS2012
		origin(Point2D<T>(x, y)), size(Size2D<U>(width, height)) {}	// for VS2012
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Operators.

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator+(const Point2D<T> &dist) const
	{
		Region<T, U> dst(*this);
		Add(this->origin, dist, dst.origin);
		return dst;
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator*(const Point2D<double> &zm) const
	{
		Region<T, U> dst(*this);
		RoundAs(Multiply(this.size, zm), dst.size);
		return dst;
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator*(double zm) const
	{
		Region<T, U> dst(*this);
		RoundAs(Multiply(this.size, zm), dst.size);
		return dst;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Methods.

	template <typename T, typename U>
	void Region<T, U>::Move(const Point2D<T> &dist)
	{
		Add(this->origin, dist, this->origin);
	}


	template <typename T, typename U>
	void Region<T, U>::Zoom(const Point2D<double> &zm)
	{
		RoundAs(Multiply(this.size, zm), this->size);
	}

	template <typename T, typename U>
	void Region<T, U>::Zoom(double zm)
	{
		RoundAs(Multiply(this.size, zm), this->size);
	}
}
#endif
