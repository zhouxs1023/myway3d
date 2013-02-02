/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __PLAYMODE_H
#define __PLAYMODE_H

#include "../Core/Source/MCore.h"
#include "PlayBackInfo.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class MotionInstance;

/**
 * Play mode base class. This class represents the playback mode of a motion.
 * Each motion instance holds a play mode class that keeps track of the motion instance's time value
 * and the loop count. The update function will operate on the passed motion instance by adjusting the current
 * time value of this motion instance.
 * The MotionInstance object is responsible for deleting the play mode object from memory.
 * This comes down to the fact that you do not need to delete the allocated play mode, because this happens
 * automatically when the motion has stopped playing.
 */
class PlayMode
{
	MEMORYOBJECTCATEGORY(PlayMode, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MISC);

	public:
		/**
		 * Default constructor.
		 */
		PlayMode();

		/**
		 * Destructor.
		 */
		virtual ~PlayMode();

		/**
		 * Returns the type identification number of the play mode class.
		 * @return The type identification number.
		 */
		virtual int GetType() const = 0;

		/**
		 * Gets the type as a description. This for example could be "PlayModeForward" or "PlayModePingPong".
		 * @return The string containing the type of the play mode.
		 */
		virtual const char* GetTypeString() const = 0;

		/**
		 * Update the motion instance time value and keep track of the current number loops.
		 * Motion instance's time value will be reset when the motion has ended and the loop number will
		 * be increased.
		 * @param timeDelta The time passed in seconds.
		 */
		virtual void UpdateMotionInstance(const double timeDelta) = 0;

		/**
		 * Returns the start time of the motion, when it is looped.
		 * A forward playing mode would have a start time of 0, because when it passed the end of the motion, it
		 * should again start at the front. A backward playing motion however will need to return the maximum time
		 * of the motion, which is returned by GetMaxTime().
		 * @result The start time of the motion, when it is looped.
		 */
		virtual float GetStartTime() const = 0;

		/**
		 * Set the motion instance on which this play mode is operating.
		 * @motionInst The motion instance to which this play mode belongs to.
		 */
		void SetMotionInstance(MotionInstance* motionInst);

		/**
		 * Return the motion instance on which this play mode is operating.
		 * @return A pointer to the motion instance to which this play mode belongs to.
		 */
		MotionInstance* GetMotionInstance() const;


	protected:
		MotionInstance* mMotionInst; /**< The motion instance to operate on. */
};

} // namespace EMotionFX


#endif