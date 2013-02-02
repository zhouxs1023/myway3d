/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __EXPRESSIONPART_H
#define __EXPRESSIONPART_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "KeyTrack.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

// forward declarations
class Actor;
class Node;

/**
 * The expression part class, which represents a separately animatable part of a facial expression.
 * This could for example be the eyes. There can be an expression part which controls if the eyes are
 * open or closed. Other examples of expression parts could be a smile, or angry looking eye brows.
 * Expression parts will be applied to an actor. A facial expression is composed out of different expresssion
 * parts. Each expression part has a weight value, which represents how much influence it will have.
 * In case of the eyes, a weight value of 0 would mean that the eyes are opened and a weight value of 1 would
 * mean that the eyes are closed. Each expression part has keyframing support, which makes it possible to animate
 * the weight values, which allows us to create complete facial animation.
 */
class ExpressionPart
{
	DECLARE_CLASS(ExpressionPart);
	MEMORYOBJECTCATEGORY(ExpressionPart, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_FACIALMOTIONS);

	public:
		/**
		 * The phoneme sets, used for lip-sync.
		 */
		enum EPhonemeSet
		{
			NEUTRAL_POSE	= 0,
			M_B_P_X			= 1,
			F_V				= 2,
			ZH_CH_SH_J		= 3,
			NG_G_K_H		= 4,
			IY_Y			= 5,
			S_Z				= 6,
			DH_TH_T_N_D		= 7,
			R_ER			= 8,
			W_UW_UH_AW		= 9,
			AE_AY_AA_IH		= 10,
			AH_AO_EH_AY		= 11,
			L				= 12,
			OW_OY_AA		= 13
		};

		/**
		 * The constructor.
		 * @param parentActor The actor to apply the expression part on.
		 * @param name The unique name of the expression part.
		 */
		ExpressionPart(Actor* parentActor, const MCore::String& name);

		/**
		 * The destructor.
		 */
		virtual ~ExpressionPart();

		/**
		 * Apply a transformation to the given position, rotation and scale, on such a way that this 
		 * expression part adjusts the given transformation data. On this way we can accumulate the effects of 
		 * different expression parts to the same node.
		 * @param actor The actor to apply the transform to.
		 * @param node The node where the given transform info belongs to, so the node which we are adjusting.
		 *             However the node itself will not be modified by this method.
		 * @param position This must contain the initial position, and will be modified inside this method as well.
		 * @param rotation This must contain the initial rotation, and will be modified inside this method as well.
		 * @param scale This must contain the initial scale, and will be modified inside this method as well.
		 *
		 */
		virtual void ApplyTransformation(Actor* actor, Node* node, MCore::Vector3& position, MCore::Quaternion& rotation, MCore::Vector3& scale) = 0;

		/**
		 * Get the unique ID of this expression part.
		 * Just like in the Node class, the ID is generated based on the name (a string).
		 * Every string containing the same text will have the same ID. With this we can reduce from expensive
		 * name compares to simple integer compares.
		 * @result The unique ID of the expression part.
		 */
		inline int GetID() const;

		/**
		 * Get the unique name of the expression part.
		 * @result The name of the expression part.
		 */
		inline const MCore::String& GetName() const;

		/**
		 * Set the weight value of the expression part.
		 * This normally is a value between 0 and 1, but can also be negative or out of this range.
		 * A value of 0 would mean this expression part has no effect. A value of 1 however would mean
		 * that the expression part has full effect. In case we have an expression part which represents
		 * the eyes being closed, a value of 1 would mean that the eyes are fully closed and a value of
		 * 0.5 would mean that the eyes are half closed and half open.
		 * Please note that if the keyframing of this expression part is enabled, the Update method will
		 * overwrite the weight value. So if you want to manually control the weight values, be sure to
		 * disable the keyframing of this expression part, using the method SetKeyframing.
		 * @param newWeight The new weight value to use.
		 */
		inline void SetWeight(const float newWeight);

		/**
		 * Gets the weight value of the expression part.
		 * This normally is a value between 0 and 1, but can also be negative or out of this range.
		 * A value of 0 would mean this expression part has no effect. A value of 1 however would mean
		 * that the expression part has full effect. In case we have an expression part which represents
		 * the eyes being closed, a value of 1 would mean that the eyes are fully closed and a value of
		 * 0.5 would mean that the eyes are half closed and half open.
		 * @param newWeight The new weight value to use.
		 */
		inline float GetWeight() const;

		/**
		 * Set the minimum weight range value of this expression part.
		 * On default this value is zero.
		 * The equations used are: new = current + delta * weight.
		 * Delta is the difference between the original pose and the pose passed to InitFromPose or the extended 
		 * constructor. This means that normally the value for weight has a range of [0..1].
		 * However, this can be changed. The range is used inside LM Studio only.
		 * @param rangeMin The minimum weight value.
		 */
		void SetRangeMin(const float rangeMin);

		/**
		 * Set the maximum weight range value of this expression part.
		 * On default this value is one.
		 * For more information about what exactly this 'range' is, see the method SetRangeMin().
		 * @param rangeMax The maximum weight value.
		 * @see SetRangeMin
		 */
		void SetRangeMax(const float rangeMax);

		/**
		 * Get the minimum weight range value of this expression part.
		 * On default this value is zero.
		 * For more information about what exactly this 'range' is, see the method SetRangeMin().
		 * @result The minumim weight range value.
		 * @see SetRangeMin
		 */
		float GetRangeMin() const;

		/**
		 * Get the maximum weight range value of this expression part.
		 * On default this value is one.
		 * For more information about what exactly this 'range' is, see the method SetRangeMin().
		 * @result The maximum weight range value.
		 * @see SetRangeMin
		 */
		float GetRangeMax() const;

		/**
		 * Get the type of expression part.
		 * You can have different types of expression parts, such as expression parts which work
		 * with bones, or which work with morphing or other techniques.
		 * @result The unique type ID of the expression part.
		 */
		virtual int GetType() const = 0;

		/**
		 * Change the name of the expression part.
		 * This will also automatically update the ID, which is returned by GetID().
		 * @param name The new name of the expression part.
		 */
		void SetName(const MCore::String& name);

		/**
		 * Set the phoneme sets. This is used to determine inside LM Studio, which phoneme sets (for lip-sync) this
		 * phoneme represents.
		 * @param phonemeSets The array with phonemesets which this phoneme represents.
		 */
		void SetPhonemeSets(const MCore::Array<EPhonemeSet>& phonemeSets);

		/**
		 * Get the phoneme sets which this phoneme represents.
		 * This is used to determine inside LM Studio, which phoneme sets (for lip-sync) this phoneme represents.
		 * @result The array with phoneme sets which this phoneme represents.
		 */
		const MCore::Array<EPhonemeSet>& GetPhonemeSets() const;

		/**
		 * Set the phoneme keyboard character. This links a character on the keyboard (like the 'A' or 'E'-key) to 
		 * a phoneme. If the character is set to 0, it means this expression part does NOT act as a phoneme.
		 * @param character The keyboard character to link this phoneme to, or 0 to set it as non-phoneme.
		 */
		void SetPhonemeCharacter(const char character);

		/**
		 * Returns the keyboard character where this phoneme is linked to in the editor.
		 * When this method returns a value of 0, it means this expression part is NOT a phoneme.
		 * @result The keyboard character, or 0 when this expression part doesn't act as phoneme.
		 */
		char GetPhonemeCharacter() const;

		/**
		 * Initializes this expresion part from a given actor representing the pose.
		 * The expression part will filter out all data which is changed compared to the base pose and
		 * store this information on a specific way so it can be used to accumulate multiple expression parts
		 * together and apply them to the actor to which this expression part is attached to.
		 * @param pose The actor representing the pose of the character when the weight value would equal 1.
		 * @param delPoseFromMem When set to true (which is default) the actor passed as first parameter to this
		 *                       method will automatically be deleted from memory when it's no longer needed by this method anymore.
		 *                       So when set to true, you cannot use this actor (pose) anymore, because it is deleted from memory.
		 *                       When set to false, you must not forget to delete this pose actor later on yourself.
		 */
		virtual void InitFromPose(Actor* pose, bool delPoseFromMem=true) = 0;

		/**
		 * Restores the parts of the actor this expression part is working on to their original node transformations.
		 * Only the parts of the actor which are affected by this expression part will be restored.
		 * Restoring means the resetting of for example bone transformations to their initial (neutral/base) pose.
		 * A morphing based expression part will for example reset the vertex positions to the base pose.
		 * @param actor The actor to apply the restore on.
		 */
		virtual void RestoreActor(Actor* actor) = 0;

		/**
		 * Checks if this expression part would influence a given node.
		 * @param node The node to perform the check with.
		 * @result Returns true if the given node will be modified by this expression part, otherwise false is returned.
		 */
		virtual bool Influences(Node* node) const = 0;

		/**
		 * Apply the relative deformations for this expression part to the given actor.
		 * @param actor The actor to apply the deformations on.
		 */
		virtual void Apply(Actor* actor) = 0;

		/**
		 * Calculate the range based weight value from a normalized weight value given by a facial animation key frame.
		 * @param weight The normalized weight value, which must be in range of [0..1].
		 * @return The weight value to be used in calculations. The returned value will be in range of [GetRangeMin()..GetRangeMax()]
		 */
		float CalcRangedWeight(const float weight);

		/**
		 * Check if this expression part is in manual mode or not.
		 * When it is in manual mode, so when this method returns true, this means that motions are not allowed to
		 * overwrite the weight of this expression part. This allows you to manually control the weight value.
		 * @result Returns true when manual mode is enabled, otherwise false is returned.
		 */
		inline bool IsInManualMode() const;

		/**
		 * Enable or disable manual mode.
		 * When it is in manual mode, this means that motions are not allowed to overwrite the weight of this expression part. 
		 * This allows you to manually control the weight value.
		 * On default manual mode is disabled, so setting the weight values when motions are being played will not have any influence in 
		 * that case, because the motions would overwrite your manual weight value.
		 * @param enabled Set to true when you want to enable manual control of the weight value, otherwise set to false.
		 */
		void SetEnableManualMode(const bool enabled);

		/**
		 * Calculates the normalized weight value that is in range of [0..1], on which this expression part would have no influence.
		 * A normalized weight of zero doesn't mean that this expression part has no influence. It is possible that the minimum range value
		 * of the slider is for example -1, while the maximum range would be 1. In that case a weight value of 0, would mean a un-normalized (ranged) weight
		 * of -1. The normalized weight that has a ranged (unnormalized) weight of zero would be 0.5 in this case.
		 * This method calculates the normalized weight value that is in range of [0..1] which would result in a ranged (un-normalized) weight of zero.
		 * @result The normalized weight value that will result in a ranged weight of zero, which would mean no influence.
		 */
		float CalcZeroInfluenceWeight() const;


	protected:
		MCore::String				mName;			/**< The unique name of the expression part. */
		MCore::Array<EPhonemeSet>	mPhonemeSets;	/**< The phoneme sets in case this expresion part is used as a phoneme. (LM Studio) */
		Actor*						mActor;			/**< The actor to apply the expression part on. */
		float						mCurWeight;		/**< The current weight value. */
		float						mRangeMin;		/**< The minimum range of the weight. */
		float						mRangeMax;		/**< The maximum range of the weight. */
		int							mID;			/**< The unique ID of the expression part, calculated from the name. */
		bool						mManualMode;	/**< If motions are allowed to overwrite the weight of this expression part, this is set to false, otherwise true. */
		char						mCharacter;		/**< The character the phoneme is linked to on the keyboard, in case it is a phoneme. (LM Studio) */
};

// include inline code
#include "ExpressionPart.inl"

} // namespace EMotionFX

#endif