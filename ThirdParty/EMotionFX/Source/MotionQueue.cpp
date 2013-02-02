/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionQueue.h"
#include "Actor.h"
#include "MotionInstance.h"
#include "MotionLayer.h"
#include "MotionSystem.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MotionQueue::MotionQueue(Actor* actor, MotionSystem* motionSystem)
{
	assert( actor != NULL && motionSystem != NULL );

	mActor			= actor;
	mMotionSystem	= motionSystem;
}


// destructor
MotionQueue::~MotionQueue()
{
	ClearAllEntries();
}


// remove a given entry from the queue
void MotionQueue::RemoveEntry(const int nr)
{
	mMotionSystem->RemoveMotionInstance( mEntries[nr].mMotion );
	mEntries.Remove(nr);
}


// update the motion queue
void MotionQueue::Update()
{
	// get the number of entries
	int numEntries = GetNumEntries();

	// if there are entries in the queue
	if (numEntries == 0)
		return;

	// if there is only one entry in the queue, we can start playing it immediately
	if (!mMotionSystem->IsPlaying())
	{
		// get the entry from the queue to play next
		MotionQueue::QueueEntry queueEntry = GetFirstEntry();

		// remove it from the queue
		RemoveFirstEntry();

		// start the motion on the queue
		mMotionSystem->StartMotion(queueEntry.mMotion, &queueEntry.mPlayInfo);

		// get out of this method, nothing more to do :)
		return;
	}

	// find the first non mixing motion
	MotionInstance* motionInst = mMotionSystem->FindFirstNonMixingMotionInst();

	// if there isn't a non mixing motion
	if (motionInst == NULL) return;

	// start the next motion
	if (motionInst->HasEnded() || motionInst->IsStopping())
	{
		// get the entry from the queue
		MotionQueue::QueueEntry queueEntry = GetFirstEntry();

		// remove it from the queue
		RemoveFirstEntry();

		// start the motion
		mMotionSystem->StartMotion(queueEntry.mMotion, &queueEntry.mPlayInfo);
	}
}

} // namespace EMotionFX