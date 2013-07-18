#if !defined(COORDINATES_H)
#define COORDINATES_H

#include <array>

namespace Imaging
{
	/** Presents a 2-D Cartesian coordinate as (x, y). */
	template <typename T>
	class Point2D : public std::array<T, 2>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Point2D(void);
		Point2D(const Point2D<T> &src);
		Point2D<T> &operator=(const Point2D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Point2D(T x, T y);
		//Point2D(const std::array<T, 2> &src);
	
		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &x, &y;
	};

	/** Presents a 3-D Cartesian coordinate as (x, y, z). */
	template <typename T>
	class Point3D : public std::array<T, 3>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Point3D(void);
		Point3D(const Point3D<T> &src);
		Point3D<T> &operator=(const Point3D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Point3D(T x, T y, T z);
		//Point3D(const std::array<T, 3> &src);
	
		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &x, &y, &z;
	};

	/** Presents the size of a 2-D Cartesian space as (width, height). */
	template <typename T>
	class Size2D : public std::array<T, 2>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Size2D(void);
		Size2D(const Size2D<T> &src);
		Size2D<T> &operator=(const Size2D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Size2D(T width, T height);

		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &width, &height;
	};

	/** Presents the size of a 3-D Cartesian space as (width, height, depth). */
	template <typename T>
	class Size3D : public std::array<T, 3>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Size3D(void);
		Size3D(const Size3D<T> &src);
		Size3D<T> &operator=(const Size3D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Size3D(T width, T height, T depth);

		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &width, &height, &depth;
	};
}

#include "coordinates_inl.h"

#endif
