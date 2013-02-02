/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionLayer.h"
#include "Actor.h"
#include "Node.h"
#include "MotionLayerSystem.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MotionLayer::MotionLayer(Actor* actor, MotionLayerSystem* motionSystem, MotionInstance* source, MotionLayer* dest, MotionLayer* parent)
{
	mActor			= actor;
	mMotionSystem	= motionSystem;
	mSource			= source;
	mParent			= parent;
	mDestLayer		= dest;
}


// destructor
MotionLayer::~MotionLayer()
{
	DECLARE_FUNCTION(~MotionLayer)

	// if there is a source motion, we have to remove it from the actor
	// the actor will also remove the motion links inside the nodes automatically
	if (mSource)
		if (!mMotionSystem->RemoveMotionInstance( mSource ))
			LOG_WARNING("Failed to remove the MotionInstance for a layer in the motion tree of actor '" + mActor->GetName() + "'...");
}


// recursively delete all child nodes from memory
void MotionLayer::DeleteChilds()
{ 
	// recursively delete the dest layer
	if (mDestLayer) 
	{ 
		mDestLayer->DeleteChilds(); 
		delete mDestLayer; 
		mDestLayer=NULL;
	}  
}


// update the layer
void MotionLayer::Update(const double timePassed)
{
	// recursive update the dest layer (start on bottom of the tree)
	if (mDestLayer)
		mDestLayer->Update(timePassed);

	// be sure not to remove the motion layer if this is the first nonmixing motion which has ended
	if (mSource && mSource->HasEnded())
	{
		if (mMotionSystem->GetMotionTree())
		{
			MotionInstance* instance = mMotionSystem->GetMotionTree()->FindFirstNonMixingMotionInst();
			if (instance != NULL && instance == mSource)
				return;
		}
	}

	// if we have a source motion
	if (mSource)
	{
		// if it has reached the end
		if (mSource->HasEnded())
		{
			// if it's not yet stopped, stop it
			if (!mSource->IsStopping())
				mMotionSystem->StopMotion(mSource, mSource->GetFadeTime());
		}
	}

	// if we're still blending we didn't reach any ends
	if (mSource->mIsBlending) return;

	if (mSource->mWeight <= 0.0f)
	{
		mMotionSystem->RemoveLayer(this);
		return;
	}

	// if we reached our destination weight value, we have to stop blending
	//if (mWeight <= mDestWeight)
	if (mSource->mWeight >= 1.0f)
	{
		// if there is a dest layer and we are not mixing
		if (mDestLayer && (!mSource->IsMixing() && (mSource->mBlendMode != BLENDMODE_ADDITIVE)))
		{
			const bool overwrite = mSource->GetMotion()->GetDoesOverwriteInNonMixMode();

			// then we can delete the complete hierarchy below this layer, since we are fully blended
			// into this layer, which overwrites the complete other transformations
			// so the other layers won't have any influence anymore
			if (overwrite)
			{
				mDestLayer->DeleteChilds();
				mMotionSystem->RemoveLayer(mDestLayer);
			}
			else // remove only motions of the same type
			{
				RemoveLayersBelow( mSource->GetMotion()->GetType() );
			}
		}
	}
}


// remove all motion layers below this layer
void MotionLayer::RemoveLayersBelow(const int motionType)
{
	// find the bottom layer
	MotionLayer* lastLayer = this;
	while (lastLayer->GetDestLayer())
		lastLayer = lastLayer->GetDestLayer();

	// traverse all layers from bottom towards the start layer
	while (lastLayer != this)
	{
		// if the current layer has a motion of the motion type we need to delete
		if (lastLayer->GetSource()->GetMotion()->GetType() == motionType)
		{
			MotionLayer* toDelete = lastLayer;
			lastLayer = lastLayer->GetParent();
			mMotionSystem->RemoveLayer( toDelete );
		}
		else
			lastLayer = lastLayer->GetParent();
	}
}


// stop a given motion (recursive function)
void MotionLayer::StopMotion(MotionInstance* motion, const float blendTime)
{
	// if this motion layer contains the motion we want to stop
	if (mSource == motion)
	{
		// start the fadeout
		mSource->SetWeight(0.0f, blendTime);

		// we are stopping the motion
		if (blendTime > 0)	// if the blendtime would be 0, the motion would already have been deleted now
			mSource->mIsStopping = true;
	}
	else // recurse in the tree
	{
		if (mDestLayer)
			mDestLayer->StopMotion(motion, blendTime);
	}
}


// returns an node transform for a given node (recursive)
NodeTransform MotionLayer::CalcNodeTransform(Node* node, bool& found)
{
	// the needed node transformations
	NodeTransform result;
	NodeTransform source;
	NodeTransform dest;

	// some status flags
	bool sourceFound = false;
	bool destFound	 = false;
	bool tempFound	 = false;

	// we always found an node transform
	found = true;

	// get the source node transform
	if (node->SearchMotionLink(mSource) != NULL)
	{
		// calculate the source transformation
		source = mSource->GetMotion()->CalcNodeTransform(mActor, node, mSource->GetCurrentTime());
		sourceFound = true;
	}
	else
	{
		// if this is not a mixing motion
		if (mSource->IsMixing() == false)
		{
			const bool overwrite = mSource->GetMotion()->GetDoesOverwriteInNonMixMode();
			if (overwrite)
			{
				source.mPosition	= node->GetOrgPos();
				source.mRotation	= node->GetOrgRot();
				source.mScale		= node->GetOrgScale();
				sourceFound			= true;
			}
		}
	}

	// apply the motion mask
	mMotionSystem->ApplyMotionMask(&source, mSource, node);

	// if there is a dest layer
	if (mDestLayer)
	{
		dest = mDestLayer->CalcNodeTransform(node, tempFound);	// recursively get the transformation, so we start at the end of the tree (the leafs)
		destFound = tempFound;
	}
	else
	{
		dest.mPosition		= node->GetOrgPos();
		dest.mRotation		= node->GetOrgRot();
		dest.mScale			= node->GetOrgScale();
		destFound			= true;		
	}


	// blend between the source and dest
	if (sourceFound && destFound)
	{
		// calculate the blend value, based on cosine interpolation
		float finalWeight = (1.0f - Math::Cos(mSource->mWeight * Math::PI)) * 0.5f;

		// check if we need to do an additive blend or not
		if ((mDestLayer != NULL) && (mSource->mBlendMode == BLENDMODE_ADDITIVE))
		{
			// calculate the relative changes
			Vector3		relPos		= source.mPosition - node->GetOrgPos();
			Vector3		relScale	= source.mScale - node->GetOrgScale();
			Quaternion	rot			= node->GetOrgRot().Slerp( source.mRotation, finalWeight );
			Quaternion	invRot		= node->GetOrgRot().Inversed();	// TODO: prestore/precalc this in Node?

			// apply relative addition to the result of the previous layer
			result.mPosition = dest.mPosition + (relPos * finalWeight);
			result.mRotation = dest.mRotation * (invRot * rot);
			result.mScale	 = dest.mScale + (relScale * finalWeight);
		}
		else
		{
			// just blend between the previous and current result, so make no additions
			result.mPosition = dest.mPosition + (source.mPosition - dest.mPosition) * finalWeight;
			result.mRotation = dest.mRotation.Slerp(source.mRotation, finalWeight);
			result.mScale	 = dest.mScale + (source.mScale - dest.mScale) * finalWeight;
		}
	}
	else	// only a source or dest
	{
		if (sourceFound)
			return source;
		else
			return dest;
	}

	// return the interpolated result
	return result;
}



// recursively search for a given motion, and return the motionlayer it's on
MotionLayer*	MotionLayer::FindMotion(MotionInstance* motion)
{
	// start at the top and process down the tree
	if (mSource == motion)
		return this;

	// check the dest layer
	if (mDestLayer)
		return mDestLayer->FindMotion(motion);

	return NULL;
}


// recursively find the first layer (top down search) which has a motion layer which is not in mixing mode
MotionInstance* MotionLayer::FindFirstNonMixingMotionInst()
{
	// if the source isn't mixing
	if (mSource && !mSource->IsMixing())
		return mSource;

	// check the dest layer
	if (mDestLayer)
		return mDestLayer->FindFirstNonMixingMotionInst();

	// return the result
	return NULL;
}

} // namespace EMotionFX