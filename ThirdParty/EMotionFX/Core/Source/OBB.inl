/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "OBB.h"

template <class T>
void TOBB<T>::Init()
{
	mCenter.Set(0, 0, 0);
	mExtents.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	mRotation.Identity();
}


template <class T>
bool TOBB<T>::Contains(const TVector3<T>& p) const
{
	// translate to box space
	TVector3<T> relPoint = p - mCenter;

	// mRotation * Point maps from world space to box space
	float f = mRotation.m44[0][0] * relPoint.x + mRotation.m44[0][1] * relPoint.y + mRotation.m44[0][2] * relPoint.z;
	if (f >= mExtents.x || f <= -mExtents.x) return false;

	f = mRotation.m44[1][0] * relPoint.x + mRotation.m44[1][1] * relPoint.y + mRotation.m44[1][2] * relPoint.z;
	if (f >= mExtents.y || f <= -mExtents.y) return false;

	f = mRotation.m44[2][0] * relPoint.x + mRotation.m44[2][1] * relPoint.y + mRotation.m44[2][2] * relPoint.z;
	if (f >= mExtents.z || f <= -mExtents.z) return false;

	return true;
}


template <class T>
void TOBB<T>::Create(const TAABB<T>& aabb, const TMatrix<T>& mat)
{
	// calculate the center and extents
	mCenter  = aabb.CalcMiddle();
	mExtents = aabb.CalcExtents();

	// transform the center
	mCenter *= mat;

	// set the roation
	mRotation = mat;
}


template <class T>
void TOBB<T>::Transform(const TMatrix<T>& transMatrix)
{
	mCenter   = mCenter * transMatrix;
	mRotation = mRotation * transMatrix;
}


template <class T>
void TOBB<T>::Transformed(const TMatrix<T>& transMatrix, TOBB<T>* outOBB) const
{
	outOBB->mExtents  = mExtents;
	outOBB->mCenter   = mCenter * transMatrix;
	outOBB->mRotation = mRotation * transMatrix;
}


template <class T>
bool TOBB<T>::IsValid() const
{
	if (mExtents.x < 0.0f) return false;
	if (mExtents.y < 0.0f) return false;
	if (mExtents.z < 0.0f) return false;
	return true;
}


template <class T>
bool TOBB<T>::IsInside(const TOBB<T>& box) const
{
	// make a 4x4 from the box & inverse it		
	TMatrix<T> M0 = box.mRotation;
	M0.SetTranslation( box.mCenter );
	TMatrix<T> M0Inv = M0.Inversed();

	// with our inversed 4x4, create box1 in space of box0
	TOBB<T> _1in0;
	Transformed(M0Inv, &_1in0);

	// this should cancel out box0's rotation, i.e. it's now an AABB
	
	// the two boxes are in the same space so now we can compare them
	// create the AABB of (box1 in space of box0)
	const TMatrix<T>& mtx = _1in0.mRotation;

	float f = Math::Abs(mtx.m44[0][0] * mExtents.x) + Math::Abs(mtx.m44[1][0] * mExtents.y) + Math::Abs(mtx.m44[2][0] * mExtents.z) - box.mExtents.x;
	if (f > _1in0.mCenter.x)	return false;
	if (-f < _1in0.mCenter.x)	return false;

	f = Math::Abs(mtx.m44[0][1] * mExtents.x) + Math::Abs(mtx.m44[1][1] * mExtents.y) + Math::Abs(mtx.m44[2][1] * mExtents.z) - box.mExtents.y;
	if (f > _1in0.mCenter.y)	return false;
	if (-f < _1in0.mCenter.y)	return false;

	f = Math::Abs(mtx.m44[0][2] * mExtents.x) + Math::Abs(mtx.m44[1][2] * mExtents.y) + Math::Abs(mtx.m44[2][2] * mExtents.z) - box.mExtents.z;
	if (f > _1in0.mCenter.z)	return false;
	if (-f < _1in0.mCenter.z)	return false;

	return true;
}


template <class T>
void TOBB<T>::GetCornerPoints(TVector3<T>* outPoints) const
{
	assert( outPoints );

	TVector3<T> right	= mRotation.GetRight();
	TVector3<T> up		= mRotation.GetUp();
	TVector3<T> forward = mRotation.GetForward();

	right	*= mExtents.x;
	up		*= mExtents.y;
	forward *= mExtents.z;

	//     7+------+6
	//     /|     /|
	//    / |    / |
	//   / 4+---/--+5
	// 3+------+2 /
	//  | /    | /
	//  |/     |/ 
	// 0+------+1

	outPoints[0] = mCenter - right - up - forward;
	outPoints[1] = mCenter + right - up - forward;
	outPoints[2] = mCenter + right + up - forward;
	outPoints[3] = mCenter - right + up - forward;
	outPoints[4] = mCenter - right - up + forward;
	outPoints[5] = mCenter + right - up + forward;
	outPoints[6] = mCenter + right + up + forward;
	outPoints[7] = mCenter - right + up + forward;
}
