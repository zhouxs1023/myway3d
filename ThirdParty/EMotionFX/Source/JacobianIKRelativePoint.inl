/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKRelativePoint.h"


// set the displacement vector of the ik constraint
inline void JacobianIKRelativePoint::SetDisplacement(const MCore::Vector3& displacement)
{
	mDisplacement = displacement;
}