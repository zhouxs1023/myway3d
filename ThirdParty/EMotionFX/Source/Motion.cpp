/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Motion.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
Motion::Motion(const String& name) : mName(name), mMaxTime(0.0)
{
}


// destructor
Motion::~Motion()
{
}

} // namespace EMotionFX