/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FRUSTUM_H
#define __FRUSTUM_H

#include "Macros.h"
#include "PlaneEq.h"
#include "Vector.h"
#include "AABB.h"
#include "Matrix4.h"
#include "SourceDeclaration.h"
#include "Algorithms.h"
#include "Array.h"

namespace MCore
{

/**
 * The frustum base class.
 * A frustum is a convex volume which we can use for several
 * visibility related things, like occlusion culling.
 * The frustum is constructed out of planes and the class provides
 * methods to quickly determine if several bounding volume objects are
 * intersection this volume or not. Also it's possible to detect if
 * these bounding volume objects are completely inside the volume or not.
 * This is especially usefull for occlusion culling. If the bounding box
 * of an object is completely inside the occluders frustums volume, it means
 * that this object will be invisible.
 * The same can be used for view frustum culling. If an objects bounding volume
 * is partially or completely inside the volume, it means it's visible to the viewer.
 * Don't forget that the frustum has to be CONVEX in order to work correctly.
 */
template <class T>
class TFrustum
{
	DECLARE_CLASS(TFrustum<>)

	public:
		/**
		 * The constructor.
		 */
		TFrustum<T>() {}

		/**
	 	 * The virtual destructor.
		 * Will get rid of all the planes automatically.
		 */
		virtual ~TFrustum<T>() { RemoveAllPlanes(); }

		/**
		 * Adds a plane to the frustum volume.
		 * @param plane The plane to add to the volume.
	 	 */
		inline void AddPlane(const TPlaneEq<T>& plane)						{ mPlanes.Add(plane); }

		/**
		 * Set the plane at a given index.
		 * @param index The plane number, which must be in range of [0..GetNumPlanes()-1]
		 * @param plane The plane to set at the given index.
		 */
		inline void SetPlane(int index, const TPlaneEq<T>& plane)			{ assert(index>=0 && index<mPlanes.GetLength()); mPlanes[index]=plane; }

		/**
		 * Returns the number of planes in the frustum volume.
		 * @result The number of planes of the frustum volume.
		 */
		inline int GetNumPlanes() const										{ return mPlanes.GetLength(); }

		/**
		 * Returns a given plane from the frustum volume.
		 * @param index The plane number, which must be in range of [0..GetNumPlanes()-1]
		 */
		inline const TPlaneEq<T>& GetPlane(int index)						{ assert(index>=0 && index<mPlanes.GetLength()); return mPlanes[index]; }

		/**
		 * Removes all planes inside the current frustum volume.
		 * After calling this method, the method GetNumPlanes() will return 0.
		 * The destructor of this class will automatically remove all planes on destruction.
	 	 */
		inline void RemoveAllPlanes()										{ mPlanes.Clear(); }

		/**
		 * Removes the plane at a given index from the frustum volume.
		 * @param index The plane number to remove. Must be in range of [0..GetNumPlanes()-1]
		 */
		inline void RemovePlane(int index)									{ assert(index>=0 && index<mPlanes.GetLength()); mPlanes.Remove(index); }
		
		/**
		 * Checks if a given axis aligned bounding box intersects (or is completely inside) the frustum volume.
		 * @param box The axis aligned bounding box to use for the check.
		 * @result True will be returned in case of an intersection with the volume (so also when completely inside), otherwise false will be returned.
		 */
		bool PartiallyContains(const TAABB<T>& box) const;

		/**
		 * Checks if a given axis aligned bounding box is completely inside the frustum volume.
		 * @param box The axis aligned bounding box to use for the check.
		 * @result True will be returned in case the box is completely inside the volume. False is returned when the box is not completely inside the volume (but partially or completely outside).
		 */
		bool CompletelyContains(const TAABB<T>& box) const;


	protected:
		Array< TPlaneEq<T> >	mPlanes;	/**< The collection of planes which form the frustum volume */
};



/**
 * The view frustum class, which is inherited from the Frustum class.
 * A view frustum is a frustum which is constructed out of exactly six planes and is used for
 * frustum culling, so to detect what objects are potentially in the view volume (on screen) and which ones are not.
 * The class contains a special method (Init) to initialize the frustum planes for given camera settings.
 * @see Init
 */
template <class T>
class TViewFrustum : public TFrustum<T>
{
	DECLARE_CLASS(TViewFrustum<>)

	public:
		/**
		 * The viewfrustum plane indices.
		 */
		enum EFrustumPlane
		{
			FRUSTUMPLANE_LEFT	= 0,
			FRUSTUMPLANE_RIGHT	= 1,
			FRUSTUMPLANE_TOP	= 2,
			FRUSTUMPLANE_BOTTOM	= 3,
			FRUSTUMPLANE_NEAR	= 4,
			FRUSTUMPLANE_FAR	= 5,
		};

		/**
		 * The constructor, which automatically creates six planes.
	 	 */
		TViewFrustum<T>()	: TFrustum<T>()	{ mPlanes.Resize(6); }

		/**
		 * The destructor.
		 */
		~TViewFrustum<T>()	{ }

		/**
		 * Initialize the frustum planes for given camera settings.
		 * @param fov The field of view of the camera, in radians.
		 * @param aspect The aspect ratio (0.75 for example).
		 * @param zNear The distance in units to the near clipping plane (1.0 for example).
		 * @param zFar The distance in units to the far clipping plane.
		 * @param viewMatrix The camera view matrix (lookat matrix). This is used to extract the right, up and forward vectors.
		 * @param viewPos The camera position.
		 */
		void Init(const T& fov, const T& aspect, const T& zNear, const T& zFar, const TMatrix<T>& viewMatrix, const TVector3<T>& viewPos);
};

// include inline code
#include "Frustum.inl"


// types
typedef TFrustum<MReal>			Frustum;
typedef TFrustum<float>			FFrustum;
typedef TFrustum<double>		DFrustum;

typedef TViewFrustum<MReal>		ViewFrustum;
typedef TViewFrustum<float>		FViewFrustum;
typedef TViewFrustum<double>	DViewFrustum;

}	// namespace MCore

#endif