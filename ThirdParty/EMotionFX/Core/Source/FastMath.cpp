/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "FastMath.h"
#include "Macros.h"
#include <math.h>
#include <float.h>
#include <limits>
#include <cfloat>


namespace MCore
{

#ifdef HIGH_PRECISION
	const MReal Math::PI						= 3.141592653589793238462643383279502884197169;
	const MReal Math::TWO_PI					= 6.28318530717958647692;
	const MReal Math::HALF_PI					= 1.57079632679489661923;
	const MReal Math::INV_PI					= 0.31830988618;
	const MReal Math::DEGREECONVERTERCONSTANT	= 57.2957795130823208767;
	const MReal Math::RADCONVERTERCONSTANT		= 0.01745329251994329576;
	const MReal Math::EPSILON					= 0.00001;
	const MReal Math::SQRT_2					= 1.4142135623730950488016887242097;
	const MReal Math::HALF_SQRT_2				= 0.70710678118654752440084436210485;
	const MReal Math::SQRT_3					= 1.7320508075688772935274463415059;
	const MReal Math::HALF_SQRT_3				= 0.86602540378443864676372317075294;
//	const MReal Math::INFINITY					= DBL_MAX;
//	const MReal Math::NEG_INFINITY				= -DBL_MAX;
#else
	const MReal Math::PI						= 3.14159265358979323846f;
	const MReal Math::TWO_PI					= 6.28318530717958647692f;
	const MReal Math::HALF_PI					= 1.57079632679489661923f;
	const MReal Math::INV_PI						= 0.31830988618f;
	const MReal Math::DEGREECONVERTERCONSTANT	= 57.2957795130823208767f;
	const MReal Math::RADCONVERTERCONSTANT		= 0.01745329251994329576f;
	const MReal Math::EPSILON					= 0.00001f;
	const MReal Math::SQRT_2					= 1.4142135623730950488016887242097f;
	const MReal Math::HALF_SQRT_2				= 0.70710678118654752440084436210485f;
	const MReal Math::SQRT_3					= 1.7320508075688772935274463415059f;
	const MReal Math::HALF_SQRT_3				= 0.86602540378443864676372317075294f;
//	const MReal Math::INFINITY					= FLT_MAX;
//	const MReal Math::NEG_INFINITY				= -FLT_MAX;
#endif

}