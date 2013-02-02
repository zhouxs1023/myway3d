/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "PlayBackInfo.h"
#include "PlayModeForward.h"
#include "PlayModeBackward.h"
//#include "PlayModePingPong.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
PlayBackInfo::PlayBackInfo()
{
	mBlendInTime		 = 0.3f;
	mBlendOutTime		 = 0.3f;
	mPlaySpeed			 = 1.0f;
	mTargetWeight		 = 1.0f;
	mNumLoops			 = FOREVER;
	mBlendMode			 = BLENDMODE_OVERWRITE;
	mPlayMode			 = new PlayModeForward();
	mPlayMask			 = PLAYMASK_POSROT;
	mPlayNow			 = true;
	mMix				 = false;
	mRepositionAfterLoop = false;
	mPriorityLevel		 = 0;
}


// destructor
PlayBackInfo::~PlayBackInfo()
{
}


// set a new play mode
void PlayBackInfo::SetPlayMode(PlayMode* mode)
{
	// check if we are dealing with a valid play mode
	if (!mode)
		return;

	// get rid of the old play mode
	if (mPlayMode)
		delete mPlayMode;

	mPlayMode = mode;
}


// return the play mode
PlayMode* PlayBackInfo::GetPlayMode() const
{
	return mPlayMode;
}

} // namespace EMotionFX