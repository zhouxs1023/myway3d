/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "PositionConstraint.h"
#include "Node.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// the constructor
PositionConstraint::PositionConstraint(Actor* actor, Node* node, Node* target) : Constraint(actor, node, target)
{
}


// the destructor
PositionConstraint::~PositionConstraint()
{
}


// the update method
void PositionConstraint::Update(const float timeDeltaInSeconds)
{
	assert(mTarget != NULL);
	assert(mNode != NULL);

	mNode->SetLocalPos( mTarget->GetLocalPos() );
}


// clone the constraint
Controller* PositionConstraint::Clone(Actor* actor)
{
	return new PositionConstraint(actor, actor->GetNodeByID(mNode->GetID()), mTarget);
}


// get the type ID
int PositionConstraint::GetType() const
{
	return PositionConstraint::TYPE_ID;
}


// get the type string
const char* PositionConstraint::GetTypeString() const
{
	return "PositionConstraint";
}


} // namespace EMotionFX