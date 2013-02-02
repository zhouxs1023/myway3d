/**
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "TransformConstraint.h"
#include "Node.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
TransformConstraint::TransformConstraint(Actor* actor, Node* node, Node* target) : Constraint(actor, node, target)
{
}


// destructor
TransformConstraint::~TransformConstraint()
{
}


// the main update method
void TransformConstraint::Update(const float timeDeltaInSeconds)
{
	assert(mTarget != NULL);
	assert(mNode != NULL);

	mNode->SetLocalPos  ( mTarget->GetLocalPos() );
	mNode->SetLocalRot  ( mTarget->GetLocalRot() );
	mNode->SetLocalScale( mTarget->GetLocalScale() );
}


// clone the constraint
Controller* TransformConstraint::Clone(Actor* actor)
{
	return new TransformConstraint(actor, actor->GetNodeByID(mNode->GetID()), mTarget);
}


// get the type ID
int TransformConstraint::GetType() const
{
	return TransformConstraint::TYPE_ID;
}


// get the type string
const char* TransformConstraint::GetTypeString() const
{
	return "TransformConstraint";
}


} // namespace EMotionFX