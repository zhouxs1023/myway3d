/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Interpolator.h"


// default constructor
template <class ReturnType, class StorageType>
Interpolator<ReturnType, StorageType>::Interpolator() : mInterpolationSource(NULL)
{
}


// constructor
template <class ReturnType, class StorageType>
Interpolator<ReturnType, StorageType>::Interpolator(KeyTrack<ReturnType, StorageType>* interpolationSource) : mInterpolationSource(interpolationSource)
{
}


// destructor
template <class ReturnType, class StorageType>
Interpolator<ReturnType, StorageType>::~Interpolator()
{
}


// returns the key track which is used as interpolation source
template <class ReturnType, class StorageType>
inline KeyTrack<ReturnType, StorageType>* Interpolator<ReturnType, StorageType>::GetKeyTrack() const
{
	return mInterpolationSource;
}


// set the key track which is used as interpolation source
template <class ReturnType, class StorageType>
inline void Interpolator<ReturnType, StorageType>::SetKeyTrack(KeyTrack<ReturnType, StorageType>* interpolationSource)
{
	mInterpolationSource = interpolationSource;
}
