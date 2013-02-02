/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SkeletalMotion.h"


inline int SkeletalMotion::GetNumParts() const
{
	return mParts.GetLength();
}


inline MotionPart* SkeletalMotion::GetPart(const int nr) const
{
	return mParts[nr];
}


inline void SkeletalMotion::AddPart(MotionPart* part)
{
	mParts.Add(part);
}