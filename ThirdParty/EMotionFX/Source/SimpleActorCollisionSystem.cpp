/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SimpleActorCollisionSystem.h"
#include "Actor.h"
#include "Node.h"
#include "Mesh.h"
#include "SimpleMesh.h"


using namespace MCore;

namespace EMotionFX
{

// constructor
SimpleActorCollisionSystem::SimpleActorCollisionSystem(Actor* actor) : ActorCollisionSystem(actor)
{
}


// destructor
SimpleActorCollisionSystem::~SimpleActorCollisionSystem()
{
}


// returns a pointer to the node it intersect with, otherwise NULL
Node* SimpleActorCollisionSystem::IntersectsCollisionMesh(const int lodLevel, const Ray& ray)
{
	// check all nodes for an intersection
	const int numNodes = mActor->GetNumNodes();
	for (int i=0; i<numNodes; i++)
	{
		Node* curNode = mActor->GetNode(i);	
		if (curNode->GetNodeCollisionSystem()->IntersectsCollisionMesh(lodLevel, ray))
			return curNode;
	}

	// there is no intersection
	return NULL;
}


// check for a collision with the collision meshes
Node* SimpleActorCollisionSystem::IntersectsCollisionMesh(const int lodLevel, const Ray& ray, Vector3* outIntersect, Vector3* outNormal, Vector2 *outUV, float* outBaryU, float* outBaryV, int* outStartIndex)
{
	Node*	closestNode = NULL;
	int			startIndex, closestStartIndex=0;
	Vector3		point;
	Vector3		closestPoint;
	float		dist, baryU, baryV, closestBaryU=0, closestBaryV=0;
	float		closestDist	= FLT_MAX;

	// check all nodes
	const int numNodes = mActor->GetNumNodes();
	for (int i=0; i<numNodes; i++)
	{
		Node* curNode = mActor->GetNode(i);

		// if there is an intersection with the ray and the given node
		if (curNode->GetNodeCollisionSystem()->IntersectsCollisionMesh(lodLevel, ray, &point, &baryU, &baryV, &startIndex))
		{
			// calculate the squared distance between the intersection point and the ray origin
			dist = (point - ray.GetOrigin()).SquareLength();

			// if it is the closest point till now, record it as closest
			if (dist < closestDist)
			{
				closestPoint	= point;
				closestDist		= dist;
				closestNode		= curNode;
				closestBaryU	= baryU;
				closestBaryV	= baryV;
				closestStartIndex = startIndex;
			}
		}
	}

	// output the closest values
	if (closestNode)
	{
		if (outIntersect)
			*outIntersect = closestPoint;

		if (outBaryU)
			*outBaryU = closestBaryU;

		if (outBaryV)
			*outBaryV = closestBaryV;

		if (outStartIndex)
			*outStartIndex = closestStartIndex;

		// calculate the interpolated normal
		if (outNormal || outUV)
		{
			Mesh*	mesh	= closestNode->GetCollisionMesh(lodLevel).GetPointer();
			int*		indices	= mesh->GetIndices();

			// calculate the normal at the intersection point
			if (outNormal)
			{
				Vector3* normals = mesh->GetNormals();
				Vector3 norm = BarycentricInterpolate<Vector3>(closestBaryU, closestBaryV, normals[ indices[closestStartIndex] ],	normals[ indices[closestStartIndex+1] ], normals[ indices[closestStartIndex+2] ] );
				norm = closestNode->GetWorldTM().Mul3x3(norm);
				norm.Normalize();
				*outNormal = norm;
			}

			// calculate the UV coordinate at the intersection point
			if (outUV)
			{
				// get the UV coordinates of the first layer
				UVVertexAttributeLayer* uvLayer = mesh->GetUVLayer(0);
				if (uvLayer != NULL)
				{
					Vector2* uvData = uvLayer->GetUVs();

					// calculate the interpolated texture coordinate
					*outUV = BarycentricInterpolate<Vector2>(closestBaryU, closestBaryV, uvData[ indices[closestStartIndex  ] ],
																						 uvData[ indices[closestStartIndex+1] ],
																						 uvData[ indices[closestStartIndex+2] ]);
				}
			}
		}
	}

	// return the result
	return closestNode;
}


// checks for collision with the real meshes
Node* SimpleActorCollisionSystem::IntersectsMesh(const int lodLevel, const Ray& ray)
{
	// check all nodes for an intersection
	const int numNodes = mActor->GetNumNodes();
	for (int i=0; i<numNodes; i++)
	{
		Node* curNode = mActor->GetNode(i);	
		if (curNode->GetNodeCollisionSystem()->IntersectsMesh(lodLevel, ray))
			return curNode;
	}

	// there is no intersection
	return NULL;
}


// checks for collision with the real meshes
Node* SimpleActorCollisionSystem::IntersectsMesh(const int lodLevel, const Ray& ray, Vector3* outIntersect, Vector3* outNormal, Vector2 *outUV, float* outBaryU, float* outBaryV, int* outStartIndex)
{
	Node*	closestNode = NULL;
	int			startIndex, closestStartIndex=0;
	Vector3		point;
	Vector3		closestPoint;
	float		dist, baryU, baryV, closestBaryU=0, closestBaryV=0;
	float		closestDist	= FLT_MAX;

	// check all nodes
	const int numNodes = mActor->GetNumNodes();
	for (int i=0; i<numNodes; i++)
	{
		Node* curNode = mActor->GetNode(i);

		// if there is an intersection with the ray and the given node
		if (curNode->GetNodeCollisionSystem()->IntersectsMesh(lodLevel, ray, &point, &baryU, &baryV, &startIndex))
		{
			// calculate the squared distance between the intersection point and the ray origin
			dist = (point - ray.GetOrigin()).SquareLength();

			// if it is the closest point till now, record it as closest
			if (dist < closestDist)
			{
				closestPoint	= point;
				closestDist		= dist;
				closestNode		= curNode;
				closestBaryU	= baryU;
				closestBaryV	= baryV;
				closestStartIndex = startIndex;
			}
		}
	}

	// output the closest values
	if (closestNode)
	{
		if (outIntersect)
			*outIntersect = closestPoint;

		if (outBaryU)
			*outBaryU = closestBaryU;

		if (outBaryV)
			*outBaryV = closestBaryV;

		if (outStartIndex)
			*outStartIndex = closestStartIndex;

		// calculate the interpolated normal
		if (outNormal || outUV)
		{
			Mesh*	mesh	= closestNode->GetMesh(lodLevel).GetPointer();
			int*		indices	= mesh->GetIndices();

			// calculate the normal at the intersection point
			if (outNormal)
			{
				Vector3* normals = mesh->GetNormals();
				Vector3 norm = BarycentricInterpolate<Vector3>(closestBaryU, closestBaryV, normals[ indices[closestStartIndex] ],	normals[ indices[closestStartIndex+1] ], normals[ indices[closestStartIndex+2] ] );
				norm = closestNode->GetWorldTM().Mul3x3(norm);
				norm.Normalize();
				*outNormal = norm;
			}

			// calculate the UV coordinate at the intersection point
			if (outUV)
			{
				// get the UV coordinates of the first layer
				UVVertexAttributeLayer* uvLayer = mesh->GetUVLayer(0);
				if (uvLayer != NULL)
				{
					Vector2* uvData = uvLayer->GetUVs();

					// calculate the interpolated texture coordinate
					*outUV = BarycentricInterpolate<Vector2>(closestBaryU, closestBaryV, uvData[ indices[closestStartIndex  ] ],
																						 uvData[ indices[closestStartIndex+1] ],
																						 uvData[ indices[closestStartIndex+2] ]);
				}
			}
		}
	}

	// return the result
	return closestNode;
}


// clone the node collision system
ActorCollisionSystem* SimpleActorCollisionSystem::Clone(Actor* newActor) const
{
	return new SimpleActorCollisionSystem(newActor);
}


// return the type ID
int SimpleActorCollisionSystem::GetType() const
{ 
	return SimpleActorCollisionSystem::TYPE_ID; 
}


} // namespace EMotionFX