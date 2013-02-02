/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "FastMath.h"


inline MReal Math::Floor(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return floor(x);
	#else
		return floorf(x);
	#endif
}


inline MReal Math::Abs(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return fabs(x);
	#else
		return fabsf(x);
	#endif
}


inline MReal Math::Ceil(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return ceil(x);
	#else
		return ceilf(x);
	#endif
}


inline MReal Math::RadiansToDegrees(const MReal rad)
{
	return (rad * Math::DEGREECONVERTERCONSTANT);
}


inline MReal Math::DegreesToRadians(const MReal deg)
{
	return (deg * RADCONVERTERCONSTANT);
}


inline MReal Math::Sin(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return sin(x);
	#else
		return sinf(x);
	#endif
}


inline MReal Math::Cos(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return cos(x);
	#else
		return cosf(x);
	#endif
}


inline MReal Math::Tan(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return tan(x);
	#else
		return tanf(x);
	#endif
}


inline MReal Math::ASin(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return asin(x);
	#else
		return asinf(x);
	#endif
}


inline MReal Math::ACos(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return acos(x);
	#else
		return acosf(x);
	#endif
}


inline MReal Math::ATan(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return atan(x);
	#else
		return atanf(x);
	#endif
}


inline MReal Math::ATan2(const MReal y, const MReal &x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return atan2(y, x);
	#else
		return atan2f(y, x);
	#endif
}



inline MReal Math::Exp(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return exp(x);
	#else
		return expf(x);
	#endif
}


inline MReal Math::Log(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return log(x);
	#else
		return logf(x);
	#endif
}


inline MReal Math::Pow(const MReal base,const MReal exponent)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return pow(base, exponent);
	#else
		return powf(base, exponent);
	#endif
}


inline MReal Math::Sqrt(const MReal x)
{
	#if defined(HIGH_PRECISION) || !defined(WIN32)
		return sqrt(x);
	#else
		return sqrtf(x);
	#endif
}


inline MReal Math::InvSqrt(const MReal x)
{
	return 1 / Sqrt(x);
}


inline MReal Math::InverseSqrt(const MReal x)
{
	return 1 / Sqrt(x);
}


inline long Math::NextPowerOfTwo(const long x)
{
	if (x<=0L		) return 0L;
	if (x<=1L		) return 1L;
	if (x<=2L		) return 2L;
	if (x<=4L		) return 4L;
	if (x<=8L		) return 8L;
	if (x<=16L	    ) return 16L;
	if (x<=32L	    ) return 32L;
	if (x<=64L 	    ) return 64L;
	if (x<=128L     ) return 128L;
	if (x<=256L     ) return 256L;
	if (x<=512L     ) return 512L;
	if (x<=1024L    ) return 1024L;
	if (x<=2048L    ) return 2048L;
	if (x<=4096L    ) return 4096L;
	if (x<=8192L    ) return 8192L;
	if (x<=16384L   ) return 16384L;
	if (x<=32768L   ) return 32768L;
	if (x<=65536L	) return 65536L;
	else			  return 0;
}