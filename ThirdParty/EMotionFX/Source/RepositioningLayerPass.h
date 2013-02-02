/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __REPOSITIONINGLAYERPASS_H
#define __REPOSITIONINGLAYERPASS_H

// include required headers
#include "LayerPass.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class MotionLayerSystem;

/**
 * The motion based actor repositioning layer pass.
 * This layer pass is responsible to apply relative movements from the actor's repositioning node to the actor itself,
 * instead of to moving the repositioning node.
 */
class RepositioningLayerPass : public LayerPass
{
	MEMORYOBJECTCATEGORY(RepositioningLayerPass, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MOTIONSYSTEMS);

	public:
		// the unique type ID of this layer pass type
		enum { TYPE_ID = 0x00000002 };

		/**
		 * The constructor.
		 * @param motionLayerSystem The motion layer system where this pass will be added to.
		 */
		RepositioningLayerPass(MotionLayerSystem* motionLayerSystem);

		/**
		 * The destructor.
		 */
		~RepositioningLayerPass();

		/**
		 * Get the unique type ID of the layer pass class.
		 * @result The unique type ID number of this layer pass type.
		 */
		int GetType() const;

		/**
		 * Process the layer pass.
		 * This will move the actor based on the relative movements of the specified motion root node of the actor.
		 * Also the movements are blend together accordingly to the weights and arrangement of the motion layers.
		 */
		void Process();

	private:
		/**
		 * Calculate the relative changes in position, rotation and scale for a given node given two time values.
		 * The time values may not include a loop!
		 * NOTE: currently only calculates the NodeTransform::mPosition value!
		 * @param instance The motion instance we need to extract the position data from (well, from the motions inside the motion instance).
		 * @param rootNode The node to extract the positional changes from.
		 * @param curTime The current time of the motion, in seconds.
		 * @param oldTime The previous (old) time of the motion.
		 * @param outTransform The transformation containing the relative changes between the old and current time.
		 */
		void CalcRelativeTransform(class MotionInstance* instance, class Node* rootNode, const float curTime, const float oldTime, class NodeTransform* outTransform) const;
};

} // namespace EMotionFX


#endif