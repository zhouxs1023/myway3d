/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Constraint.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
Constraint::Constraint(Actor* actor, Node* node, Node* target) : Controller(actor)
{
	mNode		= node;
	mTarget		= target;
}


// desctructor
Constraint::~Constraint()
{
}


// set the target node of the constraint
void Constraint::SetTarget(Node* target)
{
	if (target)
		mTarget = target;
}


// retrieve the target node of the constraint
Node* Constraint::GetTarget() const
{
	return mTarget;
}


// get the node where we apply the constraint on
Node* Constraint::GetNode() const
{
	return mNode;
}

} // namespace EMotionFX