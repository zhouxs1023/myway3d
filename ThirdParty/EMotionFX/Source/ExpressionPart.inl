/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "ExpressionPart.h"


inline void ExpressionPart::SetWeight(const float newWeight)
{
	mCurWeight = newWeight;
}

inline float ExpressionPart::GetWeight() const
{
	return mCurWeight;
}


inline int ExpressionPart::GetID() const
{
	return mID;
}


inline const MCore::String& ExpressionPart::GetName() const
{
	return mName;
}


inline float ExpressionPart::CalcRangedWeight(const float weight)
{
	return mRangeMin + (weight * (mRangeMax - mRangeMin));
}


bool ExpressionPart::IsInManualMode() const
{
	return mManualMode;
}
