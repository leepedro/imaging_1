#if !defined(COORDINATES_INL_H)
#define COORDINATES_INL_H

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// Point2D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Point2D<T>::Point2D(void) : x(at(0)), y(at(1)) {}

	template <typename T>
	Point2D<T>::Point2D(const Point2D &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 2>(src), Point2D<T>() {}
#else				// up to VS2012
		std::array<T, 2>(src), x(at(0)), y(at(1)) {}
#endif

	template <typename T>
	Point2D<T> &Point2D<T>::operator=(const Point2D<T> &src)
	{
		this->std::array<T, 2>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Point2D<T>::Point2D(T x, T y) : 
#if _MSC_VER > 1700	// from VS2013
		Point2D<T>()
#else				// up to VS2012
		x(at(0)), y(at(1))
#endif
	{
		this->x = x;
		this->y = y;
	}

	/** This is a risky implementation because if there is a member variable defined in this
	class they won't be copied from the source if the source is a derived class from
	std::array<T, N> instead of std::array<T, N>. */
	template <typename T>
	Point2D<T>::Point2D(const std::array<T, 2> &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 2>(src), Point2D<T>() {}
#else				// up to VS2012
		std::array<T, 2>(src), x(at(0)), y(at(1)) {}
#endif

	////////////////////////////////////////////////////////////////////////////////////////
	// Point3D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Point3D<T>::Point3D(void) : x(at(0)), y(at(1)), z(at(2)) {}

	template <typename T>
	Point3D<T>::Point3D(const Point3D<T> &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 3>(src), Point3D<T>() {}
#else				// up to VS2012
		std::array<T, 3>(src), x(at(0)), y(at(1)), z(at(2)) {}
#endif

	template <typename T>
	Point3D<T> &Point3D<T>::operator=(const Point3D<T> &src)
	{
		this->std::array<T, 3>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Point3D<T>::Point3D(T x, T y, T z) :
#if _MSC_VER > 1700	// from VS2013
		Point3D<T>()
#else				// up to VS2012
		x(at(0)), y(at(1)), z(at(2))
#endif
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Size2D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Size2D<T>::Size2D(void) : width(at(0)), height(at(1)) {}

	template <typename T>
	Size2D<T>::Size2D(const Size2D &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 2>(src), Size2D<T>() {}
#else				// up to VS2012
		std::array<T, 2>(src), width(at(0)), height(at(1)) {}
#endif

	template <typename T>
	Size2D<T> &Size2D<T>::operator=(const Size2D<T> &src)
	{
		this->std::array<T, 2>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Size2D<T>::Size2D(T width, T height) : 
#if _MSC_VER > 1700	// from VS2013
		Size2D<T>()
#else				// up to VS2012
		width(at(0)), height(at(1))
#endif
	{
		this->width = width;
		this->height = height;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Size3D<T>

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Size3D<T>::Size3D(void) : width(at(0)), height(at(1)), depth(at(2)) {}

	template <typename T>
	Size3D<T>::Size3D(const Size3D<T> &src) :
#if _MSC_VER > 1700	// from VS2013	
		std::array<T, 3>(src), Size3D<T>() {}
#else				// up to VS2012
		std::array<T, 3>(src), width(at(0)), height(at(1)), depth(at(2)) {}
#endif

	template <typename T>
	Size3D<T> &Size3D<T>::operator=(const Size3D<T> &src)
	{
		this->std::array<T, 3>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Size3D<T>::Size3D(T width, T height, T depth) :
#if _MSC_VER > 1700	// from VS2013
		Size3D<T>()
#else				// up to VS2012
		width(at(0)), height(at(1)), depth(at(2))
#endif
	{
		this->width = width;
		this->height = height;
		this->depth = depth;
	}
}
#endif
