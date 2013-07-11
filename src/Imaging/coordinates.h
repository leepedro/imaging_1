#if !defined(COORDINATES_H)
#define COORDINATES_H

namespace Imaging
{
	/** Presents a 2-D Cartesian coordinate as (x, y). */
	template <typename T>
	class Cartesian2D : public std::array<T, 2>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Cartesian2D(void);
		Cartesian2D(const Cartesian2D<T> &src);
		Cartesian2D<T> &operator=(const Cartesian2D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Cartesian2D(T x, T y);
		//Cartesian2D(const std::array<T, 2> &src);
	
		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &x, &y;
	};

	/** Presents a 3-D Cartesian coordinate as (x, y, z). */
	template <typename T>
	class Cartesian3D : public std::array<T, 3>
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////
		// Default constructors.
		Cartesian3D(void);
		Cartesian3D(const Cartesian3D<T> &src);
		Cartesian3D<T> &operator=(const Cartesian3D<T> &src);

		////////////////////////////////////////////////////////////////////////////////////
		// Custom constructors.
		Cartesian3D(T x, T y, T z);
		//Cartesian3D(const std::array<T, 3> &src);
	
		////////////////////////////////////////////////////////////////////////////////////
		// Accessor.
		T &x, &y, &z;
	};

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Cartesian2D<T>::Cartesian2D(void) : x(at(0)), y(at(1)) {}

	template <typename T>
	Cartesian2D<T>::Cartesian2D(const Cartesian2D &src) :
		std::array<T, 2>(src), x(at(0)), y(at(1)) {}

	template <typename T>
	Cartesian2D<T> &Cartesian2D<T>::operator=(const Cartesian2D<T> &src)
	{
		this->std::array<T, 2>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Cartesian2D<T>::Cartesian2D(T x, T y) : x(at(0)), y(at(1))
	{
		this->x = x;
		this->y = y;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Default constructors.
	template <typename T>
	Cartesian3D<T>::Cartesian3D(void) : x(at(0)), y(at(1)), z(at(2)) {}

	template <typename T>
	Cartesian3D<T>::Cartesian3D(const Cartesian3D &src) :
		std::array<T, 3>(src), x(at(0)), y(at(1)), z(at(2)) {}

	template <typename T>
	Cartesian3D<T> &Cartesian3D<T>::operator=(const Cartesian3D<T> &src)
	{
		this->std::array<T, 3>::operator=(src);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Custom constructors.
	template <typename T>
	Cartesian3D<T>::Cartesian3D(T x, T y, T z) : x(at(0)), y(at(1)), z(at(2))
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
}
#endif
