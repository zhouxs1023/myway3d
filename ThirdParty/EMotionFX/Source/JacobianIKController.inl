/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKController.h"


// return a pointer to the ik data
inline JacobianIKData* JacobianIKController::GetIKData()
{
	return mData;
}


// return node by name of the actor instance
inline Node* JacobianIKController::GetNodeByName(const char* name)
{
	return mData->GetActorInstance()->GetNodeByName(name);
}