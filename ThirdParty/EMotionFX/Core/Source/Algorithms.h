/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __ALGORITHMS_H
#define __ALGORITHMS_H

#include "FastMath.h"
#include "StandardHeaders.h"
#include "SourceDeclaration.h"


namespace MCore
{


/**
 * Calculates the CRC value of a given byte.
 * It inputs and modifies the current CRC value passed as parameter.
 * @param byteValue The byte value to generate the CRC for.
 * @param CRC The CRC value to modify.
 *
 * The calculation performed is:
 * <pre>
 * CRC = ((CRC) >> 8) ^ MCore::CRC32Table[(byteValue) ^ ((CRC) & 0x000000FF)];
 * </pre>
 */
inline void CalcCRC32(unsigned char byteValue, unsigned int& CRC);


/**
 * Converts between big and little endian for an 32 bits integer.
 * This means the byte order gets inverted.
 * A byte order of (B1 B2 B3 B4) gets converted into (B4 B3 B2 B1).
 * @value The value to convert the endian type for. The value itself will be modified.
 */
inline void ConvertIntEndian(int& value);

/**
 * Converts between big and little endian for an 16 bits word.
 * This means the byte order gets inverted.
 * A byte order of (B1 B2) gets converted into (B2 B1).
 * @value The value to convert the endian type for. The value itself will be modified.
 */
inline void ConvertWordEndian(short int& value);

/**
 * Calculate the cube root, which basically is pow(x, 1/3).
 * This also allows negative and zero values.
 * @param x The number.
 * @result The cube root.
 */
inline MReal CubeRoot(const MReal x);


/**
 * Convert a linear interpolation weight value that is between 0 and 1 into a 
 * smoothed version of that value that is also between 0 and 1.
 * This can be used to easily convert a linear interpolation into smooth looking interpolation.
 * @param linearValue The linear weight value that is between 0 and 1.
 * @result The smoothed version of the linear value, also in range of 0..1.
 */
inline MReal CalcCosineInterpolationWeight(const MReal linearValue);

/**
 * Linear interpolate from source into target.
 * @param source The source value to start interpolating from.
 * @param target The target value.
 * @param timeValue A value between 0 and 1, where 0 results in the source and 1 results in the target value.
 * @result The interpolated value.
 */
template <class T>
inline T LinearInterpolate(const T& source, const T& target, const MReal timeValue);

/**
 * Cosine interpolate from source into target.
 * @param source The source value to start interpolating from.
 * @param target The target value.
 * @param timeValue A value between 0 and 1, where 0 results in the source and 1 results in the target value.
 * @result The interpolated value.
 */
template <class T>
inline T CosineInterpolate(const T& source, const T& target, const MReal timeValue);

/**
 * Calculate an interpolated value using barycentric coordinates.
 * Given three attributes and an u and v, which range from 0..1, we can calculate the interpolated attribute value at the given u, v position.
 * @param u The barycentric u coordinate, between 0 and 1.
 * @param v The barycentric v coordinate, between 0 and 1.
 * @param pointA The first attribute value (for example the first vertex normal of a triangle).
 * @param pointB The second attribute value (for example the second vertex normal of a triangle).
 * @param pointC The third attribute value (for example the third vertex normal of the triangle).
 * @result The interpolated attribute value, at the given u and v coordinate.
 */
template <class T>
inline T BarycentricInterpolate(const MReal u, const MReal v, const T& pointA, const T& pointB, const T& pointC);


/**
 * Swaps the two objects.
 * @param a The first object.
 * @param b The second object.
 */
template<class T>
inline void Swap(T& a, T& b);

/**
 * Swaps the two objects.
 * @param a The first object.
 * @param b The second object.
 */
template<class T>
inline void Exchange(T& a, T& b);

/**
 * Checks if x is in the range of [min, max].
 * @param x The value to check.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @return True if it is in the range, false if not.
 */
//template<class T>
//inline bool InRange(const T& x, const T& min, const T& max);

/**
 * Returns the smaller value of two objects.
 * @param a The first object.
 * @param b The second object.
 * @result The smaller value of the two.
 */
template<class T>
inline T Min(const T& a, const T& b);

/**
 * Returns the greater value of two objects.
 * @param a The first object.
 * @param b The second object.
 * @result The greater value of the two.
 */
template<class T>
inline T Max(const T& a, const T& b);

/**
 * Returns the smaller value of three objects.
 * @param a The first object.
 * @param b The second object.
 * @param c The third object.
 * @result The smaller value of the three.
 */
template<class T>
inline T Min3(const T& a, const T& b, const T& c);

/**
 * Returns the greater value of three objects.
 * @param a The first object.
 * @param b The second object.
 * @param c The third object.
 * @result The greater value of the three.
 */
template<class T>
inline T Max3(const T& a, const T& b, const T& c);

/*
// also defined int Math.h so pay attention.
template<class T> inline T Abs( const T A )
{
	return (A>=(T)0) ? A : -A;
}

template<class T> T Lerp( T& A, T& B, FLOAT Alpha )
{
	return (T)(A + Alpha * (B-A));
}
*/

template<class T> 
inline T Sgn(const T A);

/**
 * Calculates the square of the value.
 * @param x The value to square.
 * @return The square.
 */
template<class T>
inline T Square(const T x);

/**
 * Calculates the square of the value.
 * @param x The value to square.
 * @return The square.
 */
template<class T>
inline T Sqr(const T x);

/**
 * Returns true if the value is negative, false if not.
 * @param x The value to check.
 * @result True if the value is negative, false if not.
 */
template<class T>
inline bool IsNegative(const T x);

/**
 * Returns true if the value is negative, false if not.
 * @param x The value to check.
 * @result True if the value is negative, false if not.
 */
template<class T>
inline bool IsPositive(const T x);

/**
 * Modifies x if the object is lower than min or greater
 * than max and returns the object.
 * @param x The object to check.
 * @param min The minimal value.
 * @param max The maximal value.
 * @return The modified object.
 */
template<class T>
inline T Clamp(T x, const T& min, const T& max);

/**
 * Returns true if the value is greater than low and lower than high, false if not.
 * @param x The value to check.
 * @result True if the value is greater than low and lower than high, false if not.
 */
template<class T>
inline bool InRange(const T& x, const T& low, const T& high);

/**
 * Performs an operation operation for each element in a container [ begin, end - 1 ].
 * @param begin An iterator that specifies the first element of the container.
 * @param end An iterator that specifies the element after the last element of the container.
 * @param operation The operation to perform on each element of the container.
 */
template< class Iterator, class Operation >
void ForEach(Iterator begin, Iterator end, Operation operation);

/**
 * Finds a value in a container[ begin, end - 1 ].
 * @param begin An iterator that specifies the first element of the container.
 * @param end An iterator that specifies the element after the last element of the container.
 * @param value The value to be found.
 * @result An iterator to the found element, or an iterator to end, if nothing was found.
 */
template< class Iterator, class ValueType >
Iterator Find(Iterator begin, Iterator end, const ValueType& value);

/**
 * Finds a value in a container [ begin, end - 1 ] using the predicate predicate.
 * A predicate is a function or a value that returns 0 if it is called for the desired value,
 * -1 if the desired value is smaller or +1 if the desired value is larger.
 * @param begin An iterator that specifies the first element of the container.
 * @param end An iterator that specifies the element after the last element of the container.
 * @param predicate The predicate used to find the element.
 * @result An iterator to the found element, or an iterator to end, if nothing was found.
 */
template< class Iterator, class ValueType, class Predicate >
Iterator FindPredicate(Iterator begin, Iterator end, Predicate predicate);

/**
 * Replaces all values oldValue by the value newValue in a container [ begin, end - 1 ].
 * @param begin An iterator that specifies the first element of the container.
 * @param end An iterator that specifies the element after the last element of the container.
 * @param oldValue The value to be replaced.
 * @param newValue The value that will replace the old value.
 */
template< class Iterator, class ValueType >
void Replace(Iterator begin, Iterator end, const ValueType& oldValue, const ValueType& newValue);

/**
 * Replaces values by the value newValue in a container [ begin, end - 1 ] using the predicate
 * predicate.
 * A predicate is a function or a value that returns 0 if it is called for the desired value,
 * -1 if the desired value is smaller or +1 if the desired value is larger.
 * @param begin An iterator that specifies the first element of the container.
 * @param end An iterator that specifies the element after the last element of the container.
 * @param predicate The predicate used to find the values to be replaced.
 * @param newValue The value that will replace the old value.
 */
template< class Iterator, class ValueType, class Predicate >
void ReplacePredicate(Iterator begin, Iterator end, Predicate predicate, const ValueType& newValue);


// include inline code
#include "Algorithms.inl"

}	// namespace MCore


#endif