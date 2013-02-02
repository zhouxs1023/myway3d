/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionSystem.h"
#include "Actor.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MotionSystem::MotionSystem(Actor* actor)
{
	assert( actor != NULL );

	mActor		 = actor;
	mMotionQueue = NULL;

	// create the motion queue
	mMotionQueue = new MotionQueue(mActor, this);
}


// destructor
MotionSystem::~MotionSystem()
{
	// delete the motion infos
	while (mMotionInstances.GetLength())
	{
		delete mMotionInstances.GetLast();
		mMotionInstances.RemoveLast();
	}

	// get rid of the motion queue
	if (mMotionQueue != NULL)
		delete mMotionQueue;
}


// play a given motion on an actor, and return the instance
MotionInstance* MotionSystem::PlayMotion(Motion* motion, PlayBackInfo* info)
{
	DECLARE_FUNCTION(PlayMotion)

	// check some things
	assert(motion != NULL);
	assert(info != NULL);

	if (motion == NULL)
		return NULL;
/*
	// if we want to play a motion which will loop forever (so never ends) and we want to put it on the queue
	if (info->mNumLoops==FOREVER && info->mPlayNow==false)
	{
		// if there is already a motion on the queue, this means the queue would end up in some kind of deadlock
		// because it has to wait until the current motion is finished with playing, before it would start this motion
		// and since that will never happen, the queue won't be processed anymore...
		// so we may simply not allow this to happen.
		if (mMotionQueue->GetNumEntries() > 0)
			throw Exception("Cannot schedule this LOOPING motion to be played later, because there are already motions queued. If we would put this motion on the queue, all motions added later on to the queue will never be processed because this motion is a looping (so never ending) one.", HERE);
	}*/

	// make sure we always mix when using additive blending
	if (info->mBlendMode == BLENDMODE_ADDITIVE && info->mMix == false)
	{
		assert(1==0);	// this shouldn't happen actually, please make sure you always mix additive motions
		info->mMix = true;
	}

	// create the motion instance and add the motion info the this actor
	MotionInstance* motionInst = CreateMotionInstance(motion, info);

	// if we want to play it immediately (so if we do NOT want to schedule it for later on)
	if (info->mPlayNow)
	{
		// start the motion for real! :)
		StartMotion(motionInst, info);
	}
	else
	{
		// schedule the motion, by adding it to the back of the motion queue
		mMotionQueue->AddEntry( MotionQueue::QueueEntry(motionInst, info) );
		motionInst->Pause();
	}

	// return the pointer to the motion info
	return motionInst;
}



// create the motion instance and add the motion info the this actor
MotionInstance* MotionSystem::CreateMotionInstance(Motion* motion, PlayBackInfo* info)
{
	// create the motion instance
	MotionInstance* motionInst = new MotionInstance( motion, mActor );

	// set the motion instance properties
	motionInst->SetPlayMode		( info->GetPlayMode() );
	motionInst->SetFadeTime		( info->mBlendOutTime );
	motionInst->SetMixMode		( info->mMix );
	motionInst->SetMaxLoops		( info->mNumLoops );
	motionInst->SetBlendMode	( info->mBlendMode );
	motionInst->SetPlaySpeed	( info->mPlaySpeed );
	motionInst->SetWeight		( info->mTargetWeight, info->mBlendInTime );
	motionInst->SetPriorityLevel( info->mPriorityLevel );
	motionInst->SetPlayMask		( info->mPlayMask );
	motionInst->SetRepositionAfterLoop( info->mRepositionAfterLoop );

	return motionInst;
}


// remove a given motion instance
bool MotionSystem::RemoveMotionInstance(MotionInstance* instance)
{
	bool isSuccess = true;

	// for all nodes which are part of the actor
	const int numNodes = mActor->GetNumNodes();
	for (int i=0; i<numNodes; i++)
	{
		// remove the motion link for the given motion instance, in the given node
		mActor->GetNode(i)->RemoveLink( instance );
	}

	// remove the motion instance from the actor
	if (!mMotionInstances.Remove( instance ))
		isSuccess = false;

	// delete the motion instance from memory
	delete instance;

	// return if it all worked :)
	return isSuccess;
}


// update motion queue and instances
void MotionSystem::Update(const double timePassed, const bool updateNodes)
{
	// update the motion queue
	mMotionQueue->Update();

	// update the motions
	UpdateMotionInstances(timePassed);
}


// apply the motion mask
void MotionSystem::ApplyMotionMask(NodeTransform* nodeTransform, MotionInstance* instance, Node* node)
{
	EPlayMask mask = instance->GetPlayMask();

	// restore position
	if ((mask & PLAYMASK_POSITION) == false)
		nodeTransform->mPosition = node->GetLocalPos();
	
	// restore rotation
	if ((mask & PLAYMASK_ROTATION) == false)
		nodeTransform->mRotation = node->GetLocalRot();

	// restore scale
	if ((mask & PLAYMASK_SCALE) == false)
		nodeTransform->mScale = node->GetLocalScale();
}

} // namespace EMotionFX