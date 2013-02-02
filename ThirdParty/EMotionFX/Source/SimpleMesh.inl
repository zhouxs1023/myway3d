/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SimpleMesh.h"


// get the number of vertices
inline int SimpleMesh::GetNumVertices() const
{ 
	return mNumVerts;
}


// get the number of faces
inline int SimpleMesh::GetNumFaces() const
{ 
	return mNumFaces;
}


// get the bounding box with write access
inline MCore::AABB& SimpleMesh::GetBoundingBox()
{ 
	return mBoundingBox; 
}


// get the bounding sphere with write access
inline MCore::BoundingSphere& SimpleMesh::GetBoundingSphere()
{ 
	return mBoundingSphere; 
}


// get the positions
inline MCore::Vector3* SimpleMesh::GetPositions()
{
	return mPositions;
}


// get the indices
inline int* SimpleMesh::GetIndices()
{
	return mIndices;
}


// get the number of indices
inline int SimpleMesh::GetNumIndices() const
{
	return mNumIndices;
}
