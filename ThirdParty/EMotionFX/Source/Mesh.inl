/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Mesh.h"


//-------------------------------------------------------------------------------------------------
// Mesh
//-------------------------------------------------------------------------------------------------

inline int Mesh::GetNumVertices() const
{ 
	return mNumVertices;
}


inline int Mesh::GetNumIndices() const
{ 
	return mNumIndices;
}


inline int Mesh::GetNumFaces() const
{
	return mNumFaces;
}


inline int Mesh::GetNumSubMeshes() const
{
	return mSubMeshes.GetLength();
}


inline SubMesh* Mesh::GetSubMesh(const int nr) const
{
	assert(nr<mSubMeshes.GetLength());
	return mSubMeshes[nr];
}


inline void Mesh::AddSubMesh(SubMesh* subMesh)
{
	mSubMeshes.Add( subMesh );
}


inline MCore::Vector3* Mesh::GetPositions()
{
	return mPositions;
}


inline MCore::Vector3* Mesh::GetOrgPositions()
{
	return mPositions + mNumVertices;
}


inline int* Mesh::GetIndices()
{
	return mIndices;
}


inline void Mesh::SetFace(const int faceNr, const int a, const int b, const int c)
{
	assert(faceNr < mNumFaces);
	
	int startIndex = faceNr * 3;
	mIndices[startIndex++]	= a;
	mIndices[startIndex++]	= b;
	mIndices[startIndex]	= c;
}


inline MCore::Vector3* Mesh::GetNormals()
{
	return mNormals;
}

inline MCore::Vector3* Mesh::GetOrgNormals()
{
	return mNormals + mNumVertices;
}


inline UVVertexAttributeLayer* Mesh::GetUVLayer(const int uvSet)
{
	return (UVVertexAttributeLayer*)FindVertexAttributeLayer( UVVertexAttributeLayer::TYPE_ID, uvSet );
}


inline MCore::Vector4* Mesh::GetTangents()
{
	return mTangents;
}


inline MCore::Vector4* Mesh::GetOrgTangents()
{
	return mTangents + mNumVertices;
}


inline int* Mesh::GetOrgVerts()
{
	return mOrgVerts;
}


inline int Mesh::GetNumOrgVertices() const
{
	return mNumOrgVerts;
}

/*
inline VertexAttribute* Mesh::GetSharedVertexAttributes(const int layerNr)
{
	assert(layerNr < mSharedVertexAttributes.GetLength());
	return mSharedVertexAttributes[ layerNr ];
}


inline int Mesh::GetNumSharedVertexAttributeLayers() const
{
	return mSharedVertexAttributes.GetLength();
}


inline int Mesh::FindSharedVertexAttributeLayerNumber(const int attributeTypeID, const int occurrence) const
{
	int layerCounter = 0;

	// check all vertex attributes of our first vertex, and find where the specific attribute is
	const int numLayers = mSharedVertexAttributes.GetLength();
	for (int i=0; i<numLayers; i++)
	{
		VertexAttribute* attributes = mSharedVertexAttributes[i];

		if ((attributes!=NULL) && (attributes->GetType() == attributeTypeID))
		{
			if (occurrence == layerCounter)
				return i;

			layerCounter++;
		}
	}

	// not found
	return -1;
}

//------

inline VertexAttribute* Mesh::GetVertexAttributes(const int layerNr)
{
	assert(layerNr < mVertexAttributes.GetLength());
	return mVertexAttributes[ layerNr ];
}

inline int Mesh::GetNumVertexAttributeLayers() const
{
	return mVertexAttributes.GetLength();
}


inline int Mesh::FindVertexAttributeLayerNumber(const int attributeTypeID, const int occurrence) const
{
	int layerCounter = 0;

	// check all vertex attributes of our first vertex, and find where the specific attribute is
	const int numLayers = mVertexAttributes.GetLength();
	for (int i=0; i<numLayers; i++)
	{
		VertexAttribute* attributes = mVertexAttributes[i];
		if ((attributes != NULL) && (attributes->GetType() == attributeTypeID))
		{
			if (layerCounter == occurrence)
				return i;

			layerCounter++;
		}
	}

	// not found
	return -1;
}
*/
