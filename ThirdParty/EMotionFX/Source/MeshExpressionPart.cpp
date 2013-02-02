/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include EMotion FX related headers
#include "Node.h"
#include "Actor.h"
#include "ExpressionPart.h"
#include "MeshExpressionPart.h"
#include "Mesh.h"
#include "SmartMeshMorphDeformer.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// basic constructor
MeshExpressionPart::MeshExpressionPart(const bool captureTransforms, const bool captureMeshDeforms, Actor* parentActor, const MCore::String& name) : ExpressionPart(parentActor, name)
{
	mCaptureTransforms	= captureTransforms;
	mCaptureMeshDeforms = captureMeshDeforms;
}


// extended constructor
MeshExpressionPart::MeshExpressionPart(const bool captureTransforms, const bool captureMeshDeforms, Actor* parentActor, Actor* pose, const MCore::String& name, const bool delPoseFromMem) : ExpressionPart(parentActor, name)
{
	mCaptureTransforms  = captureTransforms;
	mCaptureMeshDeforms = captureMeshDeforms;

	// init the expression part
	InitFromPose(pose, delPoseFromMem);
}


// destructor
MeshExpressionPart::~MeshExpressionPart()
{
	// get rid of all the deform datas
	while (mDeformDatas.GetLength())
	{
		delete mDeformDatas.GetLast();
		mDeformDatas.RemoveLast();
	}
}


// get the expressionpart type
int MeshExpressionPart::GetType() const
{ 
	return TYPE_ID; 
}


// initialize the expression part from a given actor pose
void MeshExpressionPart::InitFromPose(Actor* pose, bool delPoseFromMem)
{
	assert(mActor);
	assert(pose); 

	// filter all changed meshes
	if (mCaptureMeshDeforms)
	{
		const int numPoseNodes = pose->GetNumNodes();
		for (int i=0; i<numPoseNodes; i++)
		{
			// find if the pose node also exists in the actor where we apply this to
			Node* curNode = pose->GetNode(i);
			Node* orgNode = mActor->GetNodeByID( curNode->GetID() );

			//LOG("orgNode = '%s'", orgNode ? orgNode->GetNamePtr() : "<none>");

			// skip this node if it doesn't exist in the actor we apply this to
			if (orgNode == NULL) 
				continue;

			// get the meshes
			Mesh* orgMesh = orgNode->GetMesh(0).GetPointer();
			Mesh* curMesh = curNode->GetMesh(0).GetPointer();

			//LOG("Checking meshes");

			// if one of the nodes has no mesh, we can skip this node
			if (orgMesh==NULL || curMesh==NULL)
				continue;

			// both nodes have a mesh, lets check if they have the same number of vertices as well
			const int numOrgVerts = orgMesh->GetNumVertices();
			const int numCurVerts = curMesh->GetNumVertices();

			// if the number of original vertices is not equal to the number of current vertices, we can't use this mesh
			if (numOrgVerts != numCurVerts)
				continue;

			// check if the mesh has differences
			int numDifferent = 0;
			Vector3* orgPositions = orgMesh->GetOrgPositions();
			Vector3* curPositions = curMesh->GetOrgPositions();

			Vector3* orgNormals = orgMesh->GetOrgNormals();
			Vector3* curNormals = curMesh->GetOrgNormals();

			AABB box;
			int v;
			for (v=0; v<numOrgVerts; v++)
			{
				// calculate the delta vector between the two positions
				Vector3 deltaVec = curPositions[v] - orgPositions[v];

				// check if the vertex positions are different, so if there are mesh changes
				if (deltaVec.Length() > MCore::Math::EPSILON)
				{
					box.Encapsulate( deltaVec );
					numDifferent++;
				}
			}

			// go to the next node and mesh if there is no difference
			if (numDifferent == 0)
				continue;

			//LOG("MeshExpressionPart::Init() - orgMesh = %d verts   -   targetMesh = %d verts", numOrgVerts, numCurVerts);


			// calculate the minimum value of the box
			float minValue = box.GetMin().x;
			minValue = MCore::Min<float>(minValue, box.GetMin().y);
			minValue = MCore::Min<float>(minValue, box.GetMin().z);

			// calculate the minimum value of the box
			float maxValue = box.GetMax().x;
			maxValue = MCore::Max<float>(maxValue, box.GetMax().y);
			maxValue = MCore::Max<float>(maxValue, box.GetMax().z);

			// make sure the values won't be too small
			if (maxValue - minValue < 1.0f)
			{
				if (minValue < 0 && minValue > -1)
					minValue = -1;

				if (maxValue > 0 && maxValue < 1)
					maxValue = 1;	
			}

			// create the deformation data
			DeformData* deformData = new DeformData(orgNode, numDifferent);

			deformData->mMinValue = minValue;
			deformData->mMaxValue = maxValue;

			int curVertex = 0;
			for (v=0; v<numOrgVerts; v++)
			{
				// calculate the delta vector between the two positions
				Vector3 deltaVec = curPositions[v] - orgPositions[v];

				// check if the vertex positions are different, so if there are mesh changes
				//if (deltaVec.SquareLength() > MCore::Math::EPSILON)
				if (deltaVec.Length() > MCore::Math::EPSILON)
				{
					Vector3 deltaNormal = curNormals[v] - orgNormals[v];
					deformData->mVertexNumbers[curVertex] = v;
					deformData->mDeltas[curVertex].FromVector3(deltaVec, minValue, maxValue);
					deformData->mDeltaNormals[curVertex].FromVector3(deltaNormal, -1.0f, 1.0f);
					curVertex++;
				}
			}

			//LOG("Deformdata for '%s' with %d vertices", orgNode->GetNamePtr(), numDifferent);

			// add the deform data
			mDeformDatas.Add( deformData );

			//-------------------------------
			// create the mesh deformer
			MCore::Pointer<MeshDeformerStack>& stack = orgNode->GetMeshDeformerStack(0);

			// create the stack if it doesn't yet exist
			if (stack.GetPointer() == NULL)
			{
				stack = MCore::Pointer<MeshDeformerStack>( new MeshDeformerStack(orgMesh) );
				orgNode->SetMeshDeformerStack(stack, 0);
			}

			// add the skinning deformer to the stack
			SmartMeshMorphDeformer* deformer = new SmartMeshMorphDeformer( this, mDeformDatas.GetMaxIndex(), orgMesh );
			stack->InsertDeformer(0, deformer);
			//-------------------------------

			// make sure we end up with the same number of different vertices, otherwise the two loops
			// have different detection on if a vertex has changed or not
			assert(curVertex == numDifferent);
		}
	}

	//--------------------------------------------------------------------------------------------------

	if (mCaptureTransforms)
	{
		// check for transformation changes
		const int numPoseNodes = pose->GetNumNodes();
		for (int i=0; i<numPoseNodes; i++)
		{
			Node* orgNode = mActor->GetNodeByID( pose->GetNode(i)->GetID() );

			// skip it if it doesn't exist (so we won't crash)
			if (orgNode == NULL) 
				continue;

			Node* nodeA = pose->GetNode(i);
			Node* nodeB = orgNode;

			MCore::Vector3		posA	= nodeA->GetOrgPos();
			MCore::Vector3		posB	= nodeB->GetOrgPos();
			MCore::Quaternion	rotA	= nodeA->GetOrgRot();
			MCore::Quaternion	rotB	= nodeB->GetOrgRot();
			MCore::Vector3		scaleA	= nodeA->GetOrgScale();
			MCore::Vector3		scaleB	= nodeB->GetOrgScale();
			
			bool changed = false;

			float posDif = (posA-posB).SquareLength();
			if (posDif > 0)
				posDif = Math::Sqrt(posDif);

			// check if the position changed
			if (posDif > 0.0001f)
				changed = true;

			// check if the rotation changed
			if (!changed)
			{
				float rotDif = (rotA - rotB).SquareLength();
				if (rotDif > 0)
					rotDif = Math::Sqrt(rotDif);

				if (rotDif > 0.0001f)
					changed = true;
			}

			// check if the scale changed
			if (!changed)
			{
				float scaleDif = (scaleA-scaleB).SquareLength();
				if (scaleDif > 0)
					scaleDif = Math::Sqrt(scaleDif);

				if (scaleDif > 0.0001f)
					changed = true;
			}

			// if this node changed transformation
			if (changed)
			{
				// create a transform object form the node in the pose
				Transformation transform;
				transform.mPosition	= nodeA->GetOrgPos() - orgNode->GetOrgPos();
				transform.mRotation	= nodeA->GetOrgRot();
				transform.mScale	= nodeA->GetOrgScale() - orgNode->GetOrgScale();

				// add the new node/transform pair to the expression part
				AddTransformation(orgNode->GetID(), transform);
			}
		}

		//LOG("Num transforms = %d", mTransforms.GetLength());
	}

	// delete the pose actor when wanted
	if (delPoseFromMem)
		delete pose;
}


// restore all nodes touched by this expression part by putting them back in their original transformation
void MeshExpressionPart::RestoreActor(Actor* actor)
{
	// restore all nodes affected by this expression part to their original transformations
	const int numTransforms = mTransforms.GetLength();
	for (int i=0; i<numTransforms; i++)
	{
		Node* node = actor->GetNodeByID( mTransforms[i].GetFirst() );
		assert( node != NULL);
		if (node)
			node->SetToOriginalOrientation();
	}
}


// apply the relative transformation to the specified node
// store the result in the position, rotation and scale parameters
void MeshExpressionPart::ApplyTransformation(Actor* actor, Node* node, MCore::Vector3& position, MCore::Quaternion& rotation, MCore::Vector3& scale)
{
	// get the weight value and convert it to a range based weight value
	const float weight = CalcRangedWeight( mCurWeight );

	// calculate the new transformations for all nodes of this expression part
	const int numTransforms = mTransforms.GetLength();
	for (int i=0; i<numTransforms; i++)
	{
		const int id = mTransforms[i].GetFirst();
		if (node->GetID() == id)
		{
			// get the source node and target transformation
			Node* orgNode = actor->GetNodeByID( id );
			assert(orgNode);
			Transformation&	targetTransform = mTransforms[i].GetSecond();

			// calc new position (delta based targetTransform)
			position += targetTransform.mPosition * weight;

			//------------------
			// linear interpolate the quaternions, allowing negative weights as well
			// the Quaternion::Lerp function doesn't allow negative weights, so we have to do it like this
			MCore::Quaternion a = orgNode->GetOrgRot();
			MCore::Quaternion b = targetTransform.mRotation;

			// check if both quaternions are on the same hypersphere or not, if not, invert one
			if (a.Dot(b) < 0.0)
				a = -a;

			// take the logarithm
			a = a.LogN();
			b = b.LogN();

			// interpolate, and take the exponent of that, which is the interpolated quaternion
			Quaternion rot = MCore::LinearInterpolate<Quaternion>(a, b, weight).Exp().Normalize();
			//------------------

			//MCore::Quaternion rot		= orgNode->GetOrgRot().Lerp( targetTransform.mRotation, weight );	// the target rotation
			MCore::Quaternion invRot	= orgNode->GetOrgRot().Inversed();									// inversed original rotation
			rotation					= rotation * (invRot * rot);										// add the delta rotation

			// calc new scale (delta based targetTransform)
			scale += targetTransform.mScale * weight;

			break;
		}
	}
}


// check if this expression part influences the specified node or not
bool MeshExpressionPart::Influences(Node* node) const
{
	int i;

	// check if there is a deform data object, which works on the specified node
	const int numDatas = mDeformDatas.GetLength();
	for (i=0; i<numDatas; i++)
		if (mDeformDatas[i]->mNode == node)
			return true;

	// check all transforms
	const int numTransforms = mTransforms.GetLength();
	for (i=0; i<numTransforms; i++)
	{
		// if the node name is equal to the transform name, it means the node will be modified
		if (mTransforms[i].GetFirst() == node->GetID())
			return true;
	}

	// this expression part doesn't influence the given node
	return false;
}


// apply the deformations to a given actor
void MeshExpressionPart::Apply(Actor* actor)
{
	// get the weight value and convert it to a range based weight value
	const float weight = CalcRangedWeight( mCurWeight );

	// calculate the new transformations for all nodes of this expression part
	const int numTransforms = mTransforms.GetLength();
	for (int i=0; i<numTransforms; i++)
	{
		Node* node = actor->GetNodeByID( mTransforms[i].GetFirst() );
		if (node == NULL) continue;

		Transformation&	targetTransform = mTransforms[i].GetSecond();

		// calc new position (delta based targetTransform)
		node->SetLocalPos( node->GetLocalPos() + (targetTransform.mPosition * weight) );

		// calc new rotation (non-delta based targetTransform)
		//MCore::Quaternion rot	= node->GetOrgRot().Lerp( targetTransform.mRotation, mCurWeight );	// the target rotation

		//------------------
		// linear interpolate the quaternions, allowing negative weights as well
		// the Quaternion::Lerp function doesn't allow negative weights, so we have to do it like this
		MCore::Quaternion a = node->GetOrgRot();
		MCore::Quaternion b = targetTransform.mRotation;

		// check if both quaternions are on the same hypersphere or not, if not, invert one
		if (a.Dot(b) < 0.0)
			a = -a;

		// take the logarithm
		a = a.LogN();
		b = b.LogN();

		// interpolate, and take the exponent of that, which is the interpolated quaternion
		Quaternion rot = MCore::LinearInterpolate<Quaternion>(a, b, weight).Exp().Normalize();
		//------------------

		MCore::Quaternion invRot = node->GetOrgRot().Inversed();			// inversed original rotation
		node->SetLocalRot( node->GetLocalRot() * (invRot * rot) );		// add the delta rotation

		// calc new scale (delta based targetTransform)
		node->SetLocalScale( node->GetLocalScale() + (targetTransform.mScale * weight));
	}		
}



int MeshExpressionPart::GetNumDeformDatas() const
{
	return mDeformDatas.GetLength();
}


MeshExpressionPart::DeformData* MeshExpressionPart::GetDeformData(const int nr)
{
	return mDeformDatas[nr];
}


void MeshExpressionPart::AddDeformData(DeformData* data)
{
	mDeformDatas.Add(data);
}



// add transformation to the expression part
void MeshExpressionPart::AddTransformation(const int nodeID, const Transformation& transform)
{
	MCore::Pair<int, Transformation>	newPair(nodeID, transform);
	mTransforms.Add( newPair );
}


// get the number of transformations in this bones expression part
int MeshExpressionPart::GetNumTransformations() const
{
	return mTransforms.GetLength();
}


// get the given transformation
const MCore::Pair<int, MeshExpressionPart::Transformation>& MeshExpressionPart::GetTransformation(const int nr)
{
	assert(nr >= 0 && nr < mTransforms.GetLength());
	return mTransforms[nr];
}


// check if this expression part captures rigid transformations
bool MeshExpressionPart::GetCaptureTransforms() const
{
	return mCaptureTransforms;
}


// check if this expression part captures mesh deformations
bool MeshExpressionPart::GetCaptureMeshDeforms() const
{
	return mCaptureMeshDeforms;
}



//---------------------------------------------------
// DeformData
//---------------------------------------------------

// constructor
MeshExpressionPart::DeformData::DeformData(Node* node, int numVerts)
{
	mNode			= node;
	mNumVerts		= numVerts;
	mVertexNumbers	= (int*)MEMMGR.Allocate(numVerts*sizeof(int), MEMCATEGORY_MOTIONS_FACIALMOTIONS, 16, MeshExpressionPart::MEMORYBLOCK_ID);
	mDeltas			= (MCore::Compressed16BitVector3*)MEMMGR.Allocate(numVerts*sizeof(MCore::Compressed16BitVector3), MEMCATEGORY_MOTIONS_FACIALMOTIONS, 16, MeshExpressionPart::MEMORYBLOCK_ID);
	mDeltaNormals	= (MCore::Compressed8BitVector3*)MEMMGR.Allocate(numVerts*sizeof(MCore::Compressed8BitVector3), MEMCATEGORY_MOTIONS_FACIALMOTIONS, 16, MeshExpressionPart::MEMORYBLOCK_ID);
	mMinValue		= -10.0f;
	mMaxValue		= +10.0f;
}


// destructor
MeshExpressionPart::DeformData::~DeformData()
{
	MEMMGR.Delete( mVertexNumbers );
	MEMMGR.Delete( mDeltas );
	MEMMGR.Delete( mDeltaNormals );
}


} // namespace EMotionFX