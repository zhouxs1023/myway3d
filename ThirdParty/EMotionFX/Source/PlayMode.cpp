/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "PlayMode.h"
#include "MotionInstance.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
PlayMode::PlayMode() : mMotionInst(NULL)
{
}


// destructor
PlayMode::~PlayMode()
{
}

// set the motion instance
void PlayMode::SetMotionInstance(MotionInstance* motionInst)
{
	mMotionInst = motionInst;
}


// get the motion instance
MotionInstance* PlayMode::GetMotionInstance() const
{
	return mMotionInst;
}

} // namespace EMotionFX