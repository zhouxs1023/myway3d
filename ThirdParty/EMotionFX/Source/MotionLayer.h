/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MOTIONLAYER_H
#define __MOTIONLAYER_H

// include the core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "NodeTransform.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Actor;
class MotionInstance;
class Node;
class MotionLayerSystem;


/**
 * The motion layer class.
 * Motion layer basically contains a source motion and dest motion and a blend/weight value.
 * If a layer has a destination, it has OR a motion as dest, OR another layer as dest. On this way
 * a complete tree can be constructed. The same story goes for source of course.
 */
class MotionLayer
{
	DECLARE_CLASS(MotionLayer);
	MEMORYOBJECTCATEGORY(MotionLayer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MOTIONSYSTEMS);

	public:
		/**
		 * The constructor.
		 * @param actor The actor this layer works on.
		 * @param motionSystem A pointer to the motion system of the actor.
		 * @param source The source motion.
		 * @param dest The layer which acts as input, called the dest layer.
		 * @param parent The parent layer.
		 */
		MotionLayer(Actor* actor, MotionLayerSystem* motionSystem, MotionInstance* source, MotionLayer* dest=NULL, MotionLayer* parent=NULL);

		/**
		 * Destructor.
		 */
		~MotionLayer();

		/**
		 * Update the layer (update auto blending etc).
		 * @param timePassed The passed time.
		 */
		void Update(const double timePassed);

		/**
		 * Stop a given motion (recursive function).
		 * @param motion The motion to stop.
		 * @param blendTime The blend time (the time it takes before the motion is faded out), in seconds.
		 */
		void StopMotion(MotionInstance* motion, const float blendTime);

		/**
		 * Return the actor to which this layer belongs to.
		 * @return The actor to which this layer belongs to.
		 */
		inline Actor* GetActor() const;

		/**
		 * Return the motion system to which this layer belongs to.
         * @return A pointer to the motion system to which this layer belongs to
		 */
		inline MotionLayerSystem* GetMotionSystem() const;

		/**
		 * Return the source motion (can be NULL).
		 * @result The source motion (can be NULL).
		 */
		inline MotionInstance* GetSource() const;

		/**
		 * Return the parent layer (NULL in case of a root layer).
		 * @result The parent layer (NULL in case of a root layer).
		 */
		inline MotionLayer* GetParent() const;

		/**
		 * Returns the destination layer (can be NULL).
		 * @result The destination layer (can be NULL).
		 */
		inline MotionLayer* GetDestLayer() const;

		/**
		 * Set the source motion (source layer must now be set to NULL!).
		 * @param source The source motion (source layer must now be set to NULL!).
		 */
		inline void SetSource(MotionInstance* source);

		/**
		 * Set the parent of this layer (to create the hierarchy).
		 * @param parent The parent of this layer (to create the hierarchy).
		 */
		inline void SetParent(MotionLayer* parent);

		/**
		 * Set the dest layer (dest motion must now be set to NULL!).
		 * @param dest The dest layer (dest motion must now be set to NULL!).
		 */
		inline void SetDestLayer(MotionLayer* dest);

		/**
		 * Return an node transformation for a given node at this layer in the hierarchy (recursive method).
		 * Calling this method for the motion tree root layer would result in the final transformation of the node.
		 * @param node The node to get the transformation for.
		 * @param found This method will write 'true' in case we found the transformation for the node, otherwise 'false' will be written in this parameter variable.
		 * @result An node transformation for a given node.
		 */
		NodeTransform CalcNodeTransform(Node* node, bool& found);

		/**
		 * Recursively search for a given motion and return the motionlayer it's on.
		 * @param motion The motion to search for.
		 * @result The motionlayer the motion is on.
		 */
		MotionLayer* FindMotion(MotionInstance* motion);

		/**
		 * Recursively search for the first non mixing motion and return the motion instance.
		 * @result A pointer to the motion instance on this layer.
		 */
		MotionInstance* FindFirstNonMixingMotionInst();

		/**
		 * Recursively delete the child nodes from memory (source and dest layers).
		 */
		void DeleteChilds();

		/**
		 * Removes all layers below this layer that have the specified motion type.
		 * @param motionType All layers that have this motion type (returned by Motion::GetType()) and are below (children) of this layer, will be deleted.
		 */
		void RemoveLayersBelow(const int motionType);


	private:
		Actor*				mActor;				/**< The parent actor (where this layer belongs to). */
		MotionLayerSystem*	mMotionSystem;		/**< Motion system access pointer. */
		MotionInstance*		mSource;			/**< The source motion. */
		MotionLayer*		mParent;			/**< The parent layer, NULL in case of a root node. */
		MotionLayer*		mDestLayer;			/**< The dest layer. */
};

// include the inline code
#include "MotionLayer.inl"

} // namespace EMotionFX


#endif