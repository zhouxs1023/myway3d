/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __AABB_H
#define __AABB_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include "Vector.h"
#include "Algorithms.h"


namespace MCore
{

/**
 * An axis aligned bounding box (AABB).
 * This box is constructed out of two 3D points, a minimum and a maximum.
 * This means the box does not rotate, but always keeps aligned each axis.
 * Usage for this template could be for building a bounding volume around any given 3D object and
 * use it to accelerate ray tracing or visibility tests.
 * Default types are: AABB (Reals), FAABB (floats) and DAABB (doubles).
 * Reals can be floats or doubles, depending on if the #ifdef HIGH_PRECISION inside Macros.h is set or not.
 */
template <class T>
class TAABB
{
	DECLARE_CLASS(TAABB<>)

	public:
		/**
		 * Default constructor. Initializes the min and max point to the extremes.
		 * This means it's basically inside out at infinite size. So you are ready to start encapsulating points.
		 */
		inline TAABB<T>()														{ Init(); }

		/**
		 * Constructor which inits the minimum and maximum point of the box.
		 * @param minPnt The minimum point.
		 * @param maxPnt The maximum point.
		 */
		inline TAABB<T>(const TVector3<T> &minPnt, const TVector3<T> &maxPnt)	{ mMin=minPnt; mMax=maxPnt; }

		/**
		 * Initialize the box minimum and maximum points.
		 * Sets the points to floating point maximum and minimum. After calling this method you are ready to encapsulate points in it.
		 * Note, that the default constructor already calls this method. So you should only call it when you want to 'reset' the minimum and
		 * maximum points of the box.
		 */
		inline void Init()														{ mMin.Set(FLT_MAX, FLT_MAX, FLT_MAX); mMax.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX); }

		/**
		 * Encapsulate a point in the box.
		 * This means that the box will automatically calculate the new minimum and maximum points when needed.
		 * @param v The vector (3D point) to 'add' to the box.
		 */
		inline void Encapsulate(const TVector3<T> &v)							{ mMin.x = Min<T>(mMin.x, v.x); mMin.y = Min<T>(mMin.y, v.y); mMin.z = Min<T>(mMin.z, v.z); mMax.x = Max<T>(mMax.x, v.x); mMax.y = Max<T>(mMax.y, v.y); mMax.z = Max<T>(mMax.z, v.z); }

		/**
		 * Encapsulate another AABB with this box.
		 * This method automatically adjusts the minimum and maximum point of the box after 'adding' the given AABB to this box.
		 * @param box The AABB to 'add' to this box.
		 */
		inline void Encapsulate(const TAABB<T> &box)							{ Encapsulate(box.mMin); Encapsulate(box.mMax);}

		/**
		 * Widen the box in all dimensions with a given number of units.
		 * @param delta The delta size in units to enlarge the box with. The delta value will be added to the maximum point as well as subtracted from the minimum point.
		 */
		inline void Widen(const T delta)										{ mMin.x-=delta; mMin.y-=delta; mMin.z-=delta; mMax.x+=delta; mMax.y+=delta; mMax.z+=delta; }

		/**
		 * Translates the box with a given offset vector.
		 * This means the middle of the box will be moved by the given vector.
		 * @param offset The offset vector to translate (move) the box with.
		 */
		inline void Translate(const TVector3<T> &offset)						{ mMin+=offset; mMax+=offset; }

		/**
		 * Checks if a given point is inside this box or not.
		 * Note that the edges/planes of the box are also counted as 'inside'. 
		 * @param v The vector (3D point) to perform the test with.
		 * @result Returns true when the given point is inside the box, otherwise false is returned.
		 */
		inline bool Contains(const TVector3<T> &v) const						{ return (InRange<T>(v.x, mMin.x, mMax.x) && InRange<T>(v.z, mMin.z, mMax.z) && InRange<T>(v.y, mMin.y, mMax.y)); }

		/**
		 * Checks if a given AABB is COMPLETELY inside this box or not.
		 * Note that the edges/planes of this box are counted as 'inside'.
		 * @param b The AABB to perform the test with.
		 * @result Returns true when the AABB 'b' is COMPLETELY inside this box. If it's completely or partially outside, false will be returned.
		 */
		inline bool Contains(const TAABB<T> &b) const							{ return (Contains(b.mMin) && Contains(b.mMax)); }

		/**
		 * Checks if a given AABB partially or completely contains, so intersects, this box or not.
		 * Please note that the edges of this box are counted as 'inside'.
		 * @param b The AABB to perform the test with.
		 * @result Returns true when the given AABB 'b' is completely or partially inside this box. Only false will be returned when the given AABB 'b' is COMPLETELY outside this box.
		 */
		inline bool Intersects(const TAABB<T> &b) const							{ return !(mMin.x > b.mMax.x || mMax.x < b.mMin.x || mMin.y > b.mMax.y || mMax.y < b.mMin.y || mMin.z > b.mMax.z || mMax.z < b.mMin.z); }

		/**
		 * Calculates and returns the width of the box.
		 * The width is the distance between the minimum and maximum point, along the X-axis.
		 * @result The width of the box.
		 */
		inline T CalcWidth() const												{ return Math::Abs(mMin.x-mMax.x); }

		/**
		 * Calculates and returns the height of the box.
		 * The height is the distance between the minimum and maximum point, along the Y-axis.
		 * @result The height of the box.
		 */
		inline T CalcHeight() const												{ return Math::Abs(mMin.y-mMax.y); }

		/**
		 * Calculates and returns the depth of the box.
		 * The depth is the distance between the minimum and maximum point, along the Z-axis.
		 * @result The depth of the box.
		 */
		inline T CalcDepth() const												{ return Math::Abs(mMin.z-mMax.z); }

		/**
		 * Calculates the center/middle of the box.
		 * This is simply done by taking the average of the minimum and maximum point along each axis.
		 * @result The center (or middle) point of this box.
		 */
		inline TVector3<T> CalcMiddle() const									{ return (mMin + mMax) * 0.5; }

		/**
		 * Calculates the extents of the box.
		 * This is the vector from the center to a corner of the box.
		 * @result The vector containing the extents.
		 */
		inline TVector3<T> CalcExtents() const									{ return (mMax - mMin) * 0.5; }

		/**
		 * Calculates the radius of this box.
		 * With radius we mean the length of the vector from the center of the box to the minimum or maximum point.
		 * So if you would construct a sphere with as center the middle of this box and with a radius returned by this method, you will
		 * get the minimum sphere which exactly contains this box.
		 * @result The length of the center of the box to one of the extreme points. So the minimum radius of the bounding sphere containing this box.
		 */
		inline T CalcRadius() const												{ return (mMax-mMin).Length() * 0.5; }

		/**
		 * Get the minimum point of the box.
		 * @result The minimum point of the box.
		 */
		inline const TVector3<T>& GetMin() const								{ return mMin; }

		/**
		 * Get the maximum point of the box.
		 * @result The maximum point of the box.
		 */
		inline const TVector3<T>& GetMax() const								{ return mMax; }

		/**
		 * Set the minimum point of the box.
		 * @param minVec The vector representing the minimum point of the box.
		 */
		inline void	SetMin(const TVector3<T>& minVec)							{ mMin = minVec; }

		/**
		 * Set the maximum point of the box.
		 * @param maxVec The vector representing the maximum point of the box.
		 */
		inline void	SetMax(const TVector3<T>& maxVec)							{ mMax = maxVec; }


	private:
		TVector3<T>		mMin;	/**< The minimum point. */
		TVector3<T>		mMax;	/**< The maximum point. */
};

// default types
typedef TAABB<MReal>			AABB;
typedef TAABB<float>			FAABB;
typedef TAABB<double>			DAABB;

}	// namespace MCore

#endif