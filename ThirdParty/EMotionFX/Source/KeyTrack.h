/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __KEYTRACK_H
#define __KEYTRACK_H

// include Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "KeyFrame.h"
#include "CompressedKeyFrames.h"
#include "Interpolator.h"
#include "KeyFrameFinder.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The keyframe track.
 * This is a class holding a set of keyframes and contains methods for automatic playback.
 */
template <class ReturnType, class StorageType>
class KeyTrack
{
	DECLARE_CLASS(KeyTrack<>);
	MEMORYOBJECTCATEGORY(KeyTrack, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_KEYTRACKS);

	public:
		/// Keyframe track loop modes.
		enum EKeyTrackLoopMode
		{
			KEYTRACK_NO_LOOP = 0x000000,	/**< No looping, stops when track finished. */
			KEYTRACK_LOOP	 = 0x000001		/**< Loop when track finished. */
		};

		/**
		 * Default constructor.
		 */
		KeyTrack();

		/**
		 * Constructor.
		 * @param nrKeys The number of keyframes which the keytrack contains (preallocates this amount of keyframes).
		 */
		KeyTrack(const int nrKeys);

		/**
		 * Destructor.
		 */
		~KeyTrack();

		/**
		 * Set the keytrack loop mode.
		 * @param mode The looping mode to use.
		 */
		inline void SetLoopMode(EKeyTrackLoopMode mode);

		/**
		 * Get the current loop mode of the keytrack.
		 * @result The loop mode.
		 */
		inline EKeyTrackLoopMode GetLoopMode();
		
		/**
		 * Start automatic playback.
		 */
		inline void Play();

		/**
		 * Stop automatic playback.
		 */
		inline void Stop();

		/**
		 * Rewind the keyframer to the first key.
		 */
		inline void Rewind();

		/**
		 * Set the playback speed, where 1.0 is normal playback speed. If you want to play backwards, do NOT use negative values, but use SetPlayForwards(false) instead.
		 * @param speed The playback speed.
		 */
		inline void SetPlaySpeed(const float speed);

		/**
		 * Set the play direction.
		 * @param forward If set to true, we will play forwards, otherwise backwards.
		 */
		inline void SetPlayForwards(const bool forward);

		/**
		 * Check if we are automatically playing or not.
		 * @result Returns true when we are playing, otherwise false.
		 */
		inline bool GetPlaying();

		/**
		 * Check if we are playing forwards or backwards.
		 * @result Returns true when we are playing forwards, otherwise false.
		 */
		inline bool GetPlayForwards();

		/**
		 * Get the play speed factor, where 1.0 would be normal speed and 2.0 would be 
		 * twice the original speed and 0.2 would be slowmotion.
		 * @result The playspeed factor.
		 */
		inline float GetPlaySpeed();

		/**
		 * Return the current time in the automatic playback.
		 * @result The current time, in seconds.
		 */
		inline float GetTime();

		/**
		 * Initialize all keyframes in this keytrack.
		 * Call this after all keys are added and setup, otherwise the interpolation won't work and nothing will happen.
		 */
		void Init();

		/**
		 * Update the playback time if in automatic playback mode.
		 * @param timePassed The time passed, in seconds.
		 */
		void Update(const double timePassed);

		/**
		 * Add a key to the track (at the back).
		 * @param time The time of the keyframe, in seconds.
		 * @param value The value of the keyframe.
		 */
		inline void AddKey(const float time, const ReturnType& value);

		/**
		 * Adds a key to the track, and automatically detects where to place it.
		 * NOTE: you will have to call the Init() method again when you finished adding keys.
		 * @param time The time value of the keyframe, in seconds.
		 * @param value The value of the keyframe at that time.
		 */
		inline void AddKeySorted(const float time, const ReturnType& value);

		/**
		 * Remove a given keyframe, by number.
		 * Do not forget that you have to re-initialize the keytrack after you have removed one or more
		 * keyframes. The reason for this is that the possible tangents inside the interpolators have to be
		 * recalculated when the key structure has changed.
		 * @param keyNr The keyframe number, must be in range of [0..GetNumKeys()-1].
		 */
		inline void RemoveKey(const int keyNr);

		/**
		 * Clear all keys.
		 */
		void ClearKeys();

		/**
		 * Set the current play time, so the offset in the animation.
		 * @param newTimeValue The new time value, in seconds.
		 */
		void SetTime(const float newTimeValue);

		/**
		 * Set the current play time, so the offset in the animation. Normalized in this
		 * case means from 0.0 to 1.0. The maximum time of this animation is then 1.0.
		 * @param newTimeValue The new normalized time value, in seconds.
		 */
		inline void SetTimeNormalized(const float normalizedTimeValue);

		/**
		 * Return the current time in the automatic playback. Normalized in this
		 * case means from 0.0 to 1.0. The maximum time of this animation is then 1.0.
		 * @result The normalized current time, in seconds.
		 */
		inline float GetTimeNormalized();

		/**
		 * Get the interpolated value at the current time (use this when using automatic playback).
		 * @result Returns the value at the current time.
		 */
		inline ReturnType GetCurrentValue();

		/**
		 * Return the interpolated value at the specified time.
		 * @param currentTime The time in seconds.
		 * @result Returns the value at the specified time.
		 */
		ReturnType GetValueAtTime(const float currentTime);

		/**
		 * Get a given keyframe.
		 * @param nr The index, so the keyframe number.
		 * @result A pointer to the keyframe.
		 */
		inline KeyFrame<ReturnType, StorageType>* GetKey(const int nr);

		/**
		 * Returns the first keyframe.
		 * @result A pointer to the first keyframe.
		 */
		inline KeyFrame<ReturnType, StorageType>* GetFirstKey();

		/**
		 * Returns the last keyframe.
		 * @result A pointer to the last keyframe.
		 */
		inline KeyFrame<ReturnType, StorageType>* GetLastKey();

		/**
		 * Returns the time value of the first keyframe.
		 * @result The time value of the first keyframe, in seconds.
		 */
		inline float GetFirstTime();

		/**
		 * Return the time value of the last keyframe.
		 * @result The time value of the last keyframe, in seconds.
		 */
		inline float GetLastTime();

		/**
		 * Returns the number of keyframes in this track.
		 * @result The number of currently stored keyframes.
		 */
		inline int GetNumKeys();

		/**
		 * Find a key at a given time.
		 * @param curTime The time of the key.
		 * @return A pointer to the keyframe.
		 */
		inline KeyFrame<ReturnType, StorageType>* FindKey(const float curTime);

		/**
		 * Find a key number at a given time.
		 * You will need to interpolate between this and the next key.
		 * @param curTime The time to retreive the key for.
		 * @result Returns the key number or -1 when not found.
		 */
		inline int FindKeyNumber(const float curTime);

		/**
		 * Set the interpolator to be used for interpolation of the keyframes.
		 * The existing interpolator for this keytrack will automatically be deleted from memory.
		 * @param newInterpolator The interpolator to use.
		 */
		void SetInterpolator(Interpolator<ReturnType, StorageType>* newInterpolator);

		/**
		 * Get the currently used interpolator.
		 * @result A pointer to the interpolator.
		 */
		inline Interpolator<ReturnType, StorageType>* GetInterpolator();

		/**
		 * Make the keytrack loopable, by adding a new keyframe at the end of the keytrack.
		 * This added keyframe will have the same value as the first keyframe.
		 * @param fadeTime The relative offset after the last keyframe. If this value is 0.5, it means it will add
		 *                 a keyframe half a second after the last keyframe currently in the keytrack.
		 */
		void MakeLoopable(const float fadeTime = 0.3f);


	protected:
		MCore::Array< KeyFrame<ReturnType, StorageType> >	mKeys;			/**< The collection of keys which form the track. */
		Interpolator<ReturnType, StorageType>*				mInterpolator;	/**< The interpolator to use. */
		EKeyTrackLoopMode				mLoopMode;			/**< The loop mode [default=LOOP]. */
		float							mCurrentTime;		/**< The current time [default=0.0f]. */
		float							mPlaySpeed;			/**< Playback speed [default=1.0f]. */
		bool							mForward;			/**< Is playing forward? */
		bool							mIsPlaying;			/**< Automatic playing enabled? [default=false]. */
		bool							mInitialized;		/**< Are we initialized? */

		/**
		 * Get the time value when looping is enabled. You input a value and it outputs the value in range of the keys.
		 * @param newTime The new time value you want to use.
		 * @param minTime The minimum time of the keyframes.
		 * @param maxTime The maximum time of the keyframes.
		 * @param forward Play forwards?
		 * @result The time value between 'minTime' and 'maxTime'.
		 */
		float GetLoopTime(const float newTime, const float minTime, const float maxTime, const bool forward);
};

// include inline code
#include "KeyTrack.inl"

} // namespace EMotionFX

#endif