/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "PhysicsController.h"
#include "NodeLimitAttribute.h"
#include "NodePhysicsAttribute.h"
#include "Actor.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
PhysicsController::PhysicsController(Actor* actor) : Controller(actor)
{
}


// destructor
PhysicsController::~PhysicsController()
{
}


// retrieves the physics info attribute from the specific node
NodePhysicsAttribute* PhysicsController::GetNodePhysicsAttribute(const int index)
{
	// invalid index, out of range
	if (index >= mActor->GetNumNodes() || index < 0)
		return NULL;

	// get a pointer to the node
	Node* node = mActor->GetNode(index);

	// invalid node
	if (!node)
		return NULL;

	// a physics attribute pointer
	NodePhysicsAttribute* physicsAttribute;

	// try to get the attribute by type and type-cast it
	return physicsAttribute = (NodePhysicsAttribute*)node->GetAttributeByType( NodePhysicsAttribute::TYPE_ID );
}


// retrieves the limit attribute from the specific node
NodeLimitAttribute* PhysicsController::GetNodeLimitAttribute(const int index)
{
	// invalid index, out of range
	if (index >= mActor->GetNumNodes() || index < 0)
		return NULL;

	// get a pointer to the node
	Node* node = mActor->GetNode(index);

	// invalid node
	if (!node)
		return NULL;

	// a limit attribute pointer
	NodeLimitAttribute* limitAttribute;

	// try to get the attribute by type and type-cast it
	return limitAttribute = (NodeLimitAttribute*)node->GetAttributeByType( NodeLimitAttribute::TYPE_ID );
}

} // namespace EMotionFX