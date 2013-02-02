/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKData.h"


// get the number of constraints the ik solver has to apply
inline int JacobianIKData::GetNumConstraints() const
{
	return mConstraints.GetLength();
}


// get a given jacobian ik constraint by index
inline JacobianIKConstraint *JacobianIKData::GetConstraint(const int index) const
{
	return mConstraints.GetItem(index);
}


// get the node of the given constraint to which the constraint is refered to
inline Node* JacobianIKData::GetConstraintNode(const int index) const
{
	return mConstraints.GetItem(index)->GetNode();
}


// return the actor to which this ik data belongs to
inline Actor* JacobianIKData::GetActor() const
{
	return mActor;
}


// return the cloned actor on which the solver works on
inline Actor* JacobianIKData::GetActorInstance() const
{
	return mActorInstance;
}


// get the number of DOFs in the IK system
inline int JacobianIKData::GetNumDOF() const
{
	return mJacobianActorDOF->GetNumDOF();
}


// get the number of DOFs of the given node
inline int JacobianIKData::GetNumDOF(Node* node) const
{
	return ((NodeDOF*)(node->GetAttributeByType(NodeDOF::TYPE_ID)))->GetNumDOF();
}


// return true if the given DOF has influence in the node position
inline bool JacobianIKData::GetInfluence(Node* node, const int DOFIndex) const
{
	return mJacobianActorDOF->GetInfluence(node, DOFIndex);
}


// return the node index in the actor nodes array
inline int JacobianIKData::GetNodeID(Node* node) const
{
	return ((NodeDOF*)(node->GetAttributeByType(NodeDOF::TYPE_ID)))->GetNodeID();
}


// add a constraint which represents the goal to reach
inline void JacobianIKData::AddConstraint(JacobianIKConstraint* constraint)
{
	mConstraints.Add(constraint);
	constraint->SetIKData(this);
}


// remove a constraint which represents the goal to reach
inline void JacobianIKData::RemoveConstraint(JacobianIKConstraint* constraint)
{
	mConstraints.Remove(constraint);
}


// return the index of the first DOF of the node
inline int JacobianIKData::GetNode1stDOF(Node* node) const
{
	return mJacobianActorDOF->GetNode1stDOF(GetNodeID(node));
}


// apply the values in given array to the DOF on the actor instance
inline void JacobianIKData::SetValues(MCore::Array<float>& values)
{
	mJacobianActorDOF->SetValues(values);
}