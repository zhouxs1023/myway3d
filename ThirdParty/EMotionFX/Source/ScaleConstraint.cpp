/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "ScaleConstraint.h"
#include "Node.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
ScaleConstraint::ScaleConstraint(Actor* actor, Node* node, Node* target) : Constraint(actor, node, target)
{
}


// destructor
ScaleConstraint::~ScaleConstraint()
{
}


// main update method
void ScaleConstraint::Update(const float timeDeltaInSeconds)
{
	assert(mTarget != NULL);
	assert(mNode != NULL);

	mNode->SetLocalScale( mTarget->GetLocalScale() );
}


// clone the constraint
Controller* ScaleConstraint::Clone(Actor* actor)
{
	return new ScaleConstraint(actor, actor->GetNodeByID(mNode->GetID()), mTarget);
}


// get the type ID
int ScaleConstraint::GetType() const
{
	return ScaleConstraint::TYPE_ID;
}


// get the type string
const char* ScaleConstraint::GetTypeString() const
{
	return "ScaleConstraint";
}


} // namespace EMotionFX