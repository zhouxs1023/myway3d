/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionPart.h"


inline KeyTrack<MCore::Vector3, MCore::Vector3>& MotionPart::GetPosTrack()
{
	return mPosTrack;
}


inline KeyTrack<MCore::Quaternion, MCore::Compressed16BitQuaternion>& MotionPart::GetRotTrack()
{
	return mRotTrack;
}


inline KeyTrack<MCore::Vector3, MCore::Vector3>& MotionPart::GetScaleTrack()
{
	return mScaleTrack;
}


inline const char* MotionPart::GetName() const
{
	return mName.GetReadPtr();
}


inline const MCore::Vector3& MotionPart::GetPosePos() const
{
	return mPosePos;
}


inline const MCore::Quaternion& MotionPart::GetPoseRot() const
{
	return mPoseRot;
}


inline const MCore::Vector3& MotionPart::GetPoseScale() const
{
	return mPoseScale;
}


inline void MotionPart::SetPosePos(const MCore::Vector3& pos)
{
	mPosePos = pos;
}


inline void MotionPart::SetPoseRot(const MCore::Quaternion& rot)
{
	mPoseRot = rot;
}


inline void MotionPart::SetPoseScale(const MCore::Vector3& scale)
{
	mPoseScale = scale;
}
