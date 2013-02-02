/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Motion.h"


inline const char* Motion::GetName() const
{
	return mName.GetReadPtr();
}


inline void Motion::SetName(const MCore::String& name)
{
	mName = name;
}


inline float Motion::GetMaxTime() const
{
	return mMaxTime;
}