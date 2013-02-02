/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MOTION_H
#define __MOTION_H

#include "../Core/Source/MCore.h"
#include "MotionPart.h"
#include "NodeTransform.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Node;
class Actor;
class MotionInstance;

/**
 * The motion base class.
 * The unified motion processing system requires all motions to have a base class.
 * This base class is the Motion class (so this one). Different types of motions can be for example
 * skeletal motions (body motions) or facial motions. The main function inside this base class is the method
 * named CalcNodeTransform, which returns the node transformation for a given node.
 */
class Motion
{
	DECLARE_CLASS(Motion);
	MEMORYOBJECTCATEGORY(Motion, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MISC);

	public:
		/**
		 * Constructor.
		 * @param type The unique type identification number of this deformer. Every modificator class has its own unique ID.
		 * @param name The name/description of the motion.
		 */
		Motion(const MCore::String& name);

		/**
		 * Destructor.
		 */
		virtual ~Motion();

		/**
		 * Set the name of the motion.
		 * @param name The name of the motion.
		 */
		inline void SetName(const MCore::String& name);

		/**
		 * Returns the name of the motion.
		 * @result The name of the motion.
		 */
		inline const char* GetName() const;

		/**
		 * Returns the type identification number of the motion class.
		 * @result The type identification number.
		 */
		virtual int GetType() const = 0;

		/**
		 * Gets the type as a description. This for example could be "FacialMotion" or "SkeletalMotion".
		 * @result The string containing the type of the motion.
		 */
		virtual const char* GetTypeString() const = 0;

		/**
		 * Calculates the node transformation of the given node for this motion.
		 * @param actor The actor to apply the motion to.
		 * @param node The node to apply the motion to.
		 * @param timeValue The time value.
		 * @result The node transformation of the node.
		 */
		virtual NodeTransform CalcNodeTransform(Actor* actor, Node* node, const float timeValue) = 0;

		/**
		 * Creates the motion links inside a given actor.
		 * So we know what nodes are effected by what motions, this allows faster updates.
		 * @param actor The actor to create the links in.
		 * @param instance The motion instance to use for this link.
		 */
		virtual void CreateMotionLinks(Actor* actor, MotionInstance* instance) = 0;

		/**
		 * Get the maximum time value of the motion.
		 * @result The maximum time value, in seconds.
		 */
		inline float GetMaxTime() const;

		/**
		 * Update the maximum time of this animation. Normally called after loading a motion. The maximum time is
		 * estimated by iterating through all motion parts searching the biggest time value.
		 */
		virtual void UpdateMaxTime() = 0;

		/**
		 * Returns if this motion type should overwrite body parts that are not touched by a motion
		 * when played in non-mixing mode. Skeletal motions would return true, because if you play a non-mixing
		 * motion you want the complete body to go into the new motion.
		 * However, facial motions only need to adjust the facial bones, so basically they are always played in 
		 * mixing mode. Still, the behaviour of mixing motions is different than motions that are played in non-mixing
		 * mode. If you want to play the motion in non-mixing mode and don't want to reset the nodes that are not influenced
		 * by this motion, the motion type should return false.
		 * @param Returns true if the motion type resets nodes even when they are not touched by the motion, when playing in non-mix mode.
		 *        Otherwise false is returned.
		 */
		virtual bool GetDoesOverwriteInNonMixMode() const = 0;

		/**
		 * Make the motion loopable, by adding a new keyframe at the end of the keytracks.
		 * This added keyframe will have the same value as the first keyframe.
		 * @param fadeTime The relative offset after the last keyframe. If this value is 0.5, it means it will add
		 *                 a keyframe half a second after the last keyframe currently in the keytrack.
		 */
		virtual void MakeLoopable(const float fadeTime = 0.3f) = 0;

	protected:
		MCore::String	mName;		/**< The name or description of this motion. */
		float			mMaxTime;	/**< The maximum playback time of the animation. */
};

// include inline code
#include "Motion.inl"

} // namespace EMotionFX


#endif