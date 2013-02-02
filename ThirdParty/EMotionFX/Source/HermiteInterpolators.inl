/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "HermiteInterpolators.h"

// default constructor
template <class ReturnType, class StorageType>
HermiteInterpolator<ReturnType, StorageType>::HermiteInterpolator() : Interpolator<ReturnType, StorageType>()
{
	mTangents = NULL;
}


// constructor
template <class ReturnType, class StorageType>
HermiteInterpolator<ReturnType, StorageType>::HermiteInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource) : Interpolator<ReturnType, StorageType>(interpolationSource)
{
	mTangents = NULL;
}


// destructor
template <class ReturnType, class StorageType>
HermiteInterpolator<ReturnType, StorageType>::~HermiteInterpolator()
{
	if (mTangents)
		MEMMGR.Delete( mTangents );
}


// get a given tangent
template <class ReturnType, class StorageType>
inline ReturnType& HermiteInterpolator<ReturnType, StorageType>::GetTangent(const int keyNr)
{
	assert(mInterpolationSource);
	assert(keyNr < mInterpolationSource->GetNumKeys());
	assert(mTangents);
	return mTangents[keyNr];
}


// initialize the interpolator
template <class ReturnType, class StorageType>
void HermiteInterpolator<ReturnType, StorageType>::Init()
{
	assert(mInterpolationSource);	// make sure we have a keytrack assigned

	// allocate the new tangents
	const int numKeys = mInterpolationSource->GetNumKeys();
	mTangents = (ReturnType*)MEMMGR.ReAlloc(mTangents, sizeof(ReturnType) * numKeys, MEMCATEGORY_MOTIONS_INTERPOLATORS);

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
			mTangents[i] = (0.5 * (value - prevValue)) + (0.5 * (nextValue - value));
		}
	}
}


// hermite interpolation
template <class ReturnType, class StorageType>
ReturnType HermiteInterpolator<ReturnType, StorageType>::Interpolate(const int startKey, const double timeValue) const
{
	//				[ 2 -2  1  1]	[a]
	//[u3 u2 u 1]	[-3  3 -2 -1]	[b]
	//				[ 0  0  1  0]	[ta]
	//				[ 1  0  0  0]	[tb]

	// precalc u, u2 and u3
	double t		= timeValue;
	double t2		= t * t;
	double t3		= t2  * t;

	return	( 2*t3 + -3*t2 + 1) * mInterpolationSource->GetKey( startKey )->GetValue() +
			(-2*t3 +  3*t2)		* mInterpolationSource->GetKey( startKey + 1 )->GetValue() +
			(   t3 + -2*t2 + t)	* mTangents[ startKey ] +
			(   t3 + -t2)		* mTangents[ startKey + 1 ];
}

//-------------------------------------------


// the constructor
template <class StorageType>
HermiteQuaternionInterpolator<StorageType>::HermiteQuaternionInterpolator() : HermiteInterpolator<MCore::Quaternion, StorageType>(NULL)
{
}


// the constructor
template <class StorageType>
HermiteQuaternionInterpolator<StorageType>::HermiteQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource) : HermiteInterpolator<MCore::Quaternion, StorageType>(interpolationSource)
{
}


// the interpolation method
template <class StorageType>
MCore::Quaternion HermiteQuaternionInterpolator<StorageType>::Interpolate(const int startKey, const double timeValue) const
{
	// get the two quaternions
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

