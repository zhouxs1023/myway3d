/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionInstance.h"


inline void MotionInstance::SetFadeTime(const double fadeTime)
{
	mFadeTime = fadeTime;
}


inline double MotionInstance::GetFadeTime() const
{
	return mFadeTime;
}


inline double MotionInstance::GetPassedTime() const
{
	return mPassedTime;
}


inline double MotionInstance::GetCurrentTime() const
{
	return mCurTime;
}


inline double MotionInstance::GetMaxTime() const
{
	return mMotion->GetMaxTime();
}


inline float MotionInstance::GetPlaySpeed() const
{
	return mPlaySpeed;
}


inline Motion* MotionInstance::GetMotion() const
{
	return mMotion;
}


inline void MotionInstance::SetCurrentTime(const double time)
{
	mCurTime = time;
}

inline void MotionInstance::SetPassedTime(const double timePassed)
{
	mPassedTime = timePassed;
}

inline void MotionInstance::SetPlaySpeed(const double speed)
{
	mPlaySpeed = speed;
}


inline void MotionInstance::Rewind()
{
	SetCurrentTime(0);
}


inline bool MotionInstance::HasEnded() const
{
	return ((mMaxLoops != -1) && (mCurLoops >= mMaxLoops));
}


inline bool MotionInstance::IsStopping() const
{
	return mIsStopping;
}


inline bool MotionInstance::IsPlaying() const
{
	return (!HasEnded() && !mIsPaused);
}


inline bool MotionInstance::IsMixing() const
{
	return mIsMixing;
}


inline bool MotionInstance::IsBlending() const
{
	return mIsBlending;
}


inline void MotionInstance::SetMixMode(const bool mix)
{
	mIsMixing = mix;
}


inline void MotionInstance::Pause()
{
	mIsPaused = true;
}

inline void MotionInstance::UnPause()
{
	mIsPaused = false;
}

inline void MotionInstance::SetPause(const bool pause)
{
	mIsPaused = pause;
}

inline bool MotionInstance::IsPaused() const
{
	return mIsPaused;
}


inline void MotionInstance::SetMaxLoops(const int numLoops)
{
	mMaxLoops = numLoops;
}


inline int MotionInstance::GetMaxLoops() const
{
	return mMaxLoops;
}


inline void MotionInstance::SetNumCurrentLoops(const int numCurrentLoops)
{
	mCurLoops = numCurrentLoops;
}


inline int MotionInstance::GetNumCurrentLoops() const
{
	return mCurLoops;
}


inline bool MotionInstance::IsPlayingForever() const
{
	return (mMaxLoops == -1);
}


inline void MotionInstance::SetCurrentTimeNormalized(const double normalizedTimeValue)
{
	mCurTime = normalizedTimeValue * GetMaxTime();
}


inline double MotionInstance::GetCurrentTimeNormalized() const
{
	return mCurTime / GetMaxTime();
}


inline EMotionBlendMode MotionInstance::GetBlendMode() const
{
	return mBlendMode;
}


inline void MotionInstance::SetBlendMode(const EMotionBlendMode mode)
{
	mBlendMode = mode;
}


inline float MotionInstance::GetTargetWeight() const
{
	return mTargetWeight;
}


inline float MotionInstance::GetWeight() const
{
	return mWeight;
}


inline int MotionInstance::GetPriorityLevel() const
{
	return mPriorityLevel;
}


inline void MotionInstance::SetPriorityLevel(const int priorityLevel)
{
	mPriorityLevel = priorityLevel;
}


inline void MotionInstance::SetPlayMask(const EPlayMask mask)
{
	mPlayMask = mask;
}


inline EPlayMask MotionInstance::GetPlayMask() const
{
	return mPlayMask;
}


inline bool MotionInstance::HasLooped() const
{
	return (mCurLoops != mLastLoops);
}


inline float MotionInstance::GetLastCurrentTime() const
{
	return mLastCurTime;
}


inline bool MotionInstance::GetRepositionAfterLoop() const
{
	return mRepositionAfterLoop;
}


inline void MotionInstance::SetRepositionAfterLoop(const bool enable)
{
	mRepositionAfterLoop = enable;
}
