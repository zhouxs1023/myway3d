/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __QUATERNION_H
#define __QUATERNION_H

#include "FastMath.h"
#include "Macros.h"
#include "Vector.h"
#include "Matrix4.h"
#include "SourceDeclaration.h"
#include "Algorithms.h"

namespace MCore
{

/**
 * A quaternion template.
 * Quaternions are mostly used to represent rotations in 3D applications.
 * The advantages of quaternions over matrices are that they take up less space and that interpolation between
 * two quaternions is easier to perform. Instead of a 3x3 rotation matrix, which is 9 floats or doubles, a quaternion
 * only uses 4 floats or doubles. This template/class provides you with methods to perform all kind of operations on
 * these quaternions, from interpolation to conversion to matrices and other rotation representations.
 */
template <class T>
class TQuaternion
{
	DECLARE_CLASS(Quaternion<>)

	public:
		/**
		 * Default constructor. Sets x, y and z to 0 and w to 1.
		 */
		inline TQuaternion<T>()															: x(0), y(0), z(0), w(1.0) {}

		/**
		 * Constructor which sets the x, y, z and w.
		 * @param xVal The value of x.
		 * @param yVal The value of y.
		 * @param zVal The value of z.
		 * @param wVal The value of w.
		 */
		inline TQuaternion<T>(const T xVal, const T yVal, const T zVal, const T wVal)	: x(xVal), y(yVal), z(zVal), w(wVal) {}

		/**
		 * Copy constructor. Copies the x, y, z, w values from the other quaternion.
		 * @param other The quaternion to copy the attributes from.
		 */
		inline TQuaternion<T>(const TQuaternion& other)									: x(other.x), y(other.y), z(other.z), w(other.w) {}

		/**
		 * Constructor which creates a quaternion from a pitch, yaw and roll.
		 * @param pitch Rotation around the x-axis, in radians.
		 * @param yaw Rotation around the y-axis, in radians.
		 * @param roll Rotation around the z-axis, in radians.
		 */
		inline TQuaternion<T>(const T pitch, const T yaw, const T roll)					{ SetEuler(pitch, yaw, roll); }

		/**
		 * Constructor which creates a quaternion from a spherical rotation.
		 * @param spherical The spherical coordinates in radians, which creates an axis to rotate around.
		 * @param angle The angle to rotate around this axis.
		 */
		TQuaternion<T>(const TVector2<T>& spherical, const T angle);

		/**
		 * Constructor which creates a quaternion from an axis and angle.
		 * @param axis The axis to rotate around.
		 * @param angle The angle in radians to rotate around the given axis.
		 */
		TQuaternion<T>(const TVector3<T>& axis, const T angle);

		/**
		 * Calculates the square length of the quaternion.
		 * @result The square length (length*length).
		 */
		inline T SquareLength() const													{ return (x*x + y*y + z*z + w*w); }

		/**
		 * Calculates the length of the quaternion.
		 * It's safe, since it prevents a division by 0.
		 * @result The length of the quaternion.
		 */
		inline T Length() const															{ T sqLen=SquareLength(); if (sqLen==0) return 0; return Math::Sqrt(sqLen); }

		/**
		 * Performs a dot product on the quaternions.
		 * @param q The quaternion to multiply (dot product) this quaternion with.
		 * @result The quaternion which is the result of the dot product.
		 */
		inline T Dot(const TQuaternion<T>& q) const										{ return (x*q.x + y*q.y + z*q.z + w*q.w); }

		/**
		 * Normalize the quaternion.
		 * @result The normalized quaternion. It modifies itself, so no new quaternion is returned.
		 */
		inline TQuaternion<T>& Normalize()												{ T ooLen=1.0 / Length(); x*=ooLen; y*=ooLen; z*=ooLen; w*=ooLen; return *this; }

		/**
		 * Sets the quaternion to identity. Where x, y and z are set to 0 and w is set to 1.
		 * @result The quaternion, now set to identity.
		 */
		inline TQuaternion<T>& Identity()												{ x=y=z=0; w=1.0; return *this; }

		/**
		 * Calculate the inversed version of this quaternion.
		 * @result The inversed version of this quaternion.
		 */
		inline TQuaternion<T>& Inverse()												{ T len=1.0 / SquareLength(); x=-x*len; y=-y*len; z=-z*len; w=w*len; return *this; }

		/**
		 * Conjugate this quaternion.
		 * @result Returns itself Conjugated.
		 */
		inline TQuaternion<T>& Conjugate()												{ x=-x; y=-y; z=-z; return *this; }

		/**
		 * Calculate the inversed version of this quaternion.
		 * @result The inversed version of this quaternion.
		 */
		inline TQuaternion<T> Inversed() const											{ T len=1.0 / SquareLength(); return TQuaternion<T>(-x*len, -y*len, -z*len, w*len); }

		/**
		 * Returns the normalized version of this quaternion.
		 * @result The normalized version of this quaternion.
		 */
		inline TQuaternion<T> Normalized()	const										{ T ooLen=1.0 / Length(); return TQuaternion<T>(x*ooLen, y*ooLen, z*ooLen, w*ooLen); }

		/**
		 * Return the conjugated version of this quaternion.
		 * @result The conjugated version of this quaternion.
		 */
		inline TQuaternion<T> Conjugated() const										{ return TQuaternion<T>(-x, -y, -z, w); }

		/**
		 * Calculate the exponent of this quaternion.
		 * @result The resulting quaternion of the exp.
		 */
		inline TQuaternion<T> Exp() const												{ T r=Math::Sqrt(x*x + y*y + z*z); T expW=Math::Exp(w); T s=r>=0.00001 ? expW*Math::Sin(r)/r : 0; return TQuaternion<T>(s*x, s*y, s*z, expW * Math::Cos(r)); }

		/**
		 * Calculate the log of the quaternion.
		 * @result The resulting quaternion of the log.
		 */
		inline TQuaternion<T> LogN() const												{ T r=Math::Sqrt(x*x + y*y + z*z); T t=r>0.00001 ? Math::ATan2(r, w)/r : 0;	return TQuaternion<T>(t*x, t*y, t*z, 0.5*Math::Log(SquareLength())); }
	
		/**
		 * Setup the quaternion from a pitch, yaw and roll.
		 * @param pitch The rotation around the x-axis, in radians.
		 * @param yaw The rotation around the y-axis, in radians.
		 * @param roll, The rotation around the z-axis in radians.
		 * @result The quaternion, now initialized with the given pitch, yaw, roll rotation.
		 */
		TQuaternion<T>& SetEuler(const T pitch, const T yaw, const T roll);

		/**
		 * Convert the quaternion to an axis and angle. Which represents a rotation of the resulting angle around the resulting axis.
		 * @param axis Pointer to the vector to store the axis in.
		 * @param angle Pointer to the variable to store the angle in (will be in radians).
		 */
		void ToAxisAngle(TVector3<T>* axis, T* angle) const;

		/**
		 * Convert the quaternion to a spherical rotation.
		 * @param spherical A pointer to the 2D vector to store the spherical coordinates in radians, which build the axis.
		 * @param angle The pointer to the variable to store the angle around this axis in radians.
		 */
		void ToSpherical(TVector2<T>* spherical, T* angle) const;

		/**
		 * Extract the euler angles in radians.
		 * The x component of the resulting vector represents the rotation around the x-axis (pitch).
		 * The y component results the rotation around the y-axis (yaw) and the z component represents
		 * the rotation around the z-axis (roll).
		 * @result The 3D vector containing the euler angles in radians, around each axis.
		 */
		TVector3<T>	ToEuler() const;

		/**
		 * Convert this quaternion into a matrix.
		 * @result The matrix representing the rotation of this quaternion.
		 */
		TMatrix<T> ToMatrix() const;

		/**
		 * Setup the quaternion from a given matrix.
		 * @param m The matrix to extract the rotation from.
		 * @result The quaternion, now containing the rotation of the given matrix, in quaternion form.
		 */
		static TQuaternion<T> FromMatrix(const TMatrix<T>& m);

		/**
		 * Linear interpolate between this and another quaternion.
		 * @param to The quaternion to interpolate towards.
		 * @param t The time value, between 0 and 1.
		 * @result The quaternion at the given time in the interpolation process.
		 */
		TQuaternion<T> Lerp(const TQuaternion<T>& to, T t) const;

		/**
		 * Spherical Linear interpolate between this and another quaternion.
		 * @param to The quaternion to interpolate towards.
		 * @param t The time value, between 0 and 1.
		 * @result The quaternion at the given time in the interpolation process.
		 */
		TQuaternion<T> Slerp(const TQuaternion<T>& to, const T t) const;

		/**
		 * Spherical cubic interpolate.
		 * @param p The first quaternion.
		 * @param a The second quaternion.
		 * @param b The third quaternion.
		 * @param q The fourth quaternion.
		 * @param t The time value, between 0 and 1.
		 * @result The quaternion at the given time in the interpolation process.
		 */
		static TQuaternion<T> Squad(const TQuaternion<T>& p, const TQuaternion<T>& a, const TQuaternion<T>& b, const TQuaternion<T>& q, const T t);

		// operators
		inline TMatrix<T>				operator=(const TMatrix<T>& m)										{ return ToMatrix(); }
		inline const TQuaternion<T>&	operator=(const TQuaternion<T>& other)								{ x=other.x; y=other.y; z=other.z; w=other.w; return *this; }
		inline TQuaternion<T>			operator-() const													{ return TQuaternion<T>(-x, -y, -z, -w); }
		inline const TQuaternion<T>&	operator+=(const TQuaternion<T>& q)									{ x+=q.x; y+=q.y; z+=q.z; w+=q.w; return *this; }
		inline const TQuaternion<T>&	operator-=(const TQuaternion<T>& q)									{ x-=q.x; y-=q.y; z-=q.z; w-=q.w; return *this; }
		inline const TQuaternion<T>&	operator*=(const float f)											{ x*=f; y*=f; z*=f; w*=f; return *this; }
		inline const TQuaternion<T>&	operator*=(const double f)											{ x*=f; y*=f; z*=f; w*=f; return *this; }

		TVector3<T>						operator*(const TVector3<T>& p);									// multiply a vector by a quaternion
		TQuaternion<T>					operator/(const TQuaternion<T>& q);									// returns the ratio of two quaternions

		// attributes
		T	x, y, z, w;
};


// operators
template <class T> inline TQuaternion<T>	operator*(const TQuaternion<T>& a, const float f)				{ return TQuaternion<T>(a.x*f, a.y*f, a.z*f, a.w*f); }
template <class T> inline TQuaternion<T>	operator*(const float f, const TQuaternion<T>& b)				{ return TQuaternion<T>(f*b.x, f*b.y, f*b.z, f*b.w); }
template <class T> inline TQuaternion<T>	operator*(const TQuaternion<T>& a, const double f)				{ return TQuaternion<T>(a.x*f, a.y*f, a.z*f, a.w*f); }
template <class T> inline TQuaternion<T>	operator*(const double f, const TQuaternion<T>& b)				{ return TQuaternion<T>(f*b.x, f*b.y, f*b.z, f*b.w); }
template <class T> inline TQuaternion<T>	operator+(const TQuaternion<T>& a, const TQuaternion<T>& b)		{ return TQuaternion<T>(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
template <class T> inline TQuaternion<T>	operator-(const TQuaternion<T>& a, const TQuaternion<T>& b)		{ return TQuaternion<T>(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }
template <class T> inline TQuaternion<T>	operator*(const TQuaternion<T>& a, const TQuaternion<T>& b)		{ return TQuaternion<T>(a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,	a.w*b.y + a.y*b.w + a.z*b.x - a.x*b.z,	a.w*b.z + a.z*b.w + a.x*b.y - a.y*b.x,	a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z); }

// include the template code
#include "Quaternion.inl"

// types
typedef TQuaternion<MReal>		Quaternion;
typedef TQuaternion<float>		FQuaternion;
typedef TQuaternion<double>		DQuaternion;

} // namespace MCore

#endif