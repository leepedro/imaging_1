#if !defined(UTILITIES_H)
#define UTILITIES_H

#include <array>
#include <algorithm>
#include <vector>

namespace Imaging
{
	////////////////////////////////////////////////////////////////////////////////////////
	// safe casting
	
	/** Casts source values to given destination data type while throwing an exception if
	an integer overflow happens.

	The compiler warning messages for narrower conversions are silenced by explicit
	conversion, i.e., static_cast<T>.
	Data loss is ignored by the explicit conversion.
	Explicit conversion is implemented even for the cases that an implicit conversion can
	work.
	
	@param [in] src
	@param [out] dst
	@exception std::overflow_error	if source value is negative value while destination
	data type is unsigned, or if source value is beyond the range of destination data type

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

	NO!!! This method is flawed because minimum float value is lower than minimum integer
	value, and maximum float value is higher than maximum integer value.
	*/

	// 1) Negative risk only. (A & ~B)
	template <typename T, typename U>
	typename std::enable_if<
		(std::is_signed<T>::value && std::is_unsigned<U>::value) &&
		!(std::is_integral<T>::value && std::is_integral<U>::value && ((sizeof(T) > sizeof(U)) || 
		(std::is_unsigned<T>::value && std::is_signed<U>::value && sizeof(T) == sizeof(U)))),
		void>::type SafeCast(const T &src, U &dst)
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
		void>::type SafeCast(const T &src, U &dst)
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
		void>::type SafeCast(const T &src, U &dst)
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
		void>::type SafeCast(const T &src, U &dst)
	{
		dst = static_cast<U>(src);
	}

	/** Casts an integer value to given another integer data type while throwing an
	exception if an integer overflow happens.

	Supports only integral type for both source and destination.
	The compiler warning messages for narrowing conversions are silenced by explicit
	conversion, i.e., static_cast<T>.
	Implicit conversion is used if it is sufficient.
	
	@exception std::overflow_error	if source value is beyond the range of destination data
	type

	Two kinds of integer overflow risks are possible.

	A) Negative integer overflow risk
	Problem:
	A negative value can be misunderstood as a positive value by misinterpreting the sign 
	bit if
	 1) signed -> unsigned with any conversion
	 2) signed -> signed with narrowing conversion
	Solution:
	Check if source value is smaller than the minimum limit of destination type.

	B) Positive integer overflow risk
	Problem:
	An extremely high value can be rolled over as a negative value if
	 1) any narrowing conversion
	 2) unsigned -> signed with same data width conversion
	Solution:
	Check if the source value is larger than maximum limit of destination type.

	These two kinds of risks create four possible scenarios.
	1) Negative risk only. (A & ~B)
	2) Positive risk only. (~A & B)
	3) Both negative and positive risks. (A & B)
	4) No risk. (~A & ~B)
	If source and destination data types are identical, it belongs to the no risk case by
	the logic.

	@Example
	int a;
	unsigned int b = SafeCast<unsigned int>(a);

	These four scenarios are exclusively defined by enabling return values using type trait 
	classes. This approach minimizes run-time overhead by implementing only the required 
	check routines for given conditions. The conditions of type trait classes are computed
	during compile-time instead of run-time. We may achieve the similar result with function 
	template specialization, but we have to explicitly define them for each data type. */
		
	// 1. Negative risk only. (A & ~B)
	template <typename T, typename U>
	typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
		(((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
		(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
		!((sizeof(U) > sizeof(T)) ||
		(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
		T>::type SafeCast_(U src)
	{
		if (src < static_cast<U>(std::numeric_limits<T>::min()))
			throw std::overflow_error("Source value is too low.");
		else
			return static_cast<T>(src);
	}

	// 2. Positive risk only. (~A & B)
	template <typename T, typename U>
	typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
		(!((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
		(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
		((sizeof(U) > sizeof(T)) ||
		(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
		T>::type SafeCast_(U src)
	{
		if (src > static_cast<U>(std::numeric_limits<T>::max()))
			throw std::overflow_error("Source value is too high.");
		else
			return static_cast<T>(src);
	}

	// 3. Both negative and positive risks. (A & B)
	template <typename T, typename U>
	typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
		(((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
		(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
		((sizeof(U) > sizeof(T)) ||
		(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
		T>::type SafeCast_(U src)
	{
		if (src < static_cast<U>(std::numeric_limits<T>::min()))
			throw std::overflow_error("Source value is too low.");
		else if (src > static_cast<U>(std::numeric_limits<T>::max()))
			throw std::overflow_error("Source value is too high.");
		else
			return static_cast<T>(src);
	}

	// 4. No risk. (~A & ~B)
	// Since there is no risk of integer overflow, implicit conversion is sufficient.
	//template <typename T, typename U>
	//typename std::enable_if<(std::is_integral<T>::value && std::is_integral<U>::value) &&
	//	(!((std::is_signed<U>::value && std::is_unsigned<T>::value) ||
	//	(std::is_signed<U>::value && std::is_signed<T>::value && sizeof(U) > sizeof(T))) &&
	//	!((sizeof(U) > sizeof(T)) ||
	//	(std::is_unsigned<U>::value && std::is_signed<T>::value && sizeof(U) == sizeof(T)))),
	//	T>::type SafeCast_(U src)
	//{
	//	return src;
	//}

	/** conversion from floating point to integer with integer overflow check
	
	Ignore data precision loss. */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value && std::is_integral<T>::value,
		T>::type SafeCast_(U src)
	{
		if (static_cast<U>(std::numeric_limits<T>::max()) < src)
			throw std::overflow_error("Source value is too high.");
		else if (static_cast<U>(std::numeric_limits<T>::min()) > src)
			throw std::overflow_error("Source value is too low.");
		else
			return static_cast<T>(src);
	}

	/** narrowing conversion from floating point to floating point with integer (?) overflow
	check

	std::numeric_limits<T>::min() for floating point data types return the minimum precision
	value instead of the negative minimum value. */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value &&
		std::is_floating_point<T>::value && (sizeof(U) > sizeof(T)),
		T>::type SafeCast_(U src)
	{
		if (static_cast<U>(std::numeric_limits<T>::max()) < src)
			throw std::overflow_error("Source value is too high.");
		else if (static_cast<U>(-std::numeric_limits<T>::max()) > src)
			throw std::overflow_error("Source value is too low.");
		else
			return static_cast<T>(src);
	}


	/** safe round off operations (floating point to integer)
	
	<cmath> of C+11 supports std::round(), but VS2012 does not support it yet.
	The 'round-off from zero' algorithm, which is equivalent to the C+11 round(), is
	implemented using std::floor() and std::ceil(). */
	template <typename T, typename U>
	typename std::enable_if<std::is_floating_point<U>::value && std::is_integral<T>::value,
		T>::type RoundAs(U src)
	{
#if _MSC_VER > 1700	// from C+11
		return SafeCast_<T>(std::round(src));
#else				// up to VS2012
		if (src >= 0)
			return SafeCast_<T>(std::floor(src + 0.5));
		else
			return SafeCast_<T>(std::ceil(src - 0.5));
#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// safe arithmetic operations

	////////////////////////////////////////////////////////////////////////////////////////
	/** Overflow prevention from arithmetic operations

	Problem: Data type is implicitly assumed as the widest one between the two sources.
	If the result of an operation exceeds the data range, overflow will happen.

	@Example
	int i_max = std::numeric_limits<int>::max();
	int i_1 = i_max + 1;		// overflowed
	long long l_1 = i_max + 1;	// overflowed because i_max + 1 is assumed as int
	long long l_temp = i_max;
	long long l_1 = l_temp + 1;	// not overflowed

	Solution 1:
	1) data_type(dst) < max_data_type(src_a, src_b)
	max_src_type temp = a + b (with checking)
	dst = temp (with checking)
	2) data_type(dst) == max_data_type(src_a, src_b)
	dst = a + b (with checking)	Q. Which one between a and b is the maximum data type?
	3) data_type(dst) > max_data_type(src_a, src_b)
	dst = dst_type(a) + dst_type(b)
	-> for 2) and 3),
	if dst >= a && dst >= b,
		dst = dst_type(a) + dst_type(b) (with checking)
	-> for 1)
	if ~(dst >= a && dst >= b),
		auto temp = a + b (with checking? how?)
		dst = static_cast<>(temp) (with checking)

	Solution 2: (selected!)
	Define arithmetic functions for only the same data types.
	Checking the value in real-time.
	Explicitly convert the variables before/after the operation as necessary. */
	////////////////////////////////////////////////////////////////////////////////////////

	/** Adds two values into one returns the sum while checking integer overflow.

	@exception std::overflow_error	if the result is below or beyond the range of
	destination data type */
	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, T>::type
		SafeAdd(T a, T b);

	////////////////////////////////////////////////////////////////////////////////////////
	/** Safe implicit conversion
	
	Problem: Only the following four widening conversion cases are truly safe during data
	type conversion.
	The rest of converion scenarios are vulnerable for overflow or data loss, so compiler
	throws a warning.
	1) integer -> floating && src < dst
	2) floating -> floating && src <= dst
	3) integer -> integer && {u -> u || s -> s} && src <= dst
	4) integer -> integer && {u -> s} && src < dst
	Generally [integer -> integer && {u -> s || s -> u} && src == dst] is allowed for
	implicit conversion by compilers, but there is a risk for overflow.

	Solution: Enable function or class for only these four cases with type traits.
	If other snecenarios, i.e., narrowing conversion, are really necessary, use
	static_cast<> and be aware of data loss and overflow. */
	////////////////////////////////////////////////////////////////////////////////////////
	

	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::array<T, N>

	/** Copies one array to another with the same or different data type.

	This function template is enabled for only the cases when implicit casting is possible
	without data loss or integer overflow risk.
	It can be used as an implicit casting method as well as a copying method. */
	/** Compiler gives a warning against int -> float, so it is disabled by enforcing
	src < dst in case 1.
	Since only implicitly allowed conversion is enabled, there is no need to use a safe
	casting method. */
	template <typename T, typename U, ::size_t N>
	typename std::enable_if<	
		(std::is_integral<T>::value && std::is_floating_point<U>::value && sizeof(T) < sizeof(U)) ||
		(std::is_floating_point<T>::value && std::is_floating_point<U>::value &&
		sizeof(T) <= sizeof(U)) ||
		(std::is_integral<T>::value && std::is_integral<U>::value &&
		((std::is_unsigned<T>::value && std::is_unsigned<U>::value) ||
		(std::is_signed<T>::value && std::is_signed<U>::value)) &&
		sizeof(T) <= sizeof(U)) ||
		(std::is_integral<T>::value && std::is_integral<U>::value &&
		std::is_unsigned<T>::value && std::is_signed<U>::value && sizeof(T) < sizeof(U)), void>::type
	Copy(const std::array<T, N> &src, std::array<U, N> &dst)
	{
		std::copy(src.cbegin(), src.cend(), dst.begin());
	}

	/** Adds two std::array<T, N> objects of the same data type and length into another
	std::array<U, N> object of the same data type
	
	@exception std::overflow_error	if the result of any element is below or beyond the
	range of the data type */
	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const std::array<T, N> &b, std::array<T, N> &c);

	/** Adds two std::array<T, N> objects of the same data type and length into another
	std::array<T, N> object of the same data type.

	@NOTE This operator can be practically used only within the namespace because we cannot
	use '+' without "using namespace Imaging". (Maybe it is better that way.)
	Another way is 'c = Imaging::operator+(a, b)', but function Add(a, b, c) is better in
	that way. */
	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const std::array<T, N> &b);

	/** Adds a scalar value into all elements of an std::array<T, N> object of the same data
	type.
	
	@exception std::overflow_error	if the result of any element is below or beyond the
	range of the data type */
	template <typename T, ::size_t N>
	void Add(const std::array<T, N> &a, const T &b, std::array<T, N> &c);

	template <typename T, ::size_t N>
	std::array<T, N> operator+(const std::array<T, N> &a, const T &b);

	/** Rounds off an std::array<T, N> from floating point data type to a given data type.
		
	This function template implements the 'round-off from zero' algorithm.
	It is also enabled only if source is a floating point data type. */
	template <typename T, typename U, ::size_t N>
	typename std::enable_if<std::is_floating_point<T>::value, void>::type
		RoundAs(const std::array<T, N> &src, std::array<U, N> &dst);

	/** Multiplies an std::array<T, N> with a (double) scalar, and saves the result into an
	std::array<double, N> object. */
	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, double b, std::array<double, N> &c);

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, double b);

	/** Multiplies an std::array<T, N> with an std::array<double, N>, and saves the result into an
	std::array<double, N> object. */
	template <typename T, ::size_t N>
	void Multiply(const std::array<T, N> &a, const std::array<double, N> &b,
		std::array<double, N> &c);

	template <typename T, ::size_t N>
	std::array<double, N> operator*(const std::array<T, N> &a, const std::array<double, N> &b);

	template <typename T, ::size_t N>
	void Divide(const std::array<T, N> &a, double b, std::array<double, N> &c);

	template <typename T, ::size_t N>
	std::array<double, N> operator/(const std::array<T, N> &a, double b);

	/** Get p-norm of an std::<T, N> array.	

	@param [in] src
	@param [in] p	The order of the norm. Default = 2. Euclidean distance is a p-2 norm.
	@return The p-norm of a vector as a scalar.	*/
	template <typename T, ::size_t N>
	double GetNorm(const std::array<T, N> &src, double p = 2.0);

	/** Normalizes an std::array<T, N>, and returns the normalized vector as an
	std::array<double, N> object.

	@param [in] src
	@param [in] p	The order of the norm. Default = 2. Euclidean distance is a 2-norm.
	@return The normalized vector as an std::array<double, N> object. */
	template <typename T, ::size_t N>
	std::array<double, N> Normalize(const std::array<T, N> &src, double p = 2.0);


	////////////////////////////////////////////////////////////////////////////////////////
	// Global functions and operators for std::vector<T>

	/** Copies repeated lines of data from an std::vector<T> to another.
	
	This function is usually used to copy an ROI of data where an image is stored in an
	std::vector<T>. */
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src,
		typename std::vector<T>::size_type nElemPerLineSrc,
		typename std::vector<T>::iterator it_dst,
		typename std::vector<T>::size_type nElemPerLineDst,
		typename std::vector<T>::size_type nElemWidth,
		typename std::vector<T>::size_type nLines);

	/** Copies */
	template <typename T>
	void CopyLines(typename std::vector<T>::const_iterator it_src, ::size_t nElemPerLineSrc,
		T *dst, ::size_t nElemPerLineDst, ::size_t nElemWidth, ::size_t nLines);
}

#include "utilities_inl.h"
#endif
