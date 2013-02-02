/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionSystem.h"


// remove the given motion
inline void MotionSystem::RemoveMotion(const int nr, const bool deleteMem)
{
	assert(nr<mMotionInstances.GetLength());

	if (deleteMem)
		delete mMotionInstances[nr];

	mMotionInstances.Remove(nr);
}


// remove the given motion
inline void MotionSystem::RemoveMotion(MotionInstance* motion, const bool delMem)
{
	assert(motion);

	int nr=mMotionInstances.Find(motion);
	assert(nr!=-1);

	if (nr==-1)
		return;

	RemoveMotion(nr, delMem);
}


// update the motion infos
inline void MotionSystem::UpdateMotionInstances(const double timePassed)
{
	// update all the motion infos
	const int numInstances = mMotionInstances.GetLength();
	for (int i=0; i<numInstances; i++)
		mMotionInstances[i]->Update( timePassed );
}


// check if the given motion instance still exists within the actor, so if it hasn't been deleted from memory yet
inline bool MotionSystem::IsValidMotionInstance(MotionInstance *instance) const
{
	// if it's a null pointer, just return
	if (instance == NULL)
		return false;

	// for all motion instances currently playing in this actor
	const int numInstances = mMotionInstances.GetLength();
	for (int i=0; i<numInstances; i++)
	{
		// check if this one is the one we are searching for, if so, return that it is still valid
		if (mMotionInstances[i] == instance)
			return true;
	}

	// it's not found, this means it has already been deleted from memory and is not valid anymore
	return false;
}


// check if there is a motion instance playing, which is an instance of a specified motion
inline bool MotionSystem::IsPlayingMotion(Motion* motion) const
{
	// if the motion is NULL, return false
	if (motion == NULL)
		return false;

	// for all motion instances currently playing in this actor
	const int numInstances = mMotionInstances.GetLength();
	for (int i=0; i<numInstances; i++)
	{
		// check if the motion instance is an instance of the motion we are searching for
		if (mMotionInstances[i]->GetMotion() == motion)
			return true;
	}

	// it's not found, this means it has already been deleted from memory and is not valid anymore
	return false;
}


// return given motion instance
inline MotionInstance* MotionSystem::GetMotionInstance(const int nr) const
{
	assert(nr < mMotionInstances.GetLength());
	return mMotionInstances[nr];
}


// return number of motion instances
inline int MotionSystem::GetNumMotionInstances() const
{
	return mMotionInstances.GetLength();
}


// set a new motion queue
inline void MotionSystem::SetMotionQueue(MotionQueue* motionQueue)
{
	if (mMotionQueue != NULL)
		delete mMotionQueue;

	mMotionQueue = motionQueue;
}


// add a motion queue to the motion system
inline void MotionSystem::AddMotionQueue(MotionQueue* motionQueue)
{
	assert( motionQueue != NULL );

	// copy entries from the given queue to the motion system's one
	for(int i=0; i<motionQueue->GetNumEntries(); i++)
		mMotionQueue->AddEntry( motionQueue->GetEntry(i) );

	// get rid of the given motion queue
	delete motionQueue;
}


// return motion queue pointer
inline MotionQueue* MotionSystem::GetMotionQueue() const
{
	return mMotionQueue;
}


// return the actor to which this motion system belongs to
inline Actor* MotionSystem::GetActor() const
{
	return mActor;
}
