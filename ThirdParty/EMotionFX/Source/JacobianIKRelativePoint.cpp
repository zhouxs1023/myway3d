/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKRelativePoint.h"
#include "JacobianIKData.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
JacobianIKRelativePoint::JacobianIKRelativePoint()
{
}


// constructor
JacobianIKRelativePoint::JacobianIKRelativePoint(const float x, const float y, const float z)
{
	SetDisplacement(Vector3(x, y, z));
}


// constructor
JacobianIKRelativePoint::JacobianIKRelativePoint(const Vector3& displacement)
{
	SetDisplacement(displacement);
}


// destructor
JacobianIKRelativePoint::~JacobianIKRelativePoint()
{
}


// update the ik constraint
void JacobianIKRelativePoint::Update(const float timeDeltaInSeconds)
{
	mPosition = mNode->GetWorldPos();
}


// clone the ik constraint
JacobianIKConstraint* JacobianIKRelativePoint::Clone()
{
	return new JacobianIKRelativePoint(mDisplacement);
}


//
void JacobianIKRelativePoint::GetValues(Array<float>& values)
{
	// the required value for the constraint is the node position plus the defined displacement
	values.Add(mPosition.x + mDisplacement.x);
	values.Add(mPosition.y + mDisplacement.y); // ahm yes ... (Benny)
	values.Add(mPosition.z + mDisplacement.z);
}


//
void JacobianIKRelativePoint::GetResults(Array<float>& results)
{	
	// retrieve the node's world position
	Vector3 position = mNode->GetWorldPos();

	// the constraint result is the node position
	results.Add(position.x);
	results.Add(position.y); // ahm yes ... (Benny)
	results.Add(position.z);
}


//
void JacobianIKRelativePoint::CalcGradient(NMatrix& matrix)
{
	// retrieve the number of DOFs from the ik data
	const int numDOFs = mIKData->GetNumDOF();

	matrix.SetSize(3, numDOFs);

	// the derivative of the result is the derivative of the position
	// the gradient is the derivative of each coordinate (in each column) respect to each DOF (in each row)
	for (int i=0; i<numDOFs; i++)
	{
		Vector3 v = mIKData->CalcPosDer(mNode, i);
		matrix(0, i) = v.x;
		matrix(1, i) = v.y;
		matrix(2, i) = v.z;
	}
}


//
int JacobianIKRelativePoint::GetNumFunc() const
{
	// one function for each coordinate
	return 3;
}

}