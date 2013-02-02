/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Node.h"
#include "NodeLimitAttribute.h"
#include "LookAtConstraint.h"

// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
LookAtConstraint::LookAtConstraint(Actor* actor, Node* node, Node* target) : Constraint(actor, node, target)
{
	assert(mNode != NULL);
	
	mGoal.Set(0.0f, 0.0f, 0.0f);
	mInterpolationSpeed	= 1.0f;

	// we need to keep a copy of the rotation
	mRotationQuat = Quaternion::FromMatrix( node->GetWorldTM().Normalized() ).Normalized();

	mPostRotation.Identity();
	mPreRotation.Identity();

	// set the limits accoring to the model's current limits if it has them set, otherwise just use default values
	NodeLimitAttribute* limit = (NodeLimitAttribute*)mNode->GetAttributeByType(NodeLimitAttribute::TYPE_ID);

	// set the limits
	if (limit != NULL)
		SetEulerConstraints(limit->GetRotationMin(), limit->GetRotationMax());
	else
	{
		mEllipseOrientation.Identity();
		mEllipseRadii.Set(1.0f, 1.0f);
		mMinMaxTwist.Set(0.0f, 0.0f);
	}

	// disable the constraints
	mConstraintType = CONSTRAINT_NONE;
}


// destructor
LookAtConstraint::~LookAtConstraint()
{
}


// update the constraint
void LookAtConstraint::Update(const float timeDeltaInSeconds)
{
	// calculate the new forward vector
	Vector3 forward;
	if (mTarget)
		forward = (mTarget->GetWorldPos() - mNode->GetWorldPos()).Normalized();
	else
		forward = (mGoal - mNode->GetWorldPos()).Normalized();

	Vector3 up;
	Node* parent = mNode->GetParent();
	Matrix parentWorldTM(true); 
	if (parent != mActor) 
	{ 
		parentWorldTM = parent->GetWorldTM().Normalized(); 

		// get the up vector from the parent if it exsists 
		up = (mPreRotation * parentWorldTM).GetUp().Normalized(); 
	} 
	else // otherwise use the up vector of the node itself 
		up = (mPreRotation * mNode->GetWorldTM().Normalized()).GetUp().Normalized();

	// caclculate the right and up vector we wish to use
	Vector3 right = up.Cross( forward ).Normalized();
	up = forward.Cross( right ).Normalized();

	// build the destination rotation matrix
	Matrix destRotation(true);
	destRotation.SetRight(right);
	destRotation.SetUp(up);
	destRotation.SetForward(forward);

	// at this point, destRotation contains the world space orientation that we would like our node do be using

	// if we want to apply constraints
	if (mConstraintType != CONSTRAINT_NONE)
	{	
		// find the destination relative to the node's parent if the node has one 
		if (parent != mActor) 
			destRotation = destRotation * parentWorldTM.Inversed();

		// now destRotation contains the exact orientation we wish to apply constraints to (relative to the identity matrix)

		// multiply by the inverse of the ellipse's orientation, and rotate our coordinate system into 'ellipse space'
		destRotation = mEllipseOrientation.Inversed() * destRotation;

		// calculate the swing and twist
		SwingAndTwist sAndt( destRotation );

		// apply swing constraints
		if (mConstraintType == CONSTRAINT_ELLIPSE)		// constraint using an ellipse
		{
			sAndt.ConstrainSwingWithEllipse(mEllipseRadii);
		}
		else 
		if (mConstraintType == CONSTRAINT_RECTANGLE)	// constraint using a rectangle
		{
			// the rectangle method will be a lot faster than the ellipse, but doesn't look anywhere near as good
			// this should probably only be used for distant models using a lower LOD

			// treat the ellipse's radii as bounds for a rectangle
			sAndt.ConstrainSwingWithRectangle(mEllipseRadii);
		}

		// apply twist constraints
		sAndt.ConstrainTwist(mMinMaxTwist);

		// convert the swing and twist back to a matrix
		destRotation = sAndt.ToMatrix();

		// rotate back out of ellipse space
		destRotation = mEllipseOrientation * destRotation;

		// if we converted our coord system into the space relative to the parent, then convert it back to world space
		if (parent != mActor) 
			destRotation = destRotation * parentWorldTM;
	}

	// apply the post rotation patrix
	destRotation = mPostRotation * destRotation;

	// interpolate between the current rotation and the destination rotation
	float speed = mInterpolationSpeed * timeDeltaInSeconds * 6.0f;
	speed = MCore::Clamp<float>(speed, 0.0f, 1.0f);
	mRotationQuat = mRotationQuat.Slerp(Quaternion::FromMatrix(destRotation), speed);

	// create the rotation matrix from the rotation quat
	destRotation = mRotationQuat.ToMatrix();

	// scale the matrix and put it back in the right position
	Vector3 scale = mNode->GetWorldScale();
	destRotation.Scale(scale.x, scale.y, scale.z);
	destRotation.SetTranslation( mNode->GetWorldPos() );

	// update the matrix with forward kinematics
	mNode->RecursiveUpdateWorldTM( &destRotation );
}


// Calculate the Ellipse radii and the twist value from the min and max euler angles
void LookAtConstraint::SetEulerConstraints(const Vector3& minVals, const Vector3& maxVals)
{
	// twist is z
	mMinMaxTwist.x = minVals.z;
	mMinMaxTwist.y = maxVals.z;

	// Offset is the average
	float xOffset = (minVals.x + maxVals.x) * 0.5;
	float yOffset = (minVals.y + maxVals.y) * 0.5;

	// these lines are the same as above
	mEllipseRadii.x = maxVals.x - xOffset;
	mEllipseRadii.y = maxVals.y - yOffset;

	// build the apropriate matrices
	Matrix xOffsetMat, yOffsetMat, zRot;
	xOffsetMat.SetRotationMatrixX(xOffset);
	yOffsetMat.SetRotationMatrixY(yOffset);
	mEllipseOrientation = xOffsetMat * yOffsetMat;

	// TODO: check if this is required
	// basically just swaps the x and y axii
	zRot.SetRotationMatrixZ(-Math::HALF_PI);
	mEllipseOrientation = zRot * mEllipseOrientation;
}


// clone the constraint
Controller* LookAtConstraint::Clone(Actor* actor)
{
	Node* newNode = actor->GetNodeByID( mNode->GetID() );
	assert(newNode != NULL);

	// create the new constraint and copy its members
	LookAtConstraint* newController = new LookAtConstraint(actor, newNode, mTarget);
	newController->mPreRotation			= mPreRotation;
	newController->mPostRotation		= mPostRotation;
	newController->mEllipseOrientation	= mEllipseOrientation;
	newController->mRotationQuat		= mRotationQuat;
	newController->mGoal				= mGoal;
	newController->mEllipseRadii		= mEllipseRadii;
	newController->mMinMaxTwist			= mMinMaxTwist;
	newController->mInterpolationSpeed	= mInterpolationSpeed;
	newController->mConstraintType		= mConstraintType;

	return newController;
}


// get the type ID
int LookAtConstraint::GetType() const
{
	return LookAtConstraint::TYPE_ID;
}


// get the type string
const char* LookAtConstraint::GetTypeString() const
{
	return "LookAtConstraint";
}


// get the goal
const MCore::Vector3& LookAtConstraint::GetGoal() const
{
	return mGoal;
}


// set the goal position
void LookAtConstraint::SetGoal(const MCore::Vector3& goalWorldSpacePos)
{
	mGoal = goalWorldSpacePos;
}


// set the interpolation speed
void LookAtConstraint::SetInterpolationSpeed(const float factor)
{ 
	mInterpolationSpeed = factor; 
}


// get the interplation speed
float LookAtConstraint::GetInterpolationSpeed() const
{ 
	return mInterpolationSpeed; 
}


// set the pre-rotation matrix
void LookAtConstraint::SetPreRotation(const MCore::Matrix& mat)
{ 
	mPreRotation = mat; 
}


// get the pre-rotation matrix
const MCore::Matrix& LookAtConstraint::GetPreRotation() const
{ 
	return mPreRotation; 
}


// set the post rotation matrix
void LookAtConstraint::SetPostRotation(const MCore::Matrix& mat)
{ 
	mPostRotation = mat; 
}


// get the post rotation matrix
const MCore::Matrix& LookAtConstraint::GetPostRotation() const
{ 
	return mPostRotation; 
}


// set the ellipse orientation
void LookAtConstraint::SetEllipseOrientation(const MCore::Matrix& mat) 
{ 
	mEllipseOrientation = mat; 
}


// get the ellipse orientation
const MCore::Matrix& LookAtConstraint::GetEllipseOrientation() const 
{ 
	return mEllipseOrientation; 
}


// set the ellipse radii
void LookAtConstraint::SetEllipseRadii(const MCore::Vector2& radii)
{ 
	mEllipseRadii = radii; 
}


// set the ellipse radii using two floats
void LookAtConstraint::SetEllipseRadii(const float rx, const float ry)
{ 
	mEllipseRadii.Set(rx, ry); 
}


// set the twist rotation limits
void LookAtConstraint::SetTwist(const float minValue, const float maxValue)
{ 
	mMinMaxTwist.Set(minValue, maxValue); 
}


// set the twist rotation limits using a Vector2
void LookAtConstraint::SetTwist(const MCore::Vector2& minMaxTwist)
{ 
	mMinMaxTwist = minMaxTwist; 
}


// set the constraint type
void LookAtConstraint::SetConstraintType(EConstraintType constraintType)
{ 
	mConstraintType = constraintType; 
}


// enable or disable the controller
void LookAtConstraint::SetEnabled(const bool enable)
{ 
	mIsEnabled	  = enable;
	mRotationQuat = MCore::Quaternion::FromMatrix( mNode->GetWorldTM().Normalized() ).Normalized(); 
}


// get the constraint type
LookAtConstraint::EConstraintType LookAtConstraint::GetConstraintType() const
{
	return mConstraintType;
}



} // namespace EMotionFX
