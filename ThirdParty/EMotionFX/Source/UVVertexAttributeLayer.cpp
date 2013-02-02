/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "UVVertexAttributeLayer.h"


namespace EMotionFX
{

// constructor
UVVertexAttributeLayer::UVVertexAttributeLayer(const int numAttributes) : VertexAttributeLayer(numAttributes) 
{
	mUVs = (MCore::Vector2*)MEMMGR.Allocate(sizeof(MCore::Vector2)*numAttributes, MEMCATEGORY_GEOMETRY_VERTEXATTRIBUTES);
}


// destructor
UVVertexAttributeLayer::~UVVertexAttributeLayer() 
{
	MEMMGR.Delete( mUVs );
}


// get the unique layer type
int	UVVertexAttributeLayer::GetType() const
{
	return TYPE_ID;
}


// get the description of the vertex attributes or layer
const char* UVVertexAttributeLayer::GetTypeString() const
{
	return "UVVertexAttributeLayer";
}

} // namespace EMotionFX