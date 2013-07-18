#if !defined(UTILITIES_INL_H)
#define UTILITIES_INL_H

namespace Imaging
{
	template <typename T, typename U>
	typename std::enable_if<
		std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, void>::type
		SafeAdd(const T &a, const U &b, T &c)
	{
		if (b > 0 && a > (std::numeric_limits<T>::max() - b))
			throw std::overflow_error("Result value is too high.");
		else if (b < 0 && a < (std::numeric_limits<T>::min() - b))
			throw std::overflow_error("Result value is too low.");
		SafeCast(a + b, c);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions for std::array<T, N>

	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const std::array<T, N> &b)
	{
		std::array<T, N> c;
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(), it_b = b.cbegin();
			it_c != it_c_end; ++it_a, ++it_b, ++it_c)
			*it_c = *it_a + *it_b;	// This part is not protected against overflow.
		return c;
	}

	/** <cmath> of C+11 supports std::round(), but VS2012 does not support it yet.
	The 'round-off from zero' algorithm, which is equivalent to the C+11 round(), is
	implemented using std::floor() and std::ceil(). */
	template <typename T, typename U, ::size_t N>
	typename std::enable_if<std::is_floating_point<T>::value, void>::type
		RoundAs(const std::array<T, N> &src, std::array<U, N> &dst)
	{
		auto it_dst = dst.begin(), it_dst_end = dst.end();
		for (auto it_src = src.cbegin(); it_dst != it_dst_end; ++it_src, ++it_dst)
#if _MSC_VER > 1700	// from C+11
			SafeCast(std::round(*it_src), *it_dst);
#else				// up to VS2012
			if (*it_src >= 0)
				SafeCast(std::floor(*it_src + 0.5), *it_dst);
			else
				SafeCast(std::ceil(*it_src - 0.5), *it_dst);
#endif
	}

	/** This function template implicitly converts two source data into their widest one,
	and then copy the result into destination data, so S should be the same or wider than	
	both of T and U. */
	template <typename T, typename U, typename S, ::size_t N>
	typename std::enable_if<sizeof(T) <= sizeof(S) && sizeof(U) <= sizeof(S), void>::type
		Add(const std::array<T, N> &a, const std::array<U, N> &b, std::array<S, N> &c)
	{
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(), it_b = b.cbegin();
			it_c != it_c_end; ++it_a, ++it_b, ++it_c)
			*it_c = *it_a + *it_b;
	}

	template <typename T, ::size_t N>
	std::array<double, N> Multiply(const std::array<T, N> &a, double b)
	{
		std::array<double, N> dst;
		auto it_dst = dst.begin(), it_dst_end = dst.end();
		for (auto it_a = a.cbegin(); it_dst != it_dst_end; ++it_a, ++it_dst)
			*it_dst_end = *it_a * b;
		return dst;
	}

	template <typename T, ::size_t N>
	std::array<double, N> Multiply(const std::array<T, N> &a, const std::array<double, N> &b)
	{
		std::array<double, N> dst;
		auto it_dst = dst.begin(),	it_dst_end = dst.end();
		for (auto it_a = a.cbegin(), it_b = b.cbegin();	it_dst != it_dst_end;
			++it_a, ++it_b, ++it_dst)
			*it_dst_end = *it_a * *it_b;
		return dst;
	}

	template <typename T, ::size_t N>
	double GetNorm(const std::array<T, N> &src, double p)
	{
		double sum(0.0);
		for (auto it = src.cbegin(), it_end = src.cend(); it != it_end; ++it)
			sum += ::pow(*it, p);
		return ::pow(sum, 1.0 / p);
	}

	template <typename T, ::size_t N>
	std::array<double, N> Normalize(const std::array<T, N> &src, double p)
	{
		double norm = GetNorm(src, p);
		return Multiply(src, 1 / norm);
	}
}

#endif
