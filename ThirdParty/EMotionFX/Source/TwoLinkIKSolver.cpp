/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "TwoLinkIKSolver.h"
#include "Node.h"
#include "Actor.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// the constructor	
TwoLinkIKSolver::TwoLinkIKSolver(Actor* actor, Node* startNode, Node* midNode, Node* endNode) : Controller(actor)
{
	mHasSolution = false;
	mStartNode	 = startNode;
	mMidNode	 = midNode;
	mEndNode	 = endNode;
	mGoal		 = endNode->GetWorldPos();
	mGoalNode	 = NULL;
	mBendDirection.Set(0, -1, 0);
}


// the destructor
void TwoLinkIKSolver::Update(const float timeDeltaInSeconds)
{
	// get the worldspace positions of the three nodes
	const Vector3 posA = mStartNode->GetWorldPos();
	const Vector3 posB = mMidNode->GetWorldPos();
	const Vector3 posC = mEndNode->GetWorldPos();

	// get the goal we want (depending on if we want to use a node as goal or not)
	Vector3 goal;
	if (mGoalNode)
		goal = mGoalNode->GetWorldPos();
	else
		goal = mGoal;

	// solve the IK problem by calculating the new position of the mid node in worldspace
	Vector3 midPos;
	mHasSolution = TwoLinkIKSolver::Solve2LinkIK(posA, posB, posC, goal, mBendDirection, &midPos);

	//----------------------------------------
	// calculate the first bone orientation
	//----------------------------------------
	Vector3 forward, up, right, scale;
	Matrix mat(true);

	// if start, mid and end node aren't the same
	if (mStartNode != mMidNode && mMidNode != mEndNode)
	{
		// calculate the matrix orientation
		forward = (midPos - posA).Normalize();
		up		= mStartNode->GetWorldTM().GetUp().Normalize();
		right	= forward.Cross( up ).Normalize();
		up		= right.Cross(forward).Normalize();
		scale	= mStartNode->GetWorldScale();

		// calculate the new matrix
		mat.SetRight( forward );
		mat.SetUp( up );
		mat.SetForward( right );
		mat.Scale(scale.x, scale.y, scale.z);
		mat.SetTranslation( mStartNode->GetWorldPos() );

		// recursively update the bone matrices (forward kinematics)
		mStartNode->RecursiveUpdateWorldTM( &mat );
	}
	else
		midPos = mMidNode->GetWorldPos();

	//----------------------------------------
	// calculate the second bone orientation
	//----------------------------------------
	// get the normalized matrix of the mid node
	Matrix midMat = mMidNode->GetWorldTM().Normalized();

	// calculate the new direction vectors
	forward = (goal - midPos).Normalize();
	up		= midMat.GetUp().Normalize();
	right	= forward.Cross( up ).Normalize();
	up		= right.Cross(forward).Normalize();
	scale	= mMidNode->GetWorldScale();

	// build the matrix with the new orientation and scale
	mat.SetRight( forward );
	mat.SetUp( up );
	mat.SetForward( right );
	mat.Scale(scale.x, scale.y, scale.z);
	mat.SetTranslation( midMat.GetTranslation() );

	// recursively update all child matrices as well (forward kinematics)
	mMidNode->RecursiveUpdateWorldTM( &mat );
}


// set the goal
void TwoLinkIKSolver::SetGoal(const Vector3& goal)
{
	mGoal = goal;
}


// set the bend direction
void TwoLinkIKSolver::SetBendDirection(const Vector3& bendDir)
{
	mBendDirection = bendDir;
}


// get the start node
Node* TwoLinkIKSolver::GetStartNode() const
{
	return mStartNode;
}


// get the end node
Node* TwoLinkIKSolver::GetMidNode() const
{
	return mMidNode;
}


// get the end effector node
Node* TwoLinkIKSolver::GetEndEffector() const
{
	return mEndNode;
}


// get the goal node, which returns NULL when we have to use the vector mGoal as goal
Node* TwoLinkIKSolver::GetGoalNode() const
{
	return mGoalNode;
}


// set the goal node, or set to NULL when the mGoal vector has to be used as goal
void TwoLinkIKSolver::SetGoalNode(Node* goal)
{
	mGoalNode = goal;
}


// get the goal
const MCore::Vector3& TwoLinkIKSolver::GetGoal() const
{
	return mGoal;
}

// get the bend direction
const MCore::Vector3& TwoLinkIKSolver::GetBendDirection() const
{
	return mBendDirection;
}


// set the start node
void TwoLinkIKSolver::SetStartNode(Node* node)
{
	mStartNode = node;
}


// set the mid node
void TwoLinkIKSolver::SetMidNode(Node* node)
{
	mMidNode = node;
}


// set the end effector
void TwoLinkIKSolver::SetEndEffector(Node* node)
{
	mEndNode = node;
}


// clone the solver
Controller* TwoLinkIKSolver::Clone(Actor* actor)
{
	Node* newStartNode	= actor->GetNodeByID( mStartNode->GetID() );
	Node* newMidNode	= actor->GetNodeByID( mMidNode->GetID() );
	Node* newEndNode	= actor->GetNodeByID( mEndNode->GetID() );

	assert( newStartNode != NULL );
	assert( newMidNode != NULL );
	assert( newEndNode != NULL );

	// create the clone
	TwoLinkIKSolver* result = new TwoLinkIKSolver( actor, newStartNode, newMidNode, newEndNode );
	result->SetGoalNode( mGoalNode );
	result->SetGoal( mGoal );

	return result;
}


// get the controller type
int TwoLinkIKSolver::GetType() const
{
	return TwoLinkIKSolver::TYPE_ID;
}


// get the type string
const char* TwoLinkIKSolver::GetTypeString() const
{
	return "TwoLinkIKSolver";
}


// check if we have found a solution or not
bool TwoLinkIKSolver::HasFoundSolution() const
{
	return mHasSolution;
}


// solve the IK problem by calculating the 'knee/elbow' position
bool TwoLinkIKSolver::Solve2LinkIK(const Vector3& posA, const Vector3& posB, const Vector3& posC, const Vector3& goal, const Vector3& bendDir, Vector3* outMidPos)
{
	const Vector3 localGoal = goal - posA;

	// get the lengths of the bones A and B
	const float lengthA = (posB - posA).Length();
	const float lengthB = (posC - posB).Length();

	// calculate the matrix that rotates from IK solve space into world space
	Matrix matForward;
	TwoLinkIKSolver::CalculateMatrix(localGoal, bendDir, &matForward);
	Matrix matInverse = matForward.Transposed();

	// calculate the goal position in IK solve space
	const Vector3 R = matInverse.Mul3x3( localGoal );

	// calculate the d and e values from the equations by Ken Perlin
	const float rLen = R.Length();
	const float l = (rLen + (lengthA*lengthA - lengthB*lengthB) / rLen) * 0.5f;
	const float d = MCore::Max<float>(0.0f, MCore::Min<float>(lengthA, l));
	const float e = MCore::Math::Sqrt(lengthA*lengthA - d*d);

	// the solution on the YZ plane
	Vector3 solution(d, e, 0);

	// rotate the solution (the mid "knee/elbow" position) into world space
	*outMidPos = posA + matForward.Mul3x3( solution );

	// check if we found a solution or not
	return (d > 0 && d < lengthA);
}


// calculate the direction matrix
void TwoLinkIKSolver::CalculateMatrix(const Vector3& goal, const Vector3& bendDir, Matrix* outForward)
{
	// the inverse matrix defines a coordinate system whose x axis contains P, so X = unit(P).
	const Vector3 x = goal.Normalized();

	// the y axis of the inverse is perpendicular to P, so Y = unit( D - X(D·X) ).
	const float dot = bendDir.Dot( x );
	Vector3 y = bendDir - (dot * x);
	y.Normalize();

	// the z axis of the inverse is perpendicular to both X and Y, so Z = X×Y.
	const Vector3 z = x.Cross( y );

	// set the rotation vectors of the output matrix
	outForward->SetRight( x );
	outForward->SetUp( y );
	outForward->SetForward( z );
}


} // namespace EMotionFX