/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __PLAYBACKINFO_H
#define __PLAYBACKINFO_H

// include Core
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
class PlayMode;


// playback macros
enum	
{
	FOREVER = -1	/**< This value can be used as value for the PlayBackInfo::mNumLoops member. It means an infinite number of loops, so a never ending motion.*/
};


/**
 * The playback mask specifies what parts of the transformation of a node a motion should adjust.
 * This can be used to for example only apply the rotation of a motion on the effected nodes.
 * You can OR these items together to setup the complete mask.
 */
enum EPlayMask
{
	PLAYMASK_POSITION		= 1 << 1,	/**< The motion adjusts the position of nodes. */
	PLAYMASK_ROTATION		= 1 << 2,	/**< The motion adjusts the rotation of nodes. */
	PLAYMASK_SCALE			= 1 << 3,	/**< The motion adjusts the scale of nodes. */
	PLAYMASK_POSROT			= (PLAYMASK_POSITION | PLAYMASK_ROTATION ),					/**< The motion adjusts only position and rotation. */
	PLAYMASK_POSROTSCALE	= (PLAYMASK_POSITION | PLAYMASK_ROTATION | PLAYMASK_SCALE)	/**< The motion adjusts position, rotation and scale. */
};


/**
 * The motion blend modes.
 * These blendmodes describe how the motion gets blend together with other motions currently being played.
 */
enum EMotionBlendMode
{
	BLENDMODE_OVERWRITE	= 0,	/**< Overwrite mode. This can be used to switch from for example walk into run. */
	BLENDMODE_ADDITIVE	= 1		/**< Additive mode. This can be used to add the given motion relatively to the current result. */
};


/**
 * The playback information, which are used when playing a motion on an actor.
 * You have to pass an object of this class to the Actor::PlayMotion(...) method.
 */
class PlayBackInfo
{
	MEMORYOBJECTCATEGORY(PlayBackInfo, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MISC);

	public:
		/**
		 * The constructor.
		 * Automatically initializes all members on the following default values.
		 *
		 * <pre>
		 * Member Name          - Default Value
		 * ---------------------------------
		 * mBlendInTime         - 0.3 (seconds)
		 * mBlendOutTime        - 0.3 (seconds)
		 * mPlaySpeed           - 1.0 (original speed)
		 * mTargetWeight        - 1.0 (fully blend in)
		 * mNumLoops            - FOREVER (equals -1)
		 * mBlendMode           - BLENDMODE_OVERWRITE (overwrites motions)
		 * mPlayMode            - new PlayModeForward() (play forward)
		 * mPlayMask		    - PLAYMASK_POSROT (adjust only position and rotation)
		 * mPlayNow             - true (start playing immediately)
		 * mMix                 - false (non mixing motion)	 
		 * mPriorityLevel	    - 0 (no priority)
		 * mRepositionAfterLoop - false (will start where the motion ended)
		 *
		 * </pre>
		 * You can adjust these settings.
		 */
		PlayBackInfo();

		/**
		 * The destructor.
		 */
		~PlayBackInfo();

		/**
		 * Set the play mode. The play mode implementation must be inherited from PlayMode.
		 * Don't delete the object yourself! After passing the pointer to the function do not touch
		 * the object again!
		 * @param mode A pointer to a play mode object.
		 */
		void SetPlayMode(PlayMode* mode);

		/**
		 * Returns the playback mode handler.
		 * @result A pointer to the play mode object.
		 */
		PlayMode* GetPlayMode() const;

	public:
		float				mBlendInTime;			/**< The time, in seconds, which it will take to fully have blended to the target weight. */
		float				mBlendOutTime;			/**< The time, in seconds, which it takes to smoothly fadeout the motion, after it has been stopped playing. */
		float				mPlaySpeed;				/**< The playback speed factor. A value of 1 stands for the original speed, while for example 2 means twice the original speed. */
		float				mTargetWeight;			/**< The target weight, where 1 means fully active, and 0 means not active at all. */
		int					mNumLoops;				/**< The number of times you want to play this motion. A value of FOREVER (or -1) means it will loop forever. */
		int					mPriorityLevel;			/**< The priority level, the higher this value, the higher priority it has on overwriting other motions. */
		EMotionBlendMode	mBlendMode;				/**< The motion blend mode. Please read the MotionInstance::SetBlendMode(...) method for more information. */
		EPlayMask			mPlayMask;				/**< The playback mask. This can be used to specify what transformation elements can be adjusted by this motion. */
		bool				mMix;					/**< Set to true if you want this motion to mix or not. */
		bool				mPlayNow;				/**< Set to true if you want to start playing the motion right away. If set to false it will be scheduled for later by inserting it into the motion queue. */
		bool				mRepositionAfterLoop;	/**< Set to true when you want the actor to reset to the origin/position at first frame after the loop, or false when it should remain at the current position at the end of the motion. */

	private:
		PlayMode*			mPlayMode;				/**< The playback mode. This describes how you want to play the motion: forwards, backwards, pingpong, etc. */
};

} // namespace EMotionFX

#endif