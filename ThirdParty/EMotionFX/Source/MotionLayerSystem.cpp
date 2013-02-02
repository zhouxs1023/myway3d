/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionLayerSystem.h"
#include "Actor.h"
#include "PlayBackInfo.h"
#include "FacialLayerPass.h"
#include "RepositioningLayerPass.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MotionLayerSystem::MotionLayerSystem(Actor* actor) : MotionSystem(actor)
{
	mMotionTree = NULL;

	// add the facial layer pass to ensure smooth blending between facial motions
	AddLayerPass( new FacialLayerPass(this) );

	// add the motion based actor repositioning layer pass
	AddLayerPass( new RepositioningLayerPass(this) );
}


// destructor
MotionLayerSystem::~MotionLayerSystem()
{
	// delete the motion tree
	if (mMotionTree)
	{
		mMotionTree->DeleteChilds();
		delete mMotionTree;
		SetMotionTree(NULL);
	}

	// remove all layer passes
	RemoveAllLayerPasses();
}


// remove all layer passes
void MotionLayerSystem::RemoveAllLayerPasses(bool delFromMem)
{
	while (mLayerPasses.GetLength())
	{
		if (delFromMem)
			delete mLayerPasses.GetLast();

		mLayerPasses.RemoveLast();
	}
}


// start playing a motion
void MotionLayerSystem::StartMotion(MotionInstance* motion, PlayBackInfo* info)
{
	// create the motion links
	motion->GetMotion()->CreateMotionLinks( mActor, motion );

	// add the motion info the this actor
	mMotionInstances.Add( motion );

	// make sure it is not in pause mode
	motion->UnPause();

	// if there is no motion tree yet (the layer system)
	if (mMotionTree == NULL)
	{
		// create the motion tree root layer
		motion->SetWeight(info->mTargetWeight, info->mBlendInTime);
		SetMotionTree( new MotionLayer(mActor, this, motion, (MotionLayer*)NULL) );
	}
	else
	{
		MotionLayer* insertPos = FindInsertPos( info->mPriorityLevel );

		if (insertPos != NULL)
		{
			// get the parent of the layer to insert above
			MotionLayer* parent = insertPos->GetParent();

			// create the new layer
			MotionLayer* newLayer = new MotionLayer(mActor, this, motion, insertPos, parent);

			// insert the new layer between the parent and the insertion position layer
			insertPos->SetParent( newLayer );

			if (parent)
				parent->SetDestLayer( newLayer );

			if (mMotionTree == insertPos)
				SetMotionTree( newLayer );
		}
		else
		{
			// find the last layer
			MotionLayer* curLayer = mMotionTree;
			while (curLayer)
			{
				// if the next layer is NULL, we have to insert it below this layer
				if (curLayer->GetDestLayer() == NULL)
				{
					MotionLayer* newLayer = new MotionLayer(mActor, this, motion, NULL, curLayer);
					curLayer->SetDestLayer( newLayer );
					break;
				}

				curLayer = curLayer->GetDestLayer();
			}
		}

		// start the automatic blending process
		motion->SetWeight(info->mTargetWeight, info->mBlendInTime);
	}
}


// find the location where to insert the motion layer
// when NULL is returned, it needs to be inserted at the bottom
// insertion must happen above the returned layer
MotionLayer* MotionLayerSystem::FindInsertPos(const int priorityLevel) const
{
	MotionLayer* curLayer = mMotionTree;

	while (1)
	{
		// if the current layer doesn't exist, we have to insert at the bottom
		if (curLayer == NULL)
			return NULL;

		if (curLayer->GetSource()->GetPriorityLevel() <= priorityLevel)
			return curLayer;
		else
			curLayer = curLayer->GetDestLayer();
	}

	return NULL;
}


// stop playing the motion in <blendTime> seconds
void MotionLayerSystem::StopMotion(MotionInstance* motion, const float blendTime)
{
	// make sure we have a motion tree (so that there are motions playing), else get out of here :)
	if (mMotionTree == NULL) return;

	// find the layer the motion is in
	mMotionTree->StopMotion(motion, blendTime);
}


// return true if there is a motion playing, false if not
bool MotionLayerSystem::IsPlaying() const
{
	return ( mMotionTree != NULL );
}


// update the motion layer system
void MotionLayerSystem::Update(const double timePassed, const bool updateNodes)
{
	// update motion queue and instances (baseclass::Update())
	MotionSystem::Update(timePassed, updateNodes);

	// update the motion tree (automatic layer removement, weight updates etc.)
	if (mMotionTree) 
		mMotionTree->Update(timePassed);		

	// if we don't want to update the nodes (heavy calcs) but only update the time values, then it's time to return :)
	if (updateNodes)
	{
		// update nodes' transformation data
		UpdateNodes();

		// process all layer passes
		const int numPasses = mLayerPasses.GetLength();
		for (int i=0; i<numPasses; i++)
			mLayerPasses[i]->Process();
	}
}


// recursively find the first layer (top down search) which has a motion layer which is not in mixing mode
MotionInstance* MotionLayerSystem::FindFirstNonMixingMotionInst()
{
	return mMotionTree->FindFirstNonMixingMotionInst();
}


// try to find the motion layer for a given motion, and return NULL when not located
MotionLayer* MotionLayerSystem::FindMotionLayer(MotionInstance *motion)
{
	if (mMotionTree == NULL)
		return NULL;

	// recursively find
	return mMotionTree->FindMotion(motion);
}


// remove the given layer from the motion tree (and from memory if desired)
void MotionLayerSystem::RemoveLayer(MotionLayer* layer, const bool delFromMem)
{
	// make sure this is not a null pointer :)
	assert(layer != NULL);

	// if this is the first layer
	if (layer == mMotionTree && layer->GetDestLayer()==NULL)
	{
		if (delFromMem)
			delete layer;

		SetMotionTree(NULL);
		return;
	}


	// if this layer has a parent
	if (layer->GetParent())
	{
		MotionLayer* parent = layer->GetParent();

		// link the dest layer to the parent of this layer
		if (layer->GetDestLayer())
			layer->GetDestLayer()->SetParent( parent );

		// if the parent has a dest input layer
		if (parent->GetDestLayer())
		{
			// if the parent dest layer is the layer we want to delete
			if (parent->GetDestLayer() == layer)
			{
				// if the layer has a dest layer, set the parents dest to the dest of the layer to delete
				if (layer->GetDestLayer())
					parent->SetDestLayer( layer->GetDestLayer() );
				else
					parent->SetDestLayer(NULL);
			}
		}
	}
	else // the layer to delete has NO parent, so it's the root layer
	{
		// if this layer has no parent, it MUST be the root of the tree (the layer on top of hierarchy)
		// if not, this layer isn't in the hierarchy, you are doing something special :)
		// in that case, don't use this method to remove your layer, but just do a 'delete yourLayer' with an optional 'yourLayer->DeleteChilds' upfront.
		assert(layer == mMotionTree); 

		if (layer->GetDestLayer()) // if the layer to delete has a dest layer
		{
			// set the dest layer as root node
			layer->GetDestLayer()->SetParent(NULL);

			// set it as root in the motion tree
			SetMotionTree( layer->GetDestLayer() );
		}
	}

	// delete it from memory (does NOT delete the child layers ofcourse)
	if (delFromMem)
		delete layer;
}


// update the node transformations
void MotionLayerSystem::UpdateNodes()
{
	// no motion actions
	if (mMotionTree == NULL)	// TODO: check if this is valid
		return;

	// process all sub nodes
	const int numNodes = mActor->GetNumNodes();
	for (int i=0; i<numNodes; i++)
	{
		Node* node = mActor->GetNode(i);

		if (node->GetNumMotionLinks() > 0)
		{
			// calculate the transformation for the given node
			bool found = false;
			NodeTransform nodeTransform = mMotionTree->CalcNodeTransform(node, found);

			// set the calculated node transformation
			node->SetLocalPos  ( nodeTransform.mPosition );
			node->SetLocalRot  ( nodeTransform.mRotation );
			node->SetLocalScale( nodeTransform.mScale );
		}
		else
		{
			// TODO: uncomment the SetLocals?
			// it however would cause problems when using a FacialMotion::Apply() since that creates no motion links, which
			// result in the node transformations being reset to their original again, which can undo the facial motion apply
			// we should make some update behaviour options/flags i think

			//node->SetLocalPos  ( node->GetOrgPos() );
			//node->SetLocalRot  ( node->GetOrgRot() );
			//node->SetLocalScale( node->GetOrgScale() );
		}
	}
}


// get motion tree root
MotionLayer* MotionLayerSystem::GetMotionTree() const
{
	return mMotionTree;
}


// add a new pass
void MotionLayerSystem::AddLayerPass(LayerPass* newPass)
{
	mLayerPasses.Add( newPass );
}


// get the number of layer passes
int MotionLayerSystem::GetNumLayerPasses() const
{
	return mLayerPasses.GetLength();
}


// remove a given pass
void MotionLayerSystem::RemoveLayerPass(const int nr, bool delFromMem)
{
	if (delFromMem)
		delete mLayerPasses[nr];

	mLayerPasses.Remove( nr );
}


// remove a given pass
void MotionLayerSystem::RemoveLayerPass(LayerPass* pass, bool delFromMem)
{
	mLayerPasses.Remove( pass );

	if (delFromMem)
		delete pass;
}


// insert a layer pass at a given position
void MotionLayerSystem::InsertLayerPass(const int insertPos, LayerPass* pass)
{
	mLayerPasses.Insert(insertPos, pass);
}


// get the motion system type
int MotionLayerSystem::GetType() const
{
	return MotionLayerSystem::TYPE_ID;
}


// get the motion type string
const char* MotionLayerSystem::GetTypeString() const
{
	return "MotionLayerSystem";
}

} // namespace EMotionFX