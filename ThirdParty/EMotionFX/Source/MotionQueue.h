/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MOTIONQUEUE_H
#define __MOTIONQUEUE_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "MotionInstance.h"
#include "PlayBackInfo.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward delcaration
class Actor;
class MotionInstance;
class MotionSystem;


/**
 * The motion queue is a motion stack which stores and plays the motion, one after
 * each other, if the previous one ended. This can be used for prescriped animations:
 * Imagine you want to let your character walk, and after that play some specific run animation.
 * The motion queue schedules the motions, so that the run animation will be played when the walk
 * animation has been finished. A smooth transition between both motions can be made when desired.
 */
class MotionQueue
{
	DECLARE_CLASS(MotionQueue);
	MEMORYOBJECTCATEGORY(MotionQueue, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MISC);

	public:
		/**
		 * The queue-entry struct.
		 * This contains information about a motion which is scheduled.
		 */
		class QueueEntry
		{	
			public:
				MotionInstance*	mMotion;		/**< The motion instance we want to play. */
				PlayBackInfo	mPlayInfo;		/**< The motion playback settings. */

				/// The default constructor
				QueueEntry() : mMotion(NULL) {}

				/// The extended constructor.
				QueueEntry(MotionInstance* motion, class PlayBackInfo* info) : mMotion(motion), mPlayInfo(*info) {}
		};

		/**
		 * Constructor.
		 * @param actor The actor to which this queue belongs to.
		 * @param motionSystem A pointer to the motion system of the actor.
		 */
		MotionQueue(Actor* actor, MotionSystem* motionSystem);

		/**
		 * Destructor.
		 */
		~MotionQueue();

		/**
		 * Remove all entries from the queue.
		 * Does not delete them from the actor or from memory though.
		 */
		inline void ClearAllEntries();

		/**
		 * Add an entry to the queue.
		 * @param motion The queue entry to add to the back of the queue.
		 */
		inline void AddEntry(const QueueEntry& motion);

		/**
		 * Get the number of entries currently in the queue.
		 * @result The number of entries currently scheduled in the queue.
		 */
		inline int GetNumEntries() const;

		/**
		 * Get the first entry. 
		 * BEWARE: only call when GetNumEntries() returns a value above zero.
		 * @result A reference to the first entry, with write access.
		 */
		inline QueueEntry& GetFirstEntry();

		/**
		 * Remove the first entry from the queueu.
		 * BEWARE: only call when GetNumEntries() returns a value above zero.
		 */
		inline void RemoveFirstEntry();

		/**
		 * Get a specific queue entry.
		 * @param nr The queue entry number to get.
		 * @result A reference to the queue entry, with write access.
		 */
		inline QueueEntry& GetEntry(const int nr);

		/**
		 * Remove a given entry from the queue.
		 * @param nr The entry number to remove from the queue.
		 */
		void RemoveEntry(const int nr);

		/**
		 * Updates the motion queue.
		 * This will automatically start playing the motions on the queue when they have to be played.
		 */
		void Update();


	private:
		MCore::Array<QueueEntry>	mEntries;		/**< The motion queue entries. */
		MotionSystem*				mMotionSystem;	/**< Motion system access pointer. */
		Actor*						mActor;			/**< The actor where this queue works on. */
};

// include inline code
#include "MotionQueue.inl"

} // namespace EMotionFX


#endif