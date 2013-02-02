/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKConstraint.h"


// assign the ik data to the constraint
inline void JacobianIKConstraint::SetIKData(JacobianIKData* data)
{
	mIKData = data;
}


// set the constraint's base node
inline void JacobianIKConstraint::SetBaseNode(Node* node)
{
	mBaseNode = node;
}


// set the node to which this constraint belongs to
inline void JacobianIKConstraint::SetNode(Node* node)
{
	mNode = node;
}


// return the constraint's base node
inline Node* JacobianIKConstraint::GetBaseNode()
{
	return mBaseNode;
}


// return the node to which this constraint belongs to
inline Node* JacobianIKConstraint::GetNode()
{
	return mNode;
}