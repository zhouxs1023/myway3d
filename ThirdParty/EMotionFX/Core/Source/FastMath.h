/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FASTMATH_H
#define __FASTMATH_H

#include "Config.h"
#include "StandardHeaders.h"
#include "SourceDeclaration.h"
#include "Macros.h"


namespace MCore
{


/**
 * The math class.
 * All methods and attributes are static.
 */
class Math
{
	public:
		/**
		 * Calculates the floored version of a value.
		 * The floor function returns a value representing the largest
		 * integer that is less than or equal to x.
		 * Examples:
		 * The floor of 2.8 is 2.000000
		 * The floor of -2.8 is -3.000000
		 * @param x The value to floor.
		 * @return The value representing the largest integer that is less than or equal to x.
		 */
		static inline MReal Floor(const MReal x);

		/**
		 * Calculates the absolute value.
		 * The Abs function returns the absolute value of its parameter.
		 * Examples:
		 * The absolute value of -4 is 4
		 * The absolute value of -41567 is 41567
		 * The absolute value of -3.141593 is 3.141593
		 * @param x The value to calculate the absolute value from.
		 * @return The absolute value of the parameter.
		 */
		static inline MReal Abs(const MReal x);

		/**
		 * Calculates the ceiling of a value.
		 * The ceil function returns a value representing the smallest integer
		 * that is greater than or equal to x.
		 * Examples:
		 * The ceil of 2.8 is 3.000000
		 * The ceil of -2.8 is -2.000000
		 * @param The value to ceil.
		 * @return The value representing the smallest integer that is greater than or equal to x.
		 */
		static inline MReal Ceil(const MReal x);

		/**
		 * Radians to degree Conversion.
		 * @param rad The angle(rads) to convert.
		 * @return The converted angle in degrees.
		 */
		static inline MReal RadiansToDegrees(const MReal rad);

		/**
		 * Degree to radians Conversion.
		 * @param deg The angle (degrees) to convert.
		 * @return The converted angle in radians.
		 */
		static inline MReal DegreesToRadians(const MReal deg);

		/**
		 * Sine function.
		 * @param x Angle in radians.
		 * @return The sine of x.
		 */
		static inline MReal Sin(const MReal x);

		/**
		 * Cosine function.
		 * @param x Angle in radians.
		 * @return The cosine of x.
		 */
		static inline MReal Cos(const MReal x);

		/**
		 * Tangent function.
		 * @param x Angle in radians.
		 * @return The tangent of x.
		 */
		static inline MReal Tan(const MReal x);

		/**
		 * Arcsine function.
		 * @param x Angle in radians.
		 * @return The arcsine of x.
		 */
		static inline MReal ASin(const MReal x);

		/**
		 * Arccosine function.
		 * @param x Angle in radians.
		 * @return The arccosine of x.
		 */
		static inline MReal ACos(const MReal x);

		/**
		 * Arctangent function.
		 * @param x Angle in radians.
		 * @return The arctangent of x.
		 */
		static inline MReal ATan(const MReal x);

		/**
		 * Calculate the arctangent of y/x.
		 * @param y The value of y.
		 * @param x The value of x.
		 * @return The arctangent of y/x.
		 */
		static inline MReal ATan2(const MReal y, const MReal &x);

		/**
		 * Calculates the exponential.
		 * @param x The value.
		 * @return The exponential of x.
		 */
		static inline MReal Exp(const MReal x);

		/**
		 * Calculates logarithms.
		 * The log functions return the logarithm of x. If x is negative,
		 * these functions return an indefinite, by default. If x is 0, they
		 * return INF (infinite).
		 * @param x The value.
		 * @return The logarithm of x.
		 */
		static inline MReal Log(const MReal x);

		/**
		 * Calculates x raised to the power of y.
		 * @param base The base.
		 * @param exponent The exponent.
		 * @return X raised to the power of y.
		 */
		static inline MReal Pow(const MReal base,const MReal exponent);

		/**
		 * Calculates the square root.
		 * Parameter has to be nonnegative.
		 * @param x Nonnegative value.
		 * @return The square root.
		 */
		static inline MReal Sqrt(const MReal x);

		/**
		 * Calculates the inverse square root. 1 / Sqrt(x).
		 * Parameter has to be nonnegative.
		 * @param x Nonnegative value.
		 * @return The inverse square root.
		 */
		static inline MReal InvSqrt(const MReal x);

		/**
		 * Calculates the inverse square root. 1 / Sqrt(x).
		 * Parameter has to be nonnegative.
		 * @param x Nonnegative value.
		 * @return The inverse square root.
		 */
		static inline MReal InverseSqrt(const MReal x);

		/**
		 * Calculates the next power of two.
		 * @param x The value.
		 * @return The next power of two of the parameter.
		 */
		static inline long NextPowerOfTwo(const long x);


	public:
		static const MReal PI;
		static const MReal TWO_PI;
		static const MReal HALF_PI;
		static const MReal INV_PI;
		static const MReal EPSILON;
		static const MReal DEGREECONVERTERCONSTANT;
		static const MReal RADCONVERTERCONSTANT;
		static const MReal SQRT_2;
		static const MReal SQRT_3;
		static const MReal HALF_SQRT_2;
		static const MReal HALF_SQRT_3;
	//	static const MReal INFINITY;
	//	static const MReal NEG_INFINITY;
};


// include code
#include "FastMath.inl"

}	// namespace MCore


#endif