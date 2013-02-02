/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Controller.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

Controller::Controller(Actor* actor) : mActor(actor), mIsEnabled(true)
{
}


Controller::~Controller()
{
}


Actor* Controller::GetActor() const
{
	return mActor;
}


bool Controller::IsEnabled() const
{ 
	return mIsEnabled; 
}


void Controller::SetEnabled(const bool enabled)
{
	mIsEnabled = enabled;
}


} // namespace EMotionFX