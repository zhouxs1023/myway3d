/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SKELETALMOTION_H
#define __SKELETALMOTION_H

#include "../Core/Source/MCore.h"
#include "Motion.h"
#include "MotionPart.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
class Actor;

/**
 * The motion class.
 * A motion contains a set of motion parts. So this forms a complete skeleton
 * animation. An example of a motion would be a walk animation. Every motion can be shared between
 * different actors. So multiple actors can use the same motion, but still with different playback settings.
 */
class SkeletalMotion : public Motion
{
	DECLARE_CLASS(SkeletalMotion);
	MEMORYOBJECTCATEGORY(SkeletalMotion, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_SKELETALMOTIONS);

	public:
		// The skeletal motion type ID, returned by GetType()
		enum { TYPE_ID = 0x00000001 };

		/**
		 * Constructor.
		 * @param name The name/description of the motion.
		 */
		SkeletalMotion(const MCore::String& name);

		/**
		 * Destructor.
		 */
		~SkeletalMotion();

		/**
		 * Returns the number of motion parts.
		 * @result The number of motion parts.
		 */
		inline int GetNumParts() const;

		/**
		 * Get a given motion part.
		 * @param nr The motion part number.
		 * @result A pointer to the motion part.
		 */
		inline MotionPart* GetPart(const int nr) const;

		/**
		 * Add a given motion part to this motion.
		 * @param part The motion part to add.
		 */
		inline void AddPart(MotionPart* part);

		/**
		 * Delete all motion parts (this will clear the motion).
		 */
		void DeleteParts();

		/**
		 * Calculates the node transformation of the given node for this motion.
		 * @param actor The actor to apply the modifications to (they will not be applied inside this method though).
		 * @param node The node to calculate the transformation for.
		 * @param timeValue The time value.
		 * @result The node transformation of the node.
		 */
		NodeTransform CalcNodeTransform(Actor* actor, Node* node, const float timeValue);

		/**
		 * Returns the type identification number of the motion class.
		 * @result The type identification number.
		 */
		int GetType() const;

		/**
		 * Gets the type as a description.
		 * @result The string containing the type of the motion.
		 */
		const char* GetTypeString() const;

		/**
		 * Creates the motion links inside a given actor.
		 * So we know what nodes are effected by what motions, this allows faster updates.
		 * @param actor The actor to create the links in.
		 * @param instance The motion instance to use for this link.
		 */
		void CreateMotionLinks(Actor* actor, MotionInstance* instance);

		/**
		 * Update the maximum time of this animation. Normally called after loading a motion. The maximum time is
		 * estimated by iterating through all motion parts searching the biggest time value.
		 */
		void UpdateMaxTime();

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
		bool GetDoesOverwriteInNonMixMode() const;

		/**
		 * Make the motion loopable, by adding a new keyframe at the end of the keytracks.
		 * This added keyframe will have the same value as the first keyframe.
		 * @param fadeTime The relative offset after the last keyframe. If this value is 0.5, it means it will add
		 *                 a keyframe half a second after the last keyframe currently in the keytrack.
		 */
		void MakeLoopable(const float fadeTime = 0.3f);


	private:
		MCore::Array<MotionPart*>	mParts;		/**< The motion parts. */
};

// include inline code
#include "SkeletalMotion.inl"

} // namespace EMotionFX


#endif