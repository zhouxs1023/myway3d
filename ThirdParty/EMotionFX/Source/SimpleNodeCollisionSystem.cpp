/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SimpleNodeCollisionSystem.h"
#include "Actor.h"
#include "Node.h"
#include "Mesh.h"
#include "SimpleMesh.h"

using namespace MCore;

namespace EMotionFX
{

// constructor
SimpleNodeCollisionSystem::SimpleNodeCollisionSystem(Node* node) : NodeCollisionSystem(node)
{
}


// destructor
SimpleNodeCollisionSystem::~SimpleNodeCollisionSystem()
{
}


// returns a true if there is a collision, otherwise false
bool SimpleNodeCollisionSystem::IntersectsCollisionMesh(const int lodLevel, const Ray& ray)
{
	// make sure the LOD level exists for this node
	assert(lodLevel < mNode->GetNumCollisionMeshes());

	// get the direct pointer to the mesh, from the smartpointer
	Mesh* mesh = mNode->GetCollisionMesh(lodLevel).GetPointer();

	// if there is no mesh, there cannot be an intersection
	if (mesh == NULL)
		return false;

	// check intersection with the mesh
	if (mesh->Intersects(ray, mNode))
		return true;

	// no intersection occured or the node isn't vaid
	return false;
}


// returns a true if there is a collision, otherwise false
bool SimpleNodeCollisionSystem::IntersectsCollisionMesh(const int lodLevel, const Ray& ray, Vector3* outIntersect, float* outBaryU, float* outBaryV, int* outStartIndex)
{
	// make sure the LOD level exists for this node
	assert(lodLevel < mNode->GetNumCollisionMeshes());

	// get the direct pointer to the mesh, from the smartpointer
	Mesh* mesh = mNode->GetCollisionMesh(lodLevel).GetPointer();

	// if there is no mesh, there cannot be an intersection
	if (mesh == NULL)
		return false;

	// check for an intersection with the mesh
	if (mesh->Intersects(ray, mNode, outIntersect, outBaryU, outBaryV, outStartIndex))
		return true;

	// no intersection found or the node isn't vaid
	return false;
}


// checks for collision with the real meshes
bool SimpleNodeCollisionSystem::IntersectsMesh(const int lodLevel, const Ray& ray)
{
	// make sure the LOD level exists for this node
	assert(lodLevel < mNode->GetNumMeshes());

	// get the direct pointer to the mesh, from the smartpointer
	Mesh* mesh = mNode->GetMesh(lodLevel).GetPointer();

	// if there is no mesh, there cannot be an intersection
	if (mesh == NULL)
		return false;

	// check intersection with the mesh
	if (mesh->Intersects(ray, mNode))
		return true;

	// no intersection occured or the node isn't vaid
	return false;
}


// checks for collision with the real meshes
bool SimpleNodeCollisionSystem::IntersectsMesh(const int lodLevel, const Ray& ray, Vector3* outIntersect, float* outBaryU, float* outBaryV, int* outStartIndex)
{
	// make sure the LOD level exists for this node
	assert(lodLevel < mNode->GetNumMeshes());

	// get the direct pointer to the mesh, from the smartpointer
	Mesh* mesh = mNode->GetMesh(lodLevel).GetPointer();

	// if there is no mesh, there cannot be an intersection
	if (mesh == NULL)
		return false;

	// check for an intersection with the mesh
	if (mesh->Intersects(ray, mNode, outIntersect, outBaryU, outBaryV, outStartIndex))
		return true;

	// no intersection found or the node isn't vaid
	return false;
}


// clone the node collision system
NodeCollisionSystem* SimpleNodeCollisionSystem::Clone(Node* newNode) const
{
	return new SimpleNodeCollisionSystem(newNode);
}


// return the type ID
int SimpleNodeCollisionSystem::GetType() const
{ 
	return SimpleNodeCollisionSystem::TYPE_ID; 
}


} // namespace EMotionFX