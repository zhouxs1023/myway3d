/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Actor.h"
#include "MotionLayer.h"
#include "Motion.h"
#include "SkeletalMotion.h"
#include "MeshDeformerStack.h"
#include "Material.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "SimpleActorCollisionSystem.h"
#include "SmartMeshMorphDeformer.h"
#include "SkinningInfoVertexAttributeLayer.h"


// use the Core namespace
using namespace MCore;



namespace EMotionFX {

// constructor
Actor::Actor(const String & name)
	: Node(name)
{
	mMotionSystem = NULL;
	mOriginalActor = NULL;
	mActorCollisionSystem = NULL;
	mRepositioningNode = NULL;
	mCurrentLOD = 0;
	mNumLODs = 1;

	// set the repositioning mask to only reposition the positions
	SetRepositioningMask(true, false, false);

	// make sure we have at least allocated the first LOD of materials and facial setups
	mMaterials.Resize(1);
	mFacialSetups.Resize(1);

	// pregenerate the collision system
	mActorCollisionSystem = new SimpleActorCollisionSystem(this);

	// pregenerate the default motion system
	mMotionSystem = new MotionLayerSystem(this);
}


// destructor
Actor::~Actor()
{
	Release();
}


// update the actor
void Actor::Update(const double timePassedInSeconds,const bool isVisible,Matrix * actorTM)
{
	// motion system update:
	// 1. update motion queue and instances
	// 2. update the motion tree (automatic layer removement, weight updates etc.)
	// 3. update node transformation data
	if (mMotionSystem)
		mMotionSystem->Update(timePassedInSeconds, isVisible);

	// update all nodes
	if (isVisible) {
		// update the object space controllers
		UpdateControllers(timePassedInSeconds);

		// update the hierarchy (LocalTM and WorldTM matrices) and bounds
		ProcessHierarchy(actorTM);

		// update the post controllers
		UpdatePostControllers(timePassedInSeconds);

		// process the deformer stacks
		UpdateNodeStacks(timePassedInSeconds);
	}

	// update all attachments
	for (int i = 0; i < mAttachments.GetLength(); i++)
		mAttachments[i]->Update(timePassedInSeconds, isVisible, NULL);
}


// creates a clone of the actor (a copy).
// does NOT copy the motions and motion tree
Actor * Actor::Clone(EActorCloneFlags flags)
{
	int i;
	Actor * result = new Actor(mName);

	// copy the actor attributes
	result->mBoundingBox = mBoundingBox;
	result->mLocalRot = mLocalRot;
	result->mLocalTM = mLocalTM;
	result->mWorldTM = mWorldTM;
	result->mInvBoneTM = mInvBoneTM;
	result->mLocalScale = mLocalScale;
	result->mLocalPos = mLocalPos;
	result->mWorldScale = mWorldScale;
	result->mParent = NULL;
	result->mOrgPos = mOrgPos;
	result->mOrgRot = mOrgRot;
	result->mOrgScale = mOrgScale;
	result->mOriginalActor = NULL;
	result->mCurrentLOD = mCurrentLOD;
	result->mNumLODs = mNumLODs;
	result->mFacialSetups = mFacialSetups;
	result->mRepositioningMask = mRepositioningMask;

	// clone the materials
	if (flags & CLONE_MATERIALS) {
		result->mMaterials.Resize(mMaterials.GetLength());
		for (i = 0; i < mMaterials.GetLength(); i++) {
			// get the number of materials in the current LOD
			const int numMaterials = mMaterials[i].GetLength();

			for (int m = 0; m < numMaterials; m++) {
				// retrieve the current material
				Pointer<Material> material = mMaterials[i][m];

				// clone the material
				Pointer<Material> clone = material->Clone();

				// add the cloned material to the cloned actor
				result->AddMaterial(i, clone);
			}
		}
	} else	// share the materials
	{
		result->mMaterials.Resize(mMaterials.GetLength());
		for (i = 0; i < mMaterials.GetLength(); i++) {
			// get the number of materials in the current LOD
			const int numMaterials = mMaterials[i].GetLength();

			for (int m = 0; m < numMaterials; m++) {
				// retrieve the current material
				Pointer<Material> material = mMaterials[i][m];

				// add the material to the cloned actor (share materials)
				result->AddMaterial(i, material);
			}
		}
	}


	// create the node cloning flags
	int nodeFlags = 0;
	if (flags & CLONE_NODEATTRIBUTES)
		nodeFlags |= Node::CLONE_ATTRIBUTES;

	// clone all nodes (without parents, childs and motion links)
	const int numNodes = mNodes.GetLength();
	for (i = 0; i < numNodes; i++)
		result->mNodes.Add(mNodes[i]->Clone(result, (Node::ENodeCloneFlags) nodeFlags));

	// now we have cloned all nodes, we have a problem, since the hierarchy (parent and childs) isn't there yet!
	// to solve this, we can simply do some searches on names of nodes in the original nodes array
	// first find all the parents, once we have the parents we can also find the childs, and once we have the childs we're done! :)
	for (i = 0; i < numNodes; i++) {
		// if this node has no parent we don't need to search for it, so then skip to the next node
		if (mNodes[i]->GetParent() == (Node *)this) {
			result->mNodes[i]->SetParent(result);
			result->AddChild(result->mNodes[i]);
			continue;
		}

		bool foundParent = false;
		const int numResNodes = result->mNodes.GetLength();
		for (int a = 0; a < numResNodes && !foundParent; a++) {
			// if the node name (so also its ID) is equal to the original parent name, it means we have found the parent to use :)
			if (result->mNodes[a]->GetID() == mNodes[i]->GetParent()->GetID()) {
				result->mNodes[i]->SetParent(result->mNodes[a]);
				foundParent = true;
			}
		}

		if (foundParent == false)
			LOG("No parent found for node '%s'", result->mNodes[i]->GetNamePtr());
	}


	// ok, we found the parents, now it's time to find the childs
	for (i = 0; i < numNodes; i++) {
		// skip to the next node if this node has no childs
		if (mNodes[i]->GetNumChilds() == 0)
			continue;

		// for all childs
		const int numChilds = mNodes[i]->GetNumChilds();
		for (int c = 0; c < numChilds; c++) {
			bool foundChild = false;

			// check all nodes
			const int numResNodes = result->mNodes.GetLength();
			for (int a = 0; a < numResNodes && !foundChild; a++) {
				// compare the node name with the original child name, if it's equal we found a match
				if (result->mNodes[a]->GetID() == mNodes[i]->GetChild(c)->GetID()) {
					result->mNodes[i]->AddChild(result->mNodes[a]);
					foundChild = true;
				}
			}
		}

		// make sure we end up with the same number of childs, else something really weird happened
		assert(mNodes[i]->GetNumChilds() == result->mNodes[i]->GetNumChilds());
	}

	// clone all stacks of the nodes
	// this is needed because of some execution order conflicts (if we would have done it inside the Node::Clone)
	const int numResNodes = result->mNodes.GetLength();
	for (i = 0; i < numResNodes; i++) {
		result->mNodes[i]->CloneNodeStacksFromNode(mNodes[i], result);
		result->mNodes[i]->CloneNodeCollisionStacksFromNode(mNodes[i], result);
	}

	// clone the actor collision system
	result->SetActorCollisionSystem(mActorCollisionSystem->Clone(result));


	// clone controllers if desired
	if (flags & CLONE_CONTROLLERS) {
		// iterate through the controllers and clone them
		const int numControllers = mControllers.GetLength();
		for (i = 0; i < numControllers; i++) {
			// retrieve the current controller
			Controller * controller = mControllers[i];

			// clone the controller and add it to the clone actor
			result->AddController(controller->Clone(result));
		}

		// iterate through the post controllers and clone them
		const int numPostControllers = mPostControllers.GetLength();
		for (i = 0; i < numPostControllers; i++) {
			// retrieve the current controller
			Controller * controller = mPostControllers[i];

			// clone the controller and add it to the clone actor
			result->AddPostController(controller->Clone(result));
		}
	}

	// copy the repositioning node
	if (mRepositioningNode)
		result->mRepositioningNode = result->GetNodeByID(mRepositioningNode->GetID());
	else
		result->mRepositioningNode = NULL;

	// make sure the number of childs is the same
	assert(result->GetNumChilds() == GetNumChilds());

	return result;
}


// release allocated memory
void Actor::Release()
{
	// get rid of the motion system
	if (mMotionSystem)
		delete mMotionSystem;

	// delete the actor collision system
	if (mActorCollisionSystem != NULL)
		delete mActorCollisionSystem;

	// delete all the materials
	RemoveAllMaterials();

	// remove all nodes
	while (mNodes.GetLength()) {
		delete mNodes.GetLast();
		mNodes.RemoveLast();
	}

	// remove all controllers
	RemoveAllControllers();
	RemoveAllPostControllers();

	// if we have a facial setup, remove it (smartpointer deletion)
	RemoveAllFacialSetups();

	// remove all attachments, but not from memory
	RemoveAllAttachments(false);
}


// process the hierarchy for all nodes
void Actor::ProcessHierarchy(Matrix * actorTM)
{
	// update the local TM
	if (actorTM == NULL)
		UpdateLocalTM();
	else
		mLocalTM = *actorTM;

	// update this nodes transformations
	if (IsRootNode()) {
		mWorldTM = mLocalTM;
		mWorldScale = mLocalScale;
	} else {
		Vector3 parentScale = mParent->GetWorldScale();
		mWorldScale = Vector3(mLocalScale.x * parentScale.x, mLocalScale.y * parentScale.y, mLocalScale.z * parentScale.z);
		mWorldTM = mLocalTM * mParent->GetWorldTM();
	}

	// process the hierarchy for all childs
	int i;
	const int numChilds = mChilds.GetLength();
	for (i = 0; i < numChilds; i++)
		mChilds[i]->RecursiveHierarchyUpdate();
}


void Actor::UpdateBounds(const int lodLevel,EBoundsType boundsType,const bool includeAttachments)
{
	// re-init the boundingbox
	mBoundingBox.Init();

	// update the bounds
	if (boundsType == BOUNDS_NODE_BASED)	// node based bounds
	{
		// include the node positions
		const int numNodes = mNodes.GetLength();
		for (int i = 0; i < numNodes; i++)
			mBoundingBox.Encapsulate(mNodes[i]->GetWorldPos());
	} else if (boundsType == BOUNDS_MESH_BASED)	// mesh based bounds
	{
		CalcMeshBasedAABB(&mBoundingBox, lodLevel);
	} else {
		CalcCollisionMeshBasedAABB(&mBoundingBox, lodLevel);
	}

	// include the attachments
	if (includeAttachments) {
		const int numAttachments = mAttachments.GetLength();
		for (int i = 0; i < numAttachments; i++) {
			// make sure we have a safe LOD level
			const int numLods = mAttachments[i]->GetNumLODLevels();
			const int lod = Clamp(lodLevel, 0, numLods - 1);

			// update the bounds of the attachment
			mAttachments[i]->UpdateBounds(lod, boundsType, includeAttachments);

			// encapsulate the attachment box into this node box
			mBoundingBox.Encapsulate(mAttachments[i]->GetBoundingBox());
		}
	}

	// update boundingsphere (containing the bounding box)
	mBoundingSphere = BoundingSphere(mBoundingBox.CalcMiddle(), 0.0f);
	mBoundingSphere.Encapsulate(mBoundingBox.GetMin());
	mBoundingSphere.Encapsulate(mBoundingBox.GetMax());
}



// play a motion
MotionInstance * Actor::PlayMotion(Motion * motion,PlayBackInfo * info)
{
	assert(mMotionSystem != NULL);
	assert(motion != NULL);

	// if the user didn't specify any playback information, we will just use the default values
	if (info == NULL) {
		PlayBackInfo defaultInfo;
		return mMotionSystem->PlayMotion(motion, &defaultInfo);
	} else {
		return mMotionSystem->PlayMotion(motion, info);
	}
}



// stop playing the motion in <blendTime> seconds
void Actor::StopMotion(MotionInstance * motion,const float blendTime)
{
	assert(mMotionSystem != NULL);

	mMotionSystem->StopMotion(motion, blendTime);
}


// removes all materials from the actor
void Actor::RemoveAllMaterials()
{
	// for all LODs
	for (int i = 0; i < mMaterials.GetLength(); i++) {
		// remove all materials for this LOD
		while (mMaterials[i].GetLength()) {
			// set the smartpointer to NULL
			mMaterials[i].GetLast() = NULL;

			// no delete is needed, because it's a smartpointer, so we can just use RemoveLast only
			mMaterials[i].RemoveLast();
		}
	}
}


void Actor::AddLODModel(Actor * lodActor,bool delLODActorFromMem)
{
	DECLARE_FUNCTION(AddLODModel)
	/*
	if (lodActor->GetNumNodes() != GetNumNodes())
	throw Exception("The LOD actor has a different amount of bones than the full detail actor, this is not allowed! Only reduce the triangle count, and do not adjust the skeleton structure.", HERE);

	// relink all nodes
	int i;
	const int numNodes = GetNumNodes();
	for (i=0; i<numNodes; i++)
	{
	Node* node	= GetNode(i);
	Node* lodNode= lodActor->GetNode(i);

	// make sure the nodes are the same
	if (node->GetName() != lodNode->GetName())
	throw Exception("The LOD actor has a different amount of bones than the full detail actor, this is not allowed! Only reduce the triangle count, and do not adjust the skeleton structure.", HERE);

	// copy over the mesh and collision mesh
	node->AddLODMesh( lodNode->GetMesh(0) );
	if (node->GetMesh(mNumLODs).GetPointer())
	node->GetMesh( mNumLODs )->ReLinkAllVertexAttributeLayers( node->GetMesh(mNumLODs).GetPointer(), node, this);

	node->AddLODCollisionMesh( lodNode->GetCollisionMesh(0) );
	if (node->GetCollisionMesh(mNumLODs).GetPointer())
	node->GetCollisionMesh( mNumLODs )->ReLinkAllVertexAttributeLayers( node->GetCollisionMesh(mNumLODs).GetPointer(), node, this);

	// handle the stacks
	if (lodNode->GetMeshDeformerStack(0).GetPointer() != NULL)
	node->AddLODMeshDeformerStack( lodNode->GetMeshDeformerStack(0)->Clone(node->GetMesh(mNumLODs), this) );
	else
	node->AddLODMeshDeformerStack( lodNode->GetMeshDeformerStack(0) );

	// handle the collision stacks
	if (lodNode->GetCollisionMeshDeformerStack(0).GetPointer() != NULL)
	node->AddLODCollisionMeshDeformerStack( lodNode->GetCollisionMeshDeformerStack(0)->Clone(node->GetCollisionMesh(mNumLODs), this) );
	else
	node->AddLODCollisionMeshDeformerStack( lodNode->GetCollisionMeshDeformerStack(0) );

	// consistency check
	if (node->GetNumMeshDeformerStacks() != node->GetNumMeshes())
	throw Exception("Something is wrong, the number of meshes in node '" + node->GetName() + "' is not equal to the number of mesh deformer stacks in that node.", HERE);

	// consistency check
	if (node->GetNumCollisionMeshDeformerStacks() != node->GetNumCollisionMeshes())
	throw Exception("Something is wrong, the number of collision meshes in node '" + node->GetName() + "' is not equal to the number of collision mesh deformer stacks in that node.", HERE);
	}
	*/

	int i;
	const int numNodes = mNodes.GetLength();
	for (i = 0; i < numNodes; i++) {
		Node * node = mNodes[i];
		Node * lodNode = lodActor->GetNodeByID(node->GetID());

		// if the node couldn't be found in the lower detail level, then add NULL meshes
		if (lodNode == NULL) {
			// copy over the mesh and collision mesh
			node->AddLODMesh(Pointer<Mesh>(NULL));
			node->AddLODCollisionMesh(Pointer<Mesh>(NULL));

			// handle the stacks
			node->AddLODMeshDeformerStack(Pointer<MeshDeformerStack>(NULL));
			node->AddLODCollisionMeshDeformerStack(Pointer<MeshDeformerStack>(NULL));
		} else {
			// copy over the mesh and collision mesh
			node->AddLODMesh(lodNode->GetMesh(0));
			if (node->GetMesh(mNumLODs).GetPointer())
				node->GetMesh(mNumLODs)->ReLinkAllVertexAttributeLayers(node->GetMesh(mNumLODs).GetPointer(), node, this);

			node->AddLODCollisionMesh(lodNode->GetCollisionMesh(0));
			if (node->GetCollisionMesh(mNumLODs).GetPointer())
				node->GetCollisionMesh(mNumLODs)->ReLinkAllVertexAttributeLayers(node->GetCollisionMesh(mNumLODs).GetPointer(), node, this);

			// handle the stacks
			if (lodNode->GetMeshDeformerStack(0).GetPointer() != NULL)
				node->AddLODMeshDeformerStack(lodNode->GetMeshDeformerStack(0)->Clone(node->GetMesh(mNumLODs), this));
			else
				node->AddLODMeshDeformerStack(lodNode->GetMeshDeformerStack(0));

			// handle the collision stacks
			if (lodNode->GetCollisionMeshDeformerStack(0).GetPointer() != NULL)
				node->AddLODCollisionMeshDeformerStack(lodNode->GetCollisionMeshDeformerStack(0)->Clone(node->GetCollisionMesh(mNumLODs), this));
			else
				node->AddLODCollisionMeshDeformerStack(lodNode->GetCollisionMeshDeformerStack(0));
		}
	}


	// now copy the materials from the highest detail
	mMaterials.Resize(mNumLODs + 1);
	const int numMaterials = lodActor->GetNumMaterials(0);
	for (i = 0; i < numMaterials; i++)
		AddMaterial(mNumLODs, lodActor->GetMaterial(0, i));

	// add the facial setup from the LOD actor
	mFacialSetups.Add(lodActor->GetFacialSetup(0));

	// increase the LOD level count
	mNumLODs++;

	// remove the actor from memory if desired
	if (delLODActorFromMem)
		delete lodActor;
}


// removes all node meshes and stacks
void Actor::RemoveAllNodeMeshes()
{
	// remove all meshes
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++)
		mNodes[i]->RemoveMeshes();
}


// generate hardware shader friendly buffers
void Actor::GenerateHardwareShaderBuffers(MCore::Array<HwShaderBuffer> & outBuffers,const int lodLevel,const int maxBonesPerShader,const bool sortOnNumWeights,const bool includeSmartMeshMorphedMeshes)
{
	// face bones
	Array<Node *> faceBones;
	int lastHit = -1;

	// for all nodes in the actor
	const int numNodes = mNodes.GetLength();
	int i;
	for (i = 0; i < numNodes; ++i) {
		// get the node and mesh pointers
		Node * node = mNodes[i];
		Mesh * mesh = node->GetMesh(lodLevel).GetPointer();

		// if there is no mesh, we can simply continue to the next node
		if (mesh == NULL)
			continue;

		// check if this node has facial animation applied on it
		bool hasFacial = false;
		if (node->GetMeshDeformerStack(lodLevel).GetPointer())
			hasFacial = node->GetMeshDeformerStack(lodLevel)->HasDeformerOfType(SmartMeshMorphDeformer::TYPE_ID);

		// skip nodes that have facial animation
		if (hasFacial && !includeSmartMeshMorphedMeshes)
			continue;

		// for all submeshes we will now create a buffer for each set of weights
		// submeshes are already sorted on material, so that saves us some work :)
		const int numSubMeshes = mesh->GetNumSubMeshes();
		int s;
		for (s = 0; s < numSubMeshes; s++) {
			SubMesh * subMesh = mesh->GetSubMesh(s);
			int material = subMesh->GetMaterial();

			// start recording the primitives
			const int numOutBuffers = outBuffers.GetLength();
			int p;
			for (p = 0; p < numOutBuffers; p++)
				outBuffers[p].StartPrimitive(node, material);

			// for all faces in the submesh
			const int numIndices = subMesh->GetNumIndices();
			int f;
			for (f = 0; f < numIndices; f += 3) {
				// get the offset in mesh index array of the first vertex of this face
				int startIndex = subMesh->GetStartIndex() + f;

				// calculate the number of weights used by this face
				int maxInfluences = mesh->CalcMaxNumInfluencesForFace(startIndex);

				// gather the bones
				mesh->GatherBonesForFace(startIndex, faceBones);

				// check if there are already existing buffers where this face could be inserted in
				bool foundBufferForFace = false;

				// check the last used buffer (cache)
				if (lastHit != -1 && outBuffers[lastHit].CanHandleFace(maxInfluences, faceBones, material, maxBonesPerShader, !sortOnNumWeights)) {
					outBuffers[lastHit].AddFace(mesh, startIndex);
					foundBufferForFace = true;
				} else {
					// try to find a buffer which can handle this face
					const int numBuffers = outBuffers.GetLength();
					for (int b = 0; b < numBuffers && !foundBufferForFace; b++) {
						// create a shortcut to the buffer
						HwShaderBuffer & buffer = outBuffers[b];

						// if the buffer can take this face
						if (buffer.CanHandleFace(maxInfluences, faceBones, material, maxBonesPerShader, !sortOnNumWeights)) {
							buffer.AddFace(mesh, startIndex);
							foundBufferForFace = true;
							lastHit = b;
							break;
						}
					}

					// no buffer can handle the current face, so let's create a new buffer
					if (foundBufferForFace == false) {
						HwShaderBuffer newBuffer(material, maxInfluences);
						outBuffers.Add(newBuffer);
						outBuffers.GetLast().StartPrimitive(node, material);
						outBuffers.GetLast().AddFace(mesh, startIndex);
						lastHit = outBuffers.GetMaxIndex();
					}
				}
			} // for all faces in submesh

			// stop recording the primitives
			const int numBufs = outBuffers.GetLength();
			for (p = 0; p < numBufs; p++)
				outBuffers[p].EndPrimitive();
		} // for all submeshes
	} // for all nodes

	/*
		LOG("BEFORE BUFFER OPTIMIZATION:");
		LOG("Generated buffers = %d", outBuffers.GetLength());
		int totalVerts = 0;
		int totalIndices = 0;
		for (i=0; i<outBuffers.GetLength(); i++)
		{
			LOG("Buffer #%d/%d: infl=%d | bones=%d | verts=%d | indices=%d | prims=%d", i, outBuffers.GetLength(), outBuffers[i].GetNumInfluences(), outBuffers[i].GetNumBones(), outBuffers[i].GetNumVertices(), outBuffers[i].GetNumIndices(), outBuffers[i].GetNumPrimitives());
			totalVerts += outBuffers[i].GetNumVertices();
			totalIndices += outBuffers[i].GetNumIndices();
		}
		LOG("totalVerts = %d", totalVerts);
		LOG("totalIndices = %d (%d faces)", totalIndices, totalIndices / 3);
	*/
	//-------------------------------------
	// now that we have the buffers generated we have to optimize the buffers
	// this can be quite tricky, because what we want to do is:
	// - group buffers together, which have the same number of influences, but different materials (reduces number of small buffers)
	// - sort buffers on number of influences (reduces vertex shader switches)
	// - group primitives inside the buffers together, which share the same node and material (reduces primitive calls and increases batch sizes)

	// STEP #1: group buffers together which have the same nr of influences but different materials
	//int startBuffers = outBuffers.GetLength();
	for (i = 0; i < outBuffers.GetLength(); i++) {
		for (int a = 0; a < outBuffers.GetLength(); a++) {
			// don't try to attach this buffer to itself :)
			if (i == a)
				continue;

			// if the number of influences of both buffers is the same
			if (outBuffers[i].GetNumInfluences() == outBuffers[a].GetNumInfluences()) {
				// if attaching will fit our maximum buffers per shader requirement
				if (outBuffers[i].GetNumBones() + outBuffers[a].GetNumBones() <= maxBonesPerShader) {
					//MCore::LOG("Attaching buffer to %d", i);
					outBuffers[i].Attach(outBuffers[a]);
					outBuffers.Remove(a);
					i = 0; 
					break;
				}
			}
		}
	}

	// STEP #2: sort the buffers on number of influences
	// in just one line, isn't that cool? :) because of the < and == operator inside the HwShaderBuffer
	outBuffers.Sort();


	// STEP #3: group primitives inside the buffers
	for (i = 0; i < outBuffers.GetLength(); i++)
		outBuffers[i].OptimizePrimitives();

	//MCore::LOG("***** BEFORE=%d   AFTER=%d", startBuffers, outBuffers.GetLength());
	/*
		//-------------------------------------
		LOG("AFTER BUFFER OPTIMIZATION:");
		LOG("Generated buffers = %d", outBuffers.GetLength());
		totalVerts = 0;
		totalIndices = 0;
		for (i=0; i<outBuffers.GetLength(); i++)
		{
			LOG("Buffer #%d/%d: infl=%d | bones=%d | verts=%d | indices=%d | prims=%d", i, outBuffers.GetLength(), outBuffers[i].GetNumInfluences(), outBuffers[i].GetNumBones(), outBuffers[i].GetNumVertices(), outBuffers[i].GetNumIndices(), outBuffers[i].GetNumPrimitives());
			totalVerts += outBuffers[i].GetNumVertices();
			totalIndices += outBuffers[i].GetNumIndices();
		}
		LOG("totalVerts = %d", totalVerts);
		LOG("totalIndices = %d (%d faces)", totalIndices, totalIndices / 3);*/
}


void Actor::CalcMeshTotals(const int lodLevel,int * outNumVertices,int * outNumIndices)
{
	// the totals
	int totalVerts = 0;
	int totalIndices = 0;

	// for all nodes
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++) {
		Node * node = mNodes[i];
		Mesh * mesh = node->GetMesh(lodLevel);

		// if there is no mesh at this LOD level, skip to the next node
		if (mesh == NULL)
			continue;

		// sum the values to the totals
		totalVerts += mesh->GetNumVertices();
		totalIndices += mesh->GetNumIndices();
	}

	// output the number of vertices
	if (outNumVertices)
		*outNumVertices = totalVerts;

	// output the number of indices
	if (outNumIndices)
		*outNumIndices = totalIndices;
}


void Actor::CalcStaticMeshTotals(const int lodLevel,int * outNumVertices,int * outNumIndices)
{
	// the totals
	int totalVerts = 0;
	int totalIndices = 0;

	// for all nodes
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++) {
		Node * node = mNodes[i];
		Mesh * mesh = node->GetMesh(lodLevel);

		// if there is no mesh at this LOD level, skip to the next node
		if (mesh == NULL)
			continue;

		// the node is dynamic, and we only want static meshes, so skip to the next node
		if (node->HasDeformableMesh(lodLevel))
			continue;

		// sum the values to the totals
		totalVerts += mesh->GetNumVertices();
		totalIndices += mesh->GetNumIndices();
	}

	// output the number of vertices
	if (outNumVertices)
		*outNumVertices = totalVerts;

	// output the number of indices
	if (outNumIndices)
		*outNumIndices = totalIndices;
}


void Actor::CalcDeformableMeshTotals(const int lodLevel,int * outNumVertices,int * outNumIndices)
{
	// the totals
	int totalVerts = 0;
	int totalIndices = 0;

	// for all nodes
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++) {
		Node * node = mNodes[i];
		Mesh * mesh = node->GetMesh(lodLevel);

		// if there is no mesh at this LOD level, skip to the next node
		if (mesh == NULL)
			continue;

		// the node is not dynamic (so static), and we only want dynamic meshes, so skip to the next node
		if (!node->HasDeformableMesh(lodLevel))
			continue;

		// sum the values to the totals
		totalVerts += mesh->GetNumVertices();
		totalIndices += mesh->GetNumIndices();
	}

	// output the number of vertices
	if (outNumVertices)
		*outNumVertices = totalVerts;

	// output the number of indices
	if (outNumIndices)
		*outNumIndices = totalIndices;
}


// set new collision system
void Actor::SetActorCollisionSystem(ActorCollisionSystem * collisionSystem)
{
	if (collisionSystem != NULL) {
		// if there is a collision system delete it
		if (mActorCollisionSystem != NULL)
			delete mActorCollisionSystem;

		mActorCollisionSystem = collisionSystem;
	}
}


// returns the maximum number of weights/influences for this mesh
int Actor::CalcMaxNumInfluences(const int lodLevel)
{
	int maxInfluences = 0;
	Array<Mesh *> validMeshes;

	// add the actor's mesh
	if (GetMesh(lodLevel))
		validMeshes.Add(GetMesh(lodLevel));

	// add the nodes' meshes to the array
	int i;
	for (i = 0; i < GetNumNodes(); i++) {
		// get the node
		Node * node = GetNode(i);

		if (node) {
			// add the mesh
			if (node->GetMesh(lodLevel))
				validMeshes.Add(node->GetMesh(lodLevel));
		}
	}

	// iterate through the meshes and take the max number of influences
	for (i = 0; i < validMeshes.GetLength(); i++)
		maxInfluences = Max(maxInfluences, validMeshes[i]->CalcMaxNumInfluences());

	// return the maximum number of influences
	return maxInfluences;
}


// returns the maximum number of weights/influences for this mesh plus some extra information
int Actor::CalcMaxNumInfluences(const int lodLevel,Array<int> & vertexCounts)
{
	int maxInfluences = 0;
	Array<Mesh *> validMeshes;

	// reset vertex count array
	vertexCounts.Clear();

	// resize the vertex count array, min size must be 1, for nonskinned objects
	vertexCounts.Resize(CalcMaxNumInfluences(lodLevel) + 1);

	// reset values
	for (int k = 0; k < vertexCounts.GetLength(); k++)
		vertexCounts[k] = 0;

	// add the actor's mesh
	if (GetMesh(lodLevel))
		validMeshes.Add(GetMesh(lodLevel));

	// add the nodes' meshes to the array
	int i;
	for (i = 0; i < GetNumNodes(); i++) {
		// get the node
		Node * node = GetNode(i);

		if (node) {
			// add the mesh
			if (node->GetMesh(lodLevel))
				validMeshes.Add(node->GetMesh(lodLevel));
		}
	}

	// iterate through the meshes and take the max number of influences
	for (i = 0; i < validMeshes.GetLength(); i++) {
		// vertex count array for this mesh
		Array<int> meshVertexCounts;

		// get max influences for this mesh
		int meshMaxInfluences = validMeshes[i]->CalcMaxNumInfluences(meshVertexCounts);

		// set new max number of influences
		maxInfluences = Max(maxInfluences, meshMaxInfluences);

		// add vertex counts to the output array
		for (int j = 0; j < meshVertexCounts.GetLength(); j++)
			vertexCounts[j] += meshVertexCounts[j];
	}

	// return the maximum number of influences
	return maxInfluences;
}


//-----------------------------------------------------


// add an attachment
int Actor::AddAttachment(Actor * attachment,Node * attachTarget)
{
	assert(attachment != NULL);
	assert(attachTarget != NULL);
	assert(mNodes.Contains(attachTarget));

	// add the attachment
	mAttachments.Add(attachment);

	// link the attachment to the target
	ReLinkAttachment(attachment, attachTarget);

	// return the attachment number
	return mAttachments.GetMaxIndex();
}


// removes a given attachment
void Actor::RemoveAttachment(Actor * attachment,const bool delFromMem)
{
	assert(attachment != NULL);

	// unlink it
	attachment->SetParent(NULL);

	// delete it from memory, when desired
	if (delFromMem)
		delete attachment;

	// remove the attachment
	mAttachments.Remove(attachment);
}


// remove the attachment by index
void Actor::RemoveAttachment(const int nr,const bool delFromMem)
{
	assert(nr < mAttachments.GetLength());

	// unlink it
	mAttachments[nr]->SetParent(NULL);

	// delete it from memory, when desired
	if (delFromMem)
		delete mAttachments[nr];

	// remove the attachment
	mAttachments.Remove(nr);
}


// remove all attachments
void Actor::RemoveAllAttachments(const bool delFromMem)
{
	// continue removing the last array element until they are all gone
	while (mAttachments.GetLength()) {
		// unlink it
		mAttachments.GetLast()->SetParent(NULL);

		if (delFromMem)
			delete mAttachments.GetLast();

		mAttachments.RemoveLast();
	}
}


// relink the attachment
void Actor::ReLinkAttachment(Actor * attachment,Node * attachTarget)
{
	assert(attachment);
	assert(mAttachments.Contains(attachment));	// make sure the attachment remains inside this actor
	assert(mNodes.Contains(attachTarget));		// make sure the atttach target node is part of this actor

	// relink the attachment to the attach target
	// please note that the attachment target does not get the attachment as child
	// this means recursive functions will not directly touch the attachment itself
	attachment->SetParent(attachTarget);
}


// relink the attachment by index
void Actor::ReLinkAttachment(const int nr,Node * attachTarget)
{
	// make sure the atttach target node is part of this actor
	assert(mNodes.Contains(attachTarget));

	// relink the attachment to the attach target
	// please note that the attachment target does not get the attachment as child
	// this means recursive functions will not directly touch the attachment itself
	mAttachments[nr]->SetParent(attachTarget);
}


// check if the mesh at the given LOD is deformable
bool Actor::HasDeformableMesh(const int lodLevel) const
{
	assert(lodLevel < mNumLODs);

	// check if any of the nodes has a deformable mesh
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++)
		if (mNodes[i]->HasDeformableMesh(lodLevel))
			return true;

	// aaaah, no deformable meshes found
	return false;
}


// check if there is any mesh available
bool Actor::HasMesh() const
{
	// check if any of the nodes has a mesh
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++)
		if (mNodes[i]->HasMesh())
			return true;

	// aaaah, no meshes found
	return false;
}


// check if the collision mesh at the given LOD is deformable
bool Actor::HasDeformableCollisionMesh(const int lodLevel) const
{
	assert(lodLevel < mNumLODs);

	// check if any of the nodes has a deformable collision mesh
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++)
		if (mNodes[i]->HasDeformableCollisionMesh(lodLevel))
			return true;

	// aaaah, no deformable meshes found
	return false;
}


// set the facial setup for a given LOD level
void Actor::SetFacialSetup(const int lodLevel,const MCore::Pointer<FacialSetup> & setup)
{
	assert(lodLevel < mFacialSetups.GetLength());
	mFacialSetups[lodLevel] = setup;
}


// get the facial setup for a given LOD level
MCore::Pointer<FacialSetup> & Actor::GetFacialSetup(const int lodLevel)
{
	assert(lodLevel < mFacialSetups.GetLength());
	return mFacialSetups[lodLevel];
}


// remove all facial setups
void Actor::RemoveAllFacialSetups()
{
	// for all LODs
	int i;
	for (i = 0; i < mFacialSetups.GetLength(); i++)
		mFacialSetups[i] = NULL;

	// remove all modifiers from the stacks for each lod in all nodes
	const int numNodes = mNodes.GetLength();
	for (i = 0; i < numNodes; i++) {
		Node * curNode = mNodes[i];

		// process all LOD levels
		for (int lod = 0; lod < curNode->GetNumMeshDeformerStacks(); lod++) {
			// if we have a modifier stack
			if (curNode->GetMeshDeformerStack(lod).GetPointer() != NULL) {
				// remove all smart mesh morph deformers from that mesh deformer stack
				curNode->GetMeshDeformerStack(lod)->RemoveAllDeformersByType(SmartMeshMorphDeformer::TYPE_ID);
			}
		}
	}
}

// calculate the mesh based axis aligned bounding box
void Actor::CalcMeshBasedAABB(MCore::AABB * result,const int lodLevel)
{
	// initialize the bounding box
	result->Init();

	// process all nodes
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++) {
		Node * curNode = mNodes[i];

		// get the mesh
		Mesh * mesh = curNode->GetMesh(lodLevel).GetPointer();
		if (mesh == NULL)
			continue;

		// get the worldspace matrix
		Matrix worldTM = curNode->GetWorldTM();

		// get the position data
		Vector3 * positions = mesh->GetPositions();

		// process all vertices
		const int numVerts = mesh->GetNumVertices();
		for (int v = 0; v < numVerts; v++)
			result->Encapsulate(positions[v] * worldTM);
	}
}


// calculate the collision mesh based axis aligned bounding box
void Actor::CalcCollisionMeshBasedAABB(MCore::AABB * result,const int lodLevel)
{
	// initialize the bounding box
	result->Init();

	// process all nodes
	const int numNodes = mNodes.GetLength();
	for (int i = 0; i < numNodes; i++) {
		Node * curNode = mNodes[i];

		// get the mesh
		Mesh * mesh = curNode->GetCollisionMesh(lodLevel).GetPointer();
		if (mesh == NULL)
			continue;

		// get the worldspace matrix
		Matrix worldTM = curNode->GetWorldTM();

		// get the position data
		Vector3 * positions = mesh->GetPositions();

		// process all vertices
		const int numVerts = mesh->GetNumVertices();
		for (int v = 0; v < numVerts; v++)
			result->Encapsulate(positions[v] * worldTM);
	}
}


// find an attachment by name
Actor * Actor::FindAttachmentByName(const MCore::String & name) const
{
	const int numAttachments = mAttachments.GetLength();
	for (int i = 0; i < numAttachments; i++)
		if (mAttachments[i]->GetName().IsEqual(name))
			return mAttachments[i];

	return NULL;
}


// find the attachment pointer by id
Actor * Actor::FindAttachmentByID(const int id) const
{
	const int numAttachments = mAttachments.GetLength();
	for (int i = 0; i < numAttachments; i++)
		if (mAttachments[i]->GetID() == id)
			return mAttachments[i];

	return NULL;
}


// find the attachment number by name
int Actor::FindAttachmentNumberByName(const MCore::String & name) const
{
	const int numAttachments = mAttachments.GetLength();
	for (int i = 0; i < numAttachments; i++)
		if (mAttachments[i]->GetName().IsEqual(name))
			return i;

	return -1;
}


// find the attachment number by id
int Actor::FindAttachmentNumberByID(const int id)
{
	const int numAttachments = mAttachments.GetLength();
	for (int i = 0; i < numAttachments; i++)
		if (mAttachments[i]->GetID() == id)
			return i;

	return -1;
}


// find the attachment number by pointer
int Actor::FindAttachmentNumberByPointer(Actor * attachment) const
{
	const int numAttachments = mAttachments.GetLength();
	for (int i = 0; i < numAttachments; i++)
		if (mAttachments[i] == attachment)
			return i;

	return -1;
}


// resolve the parent/child relationships
void Actor::RelinkHierarchy()
{
	// retrieve the number of nodes
	const int numNodes = mNodes.GetLength();

	// remove all childs from the actor itself
	RemoveAllChilds();

	// iterate through all nodes and relink the child/parent relationship
	for (int i = 0; i < numNodes; i++) {
		// retrieve the current node
		Node * node = mNodes[i];

		// remove all childs from the current node
		node->RemoveAllChilds();

		// check if it is valid
		if (!node)
			continue;

		// retrieve the parent node
		Node * parent = node->GetParent();

		// do searches if the parent node is valid
		bool foundParent = false;
		if (parent) {
			// iterate through all nodes and try to find the node's parent by name
			for (int j = 0; j < numNodes; j++) {
				// retrieve the current search node
				Node * searchNode = mNodes[j];

				// if the node name (so also its ID) is equal to the original parent name, it means we have found the parent
				if (parent->GetID() == searchNode->GetID()) {
					// assign the new parent to the node and break the loop
					node->SetParent(searchNode);

					// add the new child node
					if (!searchNode->IsNodeChild(node))
						searchNode->AddChild(node);

					foundParent = true;
					break;
				}
			}
		}

		// if there has no parent been found, parent it to the actor and make it a root node
		if (!foundParent) {
			// assign the new parent to the node
			node->SetParent(this);

			// add the new child node
			if (!this->IsNodeChild(node))
				this->AddChild(node);
		}

		/*
					// copy the child array and remove all the childs from the given node
					Array<Node*> childs = node->mChilds;
					node->RemoveAllChilds();
					// retrieve the number of childs of the given node
					const int numChilds = childs.GetLength();
					// iterate through all childs and relink them
					for (int k=0; k<numChilds; k++)
					{
						// retrieve the current child
						Node* child = childs[k];
						bool foundChild = false;
						// iterate through all nodes and try to find the node's child by name
						for (int l=0; l<numNodes; l++)
						{
							// retrieve the current search node
							Node* searchNode = mNodes[l];
							// if the node name (so also its ID) is equal to the original child name, it means we have found the child
							if ( child->GetID() == searchNode->GetID())
							{
								// add the new child to the node
								node->AddChild(searchNode);
								foundChild = true;
								break;
							}
						}
						if (!foundChild)
							LOG("Actor::RerootSkeleton: No valid child found.");
					}
			*/
	}
}


// remove all controllers
void Actor::RemoveAllControllers(const bool delFromMem)
{
	// repeat until all controllers are gone
	while (mControllers.GetLength()) {
		// delete it from memory, if desired
		if (delFromMem)
			delete mControllers.GetLast();

		// remove the last controller
		mControllers.RemoveLast();
	}
}


// remove all post controllers
void Actor::RemoveAllPostControllers(const bool delFromMem)
{
	// repeat until all controllers are gone
	while (mPostControllers.GetLength()) {
		// delete it from memory, if desired
		if (delFromMem)
			delete mPostControllers.GetLast();

		// remove the last controller
		mPostControllers.RemoveLast();
	}
}


// check if the material is used by a mesh of this actor
bool Actor::IsMaterialUsed(const int lodLevel,const int index)
{
	// retrieve the given material smart pointer
	Pointer<Material> material = mMaterials[lodLevel][index];

	// retrieve the number of nodes in the given actor
	const int numNodes = mNodes.GetLength();

	// iterate through all nodes of the actor and check its meshes
	for (int i = 0; i < numNodes; i++) {
		// retrieve the current search node
		Node * node = mNodes[i];

		// retrieve the number of meshes (depending on number of level of details) the node has got
		const int numMeshes = node->GetNumMeshes();

		// check if the level of detail is not bigger than the number of meshes, else we would be out of array range
		if (lodLevel >= numMeshes)
			continue;

		// get the the mesh for the given level of detail from the node
		Pointer<Mesh> mesh = node->GetMesh(lodLevel);

		// check if the mesh is valid
		if (!mesh)
			continue;

		// get the number of sub meshes which are in the current mesh
		const int numSubMeshes = mesh->GetNumSubMeshes();

		// iterate through the sub meshes
		for (int s = 0; s < numSubMeshes; s++) {
			// retrieve the current sub mesh
			SubMesh * subMesh = mesh->GetSubMesh(s);

			// get the current search material based on the given level of detail, the sub mesh's material number and the node's material offset
			Pointer<Material> searchMaterial = mMaterials[lodLevel][(subMesh->GetMaterial() + node->GetMaterialOffset())];

			// check if the search material and the material smart pointer passed as parameter point to the same material object in memory
			// if they are equal return true, this means that the given material is used by a mesh of the actor
			if (material.GetPointer() == searchMaterial.GetPointer())
				return true;
		}
	}

	// return false, this means that no mesh uses the given material
	return false;
}


// remove the given material and reassign all material numbers of the submeshes
void Actor::RemoveMaterial(const int lodLevel,const int index)
{
	assert(lodLevel < mMaterials.GetLength());

	// first of all remove the given material
	mMaterials[lodLevel].Remove(index);

	// now we have to reassign the material numbers of the submeshes since the material order has now been changed
	// retrieve the number of nodes	
	const int numNodes = mNodes.GetLength();

	// iterate through all nodes of the actor
	for (int i = 0; i < numNodes; i++) {
		// retrieve the current node
		Node * node = mNodes[i];

		// retrieve the number of meshes (depending on number of level of details) the node has got
		const int numMeshes = node->GetNumMeshes();

		// check if the level of detail is not bigger than the number of meshes, else we would be out of array range
		if (lodLevel >= numMeshes)
			continue;

		// get the the mesh for the given level of detail from the node
		Pointer<Mesh> mesh = node->GetMesh(lodLevel);

		// check if the mesh is valid
		if (!mesh)
			continue;

		// flag which indicates whether one of the submeshes' material number is bigger than the one which has been removed
		// only the one with bigger material numbers have to be reset, since the smaller ones still point to their true materials
		bool isIndexLower = false;

		// get the number of sub meshes which are in the current mesh
		const int numSubMeshes = mesh->GetNumSubMeshes();

		// iterate through the sub meshes
		for (int s = 0; s < numSubMeshes; s++) {
			// retrieve the current sub mesh
			SubMesh * subMesh = mesh->GetSubMesh(s);

			// check if one of the submeshes' material number is bigger than the one which has been removed
			if (index < (subMesh->GetMaterial() + node->GetMaterialOffset()))
				isIndexLower = true;
		}

		// one of the sub meshes of this mesh has got a bigger material index number than the one which has been removed
		// this means the meshes would now point to a wrong material or their index would be even out of range
		// to fix that we simply decrease their material offset so that they point to the right materials again
		if (isIndexLower)
			node->SetMaterialOffset(node->GetMaterialOffset() - 1);
	}
}


// remove all materials which aren't used by the actor
void Actor::RemoveUnusedMaterials()
{
	// iterate through all level of details and do the checks
	for (int l = 0; l < mNumLODs; l++) {
		int m = 0;
		// since the amount of materials change during this operation we cannot do this with a for loop, we need something more dynamic
		while (m < mMaterials[l].GetLength()) {
			// check wheter the current search material is used by the actor
			if (!IsMaterialUsed(l, m)) {
				// the mesh isn't used by a mesh, ready to remove it
				RemoveMaterial(l, m);
			} else {
				// this one is used by a mesh, let it in and increase the looping variable so that we can check the next material
				m++;
			}
		}
	}
}


// try to find the repositioning node automatically
Node * Actor::FindBestRepositioningNode()
{
	// reset the repositioning node
	Node * result = NULL;

	// the maximum number of children of a root node, the node with the most children
	// will become our repositioning node
	int maxNumChilds = 0;

	// retrieve the number of root nodes of the actor
	const int numRootNodes = GetNumChilds();

	// traverse through all root nodes
	for (int i = 0; i < numRootNodes; ++i) {
		// get the given root node from the actor
		Node * rootNode = GetChild(i);

		// get the number of child nodes recursively
		const int numChildNodes = rootNode->GetNumChildNodesRecursive();

		// if the number of child nodes of this node is bigger than the current max number
		// this is our new candidate for the repositioning node
		if (numChildNodes > maxNumChilds)
			result = rootNode;
	}

	return result;
}


// automatically find and set the best repositioning node
void Actor::AutoSetRepositioningNode()
{
	mRepositioningNode = FindBestRepositioningNode();
}


// extract a bone list
void Actor::ExtractBoneList(const int lodLevel,Array<Node *> * outBoneList)
{
	// clear the existing items
	outBoneList->Clear();

	// for all nodes
	const int numNodes = mNodes.GetLength();
	for (int n = 0; n < numNodes; ++n) {
		Node * node = mNodes[n];
		Mesh * mesh = node->GetMesh(lodLevel).GetPointer();

		// skip nodes without meshes
		if (mesh == NULL)
			continue;

		// find the skinning information, if it doesn't exist, skip to the next node
		SkinningInfoVertexAttributeLayer * skinningLayer = (SkinningInfoVertexAttributeLayer *) mesh->FindSharedVertexAttributeLayer(SkinningInfoVertexAttributeLayer::TYPE_ID);
		if (skinningLayer == NULL)
			continue;

		// iterate through all skinning data
		const int numOrgVerts = mesh->GetNumOrgVertices();
		for (int v = 0; v < numOrgVerts; ++v) {
			// for all influences for this vertex
			const int numInfluences = skinningLayer->GetNumInfluences(v);
			for (int i = 0; i < numInfluences; ++i) {
				// get the bone
				Node * bone = skinningLayer->GetInfluence(v, i).GetBone();

				// check if it is already in the bone list, if not, add it
				if (outBoneList->Contains(bone) == false)
					outBoneList->Add(bone);
			}
		}
	}
}

} // namespace EMotionFX