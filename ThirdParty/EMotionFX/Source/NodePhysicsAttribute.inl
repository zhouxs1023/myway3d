/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "NodeLimitAttribute.h"


// set object mass
inline void NodePhysicsAttribute::SetMass(const float mass)
{
	mMass = mass;
}


// set the physics object
inline void NodePhysicsAttribute::SetPhysicsObject(PhysicsObject* object)
{
	if (mPhysicsObject)
		delete mPhysicsObject;

	mPhysicsObject = object;
}


// return the mass of the object
inline float NodePhysicsAttribute::GetMass() const
{
	return mMass;
}


// return physics object
inline NodePhysicsAttribute::PhysicsObject* NodePhysicsAttribute::GetPhysicsObject() const
{
	return mPhysicsObject;
}

//=================================================================================================

// set the width of the collision box
inline void NodePhysicsAttribute::Box::SetWidth(const float width)
{
	mBoxWidth = width;
}


// set the height of the collision box
inline void NodePhysicsAttribute::Box::SetHeight(const float height)
{
	mBoxHeight = height;
}


// set the depth of the collision box
inline void NodePhysicsAttribute::Box::SetDepth(const float depth)
{
	mBoxDepth = depth;
}


// return the width of the box
inline float NodePhysicsAttribute::Box::GetWidth() const
{
	return mBoxWidth;
}


// return the height of the box
inline float NodePhysicsAttribute::Box::GetHeight() const
{
	return mBoxHeight;
}


// return the depth of the box
inline float NodePhysicsAttribute::Box::GetDepth() const
{
	return mBoxDepth;
}

//=================================================================================================

// set the radius of the collision sphere
inline void NodePhysicsAttribute::Sphere::SetRadius(const float radius)
{
	mSphereRadius = radius;
}


// get the radius of the sphere
inline float NodePhysicsAttribute::Sphere::GetRadius() const
{
	return mSphereRadius;
}

//=================================================================================================


// set the radius of the collision cylinder
inline void NodePhysicsAttribute::Cylinder::SetRadius(const float radius)
{
	mCylinderRadius = radius;
}


// set the height of the collision cylinder
inline void NodePhysicsAttribute::Cylinder::SetHeight(const float height)
{
	mCylinderHeight = height;
}


// get the radius of the cylinder
inline float NodePhysicsAttribute::Cylinder::GetRadius() const
{
	return mCylinderRadius;
}


// get the height of the cylinder
inline float NodePhysicsAttribute::Cylinder::GetHeight() const
{
	return mCylinderHeight;
}