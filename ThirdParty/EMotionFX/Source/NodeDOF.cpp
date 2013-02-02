/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "NodeDOF.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

NodeDOF::NodeDOF(const CanonicalAxis rotOrderAxis1, const CanonicalAxis rotOrderAxis2, const CanonicalAxis rotOrderAxis3)
{
	mEuler			= false;
	mAngles[0]		= 0.0;
	mAngles[1]		= 0.0;
	mAngles[2]		= 0.0;
	mMaxValue[0]	= 0.0;
	mMaxValue[1]	= 0.0;
	mMaxValue[2]	= 0.0;
	mMinValue[0]	= 0.0;
	mMinValue[1]	= 0.0;
	mMinValue[2]	= 0.0;

	SetRotationOrder(rotOrderAxis1, rotOrderAxis2, rotOrderAxis3);

	mAxis[0].Set(0.0, 0.0, 0.0);
	mAxis[1].Set(0.0, 0.0, 0.0);
	mAxis[2].Set(0.0, 0.0, 0.0);
}


NodeDOF::NodeDOF()
{
	mEuler			= true;
	mNumDOF			= 3;
	mAngles[0]		= 0.0;
	mAngles[1]		= 0.0;
	mAngles[2]		= 0.0;
	mMaxValue[0]	= 0.0;
	mMaxValue[1]	= 0.0;
	mMaxValue[2]	= 0.0;
	mMinValue[0]	= 0.0;
	mMinValue[1]	= 0.0;
	mMinValue[2]	= 0.0;

	mAxis[0].Set(0.0, 0.0, 0.0);
	mAxis[1].Set(0.0, 0.0, 0.0);
	mAxis[2].Set(0.0, 0.0, 0.0);
}


NodeDOF::~NodeDOF()
{
}


void NodeDOF::SetRotationOrder(const CanonicalAxis rotOrderAxis1, const CanonicalAxis rotOrderAxis2, const CanonicalAxis rotOrderAxis3)
{
	mNumDOF = (rotOrderAxis1 != AxisNONE) + (rotOrderAxis2 != AxisNONE) + (rotOrderAxis3 != AxisNONE);

	mRotationOrder[0] = rotOrderAxis1;
	mRotationOrder[1] = rotOrderAxis2;
	mRotationOrder[2] = rotOrderAxis3;
}


void NodeDOF::SetLimits(const int i, const float max, const float min)
{
	assert( i < 3 && i >= 0 );

	mMaxValue[i] = max;
	mMinValue[i] = min;
}


// return the type identification number of the node DOF
int NodeDOF::GetType() const
{
	return TYPE_ID;
}


// return the type identification string of the node DOF
const char* NodeDOF::GetTypeString() const
{
	return "NodeDOF";
}


// clone the node DOF
NodeAttribute* NodeDOF::Clone()
{
	// the node DOF
	NodeDOF* clone;

	// check whether the node DOF is handled by euler angles or not and create the specific object
	if (mEuler)
		clone = new NodeDOF();
	else
		clone = new NodeDOF(mRotationOrder[0], mRotationOrder[1], mRotationOrder[2]);

	// return the newly created object
	return clone;
}


void NodeDOF::CalcValues(MCore::Matrix& localM, const MCore::Vector3& localS)
{
	// retrive the right, up and forward vectors from the local matrix
	Vector3 right	= localM.GetRight();
	Vector3 up		= localM.GetUp();
	Vector3 forward	= localM.GetForward();

	right.x /= localS.x;
	right.y /= localS.y;
	right.z /= localS.z;
	up.x /= localS.x;
	up.y /= localS.y;
	up.z /= localS.z;
	forward.x /= localS.x;
	forward.y /= localS.y;
	forward.z /= localS.z;

	MCore::Matrix localTM;

	localTM.SetRight(right);
	localTM.SetUp(up);
	localTM.SetForward(forward);

	if (mEuler)
	{
		Vector3 v = localTM.CalcEulerAngles();
		mAngles[0] = v.x;
		mAngles[1] = v.y;
		mAngles[2] = v.z;

		return;
	}

	if (mNumDOF == 3)
	{
		if (Rotation(AxisX, AxisY, AxisZ))
		{
			mAngles[1] = Math::ASin(forward.x);
			if (forward.x < 1)
			{
				if (forward.x > -1)
				{
					mAngles[0] = Math::ATan2(-forward.y, forward.z);
					mAngles[2] = Math::ATan2(-up.x, up.y);
				}
				else
				{
					mAngles[0] = -Math::ATan2(right.y, up.y);
					mAngles[2] = 0.0;
				}
			}
			else
			{
				mAngles[0] = Math::ATan2(-right.y, up.y);
				mAngles[2] = 0.0;
			}
		}

		if (Rotation(AxisX, AxisZ, AxisY))
		{
			mAngles[1] = Math::ASin(-up.x);
			if (-forward.x < 1)
			{
				if (-forward.x > -1)
				{
					mAngles[0] = Math::ATan2(up.z, up.y);
					mAngles[2] = Math::ATan2(forward.x, right.x);
				}
				else
				{
					mAngles[0] = -Math::ATan2(-right.z, forward.z);
					mAngles[2] = 0.0;
				}
			}
			else
			{
				mAngles[0] = Math::ATan2(-right.z, forward.z);
				mAngles[2] = 0.0;
			}
		}

		if (Rotation(AxisY, AxisX, AxisZ))
		{
			mAngles[1] = Math::ASin(-forward.y);
			if (-forward.y < 1)
			{
				if (-forward.y > -1)
				{
					mAngles[0] = Math::ATan2(forward.x, forward.z);
					mAngles[2] = Math::ATan2(right.y, up.y);
				}
				else
				{
					mAngles[0] = -Math::ATan2(-up.x, up.y);
					mAngles[2] = 0.0;
				}
			}
			else
			{
				mAngles[0] = -Math::ATan2(-up.x, up.y);
				mAngles[2] = 0.0;
			}
		}	
	
		if (Rotation(AxisY, AxisZ, AxisX))
		{
			mAngles[1] = Math::ASin(right.y);
			if (right.y < 1)
			{
				if (right.y > -1)
				{
					mAngles[0] = Math::ATan2(-right.z, right.x);
					mAngles[2] = Math::ATan2(-forward.y, up.y);
				}
				else
				{
					mAngles[0] = -Math::ATan2(up.z, forward.z);
					mAngles[2] = 0.0;
				}
			}
			else
			{
				mAngles[0] = Math::ATan2(up.z, forward.z);
				mAngles[2] = 0.0;
			}
		}
	
		if (Rotation(AxisZ, AxisX, AxisY))
		{
			mAngles[1] = Math::ASin(up.z);
			if (up.z < 1)
			{
				if (up.z > -1)
				{
					mAngles[0] = Math::ATan2(-up.x, up.y);
					mAngles[2] = Math::ATan2(-right.z, forward.z);
				}
				else
				{
					mAngles[0] = -Math::ATan2(forward.x, right.x);
					mAngles[2] = 0.0;
				}
			}
			else
			{
				mAngles[0] = Math::ATan2(forward.x, right.x);
				mAngles[2] = 0.0;
			}
		}

		if (Rotation(AxisZ, AxisX, AxisY))
		{
			mAngles[1] = Math::ASin(-right.z);
			if (-right.z < 1)
			{
				if (-right.z > -1)
				{
					mAngles[0] = Math::ATan2(right.y, right.x);
					mAngles[2] = Math::ATan2(up.z, forward.z);
				}
				else
				{
					mAngles[0] = -Math::ATan2(-up.x, forward.x);
					mAngles[2] = 0.0;
				}
			}
			else
			{
				mAngles[0] = Math::ATan2(-up.x, forward.x);
				mAngles[2] = 0.0;
			}
		}
	}
}


void NodeDOF::CalcAxis(MCore::Matrix& parentM)
{
	MCore::Matrix EM = parentM;
	MCore::Matrix RM;

	if (mEuler)
	{
		SetRotationOrder(AxisX, AxisY, AxisZ);
		mEuler = true;
	}

	for(int d=2; d>=0; d--)
	{
		switch(mRotationOrder[d])
		{
			case AxisX:
				mAxis[d] = EM.GetRight();
				RM = MCore::Matrix::RotationMatrixX(mAngles[d]);
				EM = RM * EM;
			break;
			case AxisY:
				mAxis[d] = EM.GetUp();
				RM = MCore::Matrix::RotationMatrixY(mAngles[d]);
				EM = RM * EM;
			break;
			case AxisZ:
				mAxis[d] = EM.GetForward();
				RM = MCore::Matrix::RotationMatrixZ(mAngles[d]);
				EM = RM * EM;
			break;
		}
	}
}


MCore::Matrix NodeDOF::GetMatrix(const float v1, const float v2, const float v3)
{
	MCore::Matrix matrix;

	if (mEuler)
		matrix.SetRotationMatrixEuler(MCore::Vector3(v1, v2, v3));
	else
		matrix = RotationMatrix(0, v1) * RotationMatrix(1, v2) * RotationMatrix(2, v3);

	return matrix;
}


MCore::Matrix NodeDOF::RotationMatrix(const int i, const float v) const
{
	MCore::Matrix matrix;
	matrix.Identity();

	switch(mRotationOrder[i])
	{
	case AxisX:
		matrix = MCore::Matrix::RotationMatrixX(v);
		break;
	case AxisY:
		matrix = MCore::Matrix::RotationMatrixY(v);
		break;
	case AxisZ:
		matrix = MCore::Matrix::RotationMatrixZ(v);
		break;
	}

	return matrix;
}

}