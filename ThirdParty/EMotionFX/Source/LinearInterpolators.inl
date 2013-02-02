/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "LinearInterpolators.h"
#include "KeyTrack.h"

//-------------------------------------------------------------------------------------------------
// LinearInterpolator
//-------------------------------------------------------------------------------------------------

// default constructor
template <class ReturnType, class StorageType>
LinearInterpolator<ReturnType, StorageType>::LinearInterpolator() : Interpolator<ReturnType, StorageType>(NULL)
{
}


// constructor
template <class ReturnType, class StorageType>
LinearInterpolator<ReturnType, StorageType>::LinearInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource) : Interpolator<ReturnType, StorageType>(interpolationSource)
{
}


// destructor
template <class ReturnType, class StorageType>
LinearInterpolator<ReturnType, StorageType>::~LinearInterpolator()
{
}


// initialize the interpolator
template <class ReturnType, class StorageType>
void LinearInterpolator<ReturnType, StorageType>::Init()
{
}


template <class ReturnType, class StorageType>
ReturnType LinearInterpolator<ReturnType, StorageType>::Interpolate(const int startKey, const double timeValue) const
{
	KeyFrame<ReturnType, StorageType>*	firstKey = mInterpolationSource->GetKey( startKey );
	KeyFrame<ReturnType, StorageType>*	nextKey  = mInterpolationSource->GetKey( startKey + 1 );

	return (1.0 - timeValue) * firstKey->GetValue() + timeValue * nextKey->GetValue();
}



//--------------------------
// Quaternion interpolation
//--------------------------

// the constructor
template <class StorageType>
LinearQuaternionInterpolator<StorageType>::LinearQuaternionInterpolator() : LinearInterpolator<MCore::Quaternion, StorageType>(NULL)
{
}


// the constructor
template <class StorageType>
LinearQuaternionInterpolator<StorageType>::LinearQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource) : LinearInterpolator<MCore::Quaternion, StorageType>(interpolationSource)
{
}


// the interpolation method
template <class StorageType>
MCore::Quaternion LinearQuaternionInterpolator<StorageType>::Interpolate(const int startKey, const double timeValue) const
{
	// get the quaternions
	MCore::Quaternion a = mInterpolationSource->GetKey( startKey )->GetValue();
	MCore::Quaternion b = mInterpolationSource->GetKey( startKey + 1 )->GetValue();

	// check if both quaternions are on the same hypersphere or not, if not, invert one
	if (a.Dot(b) < 0.0)
		a = -a;

	// take the logarithm
	a = a.LogN();
	b = b.LogN();

	// interpolate, and take the exponent of that, which is the interpolated quaternion
	return ((1.0 - timeValue) * a + (timeValue * b)).Exp().Normalize();
}
