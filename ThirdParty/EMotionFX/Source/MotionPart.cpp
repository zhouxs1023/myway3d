/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionPart.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{


// constructor
MotionPart::MotionPart()
{
}


// extended constructor
MotionPart::MotionPart(const String& name) : mName(name)
{
}


// destructor
MotionPart::~MotionPart()
{
}

} // namespace EMotionFX