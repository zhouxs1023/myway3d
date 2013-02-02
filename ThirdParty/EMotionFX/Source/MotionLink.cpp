/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionLink.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MotionLink::MotionLink(MotionPart* part, MotionInstance* instance) : mMotionPart(part), mMotionInstance(instance)
{
}


// destuctor
MotionLink::~MotionLink()
{
}

}