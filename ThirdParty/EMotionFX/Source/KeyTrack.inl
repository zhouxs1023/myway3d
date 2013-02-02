/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "KeyTrack.h"


// default constructor
template <class ReturnType, class StorageType>
KeyTrack<ReturnType, StorageType>::KeyTrack()
{
	mInitialized	= false;
	mCurrentTime	= 0.0f;
	mLoopMode		= KEYTRACK_LOOP;
	mPlaySpeed		= 1.0f;
	mIsPlaying		= false;
	mForward		= true;
	mInterpolator	= NULL;
}


// extended constructor
template <class ReturnType, class StorageType>
KeyTrack<ReturnType, StorageType>::KeyTrack(const int nrKeys)
{
	mInitialized	= false;
	mCurrentTime	= 0.0f;
	mLoopMode		= KEYTRACK_LOOP;
	mPlaySpeed		= 1.0f;
	mIsPlaying		= false;
	mForward		= true;
	mInterpolator	= NULL;

	mKeys.Resize(nrKeys);
}


// destructor
template <class ReturnType, class StorageType>
KeyTrack<ReturnType, StorageType>::~KeyTrack()
{
	ClearKeys();

	if (mInterpolator != NULL)
		delete mInterpolator;
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::Play()
{
	mIsPlaying = true;
}


template <class ReturnType, class StorageType>
void KeyTrack<ReturnType, StorageType>::ClearKeys()
{
	mKeys.Clear();
	mInitialized=false;
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::Stop()
{
	mIsPlaying = false;
}


template <class ReturnType, class StorageType>
inline ReturnType KeyTrack<ReturnType, StorageType>::GetCurrentValue()
{
	return GetValueAtTime(mCurrentTime);
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::Rewind()
{
	mCurrentTime = GetFirstTime();
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::SetPlaySpeed(const float speed)
{
	assert(speed >= 0.0f);
	mPlaySpeed=speed;
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::SetPlayForwards(const bool forward)
{
	mForward = forward;
}


template <class ReturnType, class StorageType>
inline bool KeyTrack<ReturnType, StorageType>::GetPlaying()
{
	return mIsPlaying;
}


template <class ReturnType, class StorageType>
inline bool KeyTrack<ReturnType, StorageType>::GetPlayForwards() 
{
	return mForward;
}


template <class ReturnType, class StorageType>
inline float KeyTrack<ReturnType, StorageType>::GetPlaySpeed() 
{
	return mPlaySpeed;
}


template <class ReturnType, class StorageType>
inline float KeyTrack<ReturnType, StorageType>::GetTime() 
{
	return mCurrentTime;
}


template <class ReturnType, class StorageType>
void KeyTrack<ReturnType, StorageType>::Init()
{
	// check all key time values, so we are sure the first key start at time 0
	if (mKeys.GetLength() > 0)
	{
		// get the time value of the first key, which is our minimum time
		float minTime = mKeys[0].GetTime();

		// if it's not equal to zero, we have to correct it (and all other keys as well)
		if (minTime > 0)
		{
			const int numKeys = mKeys.GetLength();
			for (int i=0; i<numKeys; i++)
				mKeys[i].SetTime( mKeys[i].GetTime() - minTime );
		}
	}

	// initialize the interpolator
	if (mInterpolator)
		mInterpolator->Init();

	mInitialized = true;
}


template <class ReturnType, class StorageType>
inline KeyFrame<ReturnType, StorageType>* KeyTrack<ReturnType, StorageType>::GetKey(const int nr)
{
	assert(nr<mKeys.GetLength());
	return &mKeys[nr];
}


template <class ReturnType, class StorageType>
inline KeyFrame<ReturnType, StorageType>* KeyTrack<ReturnType, StorageType>::GetFirstKey()
{
	if (mKeys.GetLength())
		return &mKeys[0];
	
	return NULL;
}


template <class ReturnType, class StorageType>
inline KeyFrame<ReturnType, StorageType>* KeyTrack<ReturnType, StorageType>::GetLastKey()
{
	if (mKeys.GetLength())
		return &mKeys.GetLast();
	
	return NULL;
}


template <class ReturnType, class StorageType>
inline float KeyTrack<ReturnType, StorageType>::GetFirstTime() 
{
	if (GetFirstKey())
		return GetFirstKey()->GetTime();
	
	return 0.0f;
}


template <class ReturnType, class StorageType>
inline float KeyTrack<ReturnType, StorageType>::GetLastTime() 
{
	if (GetLastKey())
		return GetLastKey()->GetTime();
	
	return 0.0f;
}


template <class ReturnType, class StorageType>
inline int KeyTrack<ReturnType, StorageType>::GetNumKeys() 
{
	return mKeys.GetLength();
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::AddKey(const float time, const ReturnType& value)
{
	//AddKey( new KeyFrame<class ReturnType, class StorageType>(time, value) );

	#ifdef _DEBUG
		if (mKeys.GetLength() > 0)
			assert(time >= mKeys.GetLast().GetTime());
	#endif

	// you have to re-init after you added a key
	mInitialized = false;

	// not the first key, so add on the end
	mKeys.Add( KeyFrame<ReturnType, StorageType>(time, value) );
}


// find a key at a given time
template <class ReturnType, class StorageType>
inline int KeyTrack<ReturnType, StorageType>::FindKeyNumber(const float curTime) 
{
	return KeyFrameFinder<ReturnType, StorageType>::FindKey(curTime, this);
}



// find a key at a given time
template <class ReturnType, class StorageType>
inline KeyFrame<ReturnType, StorageType>* KeyTrack<ReturnType, StorageType>::FindKey(const float curTime) 
{
	// find the key number
	const int keyNumber = KeyFrameFinder<ReturnType, StorageType>::FindKey(curTime, this);

	// if no key was found
	if (keyNumber == -1)
		return NULL;

	// return the pointer to the key
	return mKeys[ keyNumber ];
}



// update playback time etc
template <class ReturnType, class StorageType>
void KeyTrack<ReturnType, StorageType>::Update(const double timePassed)
{
	// if we aren't in automatic playback mode, we have nothing to do here! :)
	if (!mIsPlaying) return;

	// calculate the new delta time
	float deltaTime = (timePassed * mPlaySpeed);

	// if we are playing backwards
	if (!mForward)
		deltaTime = -deltaTime;

	// calculate the new current time, however it can still be out of range
	float newTime = mCurrentTime + deltaTime;

	// set the new curent time, automatically clamps given timevalue in a valid range
	SetTime(newTime);
}


// returns the time when looping, it kindof recursively gets the new time
// imagine there is one frame which takes 10 seconds long, while the total track animation time
// is only 7 seconds. this function will return 3 in this case, since after the 10 second frame
// the animation should be at 3 seconds, and cannot just be set to 0, because that will introduce
// out-of-sync animations
template <class ReturnType, class StorageType>
float KeyTrack<ReturnType, StorageType>::GetLoopTime(const float newTime, const float minTime, const float maxTime, const bool forward)
{
	float range = maxTime - minTime; assert(range>=0.0f);
	if (range == 0.0f) return 0.0f;

	float result = MCore::Math::Abs(newTime);

	while (result > range)
		result -= range;

	if (forward)
		return result;
	else
		return maxTime + result;
}


// set the new time value
template <class ReturnType, class StorageType>
void KeyTrack<ReturnType, StorageType>::SetTime(const float newTimeValue)
{
	float minTime = GetFirstTime();
	float maxTime = GetLastTime();
	float newTime = newTimeValue;

	// if we are in a normal looping mode
	if (mLoopMode == KEYTRACK_LOOP)
	{
		if (mForward)
		{
			if (newTime > maxTime)
				newTime = GetLoopTime(newTime, minTime, maxTime, true);

			if (newTime < minTime)
				newTime = minTime;
		}
		else
		{
			if (newTime < minTime)
				newTime = GetLoopTime(newTime, minTime, maxTime, false);

			if (newTime > maxTime)
				newTime = maxTime;
		}
	}
	else
	{
		// clamp the time into valid ranges
		if (newTime < minTime)
			newTime = minTime;
		else
			if (newTime > maxTime)
				newTime = maxTime;
	}

	// set the current time
	mCurrentTime = newTime;
}


// returns the interpolated value at a given time
template <class ReturnType, class StorageType>
ReturnType KeyTrack<ReturnType, StorageType>::GetValueAtTime(const float currentTime)
{
	assert(mInterpolator != NULL);
	assert(currentTime >= 0.0);
	assert(mKeys.GetLength() > 0);

	// find the first key to start interpolating from (between this one and the next)
	const int keyNumber = KeyFrameFinder<ReturnType, StorageType>::FindKey(currentTime, this);

	// if no key could be found
	if (keyNumber == -1)
	{
		// if there are no keys at all, simply return an empty object
		if (mKeys.GetLength() == 0)
		{
			// return an empty object
			//ReturnType result;
			//memset(&result, 0, sizeof(ReturnType));
			//return result;
			return ReturnType();
		}

		// return the last key
		return mKeys.GetLast().GetValue();
	}

	// check if we didn't reach the end of the track
	if (keyNumber+1 > mKeys.GetMaxIndex())
	{
		return mKeys.GetLast().GetValue();
	}

	// calculate the time value in range of [0..1]
	float timeValue = (currentTime - mKeys[keyNumber].GetTime()) / (mKeys[keyNumber+1].GetTime() - mKeys[keyNumber].GetTime()); 

	// interpolate between the two keys
	return mInterpolator->Interpolate(keyNumber, timeValue);
}


// adds a keyframe with automatic sorting support
template <class ReturnType, class StorageType>
void KeyTrack<ReturnType, StorageType>::AddKeySorted(const float time, const ReturnType& value)
{
	// if there are no keys yet, add it
	if (mKeys.GetLength() == 0)
	{
		mKeys.Add( KeyFrame<ReturnType, StorageType>(time, value) );
		return;
	}

	// get the keyframe time
	const float keyTime = time;

	// if we must add it at the end
	if (keyTime >= mKeys.GetLast().GetTime())
	{
		mKeys.Add( KeyFrame<ReturnType, StorageType>(time, value) );
		return;
	}

	// if we have to add it in the front
	if (keyTime < mKeys.GetFirst().GetTime())
	{
		mKeys.Insert(0, KeyFrame<ReturnType, StorageType>(time, value) );
		return;
	}

	// quickly find the location to insert, and insert it
	int place = KeyFrameFinder<ReturnType, StorageType>::FindKey(keyTime, this);
	mKeys.Insert(place+1, KeyFrame<ReturnType, StorageType>(time, value));
}


template <class ReturnType, class StorageType>
void KeyTrack<ReturnType, StorageType>::SetInterpolator(Interpolator<ReturnType, StorageType>* newInterpolator)
{
	// get rid of the existing interpolator
	if (mInterpolator)
		delete mInterpolator;

	// set the new interpolator
	mInterpolator = newInterpolator;
	mInterpolator->SetKeyTrack( this );
}


template <class ReturnType, class StorageType>
inline Interpolator<ReturnType, StorageType>* KeyTrack<ReturnType, StorageType>::GetInterpolator() 
{
	return mInterpolator;
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::SetTimeNormalized(const float normalizedTimeValue)
{
	mCurrentTime = normalizedTimeValue * mKeys[mKeys.GetMaxIndex()].GetTime();
}


template <class ReturnType, class StorageType>
inline float KeyTrack<ReturnType, StorageType>::GetTimeNormalized() 
{
	return mCurrentTime / mKeys[mKeys.GetMaxIndex()].GetTime();
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::SetLoopMode(EKeyTrackLoopMode mode)
{
	mLoopMode = mode;
}


template<class ReturnType, class StorageType>
inline typename KeyTrack<ReturnType, StorageType>::EKeyTrackLoopMode KeyTrack<ReturnType, StorageType>::GetLoopMode() 
{
	return mLoopMode;
}


template <class ReturnType, class StorageType>
inline void KeyTrack<ReturnType, StorageType>::RemoveKey(const int keyNr)
{
	mKeys.Remove(keyNr);
}


template <class ReturnType, class StorageType>
void KeyTrack<ReturnType, StorageType>::MakeLoopable(const float fadeTime)
{
	assert(fadeTime > 0);

	if (mKeys.GetLength() == 0)
		return;

	float lastTime = GetLastKey()->GetTime();
	ReturnType firstValue = GetFirstKey()->GetValue();
	AddKey(lastTime + fadeTime, firstValue);

	// re-init the track
	Init();
}
