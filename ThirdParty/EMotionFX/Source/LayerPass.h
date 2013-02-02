/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __LAYERPASS_H
#define __LAYERPASS_H

#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class MotionLayerSystem;


/**
 * The layer pass base class.
 * Layer passes can be seen as post processes that take the motion layers of the MotionLayerSystem class as input.
 * Sometimes it is needed to manually process blending of some specific properties such as facial expression weights.
 */
class LayerPass
{
	MEMORYOBJECTCATEGORY(LayerPass, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MOTIONSYSTEMS);

	public:
		/**
		 * The constructor.
		 * @param motionLayerSystem The motion layer system where this pass will be added to.
		 */
		LayerPass(MotionLayerSystem* motionLayerSystem) 	{ mMotionSystem = motionLayerSystem; }

		/**
		 * The destructor.
		 */
		virtual ~LayerPass() {}

		/**
		 * Get the unique type ID of the layer pass class.
		 * @result The unique type ID number of this layer pass type.
		 */
		virtual int GetType() const = 0;

		/**
		 * Process the layer pass.
		 */
		virtual void Process() = 0;


	protected:
		MotionLayerSystem*	mMotionSystem;	/**< The motion system where this layer pass works on. */
};


} // namespace EMotionFX

#endif