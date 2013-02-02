/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Actor.h"


// set the motion system
inline void Actor::SetMotionSystem(MotionSystem* motionSystem)
{
	if (mMotionSystem != NULL)
		delete mMotionSystem;

	mMotionSystem = motionSystem;
}


// get the motion system
inline MotionSystem* Actor::GetMotionSystem() const
{
	return mMotionSystem;
}


// get the number of nodes
inline int Actor::GetNumNodes() const
{
	return mNodes.GetLength();
}


inline Node* Actor::GetNode(const int nr) const
{
	assert(nr<mNodes.GetLength());
	return mNodes[nr];
}


inline MCore::Array<Node*>& Actor::GetNodes()
{
	return mNodes;
}


inline const MCore::AABB& Actor::GetBoundingBox() const
{
	return mBoundingBox;
}


inline const MCore::BoundingSphere& Actor::GetBoundingSphere() const
{
	return mBoundingSphere;
}


inline void Actor::AddNode(Node* node)
{
	mNodes.Add(node);
}


inline void Actor::RemoveNode(const int nr, const bool delMem)
{
	assert(nr<mNodes.GetLength());
	
	if (delMem)
		delete mNodes[nr];
	
	mNodes.Remove(nr);
}


inline void Actor::RestoreInitialPose()
{
	const int numNodes = mNodes.GetLength();
	for (int i=0; i<numNodes; i++)
		mNodes[i]->SetToOriginalOrientation();
}



inline void Actor::UpdateNodeStacks(const double& timePassedInSeconds)
{
	const int numNodes = mNodes.GetLength();
	for (int i=0; i<numNodes; i++)
	{
		Node* node = mNodes[i];

		// update the meshes
		if (mCurrentLOD < node->GetNumMeshDeformerStacks())
		{
			if (node->GetMeshDeformerStack( mCurrentLOD ))
				node->GetMeshDeformerStack( mCurrentLOD )->Update(this, node, timePassedInSeconds);
		}

		// update the collision meshes
		if (mCurrentLOD < node->GetNumCollisionMeshDeformerStacks())
		{
			if (node->GetCollisionMeshDeformerStack( mCurrentLOD ))
				node->GetCollisionMeshDeformerStack( mCurrentLOD )->Update(this, node, timePassedInSeconds);
		}
	}
}


inline void Actor::DeleteAllNodes()
{
	// remove all nodes from this actor
	while (mNodes.GetLength())
	{
		delete mNodes.GetLast();
		mNodes.RemoveLast();
	}
}


// search for a node on a specific name, and return a pointer to it
inline Node* Actor::GetNodeByName(const char* name) const
{
	const int numNodes = mNodes.GetLength();
	for (int i=0; i<numNodes; i++)
		if (mNodes[i]->GetName() == name)	// TODO: optimize
			return mNodes[i];

	return NULL;
}


// search on name, non case sensitive
inline Node* Actor::GetNodeByNameNoCase(const char* name) const
{
	const int numNodes = mNodes.GetLength();
	for (int i=0; i<numNodes; i++)
		if (mNodes[i]->GetName().IsEqualNoCase( name ))	// TODO: optimize
			return mNodes[i];

	return NULL;
}



// search for a node on ID
inline Node* Actor::GetNodeByID(const int id) const
{
	const int numNodes = mNodes.GetLength();
	for (int i=0; i<numNodes; i++)
		if (mNodes[i]->GetID() == id)
			return mNodes[i];

	return NULL;
}


// search for a node by name, and returns it's number in the array (case sensitive), returns -1 when not found
inline int Actor::GetNodeNumberByName(const char* name) const
{
	const int numNodes = mNodes.GetLength();
	for (int i=0; i<numNodes; i++)
		if (mNodes[i]->GetName() == name)
			return i;

	return -1;
}


// get a material
inline MCore::Pointer<Material>& Actor::GetMaterial(const int lodLevel, const int nr)
{
	assert(lodLevel < mMaterials.GetLength());
	assert(nr < mMaterials[lodLevel].GetLength());
	return mMaterials[lodLevel][nr];
}


// get a material by name
inline Material* Actor::GetMaterialByName(const int lodLevel, const MCore::String& name) const
{
	assert(lodLevel < mMaterials.GetLength());

	// search through all materials
	for (int i=0; i<mMaterials[lodLevel].GetLength(); i++)
		if (mMaterials[lodLevel][i].GetPointer()->GetName().IsEqual( name ))
			return mMaterials[lodLevel][i];

	// no material found
	return NULL;
}


// set a material
inline void Actor::SetMaterial(const int lodLevel, const int nr, const MCore::Pointer<Material>& mat)
{
	assert(lodLevel < mMaterials.GetLength());
	assert(nr < mMaterials[lodLevel].GetLength());
	mMaterials[lodLevel][nr] = mat;
}


// add a material
inline void Actor::AddMaterial(const int lodLevel, const MCore::Pointer<Material>& mat)
{
	assert(lodLevel < mMaterials.GetLength());
	mMaterials[lodLevel].Add(mat);
}


// get the number of materials
inline int Actor::GetNumMaterials(const int lodLevel) const
{
	assert(lodLevel < mMaterials.GetLength());
	return mMaterials[lodLevel].GetLength();
}


inline bool Actor::IsInstance() const
{
	return (mOriginalActor != NULL);
}


// returns a pointer to the original actor(if its a clone), else NULL
inline Actor* Actor::GetOriginalActor()
{
	return mOriginalActor;
}


inline int Actor::GetNumLODLevels() const
{
	return mNumLODs;
}


inline int Actor::GetCurrentLOD() const
{
	return mCurrentLOD;
}


inline void Actor::SetLOD(const int lodNumber)
{
	mCurrentLOD = MCore::Clamp(lodNumber, 0, mNumLODs-1);
}


inline int Actor::GetNumAttachments() const
{
	return mAttachments.GetLength();
}


inline Actor* Actor::GetAttachment(const int nr) const
{
	assert(nr < mAttachments.GetLength());
	return mAttachments[nr];
}


// return collision system
inline ActorCollisionSystem* Actor::GetActorCollisionSystem() const
{
	return mActorCollisionSystem;
}


// add the given controller
inline void Actor::AddController(Controller* controller)
{
	mControllers.Add(controller);
}


// insert the given controller
inline void Actor::InsertController(const int pos, Controller* controller)
{
	mControllers.Insert(pos, controller);
}


// remove the given controller
inline void Actor::RemoveController(const int nr, const bool delFromMem)
{
	assert(nr < mControllers.GetLength());

	// remove the controller from the stack
	mControllers.Remove(nr);

	// delete the controller from memory
	if (delFromMem)
		delete mControllers[nr];
}


// remove the given controller
inline void Actor::RemoveController(Controller *controller, const bool delFromMem)
{
	// remove the controller from the stack
	mControllers.Remove(controller);

	// delete the controller from memory
	if (delFromMem)
		delete controller;
}


// return the given controller
inline Controller* Actor::GetController(const int nr) const
{
	assert(nr < mControllers.GetLength());
	return mControllers[nr];
}


// return the number of controllers
inline int Actor::GetNumControllers() const
{
	return mControllers.GetLength();
}


// update all controllers
inline void Actor::UpdateControllers(const double timePassedInSeconds)
{
	// retrieve the number of controllers
	const int numControllers = mControllers.GetLength();

	// iterate through all controllers and update the endabled ones
	for (int i=0; i<numControllers; i++)
	{
		// retrieve the current controller
		Controller* controller = mControllers[i];

		// update the given controller if it is enabled
		if (controller->IsEnabled())
			controller->Update( timePassedInSeconds );
	}
}


// add the given post controller
inline void Actor::AddPostController(Controller* controller)
{
	mPostControllers.Add(controller);
}


// insert the post controller to the given position
inline void Actor::InsertPostController(const int pos, Controller* controller)
{
	mPostControllers.Insert(pos, controller);
}


// remove the given post controller
inline void Actor::RemovePostController(const int nr, const bool delFromMem)
{
	assert(nr < mPostControllers.GetLength());

	// remove the post controller from the stack
	mPostControllers.Remove(nr);

	// delete the post controller from memory
	if (delFromMem)
		delete mPostControllers[nr];
}


// remove the given post controller
inline void Actor::RemovePostController(Controller *controller, const bool delFromMem)
{
	// remove the post controller from the stack
	mPostControllers.Remove(controller);

	// delete the post controller from memory
	if (delFromMem)
		delete controller;
}


// return pointer to post controller
inline Controller* Actor::GetPostController(const int nr) const
{
	assert(nr < mPostControllers.GetLength());
	return mPostControllers[nr];
}


// return number of post controllers
inline int Actor::GetNumPostControllers() const
{
	return mPostControllers.GetLength();
}


// update all post controllers
inline void Actor::UpdatePostControllers(const double timePassedInSeconds)
{
	// retrieve the number of post controllers
	const int numControllers = mPostControllers.GetLength();

	// iterate through all controllers and update the endabled ones
	for (int i=0; i<numControllers; i++)
	{
		// retrieve the current controller
		Controller* controller = mPostControllers[i];

		// update the given controller if it is enabled
		if (controller->IsEnabled())
			controller->Update( timePassedInSeconds );
	}
}


// get the repositioning node
inline Node* Actor::GetRepositioningNode()
{
	return mRepositioningNode;
}


// set the repositioning node
inline void Actor::SetRepositioningNode(Node* repositioningNode)
{
	mRepositioningNode = repositioningNode;
}


// get the repositioning mask
inline Actor::ERepositioningMask Actor::GetRepositioningMask() const
{
	return mRepositioningMask;
}


// set the repositioning mask
inline void Actor::SetRepositioningMask(Actor::ERepositioningMask mask)
{
	mRepositioningMask = mask;
}


// set the repositioning mask based on some boolean values
inline void Actor::SetRepositioningMask(const bool positionEnabled, const bool rotationEnabled, const bool scaleEnabled)
{
	int mask = 0;

	// create the mask
	if (positionEnabled)	mask |= REPOSITION_POSITION;
	if (rotationEnabled)	mask |= REPOSITION_ROTATION;
	if (scaleEnabled)		mask |= REPOSITION_SCALE;

	// set it
	mRepositioningMask = (ERepositioningMask)mask;
}
