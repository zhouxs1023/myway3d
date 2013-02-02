/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */


inline int NodeDOF::GetNumDOF() const
{
	return mNumDOF;
}


inline void NodeDOF::SetNodeID(const int id)
{
	mNodeID = id;
}


inline int NodeDOF::GetNodeID() const
{
	return mNodeID;
}


inline bool NodeDOF::Rotation(const int r1, const int r2, const int r3) const
{
	return (!mEuler&& 
			 mRotationOrder[0] == r1&& 
			 mRotationOrder[1] == r2&&
			 mRotationOrder[2] == r3);
}


inline bool NodeDOF::Rotation(const int r1, const int r2) const
{
	return (!mEuler&& 
			 mRotationOrder[0] == r1&& 
			 mRotationOrder[1] == r2);
}


inline float NodeDOF::GetAngleValue(const int index) const
{
	return mAngles[index];
}


inline MCore::Vector3 NodeDOF::GetAxis(int index) const
{
	return mAxis[index];
}


inline void NodeDOF::SetValues(const float v1, const float v2, const float v3)
{
	mAngles[0] = v1;
	mAngles[1] = v2;
	mAngles[2] = v3;
}


inline float NodeDOF::GetMaxValue(const int index) const
{
	return mMaxValue[index];
}


inline float NodeDOF::GetMinValue(const int index) const
{
	return mMinValue[index];
}