/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SkinningInfoVertexAttributeLayer.h"
#include "Actor.h"


namespace EMotionFX
{

// destructor
SkinInfluence::~SkinInfluence()
{
}

// constructor
SkinningInfoVertexAttributeLayer::SkinningInfoVertexAttributeLayer(const int numAttributes) : VertexAttributeLayer(numAttributes)
{
	mInfluences.Resize( numAttributes );
}


// destructor
SkinningInfoVertexAttributeLayer::~SkinningInfoVertexAttributeLayer()
{
}


// get the unique layer type
int	SkinningInfoVertexAttributeLayer::GetType() const
{
	return TYPE_ID;
}


// get the description of the vertex attributes or layer
const char* SkinningInfoVertexAttributeLayer::GetTypeString() const
{
	return "SkinningInfoVertexAttribute";
}


// add a given influence (using a bone and a weight)
void SkinningInfoVertexAttributeLayer::AddInfluence(const int attributeNr, Node* bone, const float weight, const int boneNr)
{
	mInfluences[attributeNr].Add( SkinInfluence(bone, weight, boneNr) );
}


// relink the attributes to a given mesh that is part of a given node which again is part of a given actor
void SkinningInfoVertexAttributeLayer::ReLinkToNode(Mesh* mesh, Node* node, Actor* actor)
{
	const int numAttributes = mInfluences.GetLength();
	for (int i=0; i<numAttributes; i++)
	{
		const int numWeights = mInfluences[i].GetLength();
		for (int w=0; w<numWeights; w++)
		{
			// find the new bone pointer
			Node* newBone = actor->GetNodeByID( mInfluences[i][w].GetBone()->GetID() );
			assert(newBone);

			// update it
			mInfluences[i][w].SetBone( newBone );
		}
	}
}


} // namespace EMotionFX