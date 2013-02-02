/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Node.h"
#include "SimpleMesh.h"
#include "Mesh.h"
#include "MeshDeformerStack.h"
#include "Material.h"
#include "SimpleNodeCollisionSystem.h"
#include "NodeIDGenerator.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
Node::Node(const String& name) : mName(name)
{
	mParent				= NULL;
	mCollisionSystem	= NULL;

	mStacks.Add( MCore::Pointer<MeshDeformerStack>(NULL) );
	mMeshes.Add( MCore::Pointer<Mesh>(NULL) );

	mColStacks.Add( MCore::Pointer<MeshDeformerStack>(NULL) );
	mColMeshes.Add( MCore::Pointer<Mesh>(NULL) );

	mLocalTM.Identity();
	mWorldTM.Identity();
	mInvBoneTM.Identity();
	mLocalScale.Set	(1.0, 1.0, 1.0);
	mLocalPos.Set	(0.0, 0.0, 0.0);
	mWorldScale.Set	(1.0, 1.0, 1.0);

	mOrgPos.Set		(0, 0, 0);
	mOrgScale.Set	(1, 1, 1);

	// reset the material offset, so that no offset will be used
	mMaterialOffset		= 0;

	// pregenerate the collision system
	mCollisionSystem = new SimpleNodeCollisionSystem(this);

	// calculate the ID
	mID = NodeIDGenerator::GetInstance().GenerateIDForString(mName);

	if((name=="Bip01 Prop1") || (name=="Bip01 Prop2"))
		m_isProp = true;
	else 
		m_isProp = false;
}


// destructor
Node::~Node()
{ 
	// get rid of all node attributes
	RemoveAllAttributes();

	// remove all meshes
	while (mMeshes.GetLength())
	{
		mMeshes.GetLast() = NULL;	// let the smartpointer deal with deletion
		mMeshes.RemoveLast();
	}

	// get rid of the stack
	while (mStacks.GetLength())
	{
		mStacks.GetLast() = NULL;	// let the smartpointer deal with deletion
		mStacks.RemoveLast();
	}

	// get rid of the collision meshes
	while (mColMeshes.GetLength())
	{
		mColMeshes.GetLast() = NULL;	// smartpointer deletion
		mColMeshes.RemoveLast();
	}

	// get rid of the collision mesh deformer stacks
	while (mColStacks.GetLength())
	{
		mColStacks.GetLast() = NULL;	// smartpointer deletion
		mColStacks.RemoveLast();
	}

	// delete the node collision system
	if (mCollisionSystem != NULL)
		delete mCollisionSystem;

	// remove all child nodes
	RemoveAllChilds();
}


// process down the hierarchy
void Node::RecursiveHierarchyUpdate()
{
	// update the local TM
	UpdateLocalTM();

	// update this node's transformations
	if (IsRootNode())
	{
		mWorldTM	= mLocalTM;
		mWorldScale	= mLocalScale;
	}
	else
	{
		Vector3 parentScale = mParent->GetWorldScale();
		mWorldScale	= Vector3(mLocalScale.x*parentScale.x, mLocalScale.y*parentScale.y, mLocalScale.z*parentScale.z);
		mWorldTM	= mLocalTM * mParent->GetWorldTM();		
	}

		// process the hierarchy for all childs
	const int numChilds = mChilds.GetLength();
	for (int i=0; i<numChilds; i++)
		mChilds[i]->RecursiveHierarchyUpdate();
}


// recursively update the world space matrix
void Node::RecursiveUpdateWorldTM(const Matrix* worldTM)
{
	// if we have set a world matrix, then copy it, else calculate it
	if (worldTM != NULL)
	{
		mWorldTM = *worldTM;

		// calculate the local TM
		if (mParent)
			mLocalTM = mWorldTM * mParent->GetWorldTM().Inversed();
		else
			mLocalTM = mWorldTM;
	}
	else
	{
		// if there is no parent, this really won't have any influence :)
		if (mParent == NULL) return;

		// calculate the updated world space matrix
		mWorldTM = mLocalTM * mParent->GetWorldTM();
	}

	// recursively update the world matrices of the childs
	const int numChilds = mChilds.GetLength();
	for (int i=0; i<numChilds; i++)
		mChilds[i]->RecursiveUpdateWorldTM( NULL );
}


// update the local transformation matrix (construct it from the mLocalQuat and mScale)
void Node::UpdateLocalTM()
{
	mLocalTM = Matrix(mLocalPos, mLocalRot, mLocalScale);
}


// clone the node stacks
void Node::CloneNodeStacksFromNode(Node* sourceNode, Actor* actor)
{
	// get rid of the existing stacks
	while (mStacks.GetLength())
	{
		//delete mStacks.GetLast();
		mStacks.GetLast() = NULL;	// smartpointer deletion
		mStacks.RemoveLast();
	}

	// clone all stacks
	for (int a=0; a<sourceNode->mStacks.GetLength(); a++)
	{
		if (sourceNode->mStacks[a].GetPointer() != NULL)
			mStacks.Add( sourceNode->mStacks[a].GetPointer()->Clone( mMeshes[a].GetPointer(), actor) );
		else
			mStacks.Add( MCore::Pointer<MeshDeformerStack>(NULL) );
	}
}


// clone the collision node stacks
void Node::CloneNodeCollisionStacksFromNode(Node* sourceNode, Actor* actor)
{
	// get rid of the existing collision stacks
	while (mColStacks.GetLength())
	{
		//delete mColStacks.GetLast();
		mColStacks.GetLast() = NULL;	// smartpointer deletion
		mColStacks.RemoveLast();
	}

	// clone all collision stacks
	for (int a=0; a<sourceNode->mColStacks.GetLength(); a++)
	{
		if (sourceNode->mColStacks[a].GetPointer() != NULL)
			mColStacks.Add( sourceNode->mColStacks[a].GetPointer()->Clone( mColMeshes[a].GetPointer(), actor) );
		else
			mColStacks.Add( MCore::Pointer<MeshDeformerStack>(NULL) );
	}
}


// create a clone of this node
Node* Node::Clone(Actor* actor, ENodeCloneFlags flags) const
{
	Node* result			= new Node(mName);

	// copy attributes
	result->mLocalRot		= mLocalRot;
	result->mLocalTM		= mLocalTM;
	result->mWorldTM		= mWorldTM;
	result->mInvBoneTM		= mInvBoneTM;
	result->mLocalScale		= mLocalScale;
	result->mLocalPos		= mLocalPos;
	result->mWorldScale		= mWorldScale;
	result->mParent			= NULL;
	result->mOrgPos			= mOrgPos;
	result->mOrgRot			= mOrgRot;
	result->mOrgScale		= mOrgScale;
	result->mMaterialOffset	= mMaterialOffset;

	// copy the meshes (well, the pointers, not the actual mesh data, since it is instanced)
	result->mMeshes			= mMeshes;
	result->mColMeshes		= mColMeshes;

	// copy the node attributes, if desired
	if (flags & CLONE_ATTRIBUTES)
	{
		for (int i=0; i<mAttributes.GetLength(); i++)
			result->AddAttribute( mAttributes[i]->Clone() );
	}


	// clone node stacks if desired
	if (flags & CLONE_NODESTACKS)
	{
		result->CloneNodeStacksFromNode((Node*)this, actor);
	}

	// clone node collision stacks if desired
	if (flags & CLONE_NODECOLLISIONSTACKS)
	{
		result->CloneNodeCollisionStacksFromNode((Node*)this, actor);
	}


	// clone the node collision system
	if (mCollisionSystem)
		result->SetNodeCollisionSystem( mCollisionSystem->Clone(result) );

	// return the resulting clone
	return result;
}


// removes all attributes
void Node::RemoveAllAttributes()
{
	while (mAttributes.GetLength())
	{
		delete mAttributes.GetLast();
		mAttributes.RemoveLast();
	}
}


// removes all meshes and stacks from this node, for all LOD levels.
void Node::RemoveMeshes()
{
	for (int i=0; i<mMeshes.GetLength(); i++)
		RemoveMeshesForLOD(i);
}


// remove meshes for a given LOD, but keep the LOD levels intact
void Node::RemoveMeshesForLOD(const int lodLevel)
{
	mMeshes[lodLevel] = MCore::Pointer<Mesh>(NULL);
	mStacks[lodLevel] = MCore::Pointer<MeshDeformerStack>(NULL);
}


// removes all collision meshes and stacks from this node, for all LOD levels.
void Node::RemoveCollisionMeshes()
{
	for (int i=0; i<mMeshes.GetLength(); i++)
		RemoveMeshesForLOD(i);
}


// remove collision meshes for a given LOD, but keep the LOD levels intact
void Node::RemoveCollisionMeshesForLOD(const int lodLevel)
{
	mColMeshes[lodLevel] = MCore::Pointer<Mesh>(NULL);
	mColStacks[lodLevel] = MCore::Pointer<MeshDeformerStack>(NULL);
}


// set new collision system
void Node::SetNodeCollisionSystem(NodeCollisionSystem* collisionSystem)
{
	if (collisionSystem != NULL)
	{
		// if there is a collision system delete it
		if (mCollisionSystem != NULL)
		{
			delete mCollisionSystem;
		}

		mCollisionSystem = collisionSystem;
	}
}


// search for the motion part
MotionPart* Node::SearchMotionPart(Motion* motion)
{
	const int numLinks = mLinks.GetLength();
	for (int i=0; i<numLinks; i++)
	{
		if (mLinks[i].GetMotionInstance()->GetMotion() == motion)
			return mLinks[i].GetMotionPart();
	}

	return NULL;
}


// search for the motion link
MotionLink* Node::SearchMotionLink(Motion* motion)
{
	const int numLinks = mLinks.GetLength();
	for (int i=0; i<numLinks; i++)
	{
		if (mLinks[i].GetMotionInstance()->GetMotion() == motion)
			return &(mLinks[i]);
	}

	return NULL;
}


// search for the motion link
MotionLink* Node::SearchMotionLink(MotionInstance* instance)
{
	const int numLinks = mLinks.GetLength();
	for (int i=0; i<numLinks; i++)
	{
		if (mLinks[i].GetMotionInstance() == instance)
			return &mLinks[i];
	}

	return NULL;
}


// check if the mesh at the given LOD is deformable
bool Node::HasDeformableMesh(const int lodLevel) const
{
	assert(lodLevel < mMeshes.GetLength());
	return (mMeshes[lodLevel].GetPointer() && mStacks[lodLevel].GetPointer() && mStacks[lodLevel].GetPointer()->GetNumDeformers()>0);
}


// check if the collision mesh at the given LOD is deformable
bool Node::HasDeformableCollisionMesh(const int lodLevel) const
{
	assert(lodLevel < mColMeshes.GetLength());
	return (mColMeshes[lodLevel].GetPointer() && mColStacks[lodLevel].GetPointer() && mColStacks[lodLevel].GetPointer()->GetNumDeformers()>0);
}


// check if there is any mesh available
bool Node::HasMesh() const
{
	assert(mMeshes.GetLength() > 0 && mColMeshes.GetLength() > 0);
	return !(mMeshes[0].GetPointer() == NULL && mColMeshes[0].GetPointer() == NULL);
}


// calculate the mesh based axis aligned bounding box
void Node::CalcMeshBasedAABB(MCore::AABB* result, const int lodLevel)
{
	// initialize the bounding box
	result->Init();
	
	// get the mesh
	Mesh* mesh = GetMesh( lodLevel ).GetPointer();
	if (mesh == NULL) return;

	// get the position data
	Vector3* positions = mesh->GetPositions();

	// process all vertices
	const int numVerts = mesh->GetNumVertices();
	for (int v=0; v<numVerts; v++)
		result->Encapsulate( positions[v] * mWorldTM  );
}


// calculate the collision mesh based axis aligned bounding box
void Node::CalcCollisionMeshBasedAABB(MCore::AABB* result, const int lodLevel)
{
	// initialize the bounding box
	result->Init();
	
	// get the mesh
	Mesh* mesh = GetCollisionMesh( lodLevel ).GetPointer();
	if (mesh == NULL) return;

	// get the position data
	Vector3* positions = mesh->GetPositions();

	// process all vertices
	const int numVerts = mesh->GetNumVertices();
	for (int v=0; v<numVerts; v++)
		result->Encapsulate( positions[v] * mWorldTM  );
}


// checks if the given node is a child of this node
bool Node::IsNodeChild(Node* node, const bool compareNames)
{
	// retrieve the number of childs
	const int numChilds = GetNumChilds();

	// iterate through all childs and compare them with the given node
	for (int i=0; i<numChilds; i++)
	{
		// retrieve the current search node
		Node* child = mChilds[i];

		// the given node is already a child, return success
		if (compareNames)
		{
			if (node->GetName() == child->GetName())
				return true;
		}
		else
		{
			if (node == child)
				return true;
		}
	}

	// nothing found
	return false;
}


// get the total number of children
int Node::GetNumChildNodesRecursive()
{
	// the number of total child nodes which include the childs of the childs, too
	int numTotalChilds = 0;

	// retrieve the number of child nodes of the actual node
	const int numChilds = mChilds.GetLength();

	for (int i=0; i<numChilds; i++)
	{
		// retrieve the current child node
		Node* child = mChilds[i];

		if (!child)
			continue;

		// get the number of children and add it to the total number
		numTotalChilds += child->GetNumChilds();

		// go recursive!
		numTotalChilds += child->GetNumChildNodesRecursive();
	}

	return numTotalChilds;
}

} // namespace EMotionFX