/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Node.h"
#include "SimpleMesh.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// SimpleMesh
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

// constructor
SimpleMesh::SimpleMesh(const int numVertices, const int numFaces) : mPositions(NULL), mIndices(NULL), mNumFaces(0), mNumVerts(0)		
{ 
	Alloc(numVertices, numFaces); 
}


// destructor
SimpleMesh::~SimpleMesh() 
{
	Release();
}


void SimpleMesh::Alloc(const int nrVertices, const int nrFaces)
{
	// check bounds
	assert(nrVertices>0 && nrFaces>0);

	// get rid of existing data
	Release();

	// allocate vertices
	mPositions = new Vector3[nrVertices];

	// allocate indices
	mIndices = new int[nrFaces*3];

	// set numbers
	mNumVerts	= nrVertices;
	mNumFaces	= nrFaces;
	mNumIndices	= nrFaces * 3;
}



void SimpleMesh::Release()
{
	// delete the object space vertices
	if (mPositions)
	{
		delete[] mPositions;
		mPositions = NULL;
	}

	if (mIndices)
	{
		delete[] mIndices;
		mIndices = NULL;
	}

	// reset numbers
	mNumVerts	= 0;
	mNumFaces	= 0;
	mNumIndices = 0;
	mBoundingBox.Init();
	mBoundingSphere.Init();
}


// returns true if the given ray intersects with this mesh (fills outIntersect with the closest intersection)
bool SimpleMesh::Intersects(Node* node, const Ray& ray, Vector3 *outIntersect)
{
	// create the ray in object space
	Matrix	invNodeTM			= node->GetWorldTM().Inversed();
	Vector3	newOrigin			= ray.GetOrigin() * invNodeTM;
	Vector3	newDest				= ray.GetDest()   * invNodeTM;

	// create the ray
	Ray testRay(newOrigin, newDest);

	// test the ray with the bounding volumes
	if (!testRay.Intersects(mBoundingSphere)) return false;
	if (!testRay.Intersects(mBoundingBox)) return false;

	Vector3	closestPoint;
	Vector3	intersect;
	float	closestDist			= FLT_MAX;
	bool	foundIntersection	= false;

	// check all faces
	for (int i=0; i<mNumIndices; i+=3)
	{
		if (testRay.Intersects(mPositions[mIndices[i]], mPositions[mIndices[i+1]], mPositions[mIndices[i+2]], &intersect))
		{
			// calculate the square distance
			float dist = (intersect - testRay.GetOrigin()).SquareLength();

			// check if this point is closer as the previous found points
			if (dist < closestDist)
			{
				foundIntersection	= true;
				closestDist			= dist;
				closestPoint		= intersect;
			}
		}
	}

	// return the closest intersection point
	if (foundIntersection)
	{
		*outIntersect = closestPoint * node->GetWorldTM();	// in world space
		return true;
	}

	// no intersection found
	return false;
}


// returns true if the given ray intersects with this mesh
bool SimpleMesh::Intersects(Node* node, const Ray& ray)
{
	// create the ray in object space
	Matrix	invNodeTM			= node->GetWorldTM().Inversed();
	Vector3	newOrigin			= ray.GetOrigin() * invNodeTM;
	Vector3	newDest				= ray.GetDest()   * invNodeTM;

	// create the ray
	Ray testRay(newOrigin, newDest);

	// test the ray against the bounding volumes first
	if (!testRay.Intersects(mBoundingSphere)) return false;
	if (!testRay.Intersects(mBoundingBox)) return false;

	// check all faces
	for (int i=0; i<mNumIndices; i+=3)
	{
		// if there is an intersection, return true
		if (testRay.Intersects(mPositions[mIndices[i]], mPositions[mIndices[i+1]], mPositions[mIndices[i+2]]))
			return true;
	}

	// there is no intersection
	return false;
}


} // namespace EMotionFX