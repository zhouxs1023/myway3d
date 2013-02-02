/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SubMesh.h"


namespace EMotionFX
{


// constructor
SubMesh::SubMesh(Mesh* parentMesh, const int startVertex, const int startIndex, const int numVerts, const int numIndices, const int materialIndex) 
{
	mParentMesh		= parentMesh;
	mNumVertices	= numVerts;
	mNumIndices		= numIndices;
	mStartIndex		= startIndex;
	mStartVertex	= startVertex;
	mMaterial		= materialIndex;
}


// destructor
SubMesh::~SubMesh() 
{ 
}


} // namespace EMotionFX