/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#if COMPILER == COMPILER_MSVC


// include EMotion FX related headers
#include "SoftSkinDeformerSSE.h"
#include "Mesh.h"
#include "Node.h"
#include "SubMesh.h"
#include "Actor.h"
#include "SkinningInfoVertexAttributeLayer.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{


// clone this class
MeshDeformer* SoftSkinDeformerSSE::Clone(Mesh* mesh, Actor* actor)
{
	// create the new cloned deformer
	SoftSkinDeformerSSE* result = new SoftSkinDeformerSSE( mesh );

	// copy the bone info (for precalc/optimization reasons)
	result->mBones = mBones;

	// relink the nodes to the nodes in the new actor
	const int numBones = mBones.GetLength();
	for (int i=0; i<numBones; i++)
		result->mBones[i].mNode = actor->GetNodeByID( mBones[i].mNode->GetID() );
	
	// return the result
	return result;
}


// get the subtype
int SoftSkinDeformerSSE::GetSubType() const
{
	return SoftSkinDeformerSSE::SUBTYPE_ID;
}


// the main method where all calculations are done
void SoftSkinDeformerSSE::Update(Actor* actor, Node* node, const double timeDelta)
{
	DECLARE_FUNCTION(Update)

	// some vars we need
	Matrix			invNodeTM	= node->GetWorldTM().Inversed();
	Vector4			newPos, newNormal, newTangent;
	Vector4			vtxPos, normal, tangent;
	Vector3*		positions	= mMesh->GetPositions();
	Vector3*		normals		= mMesh->GetNormals();
	Vector4*		tangents	= mMesh->GetTangents();
	int*			orgVerts	= mMesh->GetOrgVerts();
	unsigned int	i;

	// precalc the skinning matrices
	const unsigned int numBones = mBones.GetLength();
	for (i=0; i<numBones; i++)
		mBones[i].mMatrix = mBones[i].mNode->GetInvBoneTM() * mBones[i].mNode->GetWorldTM() * invNodeTM;

	// skin all vertices in the mesh
	FVector4 packedWeights;
	float*	 matrixData;

	// find the skinning layer
	SkinningInfoVertexAttributeLayer* layer = (SkinningInfoVertexAttributeLayer*)mMesh->FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );
	assert( layer != NULL );

	// if there are also tangents to be skinned
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
			memset(&newPos,		0, 12);
			memset(&newNormal,	0, 12);
			memset(&newTangent,	0, 12);

			// local copies, which seem faster
			FVector4	inputVec;
			FVector4	inputNormal;
			FVector4	inputTangent;

			__asm
			{
				lea edi, inputVec
				mov esi, positions
				mov eax, dword ptr [esi]
				mov edx, dword ptr [esi + 4]
				mov ecx, dword ptr [esi + 8]
				fld1
				fst dword ptr [edi + 12]
				mov [edi], eax
				mov [edi + 4], edx
				mov [edi + 8], ecx

				lea edi, inputNormal
				mov esi, normals
				mov eax, dword ptr [esi]
				mov edx, dword ptr [esi + 4]
				mov ecx, dword ptr [esi + 8]
				fst dword ptr [edi + 12]
				mov [edi], eax
				mov [edi + 4], edx
				mov [edi + 8], ecx

				lea edi, inputTangent
				mov esi, tangents
				mov eax, dword ptr [esi]
				mov edx, dword ptr [esi + 4]
				mov ecx, dword ptr [esi + 8]
				fstp dword ptr [edi + 12]	
				mov [edi], eax
				mov [edi + 4], edx
				mov [edi + 8], ecx
			}


			// process all the influences
			SkinInfluence*	influence;
			BoneInfo*		boneInfo;
			const unsigned int numInfluences = layer->GetNumInfluences(orgVertex);
			for (i=0; i<numInfluences; i++)
			{
				// get the influence
				influence	= &layer->GetInfluence(orgVertex, i);
				boneInfo	= &mBones[ influence->GetBoneNr() ];
				matrixData	= boneInfo->mMatrix.m16;

				__asm 
				{
					// skin the position
					mov		esi, matrixData
					movups	xmm0, inputVec
					movups	xmm4, [esi]
					movups	xmm5, [esi+16]
					movups	xmm6, [esi+32]
					movups	xmm7, [esi+48]

					xorps	xmm2, xmm2

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x00
					mulps	xmm1, xmm4
					addps	xmm2, xmm1

					// initialize the weight vector
					mov		esi, influence
					mov		eax, [esi + 4]
					lea		edi, packedWeights

					movups	xmm1, xmm0
					mov		[edi], eax
					shufps	xmm1, xmm1, 0x55
					mov		[edi + 4], eax
					mulps	xmm1, xmm5
					mov		[edi + 8], eax
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xaa
					mulps	xmm1, xmm6
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xff
					mulps	xmm1, xmm7
					addps	xmm2, xmm1

					movups	xmm3, packedWeights
					mulps	xmm2, xmm3
					
					movups	xmm1, newPos
					addps	xmm1, xmm2
					movups	newPos, xmm1


					// skin the normal
					movups	xmm0, inputNormal
					xorps	xmm2, xmm2

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x00
					mulps	xmm1, xmm4
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x55
					mulps	xmm1, xmm5
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xaa
					mulps	xmm1, xmm6
					addps	xmm2, xmm1

					mulps	xmm2, xmm3
					
					movups	xmm1, newNormal
					addps	xmm1, xmm2
					movups	newNormal, xmm1


					// skin the tangent vector
					xorps	xmm2, xmm2

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x00
					mulps	xmm1, xmm4
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x55
					mulps	xmm1, xmm5
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xaa
					mulps	xmm1, xmm6
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xff
					mulps	xmm1, xmm7
					addps	xmm2, xmm1

					mulps	xmm2, xmm3
					
					movups	xmm1, newTangent
					addps	xmm1, xmm2
					movups	newTangent, xmm1
				}
			}


			// output the skinned values
			__asm
			{
				mov		edi, positions
				lea		esi, newPos
				mov		eax, [esi]
				mov		edx, [esi + 4]
				mov		ecx, [esi + 8]
				mov		[edi], eax
				mov		[edi + 4], edx
				mov		[edi + 8], ecx

				mov		edi, normals
				lea		esi, newNormal
				mov		eax, [esi]
				mov		edx, [esi + 4]
				mov		ecx, [esi + 8]
				mov		[edi], eax
				mov		[edi + 4], edx
				mov		[edi + 8], ecx

				mov		edi, tangents
				lea		esi, newTangent
				mov		eax, [esi]
				mov		edx, [esi + 4]
				mov		ecx, [esi + 8]
				mov		[edi], eax
				mov		[edi + 4], edx
				mov		[edi + 8], ecx

				add		positions, 12
				add		normals, 12
				add		tangents, 12
			}
		}
	}
	else	// there are no tangents and binormals
	{
		const unsigned int numVertices = mMesh->GetNumVertices();
		unsigned int v = 0;
		int orgVertex;
		for (v=0; v<numVertices; v++)
		{
			// get the original vertex number
			orgVertex = *(orgVerts++);

			// reset the skinned position
			memset(&newPos,		0, 12);
			memset(&newNormal,	0, 12);

			// local copies, which seem faster
			FVector4 inputVec;
			FVector4 inputNormal;

			__asm
			{
				lea edi, inputVec
				mov esi, positions
				mov eax, dword ptr [esi]
				mov edx, dword ptr [esi + 4]
				mov ecx, dword ptr [esi + 8]
				fld1
				fst dword ptr [edi + 12]
				mov [edi], eax
				mov [edi + 4], edx
				mov [edi + 8], ecx

				lea edi, inputNormal
				mov esi, normals
				mov eax, dword ptr [esi]
				mov edx, dword ptr [esi + 4]
				mov ecx, dword ptr [esi + 8]
				fstp dword ptr [edi + 12]
				mov [edi], eax
				mov [edi + 4], edx
				mov [edi + 8], ecx
			}


			// process all the influences
			SkinInfluence*	influence;
			BoneInfo*		boneInfo;
			const unsigned int numInfluences = layer->GetNumInfluences(orgVertex);
			for (i=0; i<numInfluences; i++)
			{
				// get the influence
				influence	= &layer->GetInfluence(orgVertex, i);
				boneInfo	= &mBones[ influence->GetBoneNr() ];
				matrixData	= boneInfo->mMatrix.m16;

				__asm 
				{
					// skin the position
					mov		esi, matrixData
					movups	xmm0, inputVec
					movups	xmm4, [esi]
					movups	xmm5, [esi+16]
					movups	xmm6, [esi+32]
					movups	xmm7, [esi+48]

					xorps	xmm2, xmm2

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x00
					mulps	xmm1, xmm4
					addps	xmm2, xmm1

					// initialize the weight vector
					mov		esi, influence
					mov		eax, [esi + 4]
					lea		edi, packedWeights

					movups	xmm1, xmm0
					mov		[edi], eax
					shufps	xmm1, xmm1, 0x55
					mov		[edi + 4], eax
					mulps	xmm1, xmm5
					mov		[edi + 8], eax
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xaa
					mulps	xmm1, xmm6
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xff
					mulps	xmm1, xmm7
					addps	xmm2, xmm1

					movups	xmm3, packedWeights
					mulps	xmm2, xmm3
					
					movups	xmm1, newPos
					addps	xmm1, xmm2
					movups	newPos, xmm1


					// skin the normal
					movups	xmm0, inputNormal
					xorps	xmm2, xmm2

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x00
					mulps	xmm1, xmm4
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0x55
					mulps	xmm1, xmm5
					addps	xmm2, xmm1

					movups	xmm1, xmm0
					shufps	xmm1, xmm1, 0xaa
					mulps	xmm1, xmm6
					addps	xmm2, xmm1

					mulps	xmm2, xmm3
					
					movups	xmm1, newNormal
					addps	xmm1, xmm2
					movups	newNormal, xmm1
				}
			}


			// output the skinned values
			__asm
			{
				mov		edi, positions
				lea		esi, newPos
				mov		eax, [esi]
				mov		edx, [esi + 4]
				mov		ecx, [esi + 8]
				mov		[edi], eax
				mov		[edi + 4], edx
				mov		[edi + 8], ecx

				mov		edi, normals
				lea		esi, newNormal
				mov		eax, [esi]
				mov		edx, [esi + 4]
				mov		ecx, [esi + 8]
				mov		[edi], eax
				mov		[edi + 4], edx
				mov		[edi + 8], ecx

				add		positions, 12
				add		normals, 12
			}
		}
	}
}

} // namespace EMotionFX


#endif	// #if COMPILER == COMPILER_MSVC
