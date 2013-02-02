/*
* EMotion FX 2 - Character Animation System
* Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
* All Rights Reserved.
*/

#include "SwingAndTwist.h"
#include "RootSolver.h"


namespace MCore
{

// default constructor
SwingAndTwist::SwingAndTwist()
{
	mSwing.Set(0.0f, 0.0f);
	mTwist = 0.0f;
}


// extended constructor
SwingAndTwist::SwingAndTwist(const Quaternion& quat)
{
	FromQuaternion(quat);
}


// extended constructor
SwingAndTwist::SwingAndTwist(const Matrix& mat)
{
	FromMatrix(mat);
}


// destructor
SwingAndTwist::~SwingAndTwist()
{
}


// init the swing and twist from a quaternion
void SwingAndTwist::FromQuaternion(const Quaternion& quat)
{
	// calculate the twist and swing from a quaternion
	float halfTwist = Math::ATan2(quat.z, quat.w);
	
	// calculate the twist
	mTwist = 2.0f * halfTwist;

	float beta		= Math::ATan2( Math::Sqrt(quat.x * quat.x + quat.y * quat.y), Math::Sqrt(quat.z * quat.z + quat.w * quat.w) );
	float cosHTwist = Math::Cos(halfTwist);
	float sinHTwist = Math::Sin(halfTwist);

	// calculate the swing
	mSwing.Set(	cosHTwist * quat.x - sinHTwist * quat.y, 
				sinHTwist * quat.x + cosHTwist * quat.y	);

	// swing = swing * 2 / sinc(beta)
	// sinc(x) = Sin(x)/x or 1 if x = 0;
	if (beta == 0.0f) 
		mSwing *= 2.0f;
	else
		mSwing *= (2.0f * beta / Math::Sin(beta));
}


// create from a matrix
void SwingAndTwist::FromMatrix(const MCore::Matrix& mat)
{
	FromQuaternion( Quaternion::FromMatrix(mat) );
}


// convert back into a quaternion
MCore::Quaternion SwingAndTwist::ToQuaternion()
{
	Vector3 forward(	Math::Sin(  mSwing.y ), 
						Math::Sin( -mSwing.x ), 
						Math::Cos( Math::Sqrt( mSwing.x * mSwing.x + mSwing.y * mSwing.y ) ) );

	forward.Normalize();
	Vector3 axis  = Vector3(0.0f, 0.0f, 1.0f).Cross(forward);
	float	angle = Math::ACos(Vector3(0.0f, 0.0f, 1.0f).Dot(forward));
	Quaternion qSwing(axis, angle);

	return qSwing * Quaternion(Vector3(0.0f, 0.0f, 1.0f), mTwist);
}


// convert back into a matrix
MCore::Matrix SwingAndTwist::ToMatrix()
{
	Vector3 forward(	Math::Sin(  mSwing.y ), 
						Math::Sin( -mSwing.x ), 
						Math::Cos( Math::Sqrt( mSwing.x * mSwing.x + mSwing.y * mSwing.y ) ) );

	forward.Normalize();
	Vector3 axis = Vector3(0.0f, 0.0f, 1.0f).Cross(forward);
	float angle = Math::ACos(Vector3(0.0f, 0.0f, 1.0f).Dot(forward));

	Matrix matSwing;
	matSwing.SetRotationMatrixAxisAngle(axis, angle);

	Matrix matTwist;
	matTwist.SetRotationMatrixZ(mTwist);

	//return matSwing * matTwist;
	return matTwist * matSwing;
}


/*
// convert back into a quaterion
Quaternion SwingAndTwist::ToQuat()
{
	Quaternion qX(Vector3(1.0f, 0.0f, 0.0f), mSwing.x);
	Quaternion qY(Vector3(0.0f, 1.0f, 0.0f), mSwing.y);
	Quaternion qT = qX * qY;
	float inducedTwist = 2.0f * Math::ATan2(qT.z, qT.w); // this calculates the twist induced by the swing
	return qT * Quaternion(Vector3(0.0f, 0.0f, 1.0f), mTwist - inducedTwist);
}
*/

// constrain the swing using an ellipse
bool SwingAndTwist::ConstrainSwingWithEllipse(const MCore::Vector2& ellipseRadii)
{
	// safety check, to prevent 'explosions' :)
	if ((ellipseRadii.x > -0.05 && ellipseRadii.x < 0.05) || (ellipseRadii.y > -0.05 && ellipseRadii.y < 0.05)) 
		return ConstrainSwingWithRectangle(ellipseRadii); 

	// the ellipse radii
	MCore::Vector2 radii(ellipseRadii);

	// check if the swing is outside of the spherical ellipse
	float tx = mSwing.x / radii.x;
	float ty = mSwing.y / radii.y;

	if (tx * tx + ty * ty - 1 > 0)
	{
		// if so, then find the closest point on the ellipse to the swing
		Vector2 out;
		bool transpose = false;

		// depending on what were working with, rotate our space to avoide steep slopes in the below equation.
		// because when y is large and x is close to 0, the slope of the graph gets very steep near the root causing neumerical issues.
		if (Math::Abs(mSwing.x) > Math::Abs(mSwing.y))
		{
			float temp;
			transpose	= true;
			temp		= radii.x;
			radii.x		= radii.y;
			radii.y		= temp;

			temp		= mSwing.x;
			mSwing.x	= mSwing.y;
			mSwing.y	= temp;
		}

		// now solve for t in the following equation. t will corrospond to the candidate 
		// points on the ellipse which are closest to the swing:
		// mSwing.y * t^4 + (f - e) * t^3 + (f + e) * t - mSwing.y = 0
		float rxByry = radii.x / radii.y;
		float e = 2 * (radii.y - radii.x * rxByry);
		float f = 2 * mSwing.x * rxByry;

		float c[5];	// coefficients
		float s[4]; // solutions

		// the coefficients in the equation were trying to solve
		c[4] = mSwing.y;
		c[3] = (f - e);
		c[2] = 0;
		c[1] = (f + e);
		c[0] = -mSwing.y;

		// solve it
		// now s has the root that were found and rootsFound has the number of roots found
		int rootsFound = RootSolver::SolveQuartic(c, s);

		// go through each root found and find the one that corresponds to the closest point on the ellipse 
		// to our swing
		Vector2 outTemp;
		float root = s[0];
		float rootSqr = root * root;
		float inv1pRootSqr = 1 / (1 + rootSqr);
		out.x = radii.x * (1 - rootSqr) * inv1pRootSqr;
		out.y = radii.y * 2 * root * inv1pRootSqr;

		for (int i=1; i<rootsFound; i++)
		{
			root		 = s[i];
			rootSqr		 = root * root;
			inv1pRootSqr = 1.0f / (1.0f + rootSqr);
			outTemp.x	 = radii.x * (1.0f - rootSqr) * inv1pRootSqr;
			outTemp.y	 = radii.y * 2.0f * root * inv1pRootSqr;

			if ((outTemp - mSwing).SquareLength() < (out - mSwing).SquareLength())
				out = outTemp;
		}

		// rotate back
		if (transpose)
		{
			float temp;
			temp = radii.x;
			radii.x = radii.y;
			radii.y = temp;

			temp = out.x;
			out.x = out.y;
			out.y = temp;
		}

		mSwing = out;
		return true;
	}

	return false;
}


// constrain using a rectangle
bool SwingAndTwist::ConstrainSwingWithRectangle(const MCore::Vector2& rectangleBounds)
{
	bool xCliped = true, yCliped = true;

	// Simple clipping
	if (mSwing.x > rectangleBounds.x)
		mSwing.x = rectangleBounds.x;
	else 
	{
		if (mSwing.x < -rectangleBounds.x)	
			mSwing.x = -rectangleBounds.x;
		else 
			xCliped = false;
	}

	if (mSwing.y > rectangleBounds.y)
		mSwing.y = rectangleBounds.y;
	else 
	{
		if (mSwing.y < -rectangleBounds.y)
			mSwing.y = -rectangleBounds.y;
		else 
			yCliped = false;
	}

	return (xCliped || yCliped); // return false if no clipping occurred
}


// constraint the twist
bool SwingAndTwist::ConstrainTwist(const MCore::Vector2& minmaxTwist)
{
	if (mTwist < minmaxTwist.x)
		mTwist = minmaxTwist.x;
	else
	{
		if (mTwist > minmaxTwist.y)
			mTwist = minmaxTwist.y;
		else 
			return false;
	}

	return true;
}

} // namespace EMotionFX
