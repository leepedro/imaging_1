#if !defined(UTILITIES_H)
#define UTILITIES_H

#include <array>
#include <algorithm>

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// SafeCast
	
	/** Casts source values to given destination data type while throwing an exception if
	an integer overflow was about to happen.

	The compiler warning messages for narrower conversions are silenced by static_cast.

	static_cast<T> is implemented even for the cases that an implicit conversion can work.
	
	The destination value remains unchanged if an exception was thrown.
	Q. Would it matter any way?

	@param [in] src
	@param [out] dst
	@exception std::overflow_error	if source value is negative value while destination 

	Two kinds of integer overflow risks are possible.

	A) Negative integer overflow risk
	Problem:
	A negative value can be misunderstood as a positive value by misinterpreting the sign 
	bit if
	 1) signed -> unsigned
	Solution:
	Check if source value is negative or not.

	B) Positive integer overflow risk
	Problem:
	An extremely high value can be rolled over if
	 1) integral -> integral with narrower data width
	 2) unsigned integral -> signed integral with the same data width, which is practically 
	 narrower data width
	Solution:
	Check if the source value is bigger than maximum limit of destination type.

	These two kinds of risks create four possible scenarios.
	1) Negative risk only. (A & ~B)
	2) Positive risk only. (~A & B)
	3) Both negative and positive risks. (A & B)
	4) No risk. (~A & ~B)
	If source and destination data types are identical, it belongs to the no risk case by
	the logic.

	These four scenarios are exclusively defined by enabling return values using type trait 
	classes. This approach minimizes run-time overhead by implementing only the required 
	check routines for given conditions. The conditions of type trait classes are computed
	during compile-time instead of run-time. We may achieve the similar result with function 
	template specialization, but we have to explicitly define them for each data type.
	*/

	// 1) Negative risk only. (A & ~B)
	template <typename T, typename U>
	typename std::enable_if<
		(std::is_signed<T>::value && std::is_unsigned<U>::value) &&
		!(std::is_integral<T>::value && std::is_integral<U>::value && ((sizeof(T) > sizeof(U)) || 
		(std::is_unsigned<T>::value && std::is_signed<U>::value && sizeof(T) == sizeof(U)))),
		void>::type SafeCast(const T src, U &dst)
	{
		if (src < 0)
			throw std::overflow_error("Source value is negative.");
		else
			dst = static_cast<U>(src);
	}

	// 2) Positive risk only. (~A & B)
	template <typename T, typename U>
	typename std::enable_if<
		!(std::is_signed<T>::value && std::is_unsigned<U>::value) &&
		(std::is_integral<T>::value && std::is_integral<U>::value && ((sizeof(T) > sizeof(U)) || 
		(std::is_unsigned<T>::value && std::is_signed<U>::value && sizeof(T) == sizeof(U)))),
		void>::type SafeCast(const T src, U &dst)
	{
		if (src > static_cast<T>(std::numeric_limits<U>::max()))
			throw std::overflow_error("Source value is too high.");
		else
			dst = static_cast<U>(src);
	}

	// 3) Both negative and positive risks. (A & B)
	template <typename T, typename U>
	typename std::enable_if<
		(std::is_signed<T>::value && std::is_unsigned<U>::value) &&
		(std::is_integral<T>::value && std::is_integral<U>::value && ((sizeof(T) > sizeof(U)) || 
		(std::is_unsigned<T>::value && std::is_signed<U>::value && sizeof(T) == sizeof(U)))),
		void>::type SafeCast(const T src, U &dst)
	{
		if (src < 0)
			throw std::overflow_error("Source value is negative.");
		else if (src > static_cast<T>(std::numeric_limits<U>::max()))
			throw std::overflow_error("Source value is too high.");
		else
			dst = static_cast<U>(src);
	}

	// 4) No risk. (~A & ~B)
	template <typename T, typename U>
	typename std::enable_if<
		!(std::is_signed<T>::value && std::is_unsigned<U>::value) &&
		!(std::is_integral<T>::value && std::is_integral<U>::value && ((sizeof(T) > sizeof(U)) || 
		(std::is_unsigned<T>::value && std::is_signed<U>::value && sizeof(T) == sizeof(U)))),
		void>::type SafeCast(const T src, U &dst)
	{
		dst = static_cast<U>(src);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions for std::array<T, N>

	/** Copies an std::array<> object to another one with different data types.

	This function template is enabled for only between the data types that implicit
	conversion is allowed.
	1) integer -> floating && src < dst
	2) floating -> floating && src <= dst
	2) integer -> integer && {u -> u || s -> s} && src <= dst */
	template <typename T, typename U, ::size_t N>
	typename std::enable_if<	
		(std::is_integral<T>::value && std::is_floating_point<U>::value && sizeof(T) < sizeof(U)) ||
		(std::is_floating_point<T>::value && std::is_floating_point<U>::value &&
		sizeof(T) <= sizeof(U)) ||
		(std::is_integral<T>::value && std::is_integral<U>::value &&
		((std::is_unsigned<T>::value && std::is_unsigned<U>::value) ||
		(std::is_signed<T>::value && std::is_signed<U>::value)) &&
		sizeof(T) <= sizeof(U)), void>::type
	Copy(const std::array<T, N> &src, std::array<U, N> &dst)
	{
		std::copy(src.cbegin(), src.cend(), dst.begin());
	}

	/** Rounds off an std::array<T, N> from floating point data type to a given data type.

	This function template implements the 'round-off from zero' algorithm.
	*/
	template <typename T, typename U, ::size_t N>
	typename std::enable_if<std::is_floating_point<T>::value, void>::type
	RoundAs(const std::array<T, N> &src, std::array<U, N> &dst);

	// Add a safe add function ?

	/** Adds two std::array<T, N> objects of the same data type and length into another
	std::array<U, N> object of a different (or same) data type. */
	template <typename T, typename U, ::size_t N>
	void Add(const std::array<T, N> &a, const std::array<T, N> &b, std::array<U, N> &c);

	/** Multiplies an std::array<T, N> with a (double) scalar as c = a * b.

	@param [in] a
	@param [in] b
	@return c = a * b
	*/
	template <typename T, ::size_t N>
	std::array<double, N> Multiply(const std::array<T, N> &a, double b);

	/** Multiplies an std::array<T, N> with another std::array<double, N>.

	@param [in] a
	@param [in] b
	@return c = a * b
	*/
	template <typename T, ::size_t N>
	std::array<double, N> Multiply(const std::array<T, N> &a, const std::array<double, N> &b);

	/** Get p-norm of an std::<T, N> array.	

	@param [in] src
	@param [in] p	The order of the norm. Default = 2. Euclidean distance is a 2-norm,
	i.e., p = 2.
	@return The p-norm of a vector as a scalar.
	*/
	template <typename T, ::size_t N>
	double GetNorm(const std::array<T, N> &src, double p = 2.0);

	/** Normalizes an std::array<T, N>.

	@param [in] src
	@param [in] p	The order of the norm. Default = 2. Euclidean distance is a 2-norm,
	i.e., p = 2.
	@return The normalized vector as an std::array<double, N> object.
	*/
	template <typename T, ::size_t N>
	std::array<double, N> Normalize(const std::array<T, N> &src, double p = 2.0);
}

#include "utilities_inl.h"
#endif
