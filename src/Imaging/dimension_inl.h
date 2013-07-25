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

	template <typename T>
	ImageSize<T> &ImageSize<T>::operator=(const ImageSize<T> &src)
	{
		this->Size3D<T>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.

	template <typename T>
	ImageSize<T>::ImageSize(T width, T height, T depth) : Size3D<T>(width, height, depth) {}

	////////////////////////////////////////////////////////////////////////////////////////
	// Methods.

	/** Throws an exception instead of returning false because you have to throw an
	exception at high level any way. */
	template <typename T>
	void ImageSize<T>::CheckRange(T x, T y, T c) const
	{
		if (x < 0 || x >= this->width || y < 0 || y >= this->height || c >= this->depth)
		{
			std::ostringstream errMsg;
			errMsg << "(" << x << ", " << y << ", " << c << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	// The end points are the excluding end of an ROI, so it could be up to (width, height).
	template <typename T>
	void ImageSize<T>::CheckRange(const Point2D<T> &orgn, const Size2D<T> &sz) const
	{
		Point2D<T> ptEnd = orgn + sz;
		if (orgn.x < 0 || ptEnd.x > this->width || orgn.y < 0 || ptEnd.y > this->height)
		{
			std::ostringstream errMsg;
			errMsg << "[" << orgn.x << ", " << orgn.y << "] ~ (" << ptEnd.x << ", " <<
				ptEnd.y << ") is out of range.";
			throw std::out_of_range(errMsg.str());
		}
	}

	template <typename T>
	void ImageSize<T>::CheckDepth(T depth) const
	{
		if (this->depth != depth)
			throw std::runtime_error("Depth is not matched.");
	}

	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		ImageSize<T>::GetNumElemPerFrame(void) const
	{
		return this->GetNumElemPerLine() * this->height;
	}

	/* This function makes sense for only BIP or BIL format. */
	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		ImageSize<T>::GetNumElemPerLine(void) const
	{
		return this->depth * this->width;
	}

	/* This function is valid for only BIP format. */
	/** Enabled for only integral data types. Otherwise, the logic doesn't make sense.  */
	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		ImageSize<T>::GetOffset(T x, T y, T c) const
	{
		return c + this->depth * x + this->GetNumElemPerLine() * y;
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
	bool Region<T, U>::operator==(const Region<T, U> &rhs) const
	{
		if (this->origin == rhs.origin && this->size == rhs.size)
			return true;
		else
			return false;
	}

	template <typename T, typename U>
	bool Region<T, U>::operator!=(const Region<T, U> &rhs) const
	{
		return !this->operator==(rhs);
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator+(const Point2D<T> &dist) const
	{
		Region<T, U> dst(*this);
		dst.Move(dist);
		return dst;
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator*(const Point2D<double> &zm) const
	{
		Region<T, U> dst(*this);
		dst.Zoom(zm);
		return dst;
	}

	template <typename T, typename U>
	Region<T, U> Region<T, U>::operator*(double zm) const
	{
		Region<T, U> dst(*this);
		dst.Zoom(zm);
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
		RoundAs(this->size * zm, this->size);
	}

	template <typename T, typename U>
	void Region<T, U>::Zoom(double zm)
	{
		RoundAs(this->size * zm, this->size);
	}
}
#endif
