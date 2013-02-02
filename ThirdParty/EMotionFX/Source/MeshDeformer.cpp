/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MeshDeformer.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MeshDeformer::MeshDeformer(Mesh* mesh)
{
	mMesh		= mesh;
	mIsEnabled	= true;
}


// destructor
MeshDeformer::~MeshDeformer()
{
}


bool MeshDeformer::IsEnabled() const
{ 
	return mIsEnabled; 
}


void MeshDeformer::SetEnabled(const bool enabled)
{
	mIsEnabled = enabled;
}


// reinitialize the mesh deformer
void MeshDeformer::ReInitialize()
{
}

} // namespace EMotionFX