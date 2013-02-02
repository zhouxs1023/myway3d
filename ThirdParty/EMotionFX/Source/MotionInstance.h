/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MOTIONINSTANCE_H
#define __MOTIONINSTANCE_H

#include "../Core/Source/MCore.h"
#include "Motion.h"
#include "PlayMode.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
extern enum EPlayMode;
extern enum EMotionBlendMode;
extern enum EPlayMask;


/**
 * The MotionInstance class.
 * Since Motion objects can be shared between different Actors, there needs to be a mechanism which allows this.
 * By introducing this MotionInstance class, we can create instances from Motions, where the instance also contains
 * playback information. This playback information allows us to play the same animation data at different actors
 * all with unique play positions and speeds, etc.
 */
class MotionInstance
{
	DECLARE_CLASS(MotionInstance);
	MEMORYOBJECTCATEGORY(MotionInstance, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MOTIONINSTANCES);
	friend class MotionLayer;

	public:
		/**
		 * The constructor.
		 * @param motion The motion from which this is an instance.
		 * @param actor The actor we are playing this motion instance on.
		 */
		MotionInstance(Motion* motion, Actor* actor);

		/**
		 * The destructor.
		 */
		~MotionInstance();

		/**
		 * Update the motion info.
		 * @param timePassed The time passed, in seconds.
		 */
		void Update(const double timePassed);

		/**
		 * Returns the passed time since the last update.
		 * @result The passed time since the last update, in seconds.
		 */
		inline double GetPassedTime() const;

		/**
		 * Returns the current time in the playback of the motion.
		 * @result The current time, in seconds.
		 */
		inline double GetCurrentTime() const;

		/**
		 * Get the maximum time of this motion.
		 * @result The maximum time of this motion, in seconds.
		 */
		inline double GetMaxTime() const;

		/**
		 * Return the play speed factor (1.0 is normal, 0.5 is half speed, etc.).
		 * @result The play speed factor.
		 */
		inline float GetPlaySpeed() const;

		/**
		 * Returns motion it is using.
		 * @result The original motion it's using.
		 */
		inline Motion* GetMotion() const;

		/**
		 * Set the current time in the animation (automatic wrapping/looping performed when out of range).
		 * Normalized in this case means from 0.0 to 1.0. The maximum time of this animation is then 1.0.
		 * @param newTimeValue The new normalized time value, in seconds.
		 */
		inline void SetCurrentTimeNormalized(const double normalizedTimeValue);

		/**
		 * Returns the current time in the playback of the motion. Normalized in this
		 * case means from 0.0 to 1.0. The maximum time of this animation is then 1.0.
		 * @result The normalized current time, in seconds.
		 */
		inline double GetCurrentTimeNormalized() const;

		/**
		 * Set the current time in the animation (automatic wrapping/looping performed when out of range).
		 * @param time The current time in the animation, in seconds.
		 */
		inline void SetCurrentTime(const double time);

		/**
		 * Get the current time of the previous update.
		 * @result The time value, in seconds, of the current playback time, in the previous update.
		 */
		inline float GetLastCurrentTime() const;

		/**
		 * Set the passed time of the animation (automatic wrapping/looping performed when out of range).
		 * @param time The passed time in the animation, in seconds.
		 */
		inline void SetPassedTime(const double timePassed);

		/**
		 * Set the current play speed (1.0 is normal, 0.5 is half speed, etc.).
		 * @param speed The current play speed (1.0 is normal, 0.5 is half speed, etc.).
		 */
		inline void SetPlaySpeed(const double speed);

		/**
		 * Set the play mode. The play mode implementation must be inherited from PlayMode.
		 * Don't delete the object yourself! After passing the pointer to the function do not touch
		 * the object again!
		 * @param mode A pointer to a play mode object.
		 */
		void SetPlayMode(PlayMode* mode);

		/**
		 * Return play mode.
		 * @result A pointer to the play mode.
		 */
		PlayMode* GetPlayMode() const;

		/**
		 * Set the fade time.
         * @param The fade time.
		 */
		inline void SetFadeTime(const double fadeTime);

		/**
		 * Return the time spend to fade out the motion when it is being stopped automatically
		 * when the motion has reached its end. This will only happen when the play mode is PLAYMODE_ONCE.
		 * @result The fade time, in seconds.
		 */
		inline double GetFadeTime() const;

		/**
		 * Get the motion blending mode of this motion instance.
		 * This describes how the motion gets blend with the other motions being played.
		 * For more information about what the blendmodes exactly do, read the documentation of the SetBlendMode method.
		 * @result The motion blend mode for this motion instance.
		 * @see SetBlendMode
		 */
		inline EMotionBlendMode GetBlendMode() const;

		/**
		 * Returns the current weight of the layer.
		 * This weight is in range of [0..1], where 0 means no influence and 1 means full influence.
		 * @result The current weight value.
		 */
		inline float GetWeight() const;

		/**
		 * Returns the target weight. This is the weight we are blending towards.
		 * If you specified a blendTimeInSeconds of zero in the SetWeight method, the target weight will return the same
		 * as the value returned by GetWeight().
		 * The value returned is in range of [0..1].
		 * @result The target weight value, where we are blending towards.
		 */
		inline float GetTargetWeight() const;
		
		/**
		 * Set the target weight value.
		 * This can be used to smoothly blend towards another weight value.
		 * You specify the new (target) weight value, and the time in seconds in which we should blend into that weight.
		 * A weight value of 0 means no influence, and a weight value of 1 means full influence.
		 * Please keep in mind that motion layers inside the motion layer system will automatically be removed when we are in overwrite motion blend mode
		 * and this motion reaches full influence. In order to prevent this from happening, you can blend towards a weight of for example 0.999. This will not
		 * have any visual difference compared to a weight of 1, but will prevent motion instances and layers from being removed.
		 * The same goes for motion weights of 0. Instead of motion weights of 0, you can use values like 0.001 in these cases.
		 * @param targetWeight The weight value we want to blend towards.
		 * @param blendTimeInSeconds The time, in seconds, in which we should blend from the current weight value into the specified target weight value.
		 */
		void SetWeight(const float targetWeight, const float blendTimeInSeconds=0);

		/**
		 * Set the motion blend mode of this motion instance.
		 * If you want to switch between two different motions, for example going from walk into run, you most likely
		 * want to use the BLENDMODE_OVERWRITE mode. However, there are some cases where overwriting doesn't work well.
		 * Think of a skateboard game, where you play a looping base animation, which is played while moving forwards.
		 * Now on top of this you want to bend the character's upper body to the right. So you play a mixing motion of
		 * the upper body. You now have a character, which is skate boarding, while bending his upper body to the right.
		 * Now imagine you want to have some motion which shoots. You want the character to shoot, while it is skating and
		 * while it is has bend his upper body to the right. If you would use overwrite mode, and the shoot animation adjusts
		 * the bones in the upper body, the motion of the bend will be overwritten. This means you will only see a shoot animation
		 * while skating, and not the desired, shoot, while bending right, while skating.
		 * In order to solve this, you can play the shoot motion additive, on top of the bend and skate motions. EMotion FX will then
		 * add all relative changes (compared to the original pose of the actor) to the current result, which in our case is the
		 * skating, while bending right.
		 * Playing an additive motion in mix-mode, will act the same as playing one in non-mixing mode.
		 * @param mode The blendmode to use. The default blendmode of motion instances are set to BLEND_OVERWRITE.
		 * @see EMotionBlendMode
		 */
		inline void SetBlendMode(const EMotionBlendMode mode);

		/**
		 * Rewinds the motion instance. It sets the current time to 0 seconds.
		 */
		inline void Rewind();

		/**
		 * Check if this motion instance has ended or not.
		 * This will only happen when the play mode is PLAYMODE_ONCE, because a looping motion will of course never end.
		 * @result Returns true when the motion has reached the end of the animation, otherwise false is returned.
		 */
		inline bool HasEnded() const;

		/**
		 * Set the motion to mix mode or not.
		 * @param mix Set to true when the motion has to mix, otherwise set to false.
		 */
		inline void SetMixMode(const bool mix);

		/**
		 * Checks if the motion is currently stopping or not, so if it is fading out.
		 * @result Returns true when the motion is currently stopping, so is fading out.
		 */
		inline bool IsStopping() const;

		/**
		 * Checks if the motion is currently playing or not.
		 * @result Returns true when the motion is playing, otherwise false.
		 */
		inline bool IsPlaying() const;

		/**
		 * Checks if the motion is in mix mode or not.
		 * @result Returns true when the motion is being mixed, otherwise false.
		 */
		inline bool IsMixing() const;

		/**
		 * Checks if the motion is being blended or not.
		 * A blend could be the smooth fade in after starting the motion, or a smooth fade out when stopping the animation.
		 * @result Returns true when the motion is currently in a blend process, otherwise false is returned.
		 */
		inline bool IsBlending() const;

		/**
		 * Pause the motion instance.
		 */
		inline void Pause();

		/**
		 * Unpause the motion instance.
		 */
		inline void UnPause();

		/**
		 * Set the pause mode.
		 * @param pause When true, the motion will be set to pause, else it will be unpaused.
		 */
		inline void SetPause(const bool pause);

		/**
		 * Check if the motion currently is paused or not.
		 * @result Returns true when the motion is in pause mode.
		 */
		inline bool IsPaused() const;

		/**
		 * Set the number of loops the motion should play.
		 * If you want to loop it forever, set the value to -1 or FOREVER (which is defined in Actor.h).
		 * @param numLoops The number of loops the motion should play, or -1 or FOREVER in case it should play forever.
		 */
		inline void SetMaxLoops(const int numLoops);

		/**
		 * Get the number of loops the motion will play.
		 * @result The number of times the motion will be played. This value will be -1 or FOREVER (see Actor.h for the define) in case
		 *         the motion will play forever.
		 * @see IsPlayingForever
		 */
		inline int GetMaxLoops() const;

		/**
		 * Check if the motion has looped since the last update.
		 * This is the case when the number of loops returned by GetNumCurrentLoops is not equal to the number of loops before the
		 * playback mode object has been updated.
		 * @result Returns true when the motion has looped, otherwise false is returned.
		 */
		inline bool HasLooped() const;

		/**
		 * Set the new number of times the motion has been played. Changing this value will misrepresent the exact number.
		 * @param numCurrentLoops The number of times the motion has been played.
		 */
		inline void SetNumCurrentLoops(const int numCurrentLoops);

		/**
		 * Get the number of times the motion currently has been played.
		 * @result The number of times the motion has been completely played.
		 */
		inline int GetNumCurrentLoops() const;

		/**
		 * Check if the motion will play forever or not.
		 * @result Returns true when the motion is looping forever, or false when there will be a moment where it will be stopped.
		 */
		inline bool IsPlayingForever() const;

		/**
		 * Get the actor we are playing this motion instance on.
		 * @result A pointer to the actor where this motion instance is playing on.
		 */
		inline Actor* GetActor() const;

		/**
		 * Get the priority level of the motion instance.
		 * Higher values mean less change on getting overwritten by another motion.
		 * A good example are facial motions being played on a walking character.
		 * You would first play the walk motion, with priority of say 0.
		 * After that you will play a facial motion, with mix mode, and priority level 5 for example.
		 * Now you want to change the walk motion into a run motion. If we did not set the priority level for the
		 * facial motion above 0, the run motion would overwrite the facial motion. But since the facial motion
		 * has got a higher priority, it will not be overwritten by the run motion. If we now want to change the
		 * facial motion with another one, we simply play the facial motion with the same or a higher priority level
		 * as the previous facial motion. So a priority level of 5 or higher would work in the example case.
		 * @result The priority level of the motion instance.
		 */
		inline int GetPriorityLevel() const;

		/**
		 * Set the priority level of the motion instance.
		 * Higher values mean less change on getting overwritten by another motion.
		 * A good example are facial motions being played on a walking character.
		 * You would first play the walk motion, with priority of say 0.
		 * After that you will play a facial motion, with mix mode, and priority level 5 for example.
		 * Now you want to change the walk motion into a run motion. If we did not set the priority level for the
		 * facial motion above 0, the run motion would overwrite the facial motion. But since the facial motion
		 * has got a higher priority, it will not be overwritten by the run motion. If we now want to change the
		 * facial motion with another one, we simply play the facial motion with the same or a higher priority level
		 * as the previous facial motion. So a priority level of 5 or higher would work in the example case.
		 * @result The priority level of the motion instance.
		 */
		inline void SetPriorityLevel(const int priorityLevel);

		/**
		 * Set the play mask. This specifies what transformation components (position, rotation, scale) the motion
		 * is allowed to modify. On default this value is set to PLAYMASK_POSROT, which means it can only adjust
		 * the position and rotation. So the scaling of the motion is being ignored.
		 * @param mask The playback mask. You can OR them together, to create a combination.
		 */
		inline void SetPlayMask(const EPlayMask mask);

		/**
		 * Get the play mask. This specifies what transformation components (position, rotation, scale) the motion
		 * is allowed to modify. On default this value is set to PLAYMASK_POSROT, which means it can only adjust
		 * the position and rotation. So the scaling of the motion is being ignored.
		 * @result The play mask for this motion instance.
		 */
		inline EPlayMask GetPlayMask() const;

		/**
		 * Check if this motion will reposition the actor's position to the position at the start of the motion or not.
		 * When this is disabled, so when this function returns false, it means that the actor will stay at the position where
		 * the motion has ended in the loop. So if you make a walking motion where the actor moves away from the origin, and
		 * this method returns false, the character will keep walking forward, moving the character itself also forward.
		 * When this method would return true, the character will pop back to the start of the animation, so most likely to the origin
		 * each time the loops restarts. On default repositioning to the origin after looping is disabled.
		 * Please beware that repositioning only works when motion based repositioning is enabled. This is enabled when
		 * the actor has specified a repositioning node, from which the movements will be applied to the character.
		 * @result Returns true if repositioning after the loop is enabled, otherwise false is returned.
		 * @see Actor::SetRepositioningNode
		 */
		inline bool GetRepositionAfterLoop() const;

		/**
		 * Enable or disable repositioning of the actor after looping.
		 * For more information about motion based actor repositioning, please see the GetRepositionAfterLoop method.
		 * @param enable Set to true when you want to enable repositioning of the actor after looping.
		 * @see GetRepositionAfterLoop
		 */
		inline void SetRepositionAfterLoop(const bool enable);


	private:
		double				mCurTime;				/**< The current playtime. */
		double				mPassedTime;			/**< Passed time. */
		double				mFadeTime;				/**< Fadeout speed, when playing the animation once. So when it is done playing once, it will fade out in 'mFadeTime' seconds. */
		float				mPlaySpeed;				/**< The playspeed (1.0=normal speed). */
		float				mTargetWeight;			/**< The target weight of the layer, when activating the motion. */
		float				mWeight;				/**< The current weight value, in range of [0..1]. */
		float				mWeightDelta;			/**< The precalculated weight delta value, used during blending between weights. */
		float				mLastCurTime;			/**< The last current time, so the current time in the previous update. */
		int					mCurLoops;				/**< Number of loops it currently has made (so the number of times the motion played already). */
		int					mMaxLoops;				/**< The maximum number of loops, before it has to stop. */
		int					mLastLoops;				/**< The current number of loops in the previous update. */
		int					mPriorityLevel;			/**< The priority level, where higher values mean higher priority. */
		Motion*				mMotion;				/**< The motion. */
		Actor*				mActor;					/**< The parent actor where we are playing this motion instance on. */
		PlayMode*			mPlayMode;				/**< The playback mode. */
		EMotionBlendMode	mBlendMode;				/**< The motion blend mode [default=BLENDMODE_OVERWRITE]. */
		EPlayMask			mPlayMask;				/**< The play mask, which specifies what transform components a motion should adjust [default=PLAYMASK_POSROT]. */
		bool				mIsPaused;				/**< Is the motion in pause mode? */
		bool				mIsStopping;			/**< Is this animation stopping? */
		bool				mIsBlending;			/**< Is it blending? */
		bool				mIsMixing;				/**< Is it mixing? */
		bool				mRepositionAfterLoop;	/**< Should the actor position reset to the origin/first frame after the loop has been finished? (=true) Or should it remain at the position where it ended? (=false). */
};

// include inline code
#include "MotionInstance.inl"

} // namespace EMotionFX


#endif