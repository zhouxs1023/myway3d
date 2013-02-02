/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "PlayModeForward.h"
#include "MotionInstance.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
PlayModeForward::PlayModeForward() : PlayMode()
{
}


// destructor
PlayModeForward::~PlayModeForward()
{
}


// return the type identification number
int PlayModeForward::GetType() const
{
	return TYPE_ID;
}


// return the type as a description
const char* PlayModeForward::GetTypeString() const
{
	return "PlayModeForward";
}


// get the start time after looping
float PlayModeForward::GetStartTime() const
{
	return 0.0f;
}


// update the motion instance for forward playback
void PlayModeForward::UpdateMotionInstance(const double timeDelta)
{
	// check if it is linked to a valid motion instance
	if (!mMotionInst)
		return;

	// gather information from the motion instance
	double maxTime		= mMotionInst->GetMaxTime();
	double currentTime	= mMotionInst->GetCurrentTime();
	double passedTime	= mMotionInst->GetPassedTime();
	int	   currentLoops	= mMotionInst->GetNumCurrentLoops();

	//assert(timePassed >= 0.0);
	passedTime  = timeDelta * mMotionInst->GetPlaySpeed();
	currentTime	+= passedTime;

	// check and handle the bounds of the current playtime, to make it loop etc
	if (mMotionInst->GetMaxLoops() == -1)	// if it's looping forever
	{
		if (currentTime > maxTime)
		{
			currentLoops++;
			currentTime = fmod(currentTime, maxTime);
		}

		if (currentTime < 0.0)
		{
			while (currentTime < 0.0)
				currentTime = maxTime + currentTime;
		}
	}
	else
	{
		// if we passed the end of the motion, keep it there
		if (currentTime > maxTime)
		{
			currentLoops++;
			if (currentLoops >= mMotionInst->GetMaxLoops())
			{
				currentLoops = mMotionInst->GetMaxLoops();
				currentTime = maxTime;
			}
			else
				currentTime = fmod(currentTime, maxTime);
		}
	}

	// if the current time is before the start of the motion, set it to the start of the motion
	if (currentTime < 0.0)
		currentTime = 0.0;

	// set updated, validated values again
	mMotionInst->SetCurrentTime(currentTime);
	mMotionInst->SetPassedTime(passedTime);
	mMotionInst->SetNumCurrentLoops(currentLoops);
}

} // namespace EMotionFX