/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __KEYFRAMEFINDER_H
#define __KEYFRAMEFINDER_H


namespace EMotionFX
{

// forward declaration
template <class ReturnType, class StorageType> class KeyTrack;

/**
 * The keyframe finder.
 * This is used to quickly locate the two keys inside the keytrack to interpolate between.
 * For example if there is a keyframe at every second, and we want to calculate a value at
 * time 5.6, then we need to interpolate between the key with the time 5 and the key with the time 6.
 * This class basically uses a kd-tree to perform the search. It quickly culls out parts of the keys
 * which cannot be the one which we are searching for. This means the search times are linear, no matter
 * how many keys we are searching.
 */
template <class ReturnType, class StorageType>
class KeyFrameFinder
{
	DECLARE_CLASS(KeyFrameFinder<>)

	public:
		/**
		 * The constructor.
		 */
		KeyFrameFinder();

		/**
		 * The destructor.
		 */
		~KeyFrameFinder();

		/**
		 * Locates the key number to use for interpolation at a given time value.
		 * You will interpolate between the returned keyframe number and the one after that.
		 * @param timeValue The time value you want to calculate a value at.
		 * @param keyTrack The keytrack to perform the search on.
		 * @result The key number, or -1 when no valid key could be found.
		 */
		static int FindKey(const float timeValue, KeyTrack<ReturnType, StorageType>* keyTrack);

	private:
		/**
		 * The recursive main search method, which does all the work.
		 * @param timeValue The time value to use for the search.
		 * @param low The first keyframe number to start the search for.
		 * @param high The high keyframe number to start the search for.
		 * @param keyTrack The keytrack to perform the search on.
		 * @result The key number.
		 */
		static int FindKeyNumber(const float timeValue, const int low, const int high, KeyTrack<ReturnType, StorageType>* keyTrack);
};

// include inline code
#include "KeyFrameFinder.inl"

} // namespace EMotionFX


#endif