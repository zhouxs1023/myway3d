/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FACIALSETUP_H
#define __FACIALSETUP_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "ExpressionPart.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The facial setup class, which represents the collection of animatable parts on the face, for both facial expressions and 
 * lipsync. All data to deform the face is stored inside the expression parts, which this class stores.
 * Each actor can have a facial setup for every LOD level. Facial setups can also be shared between different actors.
 * This all makes it possible to store all 'big' data once, and let it be used by many characters.
 * It also splits the actual facial animation data from the data required to deform the face.
 */
class FacialSetup
{
	MEMORYOBJECTCATEGORY(FacialSetup, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_FACIALMOTIONS);

	public:
		/**
		 * The constructor.
		 */
		FacialSetup();

		/**
		 * The destructor. Automatically removes all expression parts and phonemes from memory.
		 */
		~FacialSetup();

		/**
		 * Get the number of expression parts, which are part of this facial setup.
		 * @result The number of expression parts.
		 */
		inline int GetNumExpressionParts() const;

		/**
		 * Get a given expression part.
		 * @param nr The expression part number, must be in range of [0..GetNumExpressionParts()-1].
		 * @result A pointer to the expression part.
		 */
		inline ExpressionPart* GetExpressionPart(const int nr) const;

		/**
		 * Add an expression part to this facial setup.
		 * @param part The expression part to add.
		 */
		void AddExpressionPart(ExpressionPart* part);

		/**
		 * Remove a given expression part.
		 * @param nr The expression part number to remove, must be in range of [0..GetNumExpressionParts()-1].
		 * @param delFromMem When set to true, the expression part will be deleted from memory as well. When false, it will
		 *                   only be removed from the array of expression parts inside this facial setup.
		 */
		void RemoveExpressionPart(const int nr, const bool delFromMem=true);

		/**
		 * Remove a given expression part.
		 * @param part A pointer to the expression part to remove. This expression part must part of this facial setup.
		 * @param delFromMem When set to true, the expression part will be deleted from memory as well. When false, it will
		 *                   only be removed from the array of expression parts inside this facial setup.
		 */
		void RemoveExpressionPart(ExpressionPart* part, const bool delFromMem=true);

		/**
		 * Remove all expression parts inside this facial setup.
		 * They will also be deleted from memory.
		 */
		void RemoveAllExpressionParts();

		/**
		 * Get an expression part by its unique ID.
		 * @param id The ID to search for.
		 * @result A pointer to the expression part, which has the specified ID, or NULL when none could be found.
		 */
		ExpressionPart* GetExpressionPartByID(const int id) const;

		/**
		 * Get an expression part by its unique ID.
		 * @param id The ID to search for.
		 * @result The expression part number, or -1 when not found. You can use the returned number with the method
		 *         GetExpressionPart( nr ) in order to convert it into a direct pointer to the expression part.
		 */
		int GetExpressionPartNumberByID(const int id) const;

		//-----------------

		/**
		 * Get the number of phonemes, which are part of this facial setup.
		 * @result The number of phonemes.
		 */
		inline int GetNumPhonemes() const;

		/**
		 * Get a given phoneme.
		 * @param nr The phoneme number, must be in range of [0..GetNumPhonemes()-1].
		 * @result A pointer to the expression part, which represents the phoneme.
		 */
		inline ExpressionPart* GetPhoneme(const int nr) const;

		/**
		 * Add a phoneme to this facial setup.
		 * @param part The phoneme part to add.
		 */
		void AddPhoneme(ExpressionPart* phoneme);

		/**
		 * Remove a given phoneme.
		 * @param nr The phoneme number to remove, must be in range of [0..GetNumPhonemes()-1].
		 * @param delFromMem When set to true, the phoneme will be deleted from memory as well. When false, it will
		 *                   only be removed from the array of phonemes inside this facial setup.
		 */
		void RemovePhoneme(const int nr, const bool delFromMem=true);

		/**
		 * Remove a given phoneme.
		 * @param phoneme A pointer to the phoneme to remove. The phoneme must be part of this facial setup.
		 * @param delFromMem When set to true, the phoneme will be deleted from memory as well. When false, it will
		 *                   only be removed from the array of phonemes inside this facial setup.
		 */
		void RemovePhoneme(ExpressionPart* phoneme, const bool delFromMem=true);

		/**
		 * Remove all phonemes inside this facial setup.
		 * They will also be deleted from memory.
		 */
		void RemoveAllPhonemes();

		/**
		 * Get a phoneme by its unique ID.
		 * @param id The ID to search for.
		 * @result A pointer to the phoneme, which has the specified ID, or NULL when none could be found.
		 */
		ExpressionPart* GetPhonemeByID(const int id) const;

		/**
		 * Get a phoneme by its unique ID.
		 * @param id The ID to search for.
		 * @result The phoneme number, or -1 when not found. You can use the returned number with the method
		 *         GetPhoneme( nr ) in order to convert it into a direct pointer to the phoneme.
		 */
		int GetPhonemeNumberByID(const int id) const;

	
	protected:
		MCore::Array<ExpressionPart*>	mExpressionParts;	/**< The collection of expression parts. */
		MCore::Array<ExpressionPart*>	mPhonemes;			/**< The collection of phonemes. */
};

// include inline code
#include "FacialSetup.inl"

} // namespace EMotionFX

#endif