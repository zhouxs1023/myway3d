/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FACIALMOTIONPART_H
#define __FACIALMOTIONPART_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "KeyTrack.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The facial motion part, which is an individual part of a bigger facial motion.
 * Each facial motion part works on an expression part, which is a part of the face, which can be animated, such as
 * eye blinking. Each facial motion part contains a keytrack, which represents the animation data for the given expression part.
 * Next to that the facial motion part contains a unique ID. All expression parts having the same ID will need to have
 * this motion part to be applied on them. If you want to manually link an expression part to a facial motion part, you
 * can pass as parameter to the constructor of the motion part the value returned by ExpressionPart::GetID().
 */
class FacialMotionPart
{
	MEMORYOBJECTCATEGORY(FacialMotionPart, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_FACIALMOTIONS);

	public:
		/**
		 * The constructor.
		 * @param id The ID of the expression part (ExpressionPart) where you want to link the keyframe data to.
		 */
		FacialMotionPart(const int id) : mID(id)		{ }

		/**
		 * The destructor.
		 */
		~FacialMotionPart()	{}

		/**
		 * Set the new ID of the motion part, which basically relinks it to another expression part.
		 * @param id The new ID.
		 */
		inline void SetID(const int id)				{ mID = id; }

		/**
		 * Get the ID of this motion part.
		 * Expression parts having the same ID will need this motion part to be applied to them.
		 * @result The ID of this motion part.
		 */
		inline int GetID() const					{ return mID; }

		/**
		 * Get a pointer to the keytrack, which contains the animation data.
		 * @result A pointer to the keytrack.
		 */
		inline KeyTrack<float, MCore::Compressed8BitFloat>* GetKeyTrack()	{ return &mKeyTrack; }

	private:
		KeyTrack<float, MCore::Compressed8BitFloat>	mKeyTrack;	/**< The keytrack containing the animation data for a given expression part. */
		int	mID;		/**< The ID of the ExpressionPart where this keytrack should be used on. */
};

	
} // namespace EMotionFX

#endif