/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionLayerSystem.h"


// set new motion tree root
inline void MotionLayerSystem::SetMotionTree(MotionLayer* rootLayer)
{
	mMotionTree = rootLayer;
}