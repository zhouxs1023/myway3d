/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "FacialMotion.h"


inline int FacialMotion::GetNumExpMotionParts() const
{
	return mExpMotionParts.GetLength();
}


inline int FacialMotion::GetExpID(const int motionPartNr) const
{
	return mExpMotionParts[motionPartNr]->GetID();
}


inline KeyTrack<float, MCore::Compressed8BitFloat>* FacialMotion::GetExpKeyTrack(const int motionPartNr)
{
	return mExpMotionParts[motionPartNr]->GetKeyTrack();
}


inline int FacialMotion::GetNumPhoMotionParts() const
{
	return mPhoMotionParts.GetLength();
}


inline int FacialMotion::GetPhoID(const int motionPartNr) const
{
	return mPhoMotionParts[motionPartNr]->GetID();
}


inline KeyTrack<float, MCore::Compressed8BitFloat>* FacialMotion::GetPhoKeyTrack(const int motionPartNr)
{
	return mPhoMotionParts[motionPartNr]->GetKeyTrack();
}
