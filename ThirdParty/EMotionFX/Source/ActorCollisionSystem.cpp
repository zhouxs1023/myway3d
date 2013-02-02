/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "ActorCollisionSystem.h"


namespace EMotionFX
{

// constructor
ActorCollisionSystem::ActorCollisionSystem(Actor* actor)
{
	assert(actor != NULL);
	mActor = actor;
}


// destructor
ActorCollisionSystem::~ActorCollisionSystem()
{
}


} // namespace EMotionFX