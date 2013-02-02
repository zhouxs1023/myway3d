/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionInstance.h"
#include "Actor.h"
#include "PlaybackInfo.h"
#include "PlayModeForward.h"
#include "PlayModeBackward.h"
//#include "PlayModePingPong.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MotionInstance::MotionInstance(Motion* motion, Actor* actor)
{
	mCurTime		= 0.0;
	mLastCurTime	= 0.0f;
	mPassedTime		= 0.0;
	mPlaySpeed		= 1.0f;
	mTargetWeight	= 1.0f;
	mMotion			= motion;
	mPlayMode		= NULL;
	mPlayMask		= PLAYMASK_POSROT;
	mBlendMode		= BLENDMODE_OVERWRITE;
	mFadeTime		= 0.3;
	mIsBlending		= false;
	mIsMixing		= false;
	mIsStopping		= false;
	mIsPaused		= false;
	mLastLoops		= 0;
	mMaxLoops		= 1;
	mCurLoops		= 0;
	mWeight			= 0.0f;
	mWeightDelta	= 0.0f;
	mActor			= actor;
	mRepositionAfterLoop = false;

	// create the play mode updater
	mPlayMode = new PlayModeForward();
	mPlayMode->SetMotionInstance(this);
}


// destructor
MotionInstance::~MotionInstance()
{
	if (mPlayMode)
		delete mPlayMode;
}


// update the motion instance information
void MotionInstance::Update(const double timePassed)
{
	if (mIsPaused) return;

	// update the last number of loops
	mLastLoops = mCurLoops;

	// update the last current time
	mLastCurTime = mCurTime;

	// if we are blending towards the destination motion or layer
	if (mIsBlending)
	{
		// update the weight
		mWeight += mWeightDelta * timePassed;

		if (mWeightDelta >= 0)
		{
			if (mWeight >= mTargetWeight)
			{
				mWeight = mTargetWeight;
				mIsBlending = false;
			}
		}
		else
		{
			if (mWeight <= mTargetWeight)
			{
				mWeight = mTargetWeight;
				mIsBlending = false;
			}
		}
	}

	// update the motion instance time value
	if (mPlayMode)
		mPlayMode->UpdateMotionInstance(timePassed);
}


// start a blend to the new weight
void MotionInstance::SetWeight(const float targetWeight, const float blendTimeInSeconds)
{
	assert(blendTimeInSeconds >= 0);
	assert(targetWeight >= 0 && targetWeight <= 1);
	mTargetWeight = MCore::Clamp<float>(targetWeight, 0.0f, 1.0f);

	if (blendTimeInSeconds > 0)
	{
		mWeightDelta= (mTargetWeight - mWeight) / blendTimeInSeconds;
		mIsBlending = true;
	}
	else // blendtime is zero
	{
		mWeight		= mTargetWeight;
		mWeightDelta= 0;
		mIsBlending = false;
	}
}


// set a new play mode
void MotionInstance::SetPlayMode(PlayMode* mode)
{
	// check if we are dealing with a valid play mode
	if (!mode)
		return;

	// get rid of the old play mode
	if (mPlayMode)
		delete mPlayMode;

	mPlayMode = mode;
	mPlayMode->SetMotionInstance(this);
}


// return the play mode
PlayMode* MotionInstance::GetPlayMode() const
{
	return mPlayMode;
}

} // namespace EMotionFX