/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKActorDOF.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
JacobianIKActorDOF::JacobianIKActorDOF()
{
	mNumDOF	= 0;
	mActor	= NULL;
}


// destructor
JacobianIKActorDOF::~JacobianIKActorDOF()
{
}


// update the jacobian ik actor DOF
void JacobianIKActorDOF::Update()
{
	// retrieve the number of nodes
	const int numNodes = mActor->GetNumNodes();

	// count number of DOFs and record which is the first DOF of each node in the array
	mNumDOF = 0;
	mNode1stDOF.Clear();

	// iterate through all nodes and update the nodes
	for (int i=0; i<numNodes; i++)
	{
		mNode1stDOF.Add(mNumDOF);

		// get the current node
		Node* node = mActor->GetNode(i);

		// retrieve the node DOF from the given node
		NodeDOF* nodeDOF = (NodeDOF*)node->GetAttributeByType(NodeDOF::TYPE_ID);

		// check if the node DOF is valid
		if (!nodeDOF)
		{
			// the node DOF is not valid, create a new one
			nodeDOF = new NodeDOF();

			// add the node DOF to the node
			node->AddAttribute((NodeAttribute*)nodeDOF);
		}

		// set the new node id of the node DOF
		nodeDOF->SetNodeID(i);

		// add number of DOFs of the node DOF to global one
		mNumDOF += nodeDOF->GetNumDOF();
	}

	// calculate the influence of each DOF in each node position and store it in the influence matrix
	CalcInfluence();
}


// calculate the influence matrix
void JacobianIKActorDOF::CalcInfluence()
{
	int i;

	mInfluence.Null(mActor->GetNumNodes(), mNumDOF);
	Stack<Node*> node_stack;

	// retrieve the root node of the actor
	Node* root = mActor->FindRoot();

	// This stack allows a non-recursive hierarchy iteration
	node_stack.Push(root);

	while (node_stack.GetSize()>0)
	{
		Node* node = node_stack.Pop();

		// retrieve the number of childs from the current node
		const int numChilds = node->GetNumChilds();

		if (node != root)
		{
			// retrieve the node DOF from the given node
			NodeDOF* nodeDOF = (NodeDOF*)node->GetAttributeByType(NodeDOF::TYPE_ID);

			// retrieve the node id of the node DOF
			const int nodeID = nodeDOF->GetNodeID();

			// check wheter the node's parent is the root node of the actor
			if (node->GetParent() != root)
			{
				// get the node DOF of the parent
				NodeDOF* parentDOF = (NodeDOF*)node->GetParent()->GetAttributeByType(NodeDOF::TYPE_ID);

				// retrieve the node id of the parent DOF
				const int parentID = parentDOF->GetNodeID();

				// the node is influenced by the same DOFs as its parent...
				for (i=0; i<mNumDOF; i++)
					mInfluence(nodeID, i) = mInfluence(parentID, i);

				int pdof = mNode1stDOF[parentID];

				// ... and by its parent DOFs
				for (i=0; i<parentDOF->GetNumDOF(); i++)
					mInfluence(nodeID, pdof + i) = true;
			}
		}
		
		// introduce node childs into the stack
		for (i=0; i<numChilds; i++)
			node_stack.Push(node->GetChild(i));
	}
}


// calculate the angle values using the actual actor pose
void JacobianIKActorDOF::CalcAnglesValues()
{
	// retrieve the number of nodes of the actor
	const int numNodes = mActor->GetNumNodes();

	// Calculates angles values for each node from its local tranformation matrix
	for (int i=0; i<numNodes; i++)
	{
		// get the current node
		Node* node = mActor->GetNode(i);

		// retrieve the nodeDOF of the node
		NodeDOF* nodeDOF = (NodeDOF *)node->GetAttributeByType(NodeDOF::TYPE_ID);

		// calculate the angle values of the current node DOF
		nodeDOF->CalcValues(node->GetLocalTM(), node->GetLocalScale());
	}
}


// calculate the rotation axis for each DOF
void JacobianIKActorDOF::CalcDOFAxis()
{
	// retrieve the number of nodes of the actor
	const int numNodes = mActor->GetNumNodes();

	// calculate DOFs axis from its parent world transformation matrix 
	// knowing its angles values
	for (int i=0; i<numNodes; i++)
	{
		// get the current node
		Node* node = mActor->GetNode(i);

		// retrieve the nodeDOF of the node
		NodeDOF* nodeDOF = (NodeDOF*)node->GetAttributeByType(NodeDOF::TYPE_ID);

		// calculate the rotation axis of the current node DOF
		nodeDOF->CalcAxis(node->GetParent()->GetWorldTM());
	}
}


// get the node that includes the given node DOF
Node* JacobianIKActorDOF::GetDOFNode(const int DOFIndex)
{
	int n=0;

	while (mNode1stDOF[n] <= DOFIndex)
	{
		n++;	
	}
	n--;

	return mActor->GetNode(n);
}


// get the rotation axis for the given node DOF
Vector3 JacobianIKActorDOF::GetDOFAxis(const int DOFIndex)
{
	int n=0;

	while (mNode1stDOF[n] <= DOFIndex)
	{
		n++;
	}
	n--;

	return ((NodeDOF*)mActor->GetNode(n)->GetAttributeByType(NodeDOF::TYPE_ID))->GetAxis(DOFIndex - mNode1stDOF[n]);
}


// apply the given DOF values to the actor
void JacobianIKActorDOF::SetValues(const Array<float>& values)
{
	int i;

	// retrieve the number of nodes of the actor
	const int numNodes = mActor->GetNumNodes();

	for (i=0; i<numNodes; i++)
	{
		// get the current node from the actor
		Node* node = mActor->GetNode(i);

		// get the node DOF from the current node
		NodeDOF* nodeDOF = (NodeDOF*)node->GetAttributeByType(NodeDOF::TYPE_ID);

		int n1st = mNode1stDOF[i];

		nodeDOF->SetValues(values[n1st], values[n1st + 1], values[n1st + 2]);

		node->SetLocalRot(Quaternion::FromMatrix(nodeDOF->GetMatrix(values[n1st], values[n1st + 1], values[n1st + 2])));
	}

	// update the skeleton of the actor
	mActor->RecursiveHierarchyUpdate();

	// call to CalcAxis for each node
	for (i=0; i<numNodes; i++)
	{
		// get the current node from the actor
		Node* node = mActor->GetNode(i);

		// get the node DOF from the current node
		NodeDOF* nodeDOF = (NodeDOF*)node->GetAttributeByType(NodeDOF::TYPE_ID);

		nodeDOF->CalcAxis(node->GetParent()->GetWorldTM());
	}
}

}