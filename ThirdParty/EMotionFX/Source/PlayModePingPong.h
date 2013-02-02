/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __PLAYMODEPINGPONG_H
#define __PLAYMODEPINGPONG_H

// include the Core system
#include "../Core/Source/MCore.h"

#include "PlayMode.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The pingpong playback mode.
 * This playback mode plays the motion instance in pingpong mode.
 * With pingpong we mean it will first be played forward until the end of the motion, followed
 * by going backwards from the end to the start of the motion. A complete loop from forward and back again
 * counts as one loop.
 */
class PlayModePingPong : public PlayMode
{
	MEMORYOBJECTCATEGORY(PlayModePingPong, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MISC);

	public:
		enum { TYPE_ID = 0x00700003 };

		/**
		 * Default constructor.
		 */
		PlayModePingPong();

		/**
		 * Destructor.
		 */
		~PlayModePingPong();

		/**
		 * Returns the type identification number of the play mode class.
		 * @return The type identification number.
		 */
		int GetType() const;

		/**
		 * Gets the type as a description. This for example could be "PlayModeForward" or "PlayModePingPong".
		 * @return The string containing the type of the play mode.
		 */
		const char* GetTypeString() const;

		/**
		 * Update the motion instance time value and keep track of the current number loops.
		 * Motion instance's time value will be reset when the motion has ended and the loop number will
		 * be increased.
		 * @param timeDelta The time passed in seconds.
		 */
		void UpdateMotionInstance(const double timeDelta);

		/**
		 * Returns the start time of the motion, when it is looped.
		 * A forward playing mode would have a start time of 0, because when it passed the end of the motion, it
		 * should again start at the front. A backward playing motion however will need to return the maximum time
		 * of the motion, which is returned by GetMaxTime().
		 * @result The start time of the motion, when it is looped.
		 */
		float GetStartTime() const;

	private:
		bool mPlayingForward;
};

} // namespace EMotionFX


#endif