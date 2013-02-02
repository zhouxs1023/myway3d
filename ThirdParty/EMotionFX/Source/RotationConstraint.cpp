/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "RotationConstraint.h"
#include "Node.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
RotationConstraint::RotationConstraint(Actor* actor, Node* node, Node* target) : Constraint(actor, node, target)
{
	mDamping = 0.0f;
}

// destructor
RotationConstraint::~RotationConstraint()
{
}


// the main update method
void RotationConstraint::Update(const float timeDeltaInSeconds)
{
	assert(mTarget != NULL);
	assert(mNode != NULL);

	// damped following
	// if it is directly at the target, so without any delays, then the damp value is 0
	// otherwise the constraint has no influence, so damping is 1
	mNode->SetLocalRot( mNode->GetLocalRot().Slerp( mTarget->GetLocalRot(), 1.0f - mDamping ) );
}


// clone the constraint
Controller* RotationConstraint::Clone(Actor* actor)
{
	return new RotationConstraint(actor, actor->GetNodeByID(mNode->GetID()), mTarget);
}


// get the type ID
int RotationConstraint::GetType() const
{
	return RotationConstraint::TYPE_ID;
}


// get the type string
const char* RotationConstraint::GetTypeString() const
{
	return "RotationConstraint";
}


// set the damping value of the rotation constraint
void RotationConstraint::SetDamping(const float damping)
{
	mDamping = damping;
}


// get the damping value of the rotation constraint
float RotationConstraint::GetDamping() const
{
	return mDamping;
}

} // namespace EMotionFX