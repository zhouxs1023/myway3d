/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Node.h"


// set the local transformation matrix (object space)
inline void Node::SetLocalTM(const MCore::Matrix& mat)
{ 
	mLocalTM = mat;
}


// set the node  transformation matrix (world space)
inline void Node::SetWorldTM(const MCore::Matrix& mat)
{ 
	mWorldTM	= mat;
}


// set the local rotation quaternion (object space)
inline void Node::SetLocalRot(const MCore::Quaternion& q)
{ 
	mLocalRot = q;
}


// set the parent (NULL in case of a root node)
inline void Node::SetParent(Node *parent)
{ 
	mParent = parent;
}


// set the local position (object space)
inline void Node::SetLocalPos(const MCore::Vector3& pos)
{ 
	mLocalPos = pos;
}


// set the local scaling (object space)
inline void Node::SetLocalScale(const MCore::Vector3& scale)
{ 
	mLocalScale	= scale;
}


// set the node scaling (worldspace)
inline void Node::SetWorldScale(const MCore::Vector3& scale)
{ 
	mWorldScale = scale;	
}


// set the inverse bone transformation matrix (inverse of the nodeTM in the initial pose)
inline void Node::SetInvBoneTM(const MCore::Matrix& mat)
{ 
	mInvBoneTM = mat;
}


// set the node position (worldspace)
inline void Node::SetWorldPos(const MCore::Vector3& pos)
{ 
	//mWorldPos = pos;
	mWorldTM.SetTranslation(pos);
}


// set the original pose position (object space)
inline void Node::SetOrgPos(const MCore::Vector3& pos)
{ 
	mOrgPos = pos;
}


// set the original pose rotaiton (object space)
inline void Node::SetOrgRot(const MCore::Quaternion& rot)
{ 
	mOrgRot	= rot;
}


// set the original pose scale (object space)
inline void Node::SetOrgScale(const MCore::Vector3& scale)
{
	mOrgScale = scale;
}


// set a given motion link
inline void Node::SetMotionLink(const int nr, const MotionLink& link)
{ 
	assert(nr<mLinks.GetLength()); 
	mLinks[nr] = link; 
}


// get the name of the node
inline const MCore::String& Node::GetName() const
{ 
	return mName;
}


// get the name of the node as pointer to chars
inline const char* Node::GetNamePtr() const
{ 
	return mName.GetReadPtr();
}


// get the number of child nodes for this node
inline int Node::GetNumChilds() const
{
	return mChilds.GetLength();
}


// get a pointer to the parent (NULL if this is a root node)
inline Node* Node::GetParent() const
{ 
	return mParent;
}


// get a given child
inline Node* Node::GetChild(const int nr) const
{ 
	assert(nr<mChilds.GetLength());
	return mChilds[nr];
}


// get the local transformation matrix (object space)
inline const MCore::Matrix& Node::GetLocalTM() const
{ 
	return mLocalTM;
}


// get the node transformation matrix (world space)
inline const MCore::Matrix& Node::GetWorldTM() const
{ 
	return mWorldTM;
}

// get the local transformation matrix (object space)
inline MCore::Matrix& Node::GetLocalTM()
{ 
	return mLocalTM;
}


// get the node transformation matrix (world space)
inline MCore::Matrix& Node::GetWorldTM()
{ 
	return mWorldTM;
}



// get the inverse of the bone transformation matrix in the initial worldspace pose
inline const MCore::Matrix& Node::GetInvBoneTM() const
{ 
	return mInvBoneTM;
}


// get the local rotation quaternion (object space)
inline const MCore::Quaternion& Node::GetLocalRot() const
{ 
	return mLocalRot;
}


// calculate the world space rotation in form of a quaternion
inline const MCore::Quaternion Node::CalcWorldRot() const
{
	return MCore::Quaternion::FromMatrix( mWorldTM );
}


// get the local position (object space)
inline const MCore::Vector3& Node::GetLocalPos() const
{ 
	return mLocalPos;
}


// get the node position (world space)
inline MCore::Vector3 Node::GetWorldPos() const
{
	return mWorldTM.GetTranslation();
}


// get the local scale (object space)
inline const MCore::Vector3& Node::GetLocalScale() const
{ 
	return mLocalScale;
}


// get the node scale (object space)
inline const MCore::Vector3& Node::GetWorldScale() const
{ 
	return mWorldScale;
}


// get the number of motion links
inline int Node::GetNumMotionLinks() const
{ 
	return mLinks.GetLength();
}


// get the original pose position (world space)
inline const MCore::Vector3& Node::GetOrgPos() const
{ 
	return mOrgPos;
}


// get the original pose rotation (world space)
inline const MCore::Quaternion& Node::GetOrgRot() const
{ 
	return mOrgRot;
}


// get the original pose scale (world space)
inline const MCore::Vector3& Node::GetOrgScale() const
{ 
	return mOrgScale;
}


// get a given motion link
inline MotionLink& Node::GetMotionLink(const int nr)
{ 
	assert(nr<mLinks.GetLength()); 
	return mLinks[nr]; 
}


// add a motion link to this node
inline void Node::AddMotionLink(const MotionLink& link)
{ 
	mLinks.Add(link);
}


// add a child to this node
inline void Node::AddChild(Node* newChild)
{ 
	mChilds.Add(newChild);
}


// remove a child from this node
inline void Node::RemoveChild(Node* child)
{ 
	mChilds.Remove(child);
}


// remove all childs from this node
inline void Node::RemoveAllChilds()
{ 
	mChilds.Clear();
}


// returns true if this is a root node, so if it has no parents
inline bool Node::IsRootNode() const
{ 
	return (mParent==NULL);	
}


// returns true if this node has childs
inline bool Node::HasChilds() const
{
	return (mChilds.GetLength()>0);
}


// remove all motion links
inline void Node::RemoveLinks()
{ 
	mLinks.Clear();
}


// recursively find the root node (expensive call)
inline Node* Node::FindRoot()
{
	if (mParent) 
		return mParent->FindRoot(); 
	
	return this; 
}


// reset to the initial pose
inline void Node::SetToOriginalOrientation()
{ 
	SetLocalPos(mOrgPos);
	SetLocalRot(mOrgRot);
	SetLocalScale(mOrgScale);
}


// get the forward vector in world space
inline MCore::Vector3 Node::GetForward() const
{ 
	return mWorldTM.GetForward();
}


// get the right vector in world space
inline MCore::Vector3 Node::GetRight() const
{
	return mWorldTM.GetRight();
}


// get the up vector in world space
inline MCore::Vector3 Node::GetUp() const
{
	return mWorldTM.GetUp();
}


// get the local forward direction (object space)
inline MCore::Vector3 Node::GetLocalForward() const
{ 
	return mLocalTM.GetForward();
}


// get the local right direction (object space)
inline MCore::Vector3 Node::GetLocalRight() const
{ 
	return mLocalTM.GetRight();
}


// get the local up direction (object space)
inline MCore::Vector3 Node::GetLocalUp() const
{ 
	return mLocalTM.GetUp();
}


// move this node forward in world space
inline void Node::MoveForward(const float units)
{ 
	mLocalPos += GetForward().Normalize() * units;
}


// move this node to the right in world space
inline void Node::MoveRight(const float units)
{ 
	mLocalPos += GetRight().Normalized() * units;
}


// move this node upwards in world space
inline void Node::MoveUp(const float units)
{ 
	mLocalPos += GetUp().Normalized() * units;
}


// rotate the object around a given axis (objectspace)
inline void Node::Rotate(const MCore::Vector3& axis, const float angleRadians)
{ 
	mLocalRot = mLocalRot * MCore::Quaternion(axis, angleRadians); 
}


// remove a given motion link which has the specified motion instance
inline bool Node::RemoveLink(MotionInstance *instance)
{
	for (int i=0; i<mLinks.GetLength(); i++)
	{
		if (mLinks[i].GetMotionInstance() == instance)
		{
			mLinks.Remove(i);
			return true;
		}
	}

	return false;
}


inline void Node::SetMesh(const MCore::Pointer<Mesh>& mesh, const int lodLevel)
{
	assert(lodLevel < mMeshes.GetLength());
	mMeshes[lodLevel] = mesh;
}


inline MCore::Pointer<Mesh>& Node::GetMesh(const int lodLevel)
{
	assert(lodLevel < mMeshes.GetLength());
	return mMeshes[lodLevel];
}


inline MCore::Pointer<MeshDeformerStack>& Node::GetMeshDeformerStack(const int lodLevel)
{
	assert(lodLevel < mStacks.GetLength());
	return mStacks[lodLevel];
}


inline void Node::SetMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack, const int lodLevel)
{
	assert(lodLevel < mStacks.GetLength());
	mStacks[lodLevel] = stack;
}


//-------------------------------------------------------------------------------------

inline void Node::SetCollisionMesh(const MCore::Pointer<Mesh>& mesh, const int lodLevel)
{
	assert(lodLevel < mColMeshes.GetLength());
	mColMeshes[lodLevel] = mesh;
}


inline MCore::Pointer<Mesh>& Node::GetCollisionMesh(const int lodLevel)
{
	assert(lodLevel < mColMeshes.GetLength());
	return mColMeshes[lodLevel];
}


inline MCore::Pointer<MeshDeformerStack>& Node::GetCollisionMeshDeformerStack(const int lodLevel)
{
	assert(lodLevel < mColStacks.GetLength());
	return mColStacks[lodLevel];
}


inline void Node::SetCollisionMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack, const int lodLevel)
{
	assert(lodLevel < mColStacks.GetLength());
	mColStacks[lodLevel] = stack;
}


inline void Node::AddLODCollisionMesh(const MCore::Pointer<Mesh>& lodMesh)
{
	mColMeshes.Add( lodMesh );
}


inline void Node::AddLODCollisionMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack)
{
	mColStacks.Add( stack );
}


inline int Node::GetNumCollisionMeshes() const
{
	return mColMeshes.GetLength();
}


inline int Node::GetNumCollisionMeshDeformerStacks() const
{
	return mColStacks.GetLength();
}

//-------------------------------------------------------------------------------------

inline void Node::AddAttribute(NodeAttribute* attribute)
{
	mAttributes.Add( attribute );
}


inline int Node::GetNumAttributes() const
{
	return mAttributes.GetLength();
}


inline NodeAttribute* Node::GetAttribute(const int attributeNr)
{
	// make sure we are in range
	assert(attributeNr < mAttributes.GetLength());

	// return the attribute
	return mAttributes[attributeNr];
}


inline int Node::FindAttributeNumber(const int attributeTypeID) const
{
	// check all attributes, and find where the specific attribute is
	const int numAttributes = mAttributes.GetLength();
	for (int i=0; i<numAttributes; i++)
		if (mAttributes[i]->GetType() == attributeTypeID)
			return i;

	// not found
	return -1;
}


inline NodeAttribute* Node::GetAttributeByType(const int attributeType)
{
	// check all attributes
	const int numAttributes = mAttributes.GetLength();
	for (int i=0; i<numAttributes; i++)
		if (mAttributes[i]->GetType() == attributeType)
			return mAttributes[i];

	// not found
	return NULL;
}


inline void Node::AddLODMesh(const MCore::Pointer<Mesh>& lodMesh)
{
	mMeshes.Add( lodMesh );
}


inline void Node::AddLODMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack)
{
	mStacks.Add( stack );
}


inline int Node::GetNumMeshes() const
{
	return mMeshes.GetLength();
}


inline int Node::GetNumMeshDeformerStacks() const
{
	return mStacks.GetLength();
}

/*
inline void Node::AttachToNode(Node* node)
{
	// if we want to link
	if (node != NULL)
	{
		mParent = node;
		mParent->AddChild( this );
	}
	else // we want to unlink
		Detach();
}


inline void Node::Detach()
{
	if (mParent == NULL) return;

	// remove this node from the childs
	mParent->RemoveChild( this );

	// unlink it from the parent
	mParent = NULL;
}
*/

//=================================================================================================


inline int Node::GetID() const
{
	return mID;
}

// return collision system
inline NodeCollisionSystem* Node::GetNodeCollisionSystem() const
{
	return mCollisionSystem;
}


// set the material offset
inline void Node::SetMaterialOffset(const int materialOffset)
{
	mMaterialOffset = materialOffset;
}


// return the material offset
inline int Node::GetMaterialOffset()
{
	return mMaterialOffset;
}