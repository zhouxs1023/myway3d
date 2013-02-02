/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SubMesh.h"


inline int SubMesh::GetStartIndex() const
{
	return mStartIndex;
}


inline int SubMesh::GetStartVertex() const
{
	return mStartVertex;
}


inline MCore::Vector3* SubMesh::GetPositions()
{
	return (MCore::Vector3*)(((unsigned char*)mParentMesh->GetPositions()) + mStartVertex * sizeof(MCore::Vector3));
}


inline MCore::Vector3* SubMesh::GetOrgPositions()
{
	return (MCore::Vector3*)(((unsigned char*)mParentMesh->GetOrgPositions()) + mStartVertex * sizeof(MCore::Vector3));
}


inline MCore::Vector3* SubMesh::GetNormals()
{
	return (MCore::Vector3*)(((unsigned char*)mParentMesh->GetNormals()) + mStartVertex * sizeof(MCore::Vector3));
}


inline MCore::Vector3* SubMesh::GetOrgNormals()
{
	return (MCore::Vector3*)(((unsigned char*)mParentMesh->GetOrgNormals()) + mStartVertex * sizeof(MCore::Vector3));
}

/*
inline UVVertexAttribute* SubMesh::GetUVs(const int uvSet)
{
	UVVertexAttribute* uvStart = mParentMesh->GetUVs( uvSet );
	if (uvStart == NULL) return NULL;
	return (UVVertexAttribute*)(((unsigned char*)uvStart) + mStartVertex * sizeof(UVVertexAttribute*));
}
*/

inline MCore::Vector3* SubMesh::GetTangents()
{
	return (MCore::Vector3*)(((unsigned char*)mParentMesh->GetTangents()) + mStartVertex * sizeof(MCore::Vector3));
}


inline MCore::Vector3* SubMesh::GetOrgTangents()
{
	return (MCore::Vector3*)(((unsigned char*)mParentMesh->GetOrgTangents()) + mStartVertex * sizeof(MCore::Vector3));
}


inline int* SubMesh::GetOrgVerts()
{
	return (int*)(((unsigned char*)mParentMesh->GetOrgVerts()) + mStartVertex * sizeof(int));
}


inline int* SubMesh::GetIndices()
{
	return (int*)(((unsigned char*)mParentMesh->GetIndices()) + mStartIndex * sizeof(int));
}


inline int SubMesh::GetNumVertices() const
{
	return mNumVertices;
}


inline int SubMesh::GetNumIndices() const
{
	return mNumIndices;
}


inline Mesh* SubMesh::GetParentMesh() const
{
	return mParentMesh;
}


inline void SubMesh::SetParentMesh(Mesh *mesh)
{
	mParentMesh = mesh;
}


inline void SubMesh::SetMaterial(const int materialIndex)
{
	mMaterial = materialIndex;
}


inline int SubMesh::GetMaterial() const
{
	return mMaterial;
}