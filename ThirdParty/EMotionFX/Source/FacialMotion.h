/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FACIALMOTION_H
#define __FACIALMOTION_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Motion.h"
#include "KeyTrack.h"
#include "FacialMotionPart.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

// forward declarations
class Actor;
class Node;

/**
 * The facial motion class.
 * This class basically holds a collection of keytracks, which contain the animation data of specific expression parts.
 * The animation data is applied to the expression parts (including phonemes) inside the FacialSetup of a given actor.
 */
class FacialMotion : public Motion
{
	DECLARE_CLASS(FacialMotion);
	MEMORYOBJECTCATEGORY(FacialMotion, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_FACIALMOTIONS);

	public:
		// The facial motion type ID, returned by GetType()
		enum { TYPE_ID = 0x00000002 };

		/**
		 * Constructor.
		 * @param name The name/description of the motion.
		 */
		FacialMotion(const MCore::String& name);

		/**
		 * The destructor.
		 * Automatically gets rid of all expression parts added to this facial animation.
		 */
		~FacialMotion();

		/**
		 * Calculates the node transformation of the given node for this motion.
		 * @param actor The actor where the node belongs to.
		 * @param node The node to calculate the transformation for.
		 * @param timeValue The time value.
		 * @result The node transformation of the node.
		 */
		NodeTransform CalcNodeTransform(Actor* actor, Node* node, const float timeValue);

		/**
		 * Restores all nodes or vertex data touched by this facial animation to their original transformations.
		 * This needs to be done before you call the Apply method. The reason why this needs to be done is that
		 * the Apply method applies the modifications on a relative way. If you call Apply twice, it will apply
		 * the relative changes again to the already relatively displaced objects.
		 * Please note that this method is automatically called by the Update() method.
		 * @param actor The actor to apply the restore to.
		 */
		void RestoreActor(Actor* actor);

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
		 * Manually apply the facial animation to the given actor.
		 * This will first restore the touched parts of the actor to their initial pose, using the RestoreActor method.
		 * After that it will apply all required deformations.
		 * @param actor The actor to apply the facial motion to.
		 * @param updateWeights If set to true, the weights of the expression parts and phonemes will be set to the current keytrack
		 *                      values inside the motion parts. When set to false, they will remain unchanged by this method.
		 */
		void Apply(Actor* actor, const bool updateWeights);

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

		//-------------------

		/**
		 * Get the number of expression motion parts in this motion.
		 * @result The number of motion parts.
		 */
		inline int GetNumExpMotionParts() const;

		/**
		 * Get the ID of a given expression motion part number.
		 * @param motionPartNr The motion part number, which must be in range of [0..GetNumMotionParts()-1].
		 * @result The ID of the motion part.
		 */
		inline int GetExpID(const int motionPartNr) const;

		/**
		 * Get the expression keytrack of a given motion part number.
		 * @param motionPartNr The motion part number, which must be in range of [0..GetNumExpMotionParts()-1].
		 * @result The keytrack, which contains the animation data for a given expression part.
		 */
		inline KeyTrack<float, MCore::Compressed8BitFloat>* GetExpKeyTrack(const int motionPartNr);

		/**
		 * Add an expression motion part.
		 * The integer value in the pair is the unique ID, which is used to link the keytrack to a given expression part or
		 * phoneme (both of type ExpressionPart). The ID is generated from a string, using the NodeIDGenerator class.
		 * @param motionPart The motion part, which will link a keytrack to the correct expression parts.
		 */
		void AddExpMotionPart(FacialMotionPart* motionPart);

		/**
		 * Remove a given expression motion part.
		 * @param nr The motion part number, must be in range of [0..GetNumExpMotionParts()-1].
		 */
		void RemoveExpMotionPart(const int nr);

		/**
		 * Find the expression motion part with a specified ID.
		 * @param id The ID to search for.
		 * @result The expression motion part number, or -1 when no motion part with the given ID could be found inside this motion.
		 */
		int FindExpMotionPart(const int id) const;

		/**
		 * Get an expression part motion part.
		 * @param nr The motion part number.
		 * @result A pointer to the facial motion part.
		 */
		FacialMotionPart* GetExpMotionPart(const int nr);

		//----------

		/**
		 * Get the number of phoneme motion parts in this motion.
		 * @result The number of motion parts.
		 */
		inline int GetNumPhoMotionParts() const;

		/**
		 * Get the ID of a given phoneme motion part number.
		 * @param motionPartNr The motion part number, which must be in range of [0..GetNumPhoMotionParts()-1].
		 * @result The ID of the motion part.
		 */
		inline int GetPhoID(const int motionPartNr) const;

		/**
		 * Get the phoneme keytrack of a given motion part number.
		 * @param motionPartNr The motion part number, which must be in range of [0..GetNumPhoMotionParts()-1].
		 * @result The keytrack, which contains the animation data for a given expression part.
		 */
		inline KeyTrack<float, MCore::Compressed8BitFloat>* GetPhoKeyTrack(const int motionPartNr);

		/**
		 * Add an expression motion part.
		 * The integer value in the pair is the unique ID, which is used to link the keytrack to a given expression part or
		 * phoneme (both of type ExpressionPart). The ID is generated from a string, using the NodeIDGenerator class.
		 * @param motionPart The motion part, which will link a keytrack to the correct expression parts.
		 */
		void AddPhoMotionPart(FacialMotionPart* motionPart);

		/**
		 * Remove a given phoneme motion part.
		 * @param nr The motion part number, must be in range of [0..GetNumPhoMotionParts()-1].
		 */
		void RemovePhoMotionPart(const int nr);

		/**
		 * Find the phoneme motion part with a specified ID.
		 * @param id The ID to search for.
		 * @result The expression motion part number, or -1 when no motion part with the given ID could be found inside this motion.
		 */
		int FindPhoMotionPart(const int id) const;

		/**
		 * Get a phoneme motion part.
		 * @param nr The motion part number.
		 * @result A pointer to the facial motion part.
		 */
		FacialMotionPart* GetPhoMotionPart(const int nr);


	private:
		MCore::Array<FacialMotionPart*>	mExpMotionParts;	/**< The expression motion part. */
		MCore::Array<FacialMotionPart*>	mPhoMotionParts;	/**< The phoneme motion parts. */
};

// include inline code
#include "FacialMotion.inl"

} // namespace EMotionFX

#endif