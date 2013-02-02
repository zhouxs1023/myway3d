/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "BezierInterpolators.h"

// default constructor
template <class ReturnType, class StorageType>
BezierInterpolator<ReturnType, StorageType>::BezierInterpolator() : Interpolator<ReturnType, StorageType>()
{
	mControlPoints		= NULL;
	mNumControlPoints	= 0;
}


// constructor
template <class ReturnType, class StorageType>
BezierInterpolator<ReturnType, StorageType>::BezierInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource) : Interpolator<ReturnType, StorageType>(interpolationSource)
{
	mControlPoints		= NULL;
	mNumControlPoints	= 0;
}


// destructor
template <class ReturnType, class StorageType>
BezierInterpolator<ReturnType, StorageType>::~BezierInterpolator()
{
	if (mControlPoints)
		MEMMGR.Delete( mControlPoints );
}


// initialize the interpolator
template <class ReturnType, class StorageType>
void BezierInterpolator<ReturnType, StorageType>::Init()
{
	// allocate the new control points
	const int numKeys = mInterpolationSource->GetNumKeys();
	mControlPoints = (ReturnType*)MEMMGR.ReAlloc(mControlPoints, sizeof(ReturnType) * numKeys, MEMCATEGORY_MOTIONS_INTERPOLATORS);
	mNumControlPoints = numKeys;

	// setup temp control points
	//for (int i=0; i<numKeys; i++)
		//mControlPoints[i] = mInterpolationSource->GetKey(i)->GetValue();
}


template <class ReturnType, class StorageType>
inline ReturnType& BezierInterpolator<ReturnType, StorageType>::GetControlPoint(const int keyNr)
{
	assert(mInterpolationSource);
	assert(keyNr < mInterpolationSource->GetNumKeys());

	// if there is no control point information or the number of control points is different from the number of keys
	if (mControlPoints == NULL || mNumControlPoints != mInterpolationSource->GetNumKeys())
	{
		// (re)allocate the data, so we have enough control points
		mControlPoints = (ReturnType*)MEMMGR.ReAlloc(mControlPoints, sizeof(ReturnType) * mInterpolationSource->GetNumKeys(), MEMCATEGORY_MOTIONS_INTERPOLATORS);

		// update the number of TCBs
		mNumControlPoints = mInterpolationSource->GetNumKeys();
	}

	return mControlPoints[keyNr];
}


// the bezier interpolation
template <class ReturnType, class StorageType>
ReturnType BezierInterpolator<ReturnType, StorageType>::Interpolate(const int startKey, const double timeValue) const
{
	double t  = timeValue;
	double t2 = timeValue * timeValue;
	double t3 = t2 * timeValue;

	return (-1*t3 +  3*t2 + -3*t + 1) * mInterpolationSource->GetKey(startKey)->GetValue() +
		   ( 3*t3 + -6*t2 +  3*t	) * mControlPoints[startKey] +
		   (-3*t3 +  3*t2			) * mControlPoints[startKey+1] +
		   (   t3					) * mInterpolationSource->GetKey(startKey+1)->GetValue();
}


// the constructor
template <class StorageType>
BezierQuaternionInterpolator<StorageType>::BezierQuaternionInterpolator() : BezierInterpolator<MCore::Quaternion, StorageType>(NULL)
{
}


// the constructor
template <class StorageType>
BezierQuaternionInterpolator<StorageType>::BezierQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource) : BezierInterpolator<MCore::Quaternion, StorageType>(interpolationSource)
{
}


// the interpolation method
template <class StorageType>
MCore::Quaternion BezierQuaternionInterpolator<StorageType>::Interpolate(const int startKey, const double timeValue) const
{
	// get the quaternions
	MCore::Quaternion a = mInterpolationSource->GetKey( startKey )->GetValue();
	MCore::Quaternion b = mInterpolationSource->GetKey( startKey + 1 )->GetValue();

	// check if both quaternions are on the same hypersphere or not, if not, invert one
	if (a.Dot(b) < 0.0)
		a = -a;

	double t  = timeValue;
	double t2 = timeValue * timeValue;
	double t3 = t2 * timeValue;

	return	((-1*t3 +  3*t2 + -3*t + 1) * a.LogN() +
			( 3*t3 + -6*t2 +  3*t)		* mControlPoints[startKey] +
			(-3*t3 +  3*t2)			* mControlPoints[startKey+1] +
			(   t3)					* b.LogN()).Exp().Normalize();
}
