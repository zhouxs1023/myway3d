/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionLink.h"


inline MotionPart* MotionLink::GetMotionPart() const
{
	return mMotionPart;
}


inline MotionInstance* MotionLink::GetMotionInstance() const
{
	return mMotionInstance;
}