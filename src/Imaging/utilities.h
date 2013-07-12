#if !defined(UTILITIES_H)
#define UTILITIES_H

#include <array>

namespace Imaging
{
	// safe add

	// safe convert (?)

	////////////////////////////////////////////////////////////////////////////////////////
	// Add

	/** Adds two std::array<T, N> objects of the same data type and length into another
	std::array<U, N> of a different (or same) data type and the same length. */
	template <typename T, typename U, ::size_t N>
	void Add(const std::array<T, N> &a, const std::array<T, N> &b, std::array<U, N> &c);

	/** This function depends on implicit data type conversion, so the destination data type
	U must be the same or wider than the source data type T. */
	template <typename T, typename U, ::size_t N>
	void Add(const std::array<T, N> &a, const std::array<T, N> &b, std::array<U, N> &c)
	{
		auto it_a = a.cbegin();
		auto it_b = b.cbegin();
		auto it_c_end = c.end();
		for (auto it_c = c.begin(); it_c != it_c_end; ++it_a, ++it_b, ++it_c)
			*it_c = *it_a + *it_b;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// multiplication

	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, double b, std::array<T, N> &c);

	/** TODO: How do we check the sign of the result ? -> No need? */
	/** TODO: Should we round off? -> Yes. */
	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, double b, std::array<T, N> &c)
	{
		auto it_a = a.cbegin();
		auto it_c_end = c.end();
		for (auto it_c = c.begin(); it_c != it_c_end; ++it_a, ++it_c)
			*it_c = static_cast<T>(*it_a * b);
	}

}
#endif
