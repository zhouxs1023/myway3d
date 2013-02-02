/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FACIALLAYERPASS_H
#define __FACIALLAYERPASS_H

// include required headers
#include "LayerPass.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class MotionLayerSystem;

/**
 * The facial layer pass class.
 * This class acts as a postprocess on the layer blending of the MotionLayerSystem class.
 * It is responsible for the blending and mixing of facial expression parts.
 */
class FacialLayerPass : public LayerPass
{
	MEMORYOBJECTCATEGORY(FacialLayerPass, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MOTIONSYSTEMS);

	public:
		// the unique type ID of this layer pass type
		enum { TYPE_ID = 0x00000001 };

		/**
		 * The constructor.
		 * @param motionLayerSystem The motion layer system where this pass will be added to.
		 */
		FacialLayerPass(MotionLayerSystem* motionLayerSystem);

		/**
		 * The destructor.
		 */
		~FacialLayerPass();

		/**
		 * Get the unique type ID of the layer pass class.
		 * @result The unique type ID number of this layer pass type.
		 */
		int GetType() const;

		/**
		 * Process the layer pass.
		 * This takes care of the blending and mixing of expression part weights, which is needed
		 * to blend in and fade out and make smooth transitions between different facial motions.
		 */
		void Process();
};

} // namespace EMotionFX


#endif