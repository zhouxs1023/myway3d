/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKFixedPoint.h"


// set the ik constraint goal position
inline void JacobianIKFixedPoint::SetPosition(const MCore::Vector3& position)
{
	mPosition = position;
}