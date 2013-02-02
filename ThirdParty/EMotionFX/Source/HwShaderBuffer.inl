/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "HwShaderBuffer.h"


//-------------------------------------------------------------------------------------------------
// HwShaderInfluence
//-------------------------------------------------------------------------------------------------

inline HwShaderInfluence::HwShaderInfluence(const int boneNumber, const float weight) : mBoneNr(boneNumber), mWeight(weight) 
{
}


inline int HwShaderInfluence::GetBoneNumber() const
{
	return mBoneNr;
}


inline float HwShaderInfluence::GetWeight() const
{
	return mWeight;
}




//-------------------------------------------------------------------------------------------------
// HwShaderVertex
//-------------------------------------------------------------------------------------------------

inline HwShaderVertex::HwShaderVertex(Mesh* mesh, const int vertexNumber) : mMesh(mesh), mVertexNr(vertexNumber) 
{
}


inline void HwShaderVertex::AddInfluence(const HwShaderInfluence& influence)
{
	mInfluences.Add( influence );
}


inline int HwShaderVertex::GetVertexNumber() const
{
	return mVertexNr;
}


inline Mesh* HwShaderVertex::GetMesh() const
{
	return mMesh;
}


inline int HwShaderVertex::GetNumInfluences() const
{
	return mInfluences.GetLength();
}


inline HwShaderInfluence& HwShaderVertex::GetInfluence(const int nr)
{
	return mInfluences[nr];
}



//-------------------------------------------------------------------------------------------------
// HwShaderPrimitive
//-------------------------------------------------------------------------------------------------

inline HwShaderPrimitive::HwShaderPrimitive(const int startIndex, const int numTriangles, Node* node, const int materialNr, const int startVertex, const int numVertices)
{
	mStartIndex		= startIndex;
	mNumTriangles	= numTriangles;
	mNode			= node;
	mMaterialNr		= materialNr;
	mStartVertex	= startVertex;
	mNumVertices	= numVertices;
}


inline int HwShaderPrimitive::GetNumTriangles() const
{
	return mNumTriangles;
}


inline int HwShaderPrimitive::GetStartIndex() const
{
	return mStartIndex;
}


inline Node* HwShaderPrimitive::GetNode() const
{
	return mNode;
}


inline int HwShaderPrimitive::GetMaterialNr() const 
{ 
	return mMaterialNr; 
}

inline int HwShaderPrimitive::GetStartVertex() const 
{
	return mStartVertex; 
}


inline int HwShaderPrimitive::GetNumVertices() const 
{
	return mNumVertices; 
}


//-------------------------------------------------------------------------------------------------
// HwShaderBuffer
//-------------------------------------------------------------------------------------------------

inline int HwShaderBuffer::GetNumVertices() const			
{
	return mVertices.GetLength();
}


inline int HwShaderBuffer::GetNumIndices() const
{
	return mIndices.GetLength();
}


inline int HwShaderBuffer::GetNumPrimitives() const
{
	return mPrimitives.GetLength();
}


inline int HwShaderBuffer::GetNumBones() const
{
	return mBones.GetLength();
}

/*
inline int HwShaderBuffer::GetMaterialNumber() const
{
	return mMaterialNumber;
}
*/

inline int HwShaderBuffer::GetNumInfluences() const
{
	return mNumInfluences;
}


inline void HwShaderBuffer::StartPrimitive(Node* node, const int matNr)
{
	mStartIndex = mIndices.GetLength();
	mStartNode	= node;
	mStartVertex= mVertices.GetLength();
	mStartMatNr = matNr;
}


inline void HwShaderBuffer::EndPrimitive()
{
	// if faces have been added
	if (mStartIndex != mIndices.GetLength())
	{
		// create a new primitive
		int numTriangles = (mIndices.GetLength() - mStartIndex) / 3;
		mPrimitives.Add( HwShaderPrimitive(mStartIndex, numTriangles, mStartNode, mStartMatNr, 0, mVertices.GetLength()) );
	}
}


inline HwShaderVertex& HwShaderBuffer::GetVertex(const int vtxNr)
{
	return mVertices[vtxNr];
}


inline int HwShaderBuffer::GetIndex(const int indexNr) const
{
	return mIndices[indexNr];
}


inline const HwShaderPrimitive& HwShaderBuffer::GetPrimitive(const int primNr) const
{
	return mPrimitives[primNr];
}


inline Node* HwShaderBuffer::GetBone(const int nr) const
{
	return mBones[nr];
}
