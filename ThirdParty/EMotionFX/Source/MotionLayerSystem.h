/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MOTIONLAYERSYSTEM_H
#define __MOTIONLAYERSYSTEM_H

#include "MotionSystem.h"
#include "MotionLayer.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class LayerPass;

/**
 * The layered motion system class.
 * This class performs the blending, mixing and motion management for us.
 * The system contains a set of layers, which are linked together in a hierarchy.
 * The current implementation however manages the system on such a way that we get a stack of layers.
 * The following diagram gives an example of the layout of layers inside this system:
 *
 * <pre>
 *
 *   FINAL OUTPUT
 *        ^
 *        |
 *        |
 *   75%  |  25%
 * ---------------
 * | lay2 | run  |  layer 1 (root layer)
 * ---------------
 *   \         
 *    \
 *     \
 *  30% \   70%
 * ------\--------
 * | jump | walk |  layer 2
 * ---------------
 *   \         
 *    \
 *     \
 *      \   100%
 * ------\--------
 * |      | jump |  layer 3
 * ---------------
 * </pre>
 *
 * As you can see a layer consists of 2 inputs and a weight factor between these two inputs.
 * An input can either be a motion or the output of another layer.
 * When a motion is being played we don't have to bother about the adding and removing of layers
 * and things such as smooth transitions etc. This is all performed by this system.
 */
class MotionLayerSystem : public MotionSystem
{
	DECLARE_CLASS(MotionLayerSystem);
	MEMORYOBJECTCATEGORY(MotionLayerSystem, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MOTIONSYSTEMS);

	public:
		// the motion system type, returned by GetType()
		enum { TYPE_ID = 0x00000001 };

		/**
		 * Constructor.
 		 * @param actor The actor this layer belongs to.
		 */
		MotionLayerSystem(Actor* actor);

		/**
		 * Destructor.
		 */
		virtual ~MotionLayerSystem();

		/**
		 * Stop playing the motion in <blendTime> seconds.
		 * When <blendTime> is set to 0, it will be immediately stopped, which can cause a not so nice popping effects.
		 * When set to 1.0f, it would smoothly fade out the motion in exactly one second.
		 * @param motion The motion to play.
		 * @param blendTime The time in seconds until the motion has no influence anymore.
		 */
		void StopMotion(MotionInstance* motion, const float blendTime=0.3f);

		/**
		 * Test if there is currently a motion playing or active.
		 * Motions which are in pause mode, will also count. So in case of motions being paused, TRUE will be returned as well.
		 * @result True if there is a motion playing, false if not.
		 */
		bool IsPlaying() const;

		/**
		 * Update this character motions, if <updateNodes> is false only time values are updated.
		 * If it's true, the heavy calcs are done.
		 * NOTE: This method is automatically called by the Update method.
		 * @param timePassed The time passed since the last call.
		 * @param updateNodes If true the nodes will be updated.
		 */
		void Update(const double timePassed, const bool updateNodes);

		/**
		 * Recursively search for the first non mixing motion and return the motion instance.
		 * @result A pointer to the motion instance.
		 */
		MotionInstance* FindFirstNonMixingMotionInst();

		/**
		 * Try to find the motion layer for a given motion, and return NULL when not located.
		 * @param motion The motion to search for.
		 * @return A pointer to the motion layer if the one was found, else it return 0.
		 */
		MotionLayer* FindMotionLayer(MotionInstance* motion);

		/**
		 * Remove the specified motion layer out of the motion tree.
		 * @param layer A pointer to the motion layer to remove.
		 * @param delFromMem If true the allocated memory of the motion layer will be deleted.
		 */
		void RemoveLayer(MotionLayer* layer, const bool delFromMem=true);

		/**
		 * Set the root layer of the motion tree.
		 * @param rootLayer The root layer.
		 */
		inline void SetMotionTree(MotionLayer* rootLayer);

		/**
		 * Get the root layer of the motion tree.
		 * @return The root layer of the motion tree.
		 */
		MotionLayer* GetMotionTree() const;

		/**
		 * Get the unique motion system type ID.
		 * @result The motion system type identification number.
		 */
		int GetType() const;

		/**
		 * Get the type identification string.
		 * This can be a description or the class name of the motion system.
		 * @result A pointer to the string containing the name.
		 */
		const char* GetTypeString() const;

		/**
		 * Find the location where to insert a motion layer with a given priority level.
		 * When NULL is returned, it needs to be inserted at the bottom of the motion tree.
		 * When a non-NULL value is returned, insertion must happen above the returned motion layer.
		 * @param priorityLevel The priority level of the motion instance you want to insert.
		 * @result A pointer to the motion layer to insert above, or NULL when the new layer has to be inserted at the bottom of the tree.
		 */
		MotionLayer* FindInsertPos(const int priorityLevel) const;

		/**
		 * Remove all layer passes.
		 * @param delFromMem When set to true, the layer passes will also be deleted from memory.
		 */
		void RemoveAllLayerPasses(bool delFromMem=true);

		/**
		 * Add a layer pass. This layer will be added on the back, so processed as last.
		 * @param newPass The new pass to add.
		 */
		void AddLayerPass(LayerPass* newPass);

		/**
		 * Get the number of layer passes currently added to this motion layer system.
		 * @result The number of layer passes.
		 */
		int GetNumLayerPasses() const;

		/**
		 * Remove a given layer pass by index.
		 * @param nr The layer pass number to remove.
		 * @param delFromMem When set to true, the layer passes will also be deleted from memory.
		 */
		void RemoveLayerPass(const int nr, bool delFromMem=true);

		/**
		 * Remove a given layer pass by pointer.
		 * @param pass A pointer to the layer pass to remove.
		 * @param delFromMem When set to true, the layer passes will also be deleted from memory.
		 */
		void RemoveLayerPass(LayerPass* pass, bool delFromMem=true);

		/**
		 * Insert a layer pass in the array of layer passes.
		 * @param insertPos The index position to insert the layer pass.
		 * @param pass The layer pass to insert.
		 */
		void InsertLayerPass(const int insertPos, LayerPass* pass);


	private:
		MCore::Array<LayerPass*>	mLayerPasses;	/**< The layer passes. */
		MotionLayer*				mMotionTree;	/**< The motion tree (the root layer). */

		/**
		 * Update the nodes.
		 */
		void UpdateNodes();

		/**
		 * Start playing the specified motion on this actor.
		 * The difference with PlayMotion is that PlayMotion takes care of the calls to CreateMotionInstance, and handles the insertion into the motion queue, etc.
		 * This method should purely start the motion.
		 * @param motion The motion to play.
		 * @param info A pointer to an object containing playback information. This pointer is NOT allowed to be NULL here.
		 * @result A pointer to a created motion instance object. You can use this motion instance object to adjust and retrieve playback information at any time.
		 * @see PlayBackInfo
		 */
		void StartMotion(MotionInstance* motion, class PlayBackInfo* info);
};

// include inline code
#include "MotionLayerSystem.inl"

} // namespace EMotionFX


#endif