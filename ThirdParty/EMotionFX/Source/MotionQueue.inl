/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MotionQueue.h"


inline void MotionQueue::ClearAllEntries()
{
	while (mEntries.GetLength())
		RemoveEntry(0);
}


inline void MotionQueue::AddEntry(const MotionQueue::QueueEntry& motion)
{
	mEntries.Add(motion);
}


inline int MotionQueue::GetNumEntries() const
{
	return mEntries.GetLength();
}


inline MotionQueue::QueueEntry& MotionQueue::GetFirstEntry()
{
	assert(mEntries.GetLength() > 0);
	return mEntries[0];
}


inline void MotionQueue::RemoveFirstEntry()
{
	mEntries.RemoveFirst();
}


inline MotionQueue::QueueEntry& MotionQueue::GetEntry(const int nr)
{
	assert(nr < mEntries.GetLength());
	return mEntries[nr];
}
