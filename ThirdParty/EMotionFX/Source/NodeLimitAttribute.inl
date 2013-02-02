/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "NodeLimitAttribute.h"


// set the minimum translation values, and automatically enable the limit to be true
inline void NodeLimitAttribute::SetTranslationMin(const MCore::Vector3& translateMin)
{
	mTranslationMin = translateMin;
}


// set the maximum translation values, and automatically enable the limit to be true
inline void NodeLimitAttribute::SetTranslationMax(const MCore::Vector3& translateMax)
{
	mTranslationMax = translateMax;
}


// set the minimum rotation values, and automatically enable the limit to be true
inline void NodeLimitAttribute::SetRotationMin(const MCore::Vector3& rotationMin)
{
	mRotationMin = rotationMin;
}


// set the maximum rotation values, and automatically enable the limit to be true
inline void NodeLimitAttribute::SetRotationMax(const MCore::Vector3& rotationMax)
{
	mRotationMax = rotationMax;
}


// set the minimum scale values, and automatically enable the limit to be true
inline void NodeLimitAttribute::SetScaleMin(const MCore::Vector3& scaleMin)
{
	mScaleMin = scaleMin;
}

// set the maximum scale values, and automatically enable the limit to be true
inline void NodeLimitAttribute::SetScaleMax(const MCore::Vector3& scaleMax)
{
	mScaleMax = scaleMax;
}


// get the minimum translation values
inline const MCore::Vector3& NodeLimitAttribute::GetTranslationMin() const
{
	return mTranslationMin;
}


// get the maximum translation values
inline const MCore::Vector3& NodeLimitAttribute::GetTranslationMax() const
{
	return mTranslationMax;
}


// get the minimum rotation values
inline const MCore::Vector3& NodeLimitAttribute::GetRotationMin() const
{
	return mRotationMin;
}


// get the maximum rotation values
inline const MCore::Vector3& NodeLimitAttribute::GetRotationMax() const
{
	return mRotationMax;
}


// get the minimum scale values
inline const MCore::Vector3& NodeLimitAttribute::GetScaleMin() const
{
	return mScaleMin;
}


// get the maximum scale values
inline const MCore::Vector3& NodeLimitAttribute::GetScaleMax() const
{
	return mScaleMax;
}


// enable or disable the limit for the specified limit type
inline void NodeLimitAttribute::EnableLimit(const ELimitType limitType, const bool enable)
{
	if (enable)
		mLimitFlags |= limitType;
	else
		mLimitFlags ^= limitType;
}


// toggle limit state
inline void NodeLimitAttribute::ToggleLimit(const ELimitType limitType)
{
	bool enable = !IsLimited( limitType );
	EnableLimit(limitType, enable);
}


// determine if the specified limit is enabled or disabled
inline bool NodeLimitAttribute::IsLimited(const ELimitType limitType) const
{
	return (mLimitFlags & limitType);
}
