/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Frustum.h"


// initialize the frustum from some given settings
template <class T>
void TViewFrustum<T>::Init(const T& fov, const T& aspect, const T& zNear, const T& zFar, const TMatrix<T> &viewMatrix, const TVector3<T> &viewPos)
{
	T w = 2.0 * zNear * Math::Tan(fov*0.5);	// viewplane width
	T h = w * aspect;					// viewplane height

	TMatrix<T>  invMatrix(viewMatrix); invMatrix.Transpose();
	TVector3<T> forward		= invMatrix.GetForward();
	TVector3<T> right		= invMatrix.GetRight();
	TVector3<T> up			= invMatrix.GetUp();
	TVector3<T> normal;

	// top
	normal = (-up + (w / zNear)*forward).Normalize();
	mPlanes[FRUSTUMPLANE_TOP] = TPlaneEq<T>(normal, -normal.Dot(viewPos));

	// bottom
	normal = (up + (w / zNear)*forward).Normalize();
	mPlanes[FRUSTUMPLANE_BOTTOM] = TPlaneEq<T>(normal, -normal.Dot(viewPos));

	// left
	normal = (right + (h / zNear)*forward).Normalize();
	mPlanes[FRUSTUMPLANE_LEFT] = TPlaneEq<T>(normal, -normal.Dot(viewPos));

	// right
	normal = (-right + (h / zNear)*forward).Normalize();
	mPlanes[FRUSTUMPLANE_RIGHT] = TPlaneEq<T>(normal, -normal.Dot(viewPos));

	// near plane	
	mPlanes[FRUSTUMPLANE_NEAR] = TPlaneEq<T>(forward, -forward.Dot(viewPos));

	// far plane
	mPlanes[FRUSTUMPLANE_FAR] = TPlaneEq<T>(-forward, forward.Dot(viewPos+(forward*zFar)));
}



// checks if a given axis aligned boundingbox is partially (intersects) with this frustum
template <class T>
bool TFrustum<T>::PartiallyContains(const TAABB<T>& box) const
{
	for (int i=0; i<GetNumPlanes(); i++)	
		if (!mPlanes[i].PartiallyAbove(box))
			return false;

	return true;
}


// checks if a given axis aligned boundingbox is completely within this frustum
template <class T>
bool TFrustum<T>::CompletelyContains(const TAABB<T>& box) const
{
	for (int i=0; i<GetNumPlanes(); i++)	
		if (!mPlanes[i].CompletelyAbove(box))
			return false;

	return true;
}

