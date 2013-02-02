/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "./CCDIKSolver.h"
#include "Node.h"
#include "Actor.h"
#include "NodeLimitAttribute.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// the constructor
CCDIKSolver::CCDIKSolver(Actor* actor, Node* startNode, Node* endNode, bool cloneNodes) : Controller( actor )
{
	mStartNode	= startNode;
	mEndNode	= endNode;

	// if we want to clone the IK chain
	if (cloneNodes) 
	{
		mEndNode_Clone = endNode->Clone(actor);
		Node *tempNode = mEndNode_Clone;

		Node *i = endNode->GetParent();
		while (i != startNode)
		{
			tempNode->SetParent(i->Clone(actor));
			tempNode->GetParent()->AddChild(tempNode);
			tempNode = tempNode->GetParent();
			i = i->GetParent();
		}

		mStartNode_Clone = mStartNode->Clone(actor);
		tempNode->SetParent(mStartNode_Clone);
		mStartNode_Clone->AddChild(tempNode);
		mStartNode_Clone->SetParent(startNode->GetParent());

		// when cloning nodes, one iteration is enough to find the new solution
		mMaxIterations  = 1;
	}
	else
	{
		mEndNode_Clone	 = mEndNode;
		mStartNode_Clone = mStartNode;
		
		// The solver usually doesn't take more than 10 iterations to find the solution if its in reach.
		// Its usually below 5. If this is set higher, then the solver will meerly take longer to realise
		// that it cant reach the goal.
		mMaxIterations  = 10;
	}
	
	mHasSolution	= false;
	mDistThreshold	= 0.1f;
	mDoJointLimits  = true;
	mGoal.Set(0, 0, 0);
}


// the destructor
CCDIKSolver::~CCDIKSolver()
{
	// if cloning is disabled, simply return, since we dont' need to delete the cloned chain
	if (mStartNode_Clone == mStartNode) return;

	// delete all the cloned nodes
	Node* i = mEndNode_Clone;
	Node* startNodeParent = mStartNode_Clone->GetParent();
    while (i != startNodeParent)
	{
		Node *tempNode = i->GetParent();
		delete i;
		i = tempNode;
	}
}


// get the controller type
int CCDIKSolver::GetType() const
{
	return CCDIKSolver::TYPE_ID;
}


// get the type string
const char* CCDIKSolver::GetTypeString() const
{
	return "CCDIKSolver";
}


// set the goal position in world space
void CCDIKSolver::SetGoal(const MCore::Vector3& goal)
{
	mGoal = goal;
}


// set the mDoJointLimits variable
void CCDIKSolver::SetDoJointLimits(const bool doJointLimits)
{
	mDoJointLimits = doJointLimits;
}


// check if we want to limit the joints
bool CCDIKSolver::GetDoJointLimits() const
{
	return mDoJointLimits;
}


// get the goal position in world space
const MCore::Vector3& CCDIKSolver::GetGoal() const
{
	return mGoal;
}


// check if we found a solution or not
bool CCDIKSolver::HasFoundSolution() const
{
	return mHasSolution;
}


// set the number of maximum iterations
void CCDIKSolver::SetMaxIterations(const int numIterations)
{
	mMaxIterations = numIterations;
}


// set the distance threshold
void CCDIKSolver::SetDistThreshold(const float distThreshold)
{
	mDistThreshold = distThreshold;
}


// get the distance threshold
float CCDIKSolver::GetDistThreshold() const
{
	return mDistThreshold;
}


// get the number of maximum iterations
int CCDIKSolver::GetMaxIterations() const
{
	return mMaxIterations;
}


// clone the controller
Controller* CCDIKSolver::Clone(Actor* actor)
{
	Node* newStartNode = actor->GetNodeByID( mStartNode->GetID() );
	Node* newEndNode   = actor->GetNodeByID( mEndNode->GetID() );

	assert( newStartNode );
	assert( newEndNode );

	return new CCDIKSolver(actor, newStartNode, newEndNode, (mStartNode==mStartNode_Clone ? false : true));
}


// the main update method
void CCDIKSolver::Update(const float timeDeltaInSeconds)
{
	// if we are using cloned nodes
	if (mStartNode_Clone != mStartNode)
	{
		// make sure that the cloned nodes are all still attached to the root of the chain
		// they can and will become distant if we dont do this
		Matrix mat = mStartNode_Clone->GetLocalTM();
		mat.SetTranslation(mStartNode->GetLocalTM().GetTranslation());
		mStartNode_Clone->SetLocalTM(mat);
		mStartNode_Clone->RecursiveUpdateWorldTM(NULL);
	}

	// some values we need
	int		curIteration  = 0;
	float	dist		  = FLT_MAX;
	Node*	curNode		  = mEndNode_Clone->GetParent();

	// the CCD iteration loop
	while (curIteration < mMaxIterations && dist > mDistThreshold)
	{
		// required bone positions
		Vector3 rootPos		= curNode->GetWorldPos();
		Vector3 curEnd		= mEndNode_Clone->GetWorldPos();
		Vector3 desiredEnd	= mGoal;

		// if we are far enough from the goal
		dist = (desiredEnd - curEnd).SquareLength();
		if (dist > mDistThreshold)
		{
			// vector to the current effector pos
			Vector3 curVector = (curEnd - rootPos).Normalize();

			// desired effector position vector
			Vector3 targetVector = (mGoal - rootPos).Normalize();

			// calculate the cosine angle
			const float cosAngle = targetVector.Dot( curVector );

			// if the dot product is 1, we don't need to rotate, since it is 0 degrees
			if (cosAngle < 0.99999f)
			{
				// calculate the rotation axis
				Vector3 crossResult = curVector.Cross( targetVector );
				crossResult.Normalize();

				// get the rotation angle in radians
				const float turnAngle = Math::ACos( cosAngle );

				// rotate our matrix
				Matrix rotMat;
				rotMat.SetRotationMatrixAxisAngle(crossResult, turnAngle);

				Matrix tempMat = curNode->GetWorldTM() * rotMat;
				tempMat.SetTranslation( curNode->GetWorldPos() );

				// Get our rotational limit attribute
				NodeLimitAttribute* attribute = (NodeLimitAttribute*)curNode->GetAttributeByType( NodeLimitAttribute::TYPE_ID );

				// no need to do all these calculations if there is no valid NodeLimitAttribute or if the user
				// doesnt want us to do joint limits, or if there is no parent to constrain against
				if (attribute && mDoJointLimits && (curNode->GetParent() != NULL))
				{
					// NOTE: 3DS Max seems to need us to invert the angles
					// TODO: verify this with Maya
					Vector3 min = -attribute->GetRotationMax();
					Vector3 max = -attribute->GetRotationMin();

					// Instead of:
					// Vector3 max = attribute->GetRotationMax();
					// Vector3 min = attribute->GetRotationMin();

					// convert the matrix into local coordinates of the node
					tempMat *= curNode->GetParent()->GetWorldTM().Inversed();
					
					// calculate the euler representation of this matrix
					Vector3 tempEuler = tempMat.CalcEulerAngles();

					// apply the constraints (simple cliping)
					if (tempEuler.x > max.x) tempEuler.x = max.x;
					if (tempEuler.x < min.x) tempEuler.x = min.x;
					if (tempEuler.y > max.y) tempEuler.y = max.y;
					if (tempEuler.y < min.y) tempEuler.y = min.y;
					if (tempEuler.z > max.z) tempEuler.z = max.z;
					if (tempEuler.z < min.z) tempEuler.z = min.z;

					// rebuild the matrix from the modified euler angles
					tempMat.SetRotationMatrixEuler(tempEuler);

					// convert back into world co-ordinates
					tempMat *= curNode->GetParent()->GetWorldTM();
				}

				// add correct scale and translation to the rotation matrix
				Vector3 scale = curNode->GetWorldScale();
				tempMat.Scale(scale.x, scale.y, scale.z);
				tempMat.SetTranslation( curNode->GetWorldPos() );

				// recursively update the world TM (forward kinematics)
				curNode->RecursiveUpdateWorldTM( &tempMat );
			}

			// go one node up in the hierarchy (towards the front of the chain)
			curNode = curNode->GetParent();

			// if we reach the start of the chain, go to the end again and increment the iteration counter
			if (curNode == mStartNode_Clone->GetParent())
			{
				curNode = mEndNode_Clone->GetParent();
				curIteration++;
			}
		}
	}

	// check if we found a solution
	if (curIteration >= mMaxIterations)
		mHasSolution = false;
	else
		mHasSolution = true;

	// if we are not using cloned nodes
	// then we dont need to copy the positions from the cloned chain into the real chain
	if (mStartNode_Clone == mStartNode)
		return;

	// copy the positions of all the cloned nodes into the real nodes
	Node *n1 = mEndNode_Clone;
	Node *n2 = mEndNode;
	while (true)
	{
		n2->SetWorldTM( n1->GetWorldTM() );
		n2->SetLocalTM( n1->GetLocalTM() );

		n1 = n1->GetParent();
		n2 = n2->GetParent();

		if (n1 == mStartNode_Clone->GetParent() || n2 == mStartNode->GetParent()) 
		{
			if (n1 == mStartNode_Clone->GetParent() && n2 == mStartNode->GetParent())
				break;
			else
			{
				LOG("Something is VERY wrong in the CCD controller");
				LOG("n1='%s' - n2='%s'", n1->GetName().AsChar(), n2->GetName().AsChar());
				break;
			}
		}
	}

	// make sure that all the children that AREN'T in the cloned chain are updated properly
	mStartNode->RecursiveUpdateWorldTM(NULL);
}


} // namespace EMotionFX