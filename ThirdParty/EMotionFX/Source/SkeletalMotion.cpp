/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SkeletalMotion.h"
#include "Node.h"
#include "Actor.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
SkeletalMotion::SkeletalMotion(const String& name) : Motion(name)
{
}


// destructor
SkeletalMotion::~SkeletalMotion()
{
	DeleteParts();
}


// returns the type ID of the motion
int SkeletalMotion::GetType() const
{
	return SkeletalMotion::TYPE_ID;
}


// returns the type as string
const char* SkeletalMotion::GetTypeString() const
{
	return "SkeletalMotion";
}


// get rid of all parts
void SkeletalMotion::DeleteParts()
{
	while (mParts.GetLength())
	{
		delete mParts.GetLast();
		mParts.RemoveLast();
	}
}


NodeTransform SkeletalMotion::CalcNodeTransform(Actor* actor, Node* node, const float timeValue)
{
/*	NodeTransform result;

	// search for the motion link
	MotionLink* motionLink = node->SearchMotionLink(this);
	if (motionLink == NULL)
	{
		result.mPosition = node->GetLocalPos();
		result.mRotation = node->GetLocalRot();
		result.mScale	 = node->GetLocalScale();
		return result;
	}

	// get the current time and the motion part
	MotionPart* motionPart = motionLink->GetMotionPart();

	// get position
	if (motionPart->GetPosTrack().GetNumKeys() > 0)
		result.mPosition = motionPart->GetPosTrack().GetValueAtTime( timeValue );
	else
		result.mPosition = motionPart->GetPosePos();

	// get rotation
	if (motionPart->GetRotTrack().GetNumKeys() > 0)
		result.mRotation = motionPart->GetRotTrack().GetValueAtTime( timeValue );
	else
		result.mRotation = motionPart->GetPoseRot();

	// get scale
	if (motionPart->GetScaleTrack().GetNumKeys() > 0)
		result.mScale = motionPart->GetScaleTrack().GetValueAtTime( timeValue );
	else
		result.mScale = motionPart->GetPoseScale();

	// return the result
	return result;*/

	NodeTransform		result;
	MCore::Vector3		Pos;		/// The position.
	MCore::Vector3		Scl;		/// The scale.
 
	// search for the motion link
	MotionLink* motionLink = node->SearchMotionLink(this);
	if (motionLink == NULL)
	{
		result.mPosition = node->GetLocalPos();
		result.mRotation = node->GetLocalRot();
		result.mScale	 = node->GetLocalScale();
		return result;
	}

	// get the current time and the motion part
	MotionPart* motionPart = motionLink->GetMotionPart();

	// get rotation
	if (motionPart->GetRotTrack().GetNumKeys() > 0)
		result.mRotation = motionPart->GetRotTrack().GetValueAtTime( timeValue );
	else
		result.mRotation = motionPart->GetPoseRot();

	// rotŰ
	bool forceUsePose = false;

	if(node->IsRootNode() || node->GetParent()->IsRootNode() || node->IsPropNode()) {
		forceUsePose = true;
	}
	
	MCore::Vector3 basePos, baseScl;
	if (forceUsePose) {
		basePos = motionPart->GetPosePos();
		baseScl = motionPart->GetPoseScale();
	} 
	else {
		basePos = node->GetOrgPos();
		baseScl = node->GetOrgScale();
	}
 
	// get position
	if (motionPart->GetPosTrack().GetNumKeys() > 0) {
		Pos = motionPart->GetPosTrack().GetValueAtTime( timeValue );
		result.mPosition = (basePos + (Pos - motionPart->GetPosePos()));
	} else { 
		result.mPosition = basePos;
	}

	// get scale
	if (motionPart->GetScaleTrack().GetNumKeys() > 0) {
		Scl = motionPart->GetScaleTrack().GetValueAtTime( timeValue );
		result.mScale = (baseScl + (Scl - motionPart->GetPoseScale()));
	}
	else {
		result.mScale = baseScl;
	}

	// return the result
	return result;
}


// creates the  motion links in the nodes of the given actor
void SkeletalMotion::CreateMotionLinks(Actor* actor, MotionInstance* instance)
{
	//LOG("*** Creating links for motion '%s'", instance->GetMotion()->GetName());

	// build a quick array of nodes, copy the array info from the node array of the actor
	Array<Node*> nodeList = actor->GetNodes();

	// try to link the motion parts with the nodes
	const int numParts = mParts.GetLength();
	for (int i=0; i<numParts; i++)
	{
		MotionPart* part = mParts[i];

		//bool hasKeys = (part->GetPosTrack().GetNumKeys()>0 || part->GetRotTrack().GetNumKeys()>0 || part->GetScaleTrack().GetNumKeys()>0);
	
		bool found = false;
		for (int j=0; j<nodeList.GetLength(); j++)
		{
			Node* node = nodeList[j];

			// if the name of the motion part is the same as the name of the node
			//if (hasKeys)
			if (strcmp(part->GetName(), node->GetNamePtr()) == 0) // equal
			{
				node->AddMotionLink( MotionLink(part, instance) );
				nodeList.SwapRemove(j);
				found = true;
				break;
			}
		}
	}
}


// get the maximum time value and update the stored one
void SkeletalMotion::UpdateMaxTime()
{
	// for all motion parts
	const int numParts = mParts.GetLength();
	for (int i=0; i<numParts; i++)
	{
		MotionPart* part = mParts[i];

		// check the position track
		if (part->GetPosTrack().GetLastTime() > mMaxTime)
			mMaxTime = part->GetPosTrack().GetLastTime();

		// check the rotation track
		if (part->GetRotTrack().GetLastTime() > mMaxTime)
			mMaxTime = part->GetRotTrack().GetLastTime();

		// check the scale track
		if (part->GetScaleTrack().GetLastTime() > mMaxTime)
			mMaxTime = part->GetScaleTrack().GetLastTime();
	}	
}


// returns if we want to overwrite nodes in non-mixing mode or not
bool SkeletalMotion::GetDoesOverwriteInNonMixMode() const
{
	return true;
}


// make the motion loopable
void SkeletalMotion::MakeLoopable(const float fadeTime)
{
	// make all tracks of all motin parts loopable
	const int numParts = mParts.GetLength();
	for (int i=0; i<numParts; i++)
	{
		MotionPart* part = mParts[i];
		part->GetPosTrack().MakeLoopable( fadeTime );
		part->GetRotTrack().MakeLoopable( fadeTime );
		part->GetScaleTrack().MakeLoopable( fadeTime );
	}

	// update the maximum time
	UpdateMaxTime();
}



} // namespace EMotionFX