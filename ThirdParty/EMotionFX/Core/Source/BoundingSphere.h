/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __BOUNDINGSPHERE_H
#define __BOUNDINGSPHERE_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include "Vector.h"
#include "Algorithms.h"


namespace MCore
{

/**
 * A 3D bounding sphere template.
 * This template represents a 3D bounding sphere, which has a center point and a radius.
 * This type of bounding volume can be used to speedup collision detection or (ray) intersection tests or visibility tests.
 * Default types are: BoundingSphere (Reals), FBoundingSphere (floats) and DBoundingSphere (doubles).
 * Reals can be floats or doubles, depending on if the #ifdef HIGH_PRECISION inside Macros.h is set or not.
 */
template <class T>
class TBoundingSphere
{
	DECLARE_CLASS(TBoundingSphere<>)

	public:
		/**
		 * Default constructor.
		 * Sets the sphere center to (0,0,0) and makes the radius 0.
		 */
		inline TBoundingSphere<T>()															: mCenter(TVector3<T>(0,0,0)), mRadius(0), mRadiusSq(0) {}
		
		/**
		 * Constructor which sets the center of the sphere and it's radius.
		 * Automatically calculates the squared radius too.
		 * @param pos The center position of the sphere.
		 * @param rad The radius of the sphere.
		 */
		inline TBoundingSphere<T>(const TVector3<T>& pos, const T& rad)						: mCenter(pos), mRadius(rad), mRadiusSq(rad*rad) {}

		/**
		 * Constructor which sets the center, radius and squared radius.
		 * Use this constructor when the squared radius is already known, so an extra multiplication is eliminated.
		 * @param pos The center position of the sphere.
		 * @param rad The radius of the sphere.
		 * @param radSq The squared radius of the sphere (rad*rad).
		 */
		inline TBoundingSphere<T>(const TVector3<T>& pos, const T& rad, const T& radSq)		: mCenter(pos), mRadius(rad), mRadiusSq(radSq)	{}

		/**
		 * Initialize the spheres center, radius and square radius.
		 * This will set the center position to (0,0,0) and both the radius and squared radius to 0.
		 * Call this method when you want to reset the sphere. Note that this is already done by the default constructor.
		 */
		inline void Init()																	{ mCenter.Set(0,0,0); mRadius=mRadiusSq=0; }

		/**
		 * Encapsulate a 3D point to the sphere.
		 * Automatically adjusts the radius of the sphere. Only use this method when the center of the sphere is already known.
		 * This is the faster way of encapsulating. But again, only use this method when the center of the sphere is known upfront and won't change.
		 * If the center of the sphere should automatically adjust as well, use the Encapsulate method instead (of course that's slower too).
		 * @param v The vector representing the 3D point to encapsulate (add) to the sphere.
		 */
		inline void EncapsulateFast(const TVector3<T>& v)									{ TVector3<T> diff=(mCenter - v);	T dist=diff.Dot(diff); if (dist>mRadiusSq) { mRadiusSq=dist; mRadius=Math::Sqrt(dist); }}

		/**
		 * Check if the sphere contains a given 3D point.
		 * Note that the border of the sphere is also counted as inside.
		 * @param v The vector representing the 3D point to perform the test with.
		 * @result Returns true when 'v' is inside the spheres volume, otherwise false is returned.
		 */
		inline bool Contains(const TVector3<T>& v)											{ return ((mCenter - v).SquareLength() <= mRadiusSq); }

		/**
		 * Check if the sphere COMPLETELY contains a given other sphere.
		 * Note that the border of the sphere is also counted as inside.
		 * @param s The sphere to perform the test with.
		 * @result Returns true when 's' is completely inside this sphere. False is returned in any other case.
		 */
		inline bool Contains(const TBoundingSphere<T>& s) const								{ return ((mCenter - s.mCenter).SquareLength() <= (mRadiusSq - s.mRadiusSq)); }

		/**
		 * Check if a given sphere intersects with this sphere.
		 * Note that the border of this sphere is counted as 'inside'. So if only the borders of the spheres intersects, this is seen as intersection.
		 * @param s The sphere to perform the intersection test with.
		 * @result Returns true when 's' intersects this sphere. So if it's partially or completely inside this sphere or if the borders overlap.
		 */
		inline bool Intersects(const TBoundingSphere<T>& s) const							{ return ((mCenter - s.mCenter).SquareLength() <= (mRadiusSq + s.mRadiusSq)); }

		/**
		 * Encapsulate a given 3D point with this sphere.
		 * Automatically adjust the center and radius of the sphere after 'adding' the given point to the sphere.
		 * Note that you should only use this method when the center of the bounding sphere isnt exactly known yet.
		 * This encapsulation method will adjust the center of the sphere as well. If the center of the sphere is known upfront and it is
		 * known upfront as well that this center point won't change, you should use the Encapsulate() method instead.
		 * @param v The vector representing the 3D point to use in the encapsulation.
		 */
		void Encapsulate(const TVector3<T>& v);

		/**
		 * Checks if this sphere completely contains a given Axis Aligned Bounding Box (AABB).
		 * Note that the border of the sphere is counted as 'inside'.
		 * @param 'b' The box to perform the test with.
		 * @result Returns true when 'b' is COMPLETELY inside the spheres volume, otherwise false is returned.
		 */
		bool Contains(const TAABB<T>& b) const;

		/**
		 * Checks if a given Axis Aligned Bounding Box (AABB) intersects this sphere.
		 * Note that the border of the sphere is counted as inside.
		 * @param 'b' The box to perform the test with.
		 * @result Returns true when 'b' is completely or partially inside the volume of this sphere.
		 */
		bool Intersects(const TAABB<T>& b) const;

		/**
		 * Get the radius of the sphere.
		 * @result Returns the radius of the sphere.
		 */
		inline const T&	GetRadius() const													{ return mRadius; }

		/**
		 * Get the squared radius of the sphere.
		 * @result Returns the squared radius of the sphere (no calculations done for this), since it's already known.
		 */
		inline const T&	GetRadiusSquared() const											{ return mRadiusSq; }

		/**
		 * Get the center of the sphere. So the position of the sphere.
		 * @result Returns the center position of the sphere.
		 */
		inline const TVector3<T>& GetCenter() const											{ return mCenter; }

		/**
		 * Set the center of the sphere.
		 * @param center The center position of the sphere.
		 */
		inline void	SetCenter(const TVector3<T>& center)									{ mCenter = center; }

		/**
		 * Set the radius of the sphere. 
		 * The squared radius will automatically be updated inside this method.
		 * @param radius The radius of the sphere.
		 */
		inline void SetRadius(const T& radius)												{ mRadius = radius; mRadiusSq = radius*radius; }

	private:
		TVector3<T>	mCenter;	/**< The center of the sphere. */
		T			mRadius;	/**< The radius of the sphere. */
		T			mRadiusSq;	/**< The squared radius of the sphere (mRadius*mRadius).*/
};


// include the template code
#include "BoundingSphere.inl"

// default types
typedef TBoundingSphere<MReal>	BoundingSphere;
typedef TBoundingSphere<float>	FBoundingSphere;
typedef TBoundingSphere<double>	DBoundingSphere;

}	// namespace MCore

#endif