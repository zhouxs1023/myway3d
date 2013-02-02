/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKData.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
JacobianIKData::JacobianIKData()
{
	mJacobianActorDOF		= NULL;
	mActorInstance			= NULL;
	mJacobianActorDOF		= NULL;
}


// destructor
JacobianIKData::~JacobianIKData()
{
	// get rid of the actor DOF
	if (mJacobianActorDOF)
		delete mJacobianActorDOF;

	// get rid of the actor instance
	if (mActorInstance)
		delete mActorInstance;
}


void JacobianIKData::SetActor(Actor* actor)
{
	// delete the old instance if there is already one
	if (mActorInstance)
		delete mActorInstance;

	// create an actor instance
	mActor = actor;
	mActorInstance = mActor->Clone(Actor::CLONE_NODEATTRIBUTES);

	// create an actor dof class over the actor instance
	mJacobianActorDOF = new JacobianIKActorDOF();
	mJacobianActorDOF->Update(mActorInstance);
}


Vector3 JacobianIKData::CalcPosDer(Node* node, const int DOFIndex)
{
	if (!mJacobianActorDOF->GetInfluence(node, DOFIndex))
		return Vector3(0.0, 0.0, 0.0);
	else
	{
		// position of the DOF node. The node turns around this one
		Vector3 DOFNodePosition = mJacobianActorDOF->GetDOFNode(DOFIndex)->GetWorldPos();
		// position of the node
		Vector3 position = node->GetWorldPos();
		// DOF axis
		Vector3 DOFAxis = mJacobianActorDOF->GetDOFAxis(DOFIndex);

		// derivative of the node position
		// dP/da = DOFAxis ^ (position - DOFNodePosition)
		return DOFAxis.Cross(position - DOFNodePosition);
	}
}


void JacobianIKData::GetDOFValues(Array<float>& values)
{
	// clear the array
	values.Clear();

	// retrieve the number of nodes of the actor
	const int numNodes = mActorInstance->GetNumNodes();

	// add each angle value to the array
	for (int i=0; i<numNodes; i++)
	{
		// retrieve the node DOF
		NodeDOF* nodeDOF = ((NodeDOF*)(mActorInstance->GetNode(i)->GetAttributeByType(NodeDOF::TYPE_ID)));

		// iterate through all DOFs, get the node DOF angle value and add it to the array
		for (int j=0; j<nodeDOF->GetNumDOF(); j++)
			values.Add( nodeDOF->GetAngleValue(j) );
	}
}


void JacobianIKData::UpdateConstraints(const float timeDeltaInSeconds)
{
	// get the number of constraints
	const int numConstraints = mConstraints.Size();

	// iterate through all constraints and update them
	for (int i=0; i<numConstraints; i++)
		mConstraints[i]->Update(timeDeltaInSeconds);
}


void JacobianIKData::Apply(Array<float>& solution)
{
	mJacobianActorDOF->SetValues(solution);

	// retrieve the number of nodes of the actor
	const int numNodes = mActor->GetNumNodes();

	mActor->RestoreInitialPose();

	for (int i=0; i<numNodes; i++)
	{
		// retrieve the current instance node
		Node* instanceNode = mActorInstance->GetNode(i);

		// retrieve the current node
		Node* node = mActor->GetNode(i);

		node->SetLocalRot( instanceNode->GetLocalRot() );
		node->SetLocalPos( instanceNode->GetLocalPos() );
		//node->SetLocalScale( instanceNode->GetLocalScale() );
	}

	// needed? (Benny)
	mActor->RecursiveHierarchyUpdate();
}


void JacobianIKData::InitInstance()
{
	// retrieve the number of nodes of the actor
	const int numNodes = mActor->GetNumNodes();

	// iterate through all nodes and initialize them
	for (int i=0; i<numNodes; i++)
	{
		// retrieve the current instance node
		Node* instanceNode = mActorInstance->GetNode(i);

		// retrieve the current node
		Node* node = mActor->GetNode(i);

		instanceNode->SetLocalPos(node->GetLocalPos());
		instanceNode->SetLocalRot(node->GetLocalRot());
		instanceNode->SetLocalScale(node->GetLocalScale());

		instanceNode->SetLocalTM(node->GetLocalTM());
		instanceNode->SetWorldTM(node->GetWorldTM());

		((NodeDOF*)instanceNode->GetAttributeByType(NodeDOF::TYPE_ID))->CalcValues(instanceNode->GetLocalTM(), instanceNode->GetLocalScale());
	}
}


float JacobianIKData::GetMax(const int index)
{
	// retrieve the given node DOF
	NodeDOF* nodeDOF = (NodeDOF*)(mJacobianActorDOF->GetDOFNode(index)->GetAttributeByType(NodeDOF::TYPE_ID));

	// get the node id of the DOF node
	int nodeID = nodeDOF->GetNodeID();

	return nodeDOF->GetMaxValue( index - mJacobianActorDOF->GetNode1stDOF(nodeID) );
}


float JacobianIKData::GetMin(const int index)
{
	// retrieve the given node DOF
	NodeDOF* nodeDOF = (NodeDOF*)(mJacobianActorDOF->GetDOFNode(index)->GetAttributeByType(NodeDOF::TYPE_ID));

	// get the node id of the DOF node
	int nodeID = nodeDOF->GetNodeID();

	return nodeDOF->GetMinValue( index - mJacobianActorDOF->GetNode1stDOF(nodeID) );
}

}