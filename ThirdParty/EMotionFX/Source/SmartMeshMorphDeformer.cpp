/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include EMotion FX related headers
#include "SmartMeshMorphDeformer.h"
#include "MeshExpressionPart.h"
#include "Mesh.h"
#include "Node.h"
#include "SubMesh.h"
#include "Actor.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
SmartMeshMorphDeformer::SmartMeshMorphDeformer(MeshExpressionPart* part, const int deformDataNr, Mesh* mesh) : MeshDeformer(mesh)
{
	mMeshExpressionPart = part;
	mDeformDataNr		= deformDataNr;
	mLastNearZero		= true;
}


// destructor
SmartMeshMorphDeformer::~SmartMeshMorphDeformer()
{
}


// get the unique type id
int SmartMeshMorphDeformer::GetType() const
{
	return SmartMeshMorphDeformer::TYPE_ID;
}


// get the subtype id
int SmartMeshMorphDeformer::GetSubType() const
{
	return SmartMeshMorphDeformer::SUBTYPE_ID;
}


// clone this class
MeshDeformer* SmartMeshMorphDeformer::Clone(Mesh* mesh, Actor* actor)
{
	// create the new cloned deformer
	SmartMeshMorphDeformer* result = new SmartMeshMorphDeformer(mMeshExpressionPart, mDeformDataNr, mesh);

	// return the result
	return result;
}


// get the expression part it has to get the deform data from
MeshExpressionPart* SmartMeshMorphDeformer::GetExpressionPart() const	
{ 
	return mMeshExpressionPart; 
}


// the main method where all calculations are done
void SmartMeshMorphDeformer::Update(Actor* actor, Node* node, const double timeDelta)
{
	DECLARE_FUNCTION(Update)

	// get the deform data and number of vertices to deform
	MeshExpressionPart::DeformData* deformData = mMeshExpressionPart->mDeformDatas[ mDeformDataNr ];
	const int numDeformVerts = deformData->mNumVerts;

	// this mesh deformer can't work on this mesh, because the deformdata number of vertices is bigger than the 
	// number of vertices inside this mesh!
	// and that would make it crash, which isn't what we want
	if (numDeformVerts > mMesh->GetNumVertices())
		return;

	// get the weight value and convert it to a range based weight value
	const float weight = mMeshExpressionPart->CalcRangedWeight( mMeshExpressionPart->GetWeight() );

	// nothing to do when the weight is too small
	bool nearZero = InRange<float>(weight, -Math::EPSILON, Math::EPSILON);

	// we are near zero, and the previous frame as well, so we can return
	if (nearZero && mLastNearZero)
		return;

	// update the flag
	if (nearZero)
		mLastNearZero = true;
	else
		mLastNearZero = false;	// we moved away from zero influence

	// some vars we need
	Vector3*	positions		= mMesh->GetPositions();
	Vector3*	normals			= mMesh->GetNormals();
	int*		vertexNumbers	= deformData->mVertexNumbers;

	Compressed16BitVector3*	deltas		 = deformData->mDeltas;
	Compressed8BitVector3*	deltaNormals = deformData->mDeltaNormals;
	const float				minValue	 = deformData->mMinValue;
	const float				maxValue	 = deformData->mMaxValue;

	// process all touched vertices
	int v;
	int vtxNr;
	for (v=0; v<numDeformVerts; v++)
	{
		vtxNr = vertexNumbers[v];
		positions[vtxNr] += deltas[v].ToVector3(minValue, maxValue) * weight;
		normals  [vtxNr] += deltaNormals[v].ToVector3(-1.0f, 1.0f) * weight;
	}
}

} // namespace EMotionFX
