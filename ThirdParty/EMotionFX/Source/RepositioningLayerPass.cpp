/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include required headers
#include "RepositioningLayerPass.h"
#include "MotionLayerSystem.h"
#include "Actor.h"
#include "FacialMotion.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// the constructor
RepositioningLayerPass::RepositioningLayerPass(MotionLayerSystem* motionLayerSystem) : LayerPass(motionLayerSystem)
{
}


// the destructor
RepositioningLayerPass::~RepositioningLayerPass() 
{
}


// get the layer pass type
int RepositioningLayerPass::GetType() const
{
	return RepositioningLayerPass::TYPE_ID;
}


// get relative movements between two non-looping time values
void RepositioningLayerPass::CalcRelativeTransform(MotionInstance* instance, Node* rootNode, const float curTime, const float oldTime, NodeTransform* outTransform) const
{
	// get the facial setup of the actor
	Actor* actor = mMotionSystem->GetActor();

	// calculate the two node transformations
	NodeTransform curNodeTransform = instance->GetMotion()->CalcNodeTransform(actor, rootNode, curTime);
	NodeTransform oldNodeTransform = instance->GetMotion()->CalcNodeTransform(actor, rootNode, oldTime);

	// calculate the relative transforms	
	outTransform->mPosition = curNodeTransform.mPosition - oldNodeTransform.mPosition;
	//outTransform->mRotation = oldOrientation.mRotation.Inversed() * curOrientation.mRotation;
	//outTransform->mScale	= curOrientation.mScale - oldOrientation.mScale;
}


// the main function that processes the pass
void RepositioningLayerPass::Process()
{
	// get the repositioning node, and if it hasn't been set, we can already quit this layer pass
	Node* rootNode = mMotionSystem->GetActor()->GetRepositioningNode();
	if (rootNode == NULL)
		return;

	// get the root layer of the motion system
	MotionLayer* rootLayer = mMotionSystem->GetMotionTree();
	if (rootLayer == NULL)
		return;

	// get the facial setup of the actor
	Actor* actor = mMotionSystem->GetActor();

	// check if we actually want repositioning
	if ((int)actor->GetRepositioningMask() == 0)
		return;

	// find the last layer
	MotionLayer* curLayer = rootLayer;
	while (curLayer->GetDestLayer() != NULL)
		curLayer = curLayer->GetDestLayer();

	// the final delta
	Vector3 finalDeltaPos;

	// now curLayer contains the bottom layer
	// we can now work our way up back to the root layer again
	while (curLayer)
	{
		// get the motion instance from the layer
		MotionInstance* motionInstance = curLayer->GetSource();
		Motion* motion = motionInstance->GetMotion();

		// check if this motion influences the root node
		MotionLink* motionLink = rootNode->SearchMotionLink( motion );
		if (motionLink != NULL)
		{
			// get the weight from the motion instance
			const float weight		= motionInstance->GetWeight();
			const float finalWeight = MCore::CalcCosineInterpolationWeight( weight );	// smooth the weight

			// get the current and previous time value from the motion instance
			float curTimeValue = motionInstance->GetCurrentTime();
			float oldTimeValue = motionInstance->GetLastCurrentTime();

			// the delta movement
			Vector3 deltaPos(0.0f, 0.0f, 0.0f);

			// the relative transformation
			NodeTransform relativeTransform;

			// if the motion isn't paused
			if (motionInstance->IsPaused() == false)
			{
				// prevent looping from moving the character back to the origin if this is desired
				if (motionInstance->GetRepositionAfterLoop()==false && motionInstance->HasLooped())
				{
					// if we're playing forward
					if (motionInstance->GetPassedTime() >= 0)
						CalcRelativeTransform(motionInstance, rootNode, motion->GetMaxTime(), oldTimeValue, &relativeTransform);
					else	// we're playing backward
						CalcRelativeTransform(motionInstance, rootNode, 0.0f, oldTimeValue, &relativeTransform);

					// add the relative transform to the final values
					deltaPos += relativeTransform.mPosition;

					// get the start time
					oldTimeValue = motionInstance->GetPlayMode()->GetStartTime();
				}

				// calculate the relative movement
				CalcRelativeTransform(motionInstance, rootNode, curTimeValue, oldTimeValue, &relativeTransform);

				// add the relative transform to the final values
				deltaPos += relativeTransform.mPosition;
			}

			// blend the relative movement
			if (curLayer->GetDestLayer() != NULL)
			{
				if (motionInstance->GetBlendMode() == BLENDMODE_ADDITIVE)
				{
					finalDeltaPos += deltaPos * finalWeight;
				}
				else
				{
					finalDeltaPos = deltaPos * finalWeight + finalDeltaPos * (1.0f - finalWeight);
				}
			}
			else
			{
				finalDeltaPos = deltaPos * finalWeight;
			}
		}

		// go to the layer above the current one (towards the root layer)
		curLayer = curLayer->GetParent();
	}

	// scale the delta movement with the actor scale
	Vector3 scale = actor->GetLocalScale();
	finalDeltaPos.x *= scale.x;
	finalDeltaPos.y *= scale.y;
	finalDeltaPos.z *= scale.z;

	// get the repositioning mask
	Actor::ERepositioningMask mask = actor->GetRepositioningMask();

	// update the actor rotation
	if (mask & Actor::REPOSITION_ROTATION)
	{
		//Quaternion actorRot = actor->GetLocalRot();
		//actorRot = actorRot * finalDeltaRot;
		//actor->SetLocalRot( actorRot );
		actor->SetLocalRot( rootNode->GetLocalRot() );	// absolute
	}

	// update the actor scale
	if (mask & Actor::REPOSITION_SCALE)
		actor->SetLocalScale( rootNode->GetLocalScale() );	// absolute

	// rotate the relative movement with the rotation of the actor
	if ((mask & Actor::REPOSITION_ROTATION) == false)
	{
		Quaternion rot = actor->GetLocalRot();
		finalDeltaPos = rot * finalDeltaPos;
	}

	// reset the root node's transformation, and update the actor position
	if (mask & Actor::REPOSITION_POSITION)
	{
		actor->SetLocalPos( actor->GetLocalPos() + finalDeltaPos );	// relative
		Vector3 newRootPos = (mask & Actor::REPOSITION_ROTATION) ? Vector3(0,0,0) : (rootNode->GetOrgPos()/* * scale*/);
		rootNode->SetLocalPos( newRootPos );
	}

	if (mask & Actor::REPOSITION_ROTATION)
		rootNode->SetLocalRot	( Quaternion(0,0,0,1) );

	if (mask & Actor::REPOSITION_SCALE)
		rootNode->SetLocalScale	( Vector3(1,1,1) );
}


} // namespace EMotionFX