/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Mesh.h"
#include "SubMesh.h"
#include "Node.h"
#include "UVVertexAttributeLayer.h"
#include "SkinningInfoVertexAttributeLayer.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// default constructor
Mesh::Mesh()
{
	mNumVertices	= 0;
	mNumFaces		= 0;
	mNumIndices		= 0;
	mNumOrgVerts	= 0;
	mPositions		= NULL;
	mNormals		= NULL;
	mTangents		= NULL;
	mOrgVerts		= NULL;
	mIndices		= NULL;
	mShdMapName[0]  = '\0';
}


// allocation constructor
Mesh::Mesh(const int numVerts, const int numFaces, const int numOrgVerts)
{
	mNumVertices	= 0;
	mNumFaces		= 0;
	mNumIndices		= 0;
	mNumOrgVerts	= 0;
	mPositions		= NULL;
	mNormals		= NULL;
	mTangents		= NULL;
	mOrgVerts		= NULL;
	mIndices		= NULL;
	mShdMapName[0]  = '\0';

	// allocate the mesh data
	Allocate(numVerts, numFaces, numOrgVerts);
}


// destructor
Mesh::~Mesh() 
{ 
	ReleaseData(); 
}


// allocate mesh data
void Mesh::Allocate(const int numVerts, const int numFaces, const int numOrgVerts)
{
	// get rid of existing data
	ReleaseData();

	// allocate the new data
	if (numVerts > 0)
	{
		mPositions	= (Vector3*)MEMMGR.Allocate(sizeof(Vector3) * (numVerts<<1), MEMCATEGORY_GEOMETRY_MESHES, 16, Mesh::MEMORYBLOCK_ID);
		mNormals  	= (Vector3*)MEMMGR.Allocate(sizeof(Vector3) * (numVerts<<1), MEMCATEGORY_GEOMETRY_MESHES, 16, Mesh::MEMORYBLOCK_ID);
		mOrgVerts	= (int*)MEMMGR.Allocate(sizeof(int) * numVerts, MEMCATEGORY_GEOMETRY_MESHES, 16, Mesh::MEMORYBLOCK_ID);
	}

	// allocate the indices
	if (numFaces > 0)
		mIndices = (int*)MEMMGR.Allocate(sizeof(int) * numFaces*3, MEMCATEGORY_GEOMETRY_MESHES, 16, Mesh::MEMORYBLOCK_ID);

	// set number values
	mNumVertices= numVerts;
	mNumFaces	= numFaces;
	mNumIndices	= numFaces * 3;
	mNumOrgVerts= numOrgVerts;
}


// copy all original data over the output data
void Mesh::ResetToOriginalData()
{
	// init the final/output vertices with the original vertex positions
	if (mPositions != NULL)
		memcpy((unsigned char*)GetPositions(), (unsigned char*)GetOrgPositions(), mNumVertices*sizeof(Vector3));

	// init normals
	if (mNormals != NULL)
		memcpy((unsigned char*)GetNormals(), (unsigned char*)GetOrgNormals(), mNumVertices*sizeof(Vector3));

	// init tangents
	if (mTangents != NULL)
		memcpy((unsigned char*)GetTangents(), (unsigned char*)GetOrgTangents(), mNumVertices*sizeof(Vector4));
}


// release allocated mesh data from memory
void Mesh::ReleaseData()
{
	// get rid of all shared vertex attributes
	RemoveAllSharedVertexAttributeLayers();

	// get rid of all non-shared vertex attributes
	RemoveAllVertexAttributeLayers();

	// get rid of all sub meshes
	while (mSubMeshes.GetLength())
	{
		delete mSubMeshes.GetLast();
		mSubMeshes.RemoveLast();
	}

	// get rid of all allocated data
	if (mPositions)
		MEMMGR.Delete(mPositions);

	if (mNormals)
		MEMMGR.Delete(mNormals);

	if (mOrgVerts)
		MEMMGR.Delete(mOrgVerts);

	if (mIndices)
		MEMMGR.Delete(mIndices);

	if (mTangents)
		MEMMGR.Delete(mTangents);


	// re-init members
	mPositions		= NULL;
	mNormals		= NULL;
	mTangents		= NULL;
	mOrgVerts		= NULL;
	mIndices		= NULL;
	mNumIndices		= 0;
	mNumFaces		= 0;
	mNumVertices	= 0;
	mNumOrgVerts	= 0;
}

/*
// adds a new shared vertex attribute layer
void Mesh::AddSharedVertexAttributeLayer(VertexAttribute* vertexData)
{
	mSharedVertexAttributes.Add( vertexData );
}


// removes a given shared vertex attribute layer
void Mesh::RemoveSharedVertexAttributeLayer(const int layerNr)
{
	assert(layerNr < mSharedVertexAttributes.GetLength());
	delete[] mSharedVertexAttributes[layerNr];
	mSharedVertexAttributes.Remove( layerNr );
}


void Mesh::RemoveAllSharedVertexAttributes()
{
	// get rid of the shared vertex attributes
	while (mSharedVertexAttributes.GetLength())
	{
		delete[] mSharedVertexAttributes.GetLast();
		mSharedVertexAttributes.RemoveLast();
	}
}


void Mesh::RemoveAllVertexAttributes()
{
	// get rid of the shared vertex attributes
	while (mVertexAttributes.GetLength())
	{
		delete[] mVertexAttributes.GetLast();
		mVertexAttributes.RemoveLast();
	}
}


// adds a new vertex attribute layer
void Mesh::AddVertexAttributeLayer(VertexAttribute* vertexData)
{
	mVertexAttributes.Add( vertexData );
}


// removes a given vertex attribute layer
void Mesh::RemoveVertexAttributeLayer(const int layerNr)
{
	assert(layerNr < mVertexAttributes.GetLength());
	delete[] mVertexAttributes[layerNr];
	mVertexAttributes.Remove( layerNr );
}
*/

bool Mesh::Intersects(const Ray& ray, Node* node)
{
	Vector3*	positions	= GetPositions();
	int*		indices		= GetIndices();
	Matrix		invMat		= node->GetWorldTM().Inversed();
	Vector3		newOrigin	= ray.GetOrigin() * invMat;
	Vector3		newDest		= ray.GetDest() * invMat;

	// the test ray, in space of the node
	// on this way we do not have to convert the vertices into world space
	Ray testRay(newOrigin, newDest);

	// loop through all triangles
	for (int i=0; i<mNumIndices; i+=3)
	{
		// test the ray with the triangle
		if (testRay.Intersects(positions[indices[i]], 	positions[indices[i+1]], 	positions[indices[i+2]] ))
			return true;
	}

	// there is no intersection
	return false;
}


bool Mesh::Intersects(const Ray& ray, Node* node, Vector3* outIntersect, float* outBaryU, float* outBaryV, int* outStartIndex)
{
	Vector3*	positions		= GetPositions();
	int*		indices			= GetIndices();
	Matrix		invNodeTM		= node->GetWorldTM().Inversed();
	Vector3		newOrigin		= ray.GetOrigin() * invNodeTM;
	Vector3		newDest			= ray.GetDest() * invNodeTM;
	float		closestDist		= FLT_MAX;
	bool		hasIntersected	= false;
	int			closestStartIndex=0;
	Vector3		intersectionPoint;
	Vector3		closestIntersect;
	float		dist, baryU, baryV, closestBaryU=0, closestBaryV=0;

	// the test ray, in space of the node (object space)
	// on this way we do not have to convert the vertices into world space
	Ray testRay(newOrigin, newDest);

	// loop through all triangles
	for (int i=0; i<mNumIndices; i+=3)
	{
		// test the ray with the triangle (in object space)
		if (testRay.Intersects(positions[indices[i]], positions[indices[i+1]], positions[indices[i+2]], &intersectionPoint, &baryU, &baryV))
		{
			// calculate the squared distance between the intersection point and the ray origin
			dist = (intersectionPoint - newOrigin).SquareLength();

			// if it is the closest intersection point until now, record it as closest intersection
			if (dist < closestDist)
			{
				closestDist			= dist;
				closestIntersect	= intersectionPoint;
				hasIntersected		= true;
				closestStartIndex	= i;
				closestBaryU		= baryU;
				closestBaryV		= baryV;
			}
		}
	}

	// store the closest intersection point (in world space)
	if (hasIntersected)
	{
		if (outIntersect != NULL)
			*outIntersect = closestIntersect * node->GetWorldTM();

		if (outStartIndex != NULL)
			*outStartIndex = closestStartIndex;

		if (outBaryU)
			*outBaryU = closestBaryU;

		if (outBaryV)
			*outBaryV = closestBaryV;
	}

	// return the result
	return hasIntersected;
}


// calculate the tangent and binormal for a given triangle
void Mesh::CalcTangentAndBiNormalForFace(const Vector3& posA, const Vector3& posB, const Vector3& posC, 
											const Vector2& uvA,  const Vector2& uvB,  const Vector2& uvC, 
											Vector3* outTangent, Vector3* outBiNormal)
{
	// reset the tangent and binormal
	outTangent->Set(0, 0, 0);

	if (outBiNormal)
		outBiNormal->Set(0, 0, 0);

	Vector3 edge0;
	Vector3 edge1;

	Vector3 s;
	Vector3 t;

	// create an edge out of x, s and t
	edge0.x = posB.x - posA.x;
	edge0.y = uvB.x - uvA.x;
	edge0.z = uvB.y - uvA.y;

	edge1.x = posC.x - posA.x;
	edge1.y = uvC.x - uvA.x;
	edge1.z = uvC.y - uvA.y;


	// calculate s cross t
	Vector3 sxt = edge0.Cross( edge1 );	// the normal
	Vector3 faceNormal(sxt);


	float a = sxt.x;
	float b = sxt.y;
	float c = sxt.z;

	float ds_dx = 0.0f;
	if (Math::Abs(a) > MCore::Math::EPSILON)
	{
		ds_dx = -b / a;
	}

	float dt_dx = 0.0f;
	if (Math::Abs(a) > MCore::Math::EPSILON)
	{
		dt_dx = -c / a;
	}


	// create an edge out of y, s and t
	edge0.x = posB.y - posA.y;
	edge1.x = posC.y - posA.y;

	sxt = edge0.Cross( edge1 );

    a = sxt.x;
    b = sxt.y;
    c = sxt.z;

    float ds_dy = 0.0f;
	if (Math::Abs(a) > MCore::Math::EPSILON)
    {
        ds_dy = -b / a;
    }

    float dt_dy = 0.0f;
	if (Math::Abs(a) > MCore::Math::EPSILON)
    {
        dt_dy = -c / a;
    }


	// create an edge out of z, s and t
	edge0.x = posB.z - posA.z;
	edge1.x = posC.z - posA.z;

	sxt = edge0.Cross( edge1 );

	a = sxt.x;
	b = sxt.y;
	c = sxt.z;

	float ds_dz = 0.0f;
	if (Math::Abs(a) > MCore::Math::EPSILON)
	{
		ds_dz = -b / a;
	}

	float dt_dz = 0.0f;
	if (Math::Abs(a) > MCore::Math::EPSILON)
	{
		dt_dz = -c / a;
	}


	// calculate the tangent
	*outTangent = Vector3( ds_dx, ds_dy, ds_dz );
	float length = outTangent->Length();
	if (length > MCore::Math::EPSILON)
	{
		float ooLength = 1.0f / length;
		*outTangent *= ooLength;
	}
	else
		outTangent->Set(0, 0, 1);


	// calculate the binormal
	if (outBiNormal)
	{
		*outBiNormal = Vector3( dt_dx, dt_dy, dt_dz );
		length = outBiNormal->Length();

		if (length > MCore::Math::EPSILON)
		{
			float ooLength = 1.0f / length;
			*outBiNormal *= ooLength;
		}
		else
			*outBiNormal = outTangent->Cross( faceNormal );
	}
}


// calculate the S and T vectors
void Mesh::CalcTangents(const int uvSet)
{
	// find the uv layer, if it exists, otherwise return
	UVVertexAttributeLayer* realUVLayer = (UVVertexAttributeLayer*)FindVertexAttributeLayer( UVVertexAttributeLayer::TYPE_ID, uvSet );
	if (realUVLayer == NULL)
		return;

	// make sure we have the data allocated to store the tangents and binormals
	if (mTangents)
		MEMMGR.Delete(mTangents);

	// allocate the tangent data
	mTangents = (Vector4*)MEMMGR.Allocate(sizeof(Vector4) * (mNumVertices<<1), MEMCATEGORY_GEOMETRY_MESHES, Mesh::MEMORYBLOCK_ID);


	Array<int>		faces;		// the indices to the faces used by the given vertex
	Vector3*		positions	= GetOrgPositions();	// the positions of the vertices
	Vector3*		normals		= GetNormals();		// the vertex normals
	Vector4*		tangents	= GetTangents();	// the tangents to set
	Vector4*		orgTangents	= GetOrgTangents();	// the tangents to set
	int*			indices		= GetIndices();		// the indices (face data)
	int*			orgVerts	= GetOrgVerts();	// the original vertex numbers
	Vector2*		uvData		= realUVLayer->GetUVs();
	int				orgVertex, indexA, indexB, indexC, startIndex;
	int				i, f;

	// calculate for every vertex the tangent and binormal
	for (i=0; i<mNumVertices; i++)
	{
		// we need to find out for every vertex, which faces are using this vertex
		// then we calculate for each of these faces the tangent and binormal
		// sum them up, and take the average.
		// this works the same like calculating (pseudo)vertex normals, by taking the
		// average of all face normals.
		// if we do not do this, we get some sort of flat shaded bumpmapping, which is not cool :)
		// as you can imagine this all is not very fast to do since we have no connectivity information
		// but fortunately it's only done once.

		// get the original vertex number for this vertex
		orgVertex = orgVerts[i];

		// traverse all faces
		for (f=0; f<mNumIndices; f+=3)
		{
			// get the face indices
			indexA = indices[f];
			indexB = indices[f+1];
			indexC = indices[f+2];

			// if it uses the same original vertex as the one we are checking, add the face
			//if (orgVerts[indexA] == orgVertex)
			//if (indexA == orgVertex)
			if (indexA == i)
			{
				faces.Add(f);
				continue;
			}

			// if it uses it as second vertex
			//if (orgVerts[indexB] == orgVertex)
			//if (indexB == orgVertex)
			if (indexB == i)
			{
				faces.Add(f);
				continue;
			}

			// if it uses it as third vertex
			//if (orgVerts[indexC] == orgVertex)
			//if (indexC == orgVertex)
			if (indexC == i)
			{
				faces.Add(f);
				continue;
			}
		}

		// now that we have gathered all faces used by the vertex, we can calculate the average tangent
		Vector3 curTangent;
		Vector3 curBiNormal;
		Vector3 totalBiNormal(0,0,0);
		Vector3 totalTangent(0,0,0);
		const int numFaces = faces.GetLength();
		for (f=0; f<numFaces; f++)
		{
			// get the start index of the face
			startIndex = faces[f];

			const int indexA = indices[startIndex];
			const int indexB = indices[startIndex+1];
			const int indexC = indices[startIndex+2];

			// calculate the tangent and binormal for the face
			CalcTangentAndBiNormalForFace(  positions[indexA], positions[indexB], positions[indexC],
											uvData[indexA], uvData[indexB], uvData[indexC],
											&curTangent, &curBiNormal);

			// sum them up
			totalBiNormal += curBiNormal;
			totalTangent += curTangent;
		}

		// average the summed tangent and binormal
		if (numFaces > 0)
		{
			float mul = 1.0f / (float)numFaces;
			totalTangent *= mul;
			totalBiNormal *= mul;
		}

		//-------------------------
		Vector3 normal = normals[i].Normalize();
		Vector3 tangent = totalTangent;
		Vector3 binormal = totalBiNormal;

		tangent.Normalize();
		binormal.Normalize();
		
		Matrix tangentToObjecSpace(true);
		tangentToObjecSpace.SetRight( tangent );
		tangentToObjecSpace.SetUp( binormal );
		tangentToObjecSpace.SetForward( normal );

		//F' = F - (N dot F) N
		//G' = G - (N dot G) N - (F' dot G) F'
		tangent = tangent - (normal.Dot(tangent)) * normal;
		binormal = binormal - (normal.Dot(binormal)) * normal - (tangent.Dot(binormal)) * tangent;

		Matrix mat(true);
		mat.SetRight( tangent.Normalize() );
		mat.SetUp( binormal.Normalize() );
		mat.SetForward( normal );

		float det = tangentToObjecSpace.CalcDeterminant();
		Vector4 finalTangent(tangent.x, tangent.y, tangent.z, 0);
		finalTangent.w = (det < 0) ? -1.0f : 1.0f;;

		orgTangents[i] = finalTangent;
		tangents[i] = finalTangent;
		//------------------------------

		// get rid of the elements in the faces array
		faces.Clear();
	}
}


// creates an array of pointers to bones used by this face
void Mesh::GatherBonesForFace(const int startIndexOfFace, Array<Node*>& outBones)
{
	// get rid of existing data
	outBones.Clear();

	// try to locate the skinning attribute information
	SkinningInfoVertexAttributeLayer* skinningLayer = (SkinningInfoVertexAttributeLayer*)FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );

	// if there is no skinning info, there are no bones attached to the vertices, so we can quit
	if (skinningLayer == NULL)
		return;

	// get the index data and original vertex numbers
	int* indices = GetIndices();
	int* orgVerts= GetOrgVerts();

	// get the skinning info for all three vertices
	for (int i=0; i<3; i++)
	{
		// get the original vertex number
		// remember that a cube can have 24 vertices to render (stored in this mesh), while it has only 8 original vertices
		int originalVertex = orgVerts[ indices[startIndexOfFace+i] ];

		// traverse all influences for this vertex
		const int numInfluences = skinningLayer->GetNumInfluences(originalVertex);
		for (int n=0; n<numInfluences; n++)
		{
			// get the bone of the influence
			Node* bone = skinningLayer->GetInfluence(originalVertex, n).GetBone();

			// if it isn't yet in the output array with bones, add it
			if (outBones.Find(bone) == -1)
				outBones.Add( bone );
		}
	}
}


// returns the maximum number of weights/influences for this face
int Mesh::CalcMaxNumInfluencesForFace(const int startIndexOfFace)
{
	// try to locate the skinning attribute information
	SkinningInfoVertexAttributeLayer* skinningLayer = (SkinningInfoVertexAttributeLayer*)FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );

	// if there is no skinning info, there are no bones attached to the vertices, so we can quit
	if (skinningLayer == NULL)
		return 0;

	// get the index data and original vertex numbers
	int* indices = GetIndices();
	int* orgVerts= GetOrgVerts();

	// get the skinning info for all three vertices
	int maxInfluences = 0;
	for (int i=0; i<3; i++)
	{
		// get the original vertex number
		// remember that a cube can have 24 vertices to render (stored in this mesh), while it has only 8 original vertices
		int originalVertex = orgVerts[ indices[startIndexOfFace+i] ];

		// check if the number of influences is higher as the highest recorded value
		int numInfluences = skinningLayer->GetNumInfluences(originalVertex);
		if (maxInfluences < numInfluences)
			maxInfluences = numInfluences;
	}

	// return the maximum number of influences for this triangle
	return maxInfluences;
}


// returns the maximum number of weights/influences for this mesh
int Mesh::CalcMaxNumInfluences()
{
	int maxInfluences = 0;

	// try to locate the skinning attribute information
	SkinningInfoVertexAttributeLayer* skinningLayer = (SkinningInfoVertexAttributeLayer*)FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );

	// if there is no skinning info, there are no bones attached to the vertices, so we can quit
	if (skinningLayer == NULL)
		return 0;

	const int numOrgVerts = GetNumOrgVertices();
	for (int i=0; i<numOrgVerts; i++)
	{
		// set the number of max influences
		maxInfluences = Max(maxInfluences, skinningLayer->GetNumInfluences(i));
	}

	// return the maximum number of influences
	return maxInfluences;
}


// returns the maximum number of weights/influences for this mesh plus some extra information
int Mesh::CalcMaxNumInfluences(Array<int>& vertexCounts)
{
	int maxInfluences = 0;
	vertexCounts.Clear();

	// resize the vertex count array, min size must be 1, for nonskinned objects
	vertexCounts.Resize( CalcMaxNumInfluences() + 1 );

	// reset values
	for (int j=0; j<vertexCounts.GetLength(); j++)
		vertexCounts[j] = 0;


	// try to locate the skinning attribute information
	SkinningInfoVertexAttributeLayer* skinningLayer = (SkinningInfoVertexAttributeLayer*)FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );

	// if there is no skinning info, there are no bones attached to the vertices, so we can quit
	if (skinningLayer == NULL)
	{
		vertexCounts[0] = GetNumVertices();
		return maxInfluences;
	}

	// get the original vertex numbers
	int* orgVerts = GetOrgVerts();

	// get the vertex counts for the influences
	const int numVerts = GetNumVertices();
	for (int i=0; i<numVerts; i++)
	{
		int orgVertex = orgVerts[i];

		// increase the number of vertices for the given influence value
		vertexCounts[ skinningLayer->GetNumInfluences(orgVertex) ]++;

		// set the number of max influences
		maxInfluences = Max(maxInfluences, skinningLayer->GetNumInfluences(orgVertex));
	}

	// return the maximum number of influences
	return maxInfluences;
}


// remove a given submesh
void Mesh::RemoveSubMesh(const int nr, const bool delFromMem)
{
	SubMesh* subMesh = mSubMeshes[nr];
	mSubMeshes.Remove(nr);
	if (delFromMem)
		delete subMesh;
}


// insert a given submesh
void Mesh::InsertSubMesh(const int insertIndex, SubMesh* subMesh)
{
	mSubMeshes.Insert(insertIndex, subMesh);
}


// get the number of UV layers
int Mesh::CalcNumUVLayers() const
{
	int numUVLayers = 0;

	// check the types of all vertex attribute layers
	const int numAttributes = mVertexAttributes.GetLength();
	for (int i=0; i<numAttributes; i++)
	{
		if (mVertexAttributes[i]->GetType() == UVVertexAttributeLayer::TYPE_ID)
			numUVLayers++;
	}

	return numUVLayers;
}

//---------------------------------------------------------------

VertexAttributeLayer* Mesh::GetSharedVertexAttributeLayer(const int layerNr)
{
	assert(layerNr < mSharedVertexAttributes.GetLength());
	return mSharedVertexAttributes[layerNr];
}


void Mesh::AddSharedVertexAttributeLayer(VertexAttributeLayer* layer)
{
	assert(mSharedVertexAttributes.Contains(layer) == false);
	mSharedVertexAttributes.Add( layer );
}


int Mesh::GetNumSharedVertexAttributeLayers() const
{
	return mSharedVertexAttributes.GetLength();
}


int Mesh::FindSharedVertexAttributeLayerNumber(const int layerTypeID, const int occurrence) const
{
	int layerCounter = 0;

	// check all vertex attributes of our first vertex, and find where the specific attribute is
	const int numLayers = mSharedVertexAttributes.GetLength();
	for (int i=0; i<numLayers; i++)
	{
		VertexAttributeLayer* layer = mSharedVertexAttributes[i];
		if (layer->GetType() == layerTypeID)
		{
			if (occurrence == layerCounter)
				return i;

			layerCounter++;
		}
	}

	// not found
	return -1;
}


// find the vertex attribute layer and return a pointer
VertexAttributeLayer* Mesh::FindSharedVertexAttributeLayer(const int layerTypeID, const int occurence) const
{
	int layerNr = FindSharedVertexAttributeLayerNumber(layerTypeID, occurence);
	if (layerNr == -1) 
		return NULL;

	return mSharedVertexAttributes[layerNr];
}



// delete all shared attribute layers
void Mesh::RemoveAllSharedVertexAttributeLayers()
{
	while (mSharedVertexAttributes.GetLength())
	{
		delete mSharedVertexAttributes.GetLast();
		mSharedVertexAttributes.RemoveLast();
	}
}


// remove a layer by its index
void Mesh::RemoveSharedVertexAttributeLayer(const int layerNr)
{
	assert(layerNr < mSharedVertexAttributes.GetLength());
	delete mSharedVertexAttributes[layerNr];
	mSharedVertexAttributes.Remove(layerNr);
}


int Mesh::GetNumVertexAttributeLayers() const
{
	return mVertexAttributes.GetLength();
}


VertexAttributeLayer* Mesh::GetVertexAttributeLayer(const int layerNr)
{
	assert(layerNr < mVertexAttributes.GetLength());
	return mVertexAttributes[layerNr];
}


void Mesh::AddVertexAttributeLayer(VertexAttributeLayer* layer)
{
	assert( mVertexAttributes.Contains(layer) == false );
	mVertexAttributes.Add( layer );
}


// find the layer number
int Mesh::FindVertexAttributeLayerNumber(const int layerTypeID, const int occurrence) const
{
	int layerCounter = 0;

	// check all vertex attributes of our first vertex, and find where the specific attribute is
	const int numLayers = mVertexAttributes.GetLength();
	for (int i=0; i<numLayers; i++)
	{
		VertexAttributeLayer* layer = mVertexAttributes[i];
		if (layer->GetType() == layerTypeID)
		{
			if (occurrence == layerCounter)
				return i;

			layerCounter++;
		}
	}

	// not found
	return -1;
}


// find the vertex attribute layer and return a pointer
VertexAttributeLayer* Mesh::FindVertexAttributeLayer(const int layerTypeID, const int occurence) const
{
	int layerNr = FindVertexAttributeLayerNumber(layerTypeID, occurence);
	if (layerNr == -1) 
		return NULL;

	return mVertexAttributes[layerNr];
}


void Mesh::RemoveAllVertexAttributeLayers()
{
	while (mVertexAttributes.GetLength())
	{
		delete mVertexAttributes.GetLast();
		mVertexAttributes.RemoveLast();
	}
}


void Mesh::RemoveVertexAttributeLayer(const int layerNr)
{
	assert(layerNr < mVertexAttributes.GetLength());
	delete mVertexAttributes[layerNr];
	mVertexAttributes.Remove(layerNr);
}


// relink the layers to a new node mesh
void Mesh::ReLinkAllVertexAttributeLayers(Mesh* newMesh, Node* newNode, Actor* newActor)
{
	int i;

	// relink the non-shared vertex attribute layers
	int numLayers = mVertexAttributes.GetLength();
	for (i=0; i<numLayers; i++)
		mVertexAttributes[i]->ReLinkToNode(newMesh, newNode, newActor);

	// relink the shared vertex attribute layers
	numLayers = mSharedVertexAttributes.GetLength();
	for (i=0; i<numLayers; i++)
		mSharedVertexAttributes[i]->ReLinkToNode(newMesh, newNode, newActor);
}


} // namespace EMotionFX