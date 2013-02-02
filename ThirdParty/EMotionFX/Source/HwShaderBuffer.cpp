/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "HwShaderBuffer.h"
#include "Mesh.h"
#include "Node.h"
#include "SkinningInfoVertexAttributeLayer.h"

namespace EMotionFX
{


// constructor
HwShaderBuffer::HwShaderBuffer(const int materialNumber, const int numInfluences)
{
	mNumInfluences		= numInfluences;
	mMaterialNumber		= materialNumber;
	mStartIndex			= 0;
}



// destructor
HwShaderBuffer::~HwShaderBuffer()
{
	mVertices.Clear();
	mIndices.Clear();
	mPrimitives.Clear();
	mBones.Clear();
}



// returns true if the buffer can handle a face with the specified properties, otherwise false is returned
bool HwShaderBuffer::CanHandleFace(const int numInfluences, const MCore::Array<Node*>& faceBones, const int materialNumber, const int maxBonesPerBuffer, const bool allowDifferentWeights)
{
	// if the face hasn't got the same material and if the index buffer isn't full yet
	if (mMaterialNumber != materialNumber)
		return false;

	// check the weights / influences
	if (!allowDifferentWeights && numInfluences != mNumInfluences)
		return false;

	// non-skinned objects must be in a separate buffer
	if ((mNumInfluences == 0) && (numInfluences != 0))
		return false;

	// keep non-skinned objects out of this buffer
	if ((numInfluences == 0) && (mNumInfluences != 0) && allowDifferentWeights)
		return false;

	// calculate how many new bones would be added to this buffer
	int numNewBones = 0;
	const int numFaceBones = faceBones.GetLength();
	for (int i=0; i<numFaceBones; i++)
	{
		if (mBones.Find( faceBones[i] ) == -1)
			numNewBones++;
	}

	// check if that would reach the maximum number of bones or not, if so, this buffer cannot handle this face
	if (mBones.GetLength() + numNewBones > maxBonesPerBuffer)
		return false;

	// yeah, this buffer can handle it :)
	return true;
}


// add a given mesh face to the buffer
void HwShaderBuffer::AddFace(Mesh* mesh, const int startIndex)
{
	// get some pointers
	int* indices  = mesh->GetIndices();
	int* orgVerts = mesh->GetOrgVerts();

	// try to locate the skinning attribute information
	SkinningInfoVertexAttributeLayer* skinningLayer = (SkinningInfoVertexAttributeLayer*)mesh->FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );

	if (mIndices.GetLength() % 100 == 0)
		mIndices.Reserve ( mIndices.GetLength()  + 100 );

	if (mVertices.GetLength() % 100 == 0)
		mVertices.Reserve( mVertices.GetLength() + 100 );

	if (mBones.GetLength() % 20 == 0)
		mBones.Reserve( mBones.GetLength() + 20 );

	// for all vertices of the face
	for (int i=0; i<3; ++i)
	{
		int vertexNumber = indices[startIndex + i];

		// find out if this vertex is already added
		bool vertexFound = false;
		const int numVertices = mVertices.GetLength();
		for (int v=mStartVertex; v<numVertices && !vertexFound; v++)
		{
			if (mVertices[v].mVertexNr == vertexNumber)
			{
				mIndices.Add( v );
				vertexFound = true;
			}
		}

		// skip to the next vertex :)
		if (vertexFound) continue;	

		// create the vertex
		HwShaderVertex vtx(mesh, vertexNumber);

		// if there is skinning info
		if (skinningLayer != NULL)
		{
			// get the original vertex number
			int orgVertex = orgVerts[ vertexNumber ];

			// add the influences
			const int numInfluences = skinningLayer->GetNumInfluences(orgVertex);
			vtx.mInfluences.Reserve( mNumInfluences );
			for (int s=0; s<numInfluences; s++)
			{
				// get the weight and bone
				float weight = skinningLayer->GetInfluence(orgVertex, s).GetWeight();
				Node* bone	 = skinningLayer->GetInfluence(orgVertex, s).GetBone();

				//MCore::LOG("bone = %s", bone->GetNamePtr());

				// now find the bone number
				int boneNumber  = mBones.Find( bone );

				// if it doesn't exist yet, we need to add it
				if (boneNumber == -1)
				{
					mBones.Add( bone );
					boneNumber = mBones.GetMaxIndex();
				}

				// add the influence to the vertex
				vtx.AddInfluence( HwShaderInfluence(boneNumber, weight) );
			}

			// add NULL influences, which have no influence, but in order to give the vertex the same amount of influences
			// as the buffer specifies. This saves some potential problems later on.
			int numExtraInfluences = mNumInfluences - vtx.GetNumInfluences();
			for (int a=0; a<numExtraInfluences; a++)
				vtx.AddInfluence( HwShaderInfluence(0, 0) );
		}

		// add the vertex to the buffer
		mVertices.Add( vtx );

		// add the index to the indices
		mIndices.Add( mVertices.GetMaxIndex() );
	}
}


void HwShaderBuffer::Attach(const HwShaderBuffer& buffer)
{
	// get the vertex and index offsets
	int vertexOffset = mVertices.GetLength();
	int indexOffset	 = mIndices.GetLength();

	// attach the vertices
	mVertices += buffer.mVertices;

	// attach the indices and relink their values
	int i;
	const int bufIndices = buffer.GetNumIndices();
	mIndices.Reserve( mIndices.GetLength() + bufIndices );
	for (i=0; i<bufIndices; i++)
	{
		int index = vertexOffset + buffer.GetIndex(i);
		mIndices.Add( index );
	}

	// attach the bones
	const int numBones = buffer.GetNumBones();
	mBones.Reserve(mBones.GetLength() + numBones);
	for (i=0; i<numBones; i++)
	{
		// check if we already have this bone in our buffer
		int boneNr = mBones.Find( buffer.GetBone(i) );

		// add the bone if we don't have it yet
		if (boneNr == -1)
			mBones.Add( buffer.GetBone(i) );
	}

	// relink the bone numbers inside the vertex weights
	if (mNumInfluences > 0)
	{
		// process all new added vertices
		const int numVerts = mVertices.GetLength();
		for (int v=vertexOffset; v<numVerts; v++)
		{
			HwShaderVertex& vertex = mVertices[v];

			// process all vertex influences
			const int numInfluences = vertex.GetNumInfluences();
			for (int i=0; i<numInfluences; i++)
			{
				// get the original bone number and a pointer to the original bone
				int		boneNr	= vertex.GetInfluence(i).GetBoneNumber();
				Node*	orgBone = buffer.GetBone(boneNr);

				// find the new bone number, and adjust it in the vertex
				int newBoneNr = mBones.Find(orgBone);
				vertex.GetInfluence(i).mBoneNr = newBoneNr;
			}
		}
	}

	// attach the primitives
	const int numPrimitives = buffer.GetNumPrimitives();
	mPrimitives.Reserve( mPrimitives.GetLength() + numPrimitives );
	for (i=0; i<numPrimitives; i++)
	{
		mPrimitives.Add( buffer.GetPrimitive(i) );
		mPrimitives.GetLast().mStartIndex += indexOffset;
		mPrimitives.GetLast().mStartVertex += vertexOffset;
	}
}


// optimize the primitives
void HwShaderBuffer::OptimizePrimitives()
{
	// first sort the primitives on material
	mPrimitives.Sort();
/*
	int before = mPrimitives.GetLength();

	//MCore::LOG("---------------");
	int lastPrim = -1;
	int after = 0;
	for (int i=0; i<mPrimitives.GetLength(); i++)
	{
		if (lastPrim != mPrimitives[i].GetMaterialNr())
		{
			after++;
			lastPrim = mPrimitives[i].GetMaterialNr();
		}
		//MCore::LOG("#%d = %d", i, mPrimitives[i].GetMaterialNr());
	}

	if (before != after)
		MCore::LOG("Before = %d     After = %d", before, after);
*/

}


} // namespace EMotionFX