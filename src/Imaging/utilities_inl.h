#if !defined(UTILITIES_INL_H)
#define UTILITIES_INL_H

namespace Imaging
{
	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
		SafeAdd(T a, T b)
	{
		if (b > 0 && a > (std::numeric_limits<T>::max() - b))
			throw std::overflow_error("Result value is too high.");
		else if (b < 0 && a < (std::numeric_limits<T>::min() - b))
			throw std::overflow_error("Result value is too low.");
		return a + b;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::array<T, N>

	/** If any operation produces integer overflow, SafeAdd() function will throw an
	exception. */
	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const std::array<T, N> &b, std::array<T, N> &c)
	{
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(), it_b = b.cbegin();
			it_c != it_c_end; ++it_a, ++it_b, ++it_c)
			*it_c = SafeAdd(*it_a, *it_b);
	}

	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const std::array<T, N> &b)
	{
		std::array<T, N> c;
		Add(a, b, c);
		return c;
	}

	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const T &b, std::array<T, N> &c)
	{
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(); it_c != it_c_end; ++it_a, ++it_c)
			*it_c = SafeAdd(*it_a, b);
	}

	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const T &b)
	{
		std::array<T, N> c;
		Add(a, b, c);
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
			*it_dst = static_cast<U>(std::round(*it_src));
			//SafeCast(std::round(*it_src), *it_dst);
#else				// up to VS2012
			if (*it_src >= 0)
				*it_dst = static_cast<U>(std::floor(*it_src + 0.5));
				//SafeCast(std::floor(*it_src + 0.5), *it_dst);
			else
				*it_dst = static_cast<U>(std::ceil(*it_src - 0.5));
				//SafeCast(std::ceil(*it_src - 0.5), *it_dst);
#endif
	}

	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, double b, std::array<double, N> &c)
	{
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(); it_c != it_c_end; ++it_a, ++it_c)
			*it_c = *it_a * b;
	}

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, double b)
	{
		std::array<double, N> c;
		Multiply(a, b, c);
		return c;
	}

	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, const std::array<double, N> &b,
		std::array<double, N> &c)
	{
		auto it_b = b.cbegin();
		auto it_c = c.begin(), it_c_end = c.end();
		for (auto it_a = a.cbegin(); it_c != it_c_end; ++it_a, ++it_b, ++it_c)
			*it_c = *it_a * *it_b;
	}

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, const std::array<double, N> &b)
	{
		std::array<double, N> c;
		Multiply(a, b, c);
		return c;
	}

	template <typename T, ::size_t N>
	void Divide(const std::array<T, N> &a, double b, std::array<double, N> &c)
	{
		Multiply(a, 1.0 / b, c);
	}

	template <typename T, ::size_t N>
	std::array<double, N> operator/(const std::array<T, N> &a, double b)
	{
		std::array<double, N> c;
		Divide(a, b, c);
		return c;
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
		return src / norm;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::vector<T, N>
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src,
		typename std::vector<T>::size_type nElemPerLineSrc,
		typename std::vector<T>::iterator it_dst,
		typename std::vector<T>::size_type nElemPerLineDst,
		typename std::vector<T>::size_type nElemWidth,
		typename std::vector<T>::size_type nLines)
	{
		for (auto H = 0; H != nLines; ++H)
		{
			std::copy(it_src, it_src + nElemWidth, it_dst);
			it_src += nElemPerLineSrc;
			it_dst += nElemPerLineDst;
		}
	}

	// Implemented stdext::checked_array_iterator<> class to bypass C4996 warning.
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src, ::size_t nElemPerLineSrc,
		T *dst, ::size_t nElemPerLineDst, ::size_t nElemWidth, ::size_t nLines)
	{
		for (auto H = 0; H != nLines; ++H)
		{
			std::copy(it_src, it_src + nElemWidth,
				stdext::checked_array_iterator<T *>(dst, nElemPerLineDst));
			it_src += nElemPerLineSrc;
			dst += nElemPerLineDst;
		}
	}
}

#endif
