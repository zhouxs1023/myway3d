/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "TCBSplineInterpolators.h"


// default constructor
template <class ReturnType, class StorageType>
TCBSplineInterpolator<ReturnType, StorageType>::TCBSplineInterpolator() : HermiteInterpolator<ReturnType, StorageType>() 
{
	mTCBs		= 0;
	mNumTCBs	= NULL;
}

// constructor
template <class ReturnType, class StorageType>
TCBSplineInterpolator<ReturnType, StorageType>::TCBSplineInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource) : HermiteInterpolator<ReturnType, StorageType>(interpolationSource)
{
	mNumTCBs	= 0;
	mTCBs		= NULL;
}


// destructor
template <class ReturnType, class StorageType>
TCBSplineInterpolator<ReturnType, StorageType>::~TCBSplineInterpolator()
{
	// get rid of the allocated TCB data
	if (mTCBs)
		MEMMGR.Delete( mTCBs );
}


template <class ReturnType, class StorageType>
inline typename TCBSplineInterpolator<ReturnType, StorageType>::TCBInfo& TCBSplineInterpolator<ReturnType, StorageType>::GetTCB(const int keyNr)
{
	assert(mInterpolationSource);
	assert(keyNr < mInterpolationSource->GetNumKeys());

	// if there is no TCB information or the number of TCBs is different from the number of keys
	if (mTCBs == NULL || mNumTCBs != mInterpolationSource->GetNumKeys())
	{
		// (re)allocate the data, so we have enough TCB elements
		mTCBs = (TCBInfo*)MEMMGR.ReAlloc(mTCBInfo, sizeof(TCBInfo) * mInterpolationSource->GetNumKeys(), MEMCATEGORY_MOTIONS_INTERPOLATORS);

		// update the number of TCBs
		mNumTCBs = mInterpolationSource->GetNumKeys();
	}

	return mTCBs[keyNr];
}



// initialize the interpolator
template <class ReturnType, class StorageType>
void TCBSplineInterpolator<ReturnType, StorageType>::Init()
{
	assert(mInterpolationSource);	// make sure we have a key track assigned

	// get rid of existing tangents
	if (mTangents)
		MEMMGR.Delete(mTangents);

	// allocate the new tangents
	const int numKeys = mInterpolationSource->GetNumKeys();
	mTangents = MEMMGR.Allocate(numKeys * sizeof(ReturnType), MEMCATEGORY_MOTIONS_INTERPOLATORS);

	// calculate all tangents
	for (int i=0; i<numKeys; i++)
	{
		// if there is no previous or next key
		if (i==0 || i==numKeys-1)
		{
			ReturnType tangent;
			memset(&tangent, 0, sizeof(ReturnType));
			mTangents[i] = tangent;			
		}
		else	// if there is a previous or next key
		{
			const ReturnType& prevValue	= mInterpolationSource->GetKey(i-1)->GetValue();
			const ReturnType& value		= mInterpolationSource->GetKey(i  )->GetValue();
			const ReturnType& nextValue	= mInterpolationSource->GetKey(i+1)->GetValue();

			// if there is TCB information
			if (mTCBs != NULL)
			{
				// get the TCB values
				const float tension		= mTCBs[i].mTension;
				const float continuity	= mTCBs[i].mContinuity;
				const float bias		= mTCBs[i].mBias;

				// if the tension, bias and continuity are equal to zero, we can use standard hermite interpolation
				//if (tension==0.0 && bias==0.0 && continuity==0.0)
					//mTangents[i] = (0.5 * (value - prevValue)) + (0.5 * (nextValue - value));	// normal hermite interpolation
				//else
					mTangents[i] = (((1.0-tension)*(1.0+bias)*(1.0+continuity))*0.5) * (value - prevValue) + (((1.0-tension)*(1.0-bias)*(1.0-continuity))*0.5) * (nextValue - value);
			}
			else	// there is no TCB info, so just use standard hermite interpolation
				mTangents[i] = (0.5 * (value - prevValue)) + (0.5 * (nextValue - value));
		}
	}
}


// the constructor
template <class StorageType>
TCBSplineQuaternionInterpolator<StorageType>::TCBSplineQuaternionInterpolator() : TCBSplineInterpolator<MCore::Quaternion, StorageType>(NULL)
{
}


// the constructor
template <class StorageType>
TCBSplineQuaternionInterpolator<StorageType>::TCBSplineQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource) : TCBSplineInterpolator<MCore::Quaternion, StorageType>(interpolationSource)
{
}


// the interpolation method
template <class StorageType>
MCore::Quaternion TCBSplineQuaternionInterpolator<StorageType>::Interpolate(const int startKey, const double timeValue) const
{
	// get the quaternions
	MCore::Quaternion a = mInterpolationSource->GetKey( startKey )->GetValue();
	MCore::Quaternion b = mInterpolationSource->GetKey( startKey + 1 )->GetValue();

	// check if both quaternions are on the same hypersphere or not, if not, invert one
	if (a.Dot(b) < 0.0)
		a = -a;

	//				[ 2 -2  1  1]	[a]
	//[u3 u2 u 1]	[-3  3 -2 -1]	[b]
	//				[ 0  0  1  0]	[ta]
	//				[ 1  0  0  0]	[tb]

	// interpolate, and take the exponent of that, which is the interpolated quaternion
	double t	= timeValue;
	double t2	= t * t;
	double t3	= t2  * t;

	return (( 2*t3 + -3*t2 + 1) * a.LogN()  +
			(-2*t3 +  3*t2)		* b.LogN()  +
			(   t3 + -2*t2 + t)	* mTangents[startKey] +
			(   t3 + -t2)		* mTangents[startKey+1] ).Exp().Normalize();
}