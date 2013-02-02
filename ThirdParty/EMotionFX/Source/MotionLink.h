/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MOTIONLINK_H
#define __MOTIONLINK_H

#include "../Core/Source/MCore.h"
#include "MotionInstance.h"
#include "MotionPart.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * A motion link.
 * This class links the nodes with the motion parts.
 * Not directly by this class, but it helps forming the link, so that we know what MotionPart effects what Node.
 * MotionLinks are stored inside the Node objects.
 */
class MotionLink
{
	DECLARE_CLASS(MotionLink);
	MEMORYOBJECTCATEGORY(MotionLink, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MOTIONLINKS);

	public:
		/**
		 * Constructor.
		 * @param part The motion part.
		 * @param instance The Motion instance.
		 */
		MotionLink(MotionPart* part, MotionInstance* instance);

		/**
		 * Destructor.
		 */
		~MotionLink();

		/**
		 * Returns the motion part.
		 * @return The motion part.
		 */
		inline MotionPart* GetMotionPart() const;

		/**
		 * Returns the motion info.
		 * @return The motion info.
		 */
		inline MotionInstance* GetMotionInstance() const;


	private:
		MotionPart*		mMotionPart;		/**< The motion part. */
		MotionInstance*	mMotionInstance;	/**< The motion instance. */
};

// include the inline code
#include "MotionLink.inl"

} // namespace EMotionFX


#endif