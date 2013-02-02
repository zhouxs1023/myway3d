/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "PlayModeBackward.h"
#include "MotionInstance.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
PlayModeBackward::PlayModeBackward() : PlayMode()
{
}


// destructor
PlayModeBackward::~PlayModeBackward()
{
}


// return the type identification number
int PlayModeBackward::GetType() const
{
	return TYPE_ID;
}


// return the type as a description
const char* PlayModeBackward::GetTypeString() const
{
	return "PlayModeBackward";
}


// get the start time after looping
float PlayModeBackward::GetStartTime() const
{
	return mMotionInst->GetMaxTime();
}


// update the motion instance for reverse playback
void PlayModeBackward::UpdateMotionInstance(const double timeDelta)
{
	// check if it is linked to a valid motion instance
	if (!mMotionInst)
		return;

	// gather information from the motion instance
	double maxTime		= mMotionInst->GetMaxTime();
	double currentTime	= mMotionInst->GetCurrentTime();
	double passedTime	= mMotionInst->GetPassedTime();
	int	   currentLoops	= mMotionInst->GetNumCurrentLoops();

	passedTime  = -(timeDelta * mMotionInst->GetPlaySpeed());
	currentTime	+= passedTime;

	// check and handle the bounds of the current playtime, to make it loop etc
	if (mMotionInst->GetMaxLoops() == -1)	// if it's looping forever
	{
		if (currentTime < 0.0)
		{
			currentLoops++;
			currentTime = maxTime + currentTime;
		}

		if (currentTime > maxTime)
		{
			while (currentTime > maxTime)
				currentTime = 2.0 * maxTime - currentTime;
		}
	}
	else
	{
		// if we passed the end of the motion, keep it there
		if (currentTime < 0.0)
		{
			currentLoops++;
			if (currentLoops >= mMotionInst->GetMaxLoops())
			{
				currentLoops = mMotionInst->GetMaxLoops();
				currentTime = 0.0;
			}
			else
				currentTime = maxTime + currentTime;
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