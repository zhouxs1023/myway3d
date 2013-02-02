/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "KeyFrameFinder.h"


// the constructor
template <class ReturnType, class StorageType>
KeyFrameFinder<ReturnType, StorageType>::KeyFrameFinder()
{
}



// the destructor
template <class ReturnType, class StorageType>
KeyFrameFinder<ReturnType, StorageType>::~KeyFrameFinder()
{
}


// returns the keyframe number to use for interpolation
template <class ReturnType, class StorageType>
int KeyFrameFinder<ReturnType, StorageType>::FindKey(const float timeValue, KeyTrack<ReturnType, StorageType>* keyTrack)
{
	// if we haven't got any keys, return -1, which means no key found
	if (keyTrack->GetNumKeys() == 0)
		return -1;

	// get the mid point where we will perform the split
	const int midPoint = keyTrack->GetNumKeys() >> 1;

	// set the splitting time
	if (timeValue <= keyTrack->GetKey( midPoint )->GetTime())
		return FindKeyNumber(timeValue, 0, midPoint, keyTrack);
	else
		return FindKeyNumber(timeValue, midPoint, keyTrack->GetNumKeys(), keyTrack);
}


// find the keyframe number inside the key track
template <class ReturnType, class StorageType>
int KeyFrameFinder<ReturnType, StorageType>::FindKeyNumber(const float timeValue, const int low, const int high, KeyTrack<ReturnType, StorageType>* keyTrack)
{
	const int midKey  = (low + high) >> 1;
	const int numKeys = midKey - low;

	// if there are only the given number of keys to check, perform the search
	if (numKeys < 10)
	{
		const int maxKeys = keyTrack->GetNumKeys() - 1;

		// check all keys in our range
		for (int i=low; i<=high; i++)
		{
			// if it's not the last key
			if (i < maxKeys)
			{
				// if the is between the current and next key, that means we found it!
				if (keyTrack->GetKey(i)->GetTime() <= timeValue && keyTrack->GetKey(i+1)->GetTime() >= timeValue)
					return i;
			}
		}

		// the result has not been found
		return -1;
	}

	// recursively split
	if (timeValue <= keyTrack->GetKey(midKey)->GetTime())
		return FindKeyNumber(timeValue, low, midKey, keyTrack);
	else
		return FindKeyNumber(timeValue, midKey, high, keyTrack);
}