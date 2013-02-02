/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "FacialSetup.h"


inline int FacialSetup::GetNumExpressionParts() const
{
	return mExpressionParts.GetLength();
}

inline ExpressionPart* FacialSetup::GetExpressionPart(const int nr) const
{
	return mExpressionParts[nr];
}

inline int FacialSetup::GetNumPhonemes() const
{
	return mPhonemes.GetLength();
}

inline ExpressionPart* FacialSetup::GetPhoneme(const int nr) const
{
	return mPhonemes[nr];
}
