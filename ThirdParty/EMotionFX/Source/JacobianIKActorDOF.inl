/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKActorDOF.h"


// get the number of DOFs applied to the actor
inline int JacobianIKActorDOF::GetNumDOF() const
{
	return mNumDOF;
}


// update the attributes of the actor's data
inline void JacobianIKActorDOF::Update(Actor* actor)
{
	if (mActor && actor!=mActor)
		mActor = actor; // ERROR!!!

	mActor = actor;

	Update();
}


// return true if the given DOF has influence in the 'node' position
inline bool JacobianIKActorDOF::GetInfluence(Node* node, const int DOFIndex)
{
	// retrieve the given node DOF
	NodeDOF* nodeDOF = (NodeDOF*)node->GetAttributeByType(NodeDOF::TYPE_ID);

	// return if influenced
	return mInfluence(nodeDOF->GetNodeID(), DOFIndex);
}


// return the index of the first DOF referred to any of the node's rotations
inline int JacobianIKActorDOF::GetNode1stDOF(const int index)
{
	return mNode1stDOF[index];
}


// return the actor to which the actor DOF is applied to
inline Actor* JacobianIKActorDOF::GetActor()
{
	return mActor;
}