/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKFixedPoint.h"
#include "JacobianIKData.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
JacobianIKFixedPoint::JacobianIKFixedPoint()
{
}


// constructor
JacobianIKFixedPoint::JacobianIKFixedPoint(const float x, const float y, const float z)
{
	SetPosition(Vector3(x, y, z));
}


// constructor
JacobianIKFixedPoint::JacobianIKFixedPoint(const Vector3& position)
{
	SetPosition(position);
}


// destructor
JacobianIKFixedPoint::~JacobianIKFixedPoint()
{
}


// clone the ik constraint
JacobianIKConstraint* JacobianIKFixedPoint::Clone()
{
	return new JacobianIKFixedPoint(mPosition);
}


// update the ik constraint
void JacobianIKFixedPoint::Update(const float timeDeltaInSeconds) 
{
}


// return in 'values' the requested position for the constrained node
void JacobianIKFixedPoint::GetValues(Array<float>& values)
{
	values.Add(mPosition.x);
	values.Add(mPosition.y); // ahm yes ... (Benny)
	values.Add(mPosition.z);
}


//
void JacobianIKFixedPoint::GetResults(Array<float>& results)
{	
	// retrieve the node's world position
	Vector3 position = mNode->GetWorldPos();

	// the constraint result is the node position
	results.Add(position.x);
	results.Add(position.y); // ahm yes ... (Benny)
	results.Add(position.z);
}


//
void JacobianIKFixedPoint::CalcGradient(NMatrix& matrix)
{
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
int JacobianIKFixedPoint::GetNumFunc() const
{
	// one function for each coordinate
	return 3;
}

}