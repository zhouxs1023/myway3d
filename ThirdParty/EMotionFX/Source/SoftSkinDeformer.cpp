/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include EMotion FX related headers
#include "SoftSkinDeformer.h"
#include "Mesh.h"
#include "Node.h"
#include "SubMesh.h"
#include "Actor.h"
#include "SkinningInfoVertexAttributeLayer.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
SoftSkinDeformer::SoftSkinDeformer(Mesh* mesh) : MeshDeformer(mesh)
{
	ReInitialize();
}


// destructor
SoftSkinDeformer::~SoftSkinDeformer()
{
	mBones.Clear();
}


// get the type id
int SoftSkinDeformer::GetType() const
{
	return TYPE_ID;
}


// get the subtype id
int SoftSkinDeformer::GetSubType() const
{
	return SUBTYPE_ID;
}


// clone this class
MeshDeformer* SoftSkinDeformer::Clone(Mesh *mesh, Actor* actor)
{
	// create the new cloned deformer
	SoftSkinDeformer* result = new SoftSkinDeformer( mesh );

	// copy the bone info (for precalc/optimization reasons)
	result->mBones = mBones;

	// relink the nodes to the nodes in the new actor
	const int numBones = mBones.GetLength();
	for (int i=0; i<numBones; i++)
		result->mBones[i].mNode = actor->GetNodeByID( mBones[i].mNode->GetID() );
	
	// return the result
	return result;
}


// the main method where all calculations are done
void SoftSkinDeformer::Update(Actor* actor, Node* node, const double timeDelta)
{
	DECLARE_FUNCTION(Update)

	// some vars we need
	Matrix			invNodeTM	= node->GetWorldTM().Inversed();
	Vector3			newPos, newNormal, newTangent;
	Vector3			vtxPos, normal, tangent;
	Vector3*		positions	= mMesh->GetPositions();
	Vector3*		normals		= mMesh->GetNormals();
	Vector4*		tangents	= mMesh->GetTangents();
	int*			orgVerts	= mMesh->GetOrgVerts();
	unsigned int	i;
	
	// precalc the skinning matrices
	const unsigned int numBones = mBones.GetLength();
	for (i=0; i<numBones; i++)
		mBones[i].mMatrix = mBones[i].mNode->GetInvBoneTM() * mBones[i].mNode->GetWorldTM() * invNodeTM;

	// find the skinning layer
	SkinningInfoVertexAttributeLayer* layer = (SkinningInfoVertexAttributeLayer*)mMesh->FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );
	assert( layer != NULL );

	// if there are tangents and binormals to skin
	if (mMesh->GetTangents() != NULL)
	{
		const unsigned int numVertices = mMesh->GetNumVertices();
		unsigned int v = 0;
		int orgVertex;

		for (v=0; v<numVertices; v++)
		{
			// get the original vertex number
			orgVertex = *(orgVerts++);

			// reset the skinned position
			newPos.Set		(0.0f, 0.0f, 0.0f);
			newNormal.Set	(0.0f, 0.0f, 0.0f);
			newTangent.Set	(0.0f, 0.0f, 0.0f);

			float tangentW = tangents[v].w;

			vtxPos.Set	(positions->x,	positions->y,	positions->z);
			normal.Set	(normals->x,	normals->y,		normals->z);
			tangent.Set	(tangents->x,	tangents->y,	tangents->z);

			// process all the influences
			SkinInfluence*	influence;
			BoneInfo*		boneInfo;
			float weight;
			const unsigned int numInfluences = layer->GetNumInfluences(orgVertex);
			for (i=0; i<numInfluences; i++)
			{
				// get the influence
				influence	= &layer->GetInfluence(orgVertex, i);
				boneInfo	= &mBones[ influence->GetBoneNr() ];
				weight		= influence->GetWeight();

				// perform the skinning
				newPos		+= (vtxPos * boneInfo->mMatrix) * weight;
				newNormal	+= boneInfo->mMatrix.Mul3x3(normal) * weight;
				newTangent	+= (tangent * boneInfo->mMatrix) * weight;
			}

			// output the skinned values
			positions->Set	(newPos.x,		newPos.y,		newPos.z);					positions++;
			normals->Set	(newNormal.x,	newNormal.y,	newNormal.z);				normals++;
			tangents->Set	(newTangent.x,	newTangent.y,	newTangent.z, tangentW);	tangents++;
		}
	}
	else	// there are no tangents and binormals to skin
	{
		const unsigned int numVertices = mMesh->GetNumVertices();
		unsigned int v = 0;
		int orgVertex;

		for (v=0; v<numVertices; v++)
		{
			// get the original vertex number
			orgVertex = *(orgVerts++);

			// reset the skinned position
			newPos.Set	  (0.0f, 0.0f, 0.0f);
			newNormal.Set (0.0f, 0.0f, 0.0f);

			vtxPos.Set	(positions->x,	positions->y,	positions->z);
			normal.Set	(normals->x,	normals->y,		normals->z);

			// process all the influences
			SkinInfluence*	influence;
			BoneInfo*		boneInfo;
			float weight;
			const unsigned int numInfluences = layer->GetNumInfluences(orgVertex);
			for (i=0; i<numInfluences; i++)
			{
				// get the influence
				influence	= &layer->GetInfluence(orgVertex, i);
				boneInfo	= &mBones[ influence->GetBoneNr() ];
				weight		= influence->GetWeight();

				// perform the skinning
				newPos		+= (vtxPos * boneInfo->mMatrix) * weight;
				newNormal	+= boneInfo->mMatrix.Mul3x3(normal) * weight;
			}

			// output the skinned values
			positions->Set	(newPos.x,		newPos.y,		newPos.z);		positions++;
			normals->Set	(newNormal.x,	newNormal.y,	newNormal.z);	normals++;
		}
	}
}


// reinitialize the mesh deformer
void SoftSkinDeformer::ReInitialize()
{
	// clear the bone information array
	mBones.Clear();

	// the the applied mesh is invalid return
	if (!mMesh)
		return;

	// the array with used bones
	Array<Node*> usedBones;

	// get the attribute number	
	SkinningInfoVertexAttributeLayer* skinningLayer = (SkinningInfoVertexAttributeLayer*)mMesh->FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );
	assert(skinningLayer);

	// find out what bones this mesh uses
	const int numOrgVerts = mMesh->GetNumOrgVertices();
	for (int i=0; i<numOrgVerts; i++)
	{
		// now we have located the skinning information for this vertex, we can see if our bones array
		// already contains the bone it uses by traversing all influences for this vertex, and checking
		// if the bone of that influence already is in the array with used bones
		const int numInfluences = skinningLayer->GetNumInfluences(i);
		for (int a=0; a<numInfluences; a++)
		{
			SkinInfluence& influence = skinningLayer->GetInfluence(i, a);

			// get the bone index in the array
			int boneIndex = usedBones.Find( influence.GetBone() );

			// if the bone is not found in our array
			if (boneIndex == -1)
			{
				// add it to the back of the array
				usedBones.Add( influence.GetBone() );

				// set the index number for the bone
				boneIndex = usedBones.GetMaxIndex();

				// add the bone to the array of bones in this deformer
				BoneInfo boneInfo;
				boneInfo.mNode = influence.GetBone();
				boneInfo.mMatrix.Identity();
				mBones.Add( boneInfo );
			}

			// set the bone number in the influence
			influence.SetBoneNr( boneIndex );
		}
	}

	// get rid of all items in the used bones array
	//LOG("Number of bones used = %d", usedBones.GetLength());
	usedBones.Clear();
}

} // namespace EMotionFX