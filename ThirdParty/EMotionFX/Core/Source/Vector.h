/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __VECTOR_H
#define __VECTOR_H

#include "Macros.h"
#include "FastMath.h"
#include "Config.h"
#include "SourceDeclaration.h"
#include "Algorithms.h"


namespace MCore
{

/**
 * The 4D vector template/class, where the x, y, z and w attributes are public.
 */
template <class T>
class TVector4
{
	DECLARE_CLASS(TVector4<>)

	public:
		/**
		 * Default constructor. Does NOT initialize x, y, z, w.
		 */
		inline TVector4<T>()																	{}

		/**
		 * Constructor which sets the x, y, z and w.
		 * @param vx The value for x.
		 * @param vy The value for y.
		 * @param vz The value for z.
		 * @param vw The value for w.
		 */
		inline TVector4<T>(const T vx, const T vy, const T vz, const T vw)						: x(vx), y(vy), z(vz), w(vw){}

		/**
		 * Copy constructor.
		 * @param v The vector to copy the x, y, z and w from.
		 */
		inline TVector4<T>(const TVector4<T>& v)												: x(v.x), y(v.y), z(v.z), w(v.w) {}

		/**
		 * Set the values for x, y, z and w.
		 * @param vx The value for x.
		 * @param vy The value for y.
		 * @param vz The value for z.
		 * @param vw The value for w.
		 */
		inline void	Set(const T vx, const T vy, const T vz, const T vw)							{ x=vx; y=vy; z=vz; w=vw; }

		/**
		 * Perform a dot product between this vector and the specified one.
		 * The dot product works like this: (x*v.x + y*v.y + z*v.z + w*v.w) where v is the other vector.
		 * If both vectors are normalized, so have a length of 1 we will get the cosine angle (dotResult=Cos(angle)) between the two vectors. So to get the angle in radians, we do: ACos(dotResult).
		 * @param v The vector to perform the dot product with.
		 * @result The result of the dotproduct, which is as explained above, the cosine of the angle between the two vectors when both vectors are normalized (have a length of 1).
		 */
		inline T Dot(const TVector4<T> &v) const												{ return (x*v.x + y*v.y + z*v.z + w*v.w); }

		/**
		 * Calculates the square length of this vector.
		 * @result The square length of this vector (length*length).
		 */
		inline T SquareLength() const															{ return (x*x + y*y + z*z + w*w); }

		/**
		 * Calculates the length of this vector.
		 * @result The length of this vector.
		 */
		inline T Length() const																	{ return Math::Sqrt(SquareLength()); }

		/**
		 * Normalize this vector (make its length 1).
		 * Beware that the current length may not be 0! Because in that case we get a division by zero.
		 * @result The same vector, but now normalized.
		 */
		inline TVector4<T>&	Normalize()															{ const T len = 1.0/Length(); x*=len; y*=len; z*=len; w*=len; return *this; }

		/**
		 * Returns the normalized version of this vector (the current length may not be 0!).
		 * @result The normalized version of this vector.
		 */
		inline TVector4<T> Normalized() const													{ const T len = 1.0/Length(); return TVector4<T>(x*len, y*len, z*len, w*len); }

		// operators
		inline TVector4<T>			operator-() const											{ return TVector4<T>(-x, -y, -z, -w); }
		inline const TVector4<T>&	operator+=(const TVector4<T>& v)							{ x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }
		inline const TVector4<T>&	operator-=(const TVector4<T>& v)							{ x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; }
		inline const TVector4<T>&	operator*=(const float m)									{ x*=m; y*=m; z*=m; w*=m; return *this; }
		inline const TVector4<T>&	operator*=(const double m)									{ x*=m; y*=m; z*=m; w*=m; return *this; }
		inline const TVector4<T>&	operator=(const TVector4<T>& v)								{ x=v.x; y=v.y; z=v.z; w=v.w; return *this; }
		inline const TVector4<T>&	operator/=(const float d)									{ T iMul=1.0/d; x*=iMul; y*=iMul; z*=iMul; w*=iMul; return *this; }
		inline const TVector4<T>&	operator/=(const double d)									{ T iMul=1.0/d; x*=iMul; y*=iMul; z*=iMul; w*=iMul; return *this; }
		inline bool					operator==(const TVector4<T>& v)							{ return ((v.x==x) && (v.y==y) && (v.z==z) && (v.w==w)); }
		inline bool					operator!=(const TVector4<T>& v)							{ return ((v.x!=x) || (v.y!=y) || (v.z!=z) || (v.w!=w)); }

		inline T&					operator[](int row)											{ return ((T*)&x)[row]; }
		inline operator				T*()														{ return (T*)&x; }
		inline operator				const T*() const											{ return (const T*)&x; }

		inline friend TVector4<T>	operator* (const TVector4<T>& v, const TVector4<T>&	v2)		{ return TVector4<T>(v.x*v2.x, v.y*v2.y, v.z*v2.z, v.w*v2.w); }
		inline friend TVector4<T>	operator+ (const TVector4<T>& v, const TVector4<T>&	v2)		{ return TVector4<T>(v.x+v2.x, v.y+v2.y, v.z+v2.z, v.w+v2.w); }
		inline friend TVector4<T>	operator- (const TVector4<T>& v, const TVector4<T>&	v2)		{ return TVector4<T>(v.x-v2.x, v.y-v2.y, v.z-v2.z, v.w-v2.w); }
		inline friend TVector4<T>	operator* (const TVector4<T>& v, const float m)				{ return TVector4<T>(v.x*m, v.y*m, v.z*m, v.w*m); }
		inline friend TVector4<T>	operator* (const float m,		 const TVector4<T>&	v)		{ return TVector4<T>(m*v.x, m*v.y, m*v.z, m*v.w); }
		inline friend TVector4<T>	operator* (const TVector4<T>& v, const double m)			{ return TVector4<T>(v.x*m, v.y*m, v.z*m, v.w*m); }
		inline friend TVector4<T>	operator* (const double m,		 const TVector4<T>&	v)		{ return TVector4<T>(m*v.x, m*v.y, m*v.z, m*v.w); }
		inline friend TVector4<T>	operator/ (const TVector4<T>& v, const float d)				{ const T iMul=1.0/d; return TVector4<T>(v.x*iMul, v.y*iMul, v.z*iMul, v.w*iMul); }
		inline friend TVector4<T>	operator/ (const float d,		 const TVector4<T>&	v)		{ return TVector4<T>(d/v.x, d/v.y, d/v.z, d/v.w); }
		inline friend TVector4<T>	operator/ (const TVector4<T>& v, const double d)			{ const T iMul=1.0/d; return TVector4<T>(v.x*iMul, v.y*iMul, v.z*iMul, v.w*iMul); }
		inline friend TVector4<T>	operator/ (const double d,		 const TVector4<T>&	v)		{ return TVector4<T>(d/v.x, d/v.y, d/v.z, d/v.w); }

		// attributes
		T	x, y, z, w;
};



/**
 * The 3D vector template/class, where the x, y and z attributes are public.
 */
template <class T>
class TVector3
{
	DECLARE_CLASS(TVector3<>)

	public:
		/**
		 * Default constructor. Does not initialize any member attributes. So x, y and z contain unknown values.
		 */
		inline TVector3<T>()																	{ }

		/**
		 * Constructor which sets the x, y and z.
		 * @param vx The value for x.
		 * @param vy The value for y.
		 * @param vz The value for z.
		 */
		inline TVector3<T>(const T vx, const T vy, const T vz)									: x(vx), y(vy), z(vz)		{ }

		/**
		 * Copy constructor.
		 * @param v The vector to copy the x, y and z from.
		 */
		inline TVector3<T>(const TVector3<T>& v)												: x(v.x), y(v.y), z(v.z)	{ }

		/**
		 * Set the values for x, y and z.
		 * @param vx The value for x.
		 * @param vy The value for y.
		 * @param vz The value for z.
		 */
		inline void	Set(const T vx, const T vy, const T vz)										{ x=vx; y=vy; z=vz; }

		/**
		 * Perform a dot product between this vector and the specified one.
		 * The dot product works like this: (x*v.x + y*v.y + z*v.z) where v is the other vector.
		 * If both vectors are normalized, so have a length of 1 we will get the cosine angle (dotResult=Cos(angle)) between the two vectors. So to get the angle in radians, we do: ACos(dotResult).
		 * @param v The vector to perform the dot product with.
		 * @result The result of the dotproduct, which is as explained above, the cosine of the angle between the two vectors when both vectors are normalized (have a length of 1).
		 */
		inline T Dot(const TVector3<T> &v) const												{ return (x*v.x + y*v.y + z*v.z); }

		/**
		 * Perform the cross product between this vector and another specified one.
		 * The result of the cross product is the vector perpendicular to the other two.
		 * An easy way to understand how it works: This vector and the vector passed as parameter make a plane. The result will be the normal of this plane.
		 * @param v The other vector to do the cross product with (so the other vector on the plane).
		 * @result The result of the cross product (the normal of the plane).
		 */
		inline TVector3<T> Cross(const TVector3<T> &v) const									{ return TVector3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }

		/**
		 * Calculates the square length of this vector.
		 * @result The square length of this vector (length*length).
		 */
		inline T SquareLength() const															{ return (x*x + y*y + z*z); }

		/**
		 * Calculates the length of this vector.
		 * @result The length of this vector.
		 */
		inline T Length() const																	{ return Math::Sqrt(SquareLength()); }

		/**
		 * Normalize this vector (make its length 1).
		 * Beware that the current length may not be 0! Because in that case we get a division by zero.
		 * @result The same vector, but now normalized.
		 */
		inline TVector3<T>&	Normalize()															{ const T len = 1.0/Length();	x*=len; y*=len; z*=len; return *this; }

		/**
		 * Returns the normalized version of this vector (the current length may not be 0!).
		 * @result The normalized version of this vector.
		 */
		inline TVector3<T> Normalized() const													{ const T len = 1.0/Length(); return TVector3<T>(x*len, y*len, z*len); }

		/**
		 * Calculate the reflection vector.
		 * With reflection vector we mean this vector reflected along the specified normal.
		 * <pre>
		 * this  N    R
		 *  ^    ^    ^
		 *   \   |   /
		 *    \  |  /
		 *     \ | /
		 *      \|/
		 * --------------
		 * </pre>
		 * See above for how the reflection is calculated.
		 * 
		 * this = This vector, so pointing away from the surface. <br>
		 * N    = The normal of the surface (parameter 'n' in this method). <br>
		 * R    = The reflected vector, returned by this method.<br>
		 * 
		 * @param n The normal to use as center of reflection.
		 * @result The reflected version of this vector.
		 */
		inline TVector3<T> Reflect(const TVector3<T>& n) const									{ const T fac=2*(n.x*x+n.y*y+n.z*z); return TVector3<T>(fac*n.x-x,fac*n.y-y,fac*n.z-z); }

		// operators
		inline TVector3<T>			operator-() const											{ return TVector3<T>(-x, -y, -z); }
		inline const TVector3<T>&	operator+=(const TVector3<T>& v)							{ x+=v.x; y+=v.y; z+=v.z; return *this; }
		inline const TVector3<T>&	operator-=(const TVector3<T>& v)							{ x-=v.x; y-=v.y; z-=v.z; return *this; }
		inline const TVector3<T>&	operator*=(float m)											{ x*=m; y*=m; z*=m; return *this; }
		inline const TVector3<T>&	operator*=(double m)										{ x*=m; y*=m; z*=m; return *this; }
		inline const TVector3<T>&	operator=(const TVector3<T>& v)								{ x=v.x; y=v.y; z=v.z; return *this; }
		inline const TVector3<T>&	operator/=(float d)											{ const T iMul=1.0/d; x*=iMul; y*=iMul; z*=iMul; return *this; }
		inline const TVector3<T>&	operator/=(double d)										{ const T iMul=1.0/d; x*=iMul; y*=iMul; z*=iMul; return *this; }
		inline bool					operator==(const TVector3<T>& v)							{ return ((v.x==x) && (v.y==y) && (v.z==z)); }
		inline bool					operator!=(const TVector3<T>& v)							{ return ((v.x!=x) || (v.y!=y) || (v.z!=z)); }

		inline T&					operator[](int row)											{ return ((T*)&x)[row]; }
		inline operator				T*()														{ return (T*)&x; }
		inline operator				const T*() const											{ return (const T*)&x; }

		inline friend TVector3<T>	operator* (const TVector3<T>& v, const TVector3<T>&	v2)		{ return TVector3<T>(v.x*v2.x, v.y*v2.y, v.z*v2.z); }
		inline friend TVector3<T>	operator+ (const TVector3<T>& v, const TVector3<T>&	v2)		{ return TVector3<T>(v.x+v2.x, v.y+v2.y, v.z+v2.z); }
		inline friend TVector3<T>	operator- (const TVector3<T>& v, const TVector3<T>&	v2)		{ return TVector3<T>(v.x-v2.x, v.y-v2.y, v.z-v2.z); }
		inline friend TVector3<T>	operator* (const TVector3<T>& v, const float m)				{ return TVector3<T>(v.x*m, v.y*m, v.z*m); }
		inline friend TVector3<T>	operator* (const float m,		 const TVector3<T>&	v)		{ return TVector3<T>(m*v.x, m*v.y, m*v.z); }
		inline friend TVector3<T>	operator* (const TVector3<T>& v, const double m)			{ return TVector3<T>(v.x*m, v.y*m, v.z*m); }
		inline friend TVector3<T>	operator* (const double m,		 const TVector3<T>&	v)		{ return TVector3<T>(m*v.x, m*v.y, m*v.z); }
		inline friend TVector3<T>	operator/ (const TVector3<T>& v, const float d)				{ T iMul=1.0/d; return TVector3<T>(v.x*iMul, v.y*iMul, v.z*iMul); }
		inline friend TVector3<T>	operator/ (const float d,		 const TVector3<T>&	v)		{ return TVector3<T>(d/v.x, d/v.y, d/v.z); }
		inline friend TVector3<T>	operator/ (const TVector3<T>& v, const double d)			{ T iMul=1.0/d; return TVector3<T>(v.x*iMul, v.y*iMul, v.z*iMul); }
		inline friend TVector3<T>	operator/ (const double d,		 const TVector3<T>&	v)		{ return TVector3<T>(d/v.x, d/v.y, d/v.z); }


		// attributes
		T	x, y, z;
};



/**
 * The 2D vector template/class, where the x and y attributes are public.
 */
template <class T>
class TVector2
{
	DECLARE_CLASS(TVector2<>)

	public:
		/**
		 * Default constructor. Does not initialize any member attributes. So x and y contain unknown values.
		 */
		inline TVector2<T>()																	{}

		/**
		 * Constructor which sets the x and y values.
		 * @param vx The value for x.
		 * @param vy The value for y.
		 */
		inline TVector2<T>(const T vx, const T vy)												: x(vx), y(vy)		{}

		/**
		 * Copy constructor which copies the x and y value from the specified vector.
		 * @param v The vector to copy the x and y values from.
		 */
		inline TVector2<T>(const TVector2<T> &v)												: x(v.x), y(v.y)	{}

		/**
		 * Set the values for x and y.
		 * @param vx The value for x.
		 * @param vy The value for y.
		 */
		inline void	Set(const T vx, const T vy)													{ x=vx; y=vy; }

		/**
		 * Calculates the square length of this vector.
		 * @result The square length of this vector (length*length).
		 */
		inline T SquareLength() const															{ return (x*x + y*y); }

		/**
		 * Calculates the length of this vector.
		 * @result The length of this vector.
		 */
		inline T Length() const																	{ return Math::Sqrt(x*x + y*y); }

		/**
		 * Returns the cross product between this and another given vector.
		 * @param The other vector.
		 * @result The result of the cross product.
		 */
		inline T Cross(const TVector2<T> &v) const												{ return x*v.y - y*v.x;  }

		/**
		 * Perform a dot product between this vector and the specified one.
		 * The dot product works like this: (x*v.x + y*v.y) where v is the other vector.
		 * If both vectors are normalized, so have a length of 1 we will get the cosine angle (dotResult=Cos(angle)) between the two vectors. So to get the angle in radians, we do: ACos(dotResult).
		 * @param v The vector to perform the dot product with.
		 * @result The result of the dotproduct, which is as explained above, the cosine of the angle between the two vectors when both vectors are normalized (have a length of 1).
		 */
		inline T Dot(const TVector2<T> &v) const												{ return (x*v.x + y*v.y); }

		/**
		 * Normalize this vector (make its length 1).
		 * Beware that the current length may not be 0! Because in that case we get a division by zero.
		 * @result The same vector, but now normalized.
		 */
		inline TVector2<T>&	Normalize()															{ T l=Length();	 T ooL=1.0/l;	x*=ooL; y*=ooL; return *this; }

		/**
		 * Returns the normalized version of this vector (the current length may not be 0!).
		 * @result The normalized version of this vector.
		 */
		inline TVector2<T> Normalized() const													{ const T ooL=1.0/Length(); return TVector2<T>(x*ooL, y*ooL); }

		// operators
		inline bool					operator==(const TVector2<T>& vec)							{ return ((x==vec.x) && (y==vec.y)); }
		inline bool					operator!=(const TVector2<T>& vec)							{ return ((x!=vec.x) || (y!=vec.y)); }
		inline TVector2<T>			operator-() const											{ return TVector2<T>(-x, -y); }
		inline const TVector2<T>&	operator+=(const TVector2<T>& vec)							{ x+=vec.x; y+=vec.y; return *this; }
		inline const TVector2<T>&	operator-=(const TVector2<T>& vec)							{ x-=vec.x; y-=vec.y; return *this; }	
		inline const TVector2<T>&	operator*=(float s)											{ x*=s; y*=s; return *this; }
		inline const TVector2<T>&	operator*=(double s)										{ x*=s; y*=s; return *this; }
		inline const TVector2<T>&	operator/=(float d)											{ const T ooD=1.0/d; x*=ooD; y*=ooD; return *this; }
		inline const TVector2<T>&	operator/=(double d)										{ const T ooD=1.0/d; x*=ooD; y*=ooD; return *this; }
		inline const TVector2<T>&	operator=(const TVector2<T>& v)								{ x=v.x; y=v.y; return *this; }
		inline const TVector2<T>&	operator*=(const TVector2<T>& vec)							{ x*=vec.x; y*=vec.y; return *this; }

		inline T&					operator[](int row)											{ return ((T*)&x)[row]; }
		inline operator				T*()														{ return (T*)&x; }
		inline operator				const T*() const											{ return (const T*)&x; }

		inline friend TVector2<T>	operator* (const TVector2<T>& v1,	const TVector2<T>& v2)	{ return TVector2<T>(v1.x*v2.x, v1.y*v2.y); }
		inline friend TVector2<T>	operator+ (const TVector2<T>& v1,	const TVector2<T>& v2)	{ return TVector2<T>(v1.x+v2.x, v1.y+v2.y); }
		inline friend TVector2<T>	operator- (const TVector2<T>& v1,	const TVector2<T>& v2)	{ return TVector2<T>(v1.x-v2.x, v1.y-v2.y); }
		inline friend TVector2<T>	operator* (const TVector2<T>& v,	const float m)			{ return TVector2<T>(v.x*m, v.y*m); }
		inline friend TVector2<T>	operator* (const TVector2<T>& v,	const double m)			{ return TVector2<T>(v.x*m, v.y*m); }
		inline friend TVector2<T>	operator/ (const TVector2<T>& v,	const float d)			{ const T ooD=1.0/d; return TVector2<T>(v.x*ooD, v.y*ooD); }
		inline friend TVector2<T>	operator/ (const TVector2<T>& v,	const double d)			{ const T ooD=1.0/d; return TVector2<T>(v.x*ooD, v.y*ooD); }
		inline friend TVector2<T>	operator* (const float s,			const TVector2<T>& v)	{ return TVector2<T>(s*v.x, s*v.y); }
		inline friend TVector2<T>	operator* (const double s,			const TVector2<T>& v)	{ return TVector2<T>(s*v.x, s*v.y); }
		inline friend TVector2<T>	operator/ (const float d,			const TVector2<T>& v)	{ return TVector2<T>(d/v.x, d/v.y); }
		inline friend TVector2<T>	operator/ (const double d,			const TVector2<T>& v)	{ return TVector2<T>(d/v.x, d/v.y); }

		// attributes
		T	x, y;
};

typedef TVector4<MReal>		Vector4;
typedef TVector4<double>	DVector4;
typedef TVector4<int>		IVector4;
typedef TVector4<float>		FVector4;

typedef TVector3<MReal>		Vector3;
typedef TVector3<float>		FVector3;
typedef TVector3<double>	DVector3;
typedef TVector3<int>		IVector3;

typedef TVector2<MReal>		Vector2;
typedef TVector2<float>		FVector2;
typedef TVector2<double>	DVector2;
typedef TVector2<int>		IVector2;

}	// namespace MCore

#endif
