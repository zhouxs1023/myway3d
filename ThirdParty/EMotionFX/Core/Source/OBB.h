/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __OBB_H
#define __OBB_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include "Vector.h"
#include "Algorithms.h"


namespace MCore
{

/**
 * 3D Oriented Bounding Box (OBB) template.
 * This is basically a AABB with an arbitrary rotation.
 */
template <class T>
class TOBB
{
	DECLARE_CLASS(TOBB<>)

	public:
		/**
		 * The constructor.
		 * This does not initialize any members, so makes the box invalid.
		 */
		inline TOBB<T>() {}
		
		/**
		 * Construct the OBB from a given axis aligned bounding box and a transformation.
		 * @param aabb The axis aligned bounding box.
		 * @param transformation The transformation of the box.
		 */
		inline TOBB<T>(const TAABB<T>& aabb, const TMatrix<T>& transformation)	{ Create(aabb, transformation);	}

		/**
		 * Construct the OBB from a center, extends and a rotation.
		 * @param center The center of the box.
		 * @param extents The extents of the box, which start at the center of the box.
		 * @param rot The matrix, representing the transformation of the box.
		 */
		inline TOBB<T>(const TVector3<T>& center, const TVector3<T>& extents, const TMatrix<T> rot) : mCenter(center), mExtents(extents), mRotation(rot)	{}

		/**
		 * Reset the OBB with as center 0,0,0, infinite negative extents and no rotation.
		 * This makes the box an invalid box as well, because the extents have not been set.
		 */
		void Init();

		/**
		 * Check if this box OBB contains a given point or not.
		 * @param p The point to check.
		 * @result Returns true when the point is inside this box, otherwise false is returned.
		 */
		bool Contains(const TVector3<T>& p)	const;

		/**
		 * Check if this OBB is inside another specified box.
		 * @param box The OBB to check.
		 * @result Returns true when this OBB is inside the box specified as parameter.
		 */
		bool IsInside(const TOBB<T>& box) const;

		/**
		 * Create the OBB from a given AABB and a matrix.
		 * @param aabb The axis aligned bounding box.
		 * @param mat The matrix, which represents the orientation of the box.
		 */
		void Create(const TAABB<T>& aabb, const TMatrix<T>& mat);

		/**
		 * Transform this OBB with a given matrix.
		 * This means the transformation specified as parameter will be applied to the current transformation of the OBB.
		 * So the transformation specified is NOT an absolute rotation, but a relative transformation.
		 * @param transMatrix The relative transformation matrix, to be applied to the current transformation.
		 */
		void Transform(const TMatrix<T>& transMatrix);

		/**
		 * Calculate the transformed version of this OBB.
		 * @param rotMatrix The transformation matrix to be applied to the rotation of this OBB, so not an absolute rotation!
		 * @param outOBB A pointer to the OBB to fill with the rotated version of this OBB.
		 */
		void Transformed(const TMatrix<T>& rotMatrix, TOBB<T>* outOBB) const;

		/**
		 * Check if this is a valid OBB or not.
		 * The box is only valid if the extents are non-negative.
		 * @result Returns true when the OBB is valid, otherwise false is returned.
		 */
		bool IsValid() const;

		/**
		 * Set the center of the box.
		 * @param center The new center of the box.
		 */
		inline void SetCenter(const TVector3<T>& center)				{ mCenter = center;	}

		/**
		 * Set the extents of the box.
		 * @param extents The new extents of the box.
		 */
		inline void SetExtents(const TVector3<T>& extents)				{ mExtents = extents; }

		/**
		 * Set the transformation of the box.
		 * @param transform The new transformation of the box.
		 */
		inline void SetTransformation(const TMatrix<T>& transform)		{ mRotation = transform; }

		/**
		 * Get the center of the box.
		 * @result The center point of the box.
		 */
		inline const TVector3<T>& GetCenter() const						{ return mCenter; }

		/**
		 * Get the extents of the box.
		 * @result The extents of the box, which start at the center.
		 */
		inline const TVector3<T>& GetExtents() const					{ return mExtents; }

		/**
		 * Get the transformation of the box.
		 * @result The transformation of the box.
		 */
		inline const TMatrix<T>& GetTransformation() const				{ return mRotation; }

		/**
		 * Get the 8 corner points of the box.
		 * @param outPoints the array of at least 8 vectors to write the points in.
		 */
		void GetCornerPoints(TVector3<T>* outPoints) const;

	private:
		TMatrix<T>		mRotation;	/**< The rotation of the box. */
		TVector3<T>		mCenter;	/**< The center of the box. */
		TVector3<T>		mExtents;	/**< The extents of the box. */
};

// include the template code
#include "OBB.inl"

// default types
typedef TOBB<MReal>				OBB;
typedef TOBB<float>				FOBB;
typedef TOBB<double>			DOBB;

}	// namespace MCore

#endif