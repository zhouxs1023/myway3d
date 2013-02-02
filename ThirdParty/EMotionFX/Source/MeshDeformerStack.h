/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MESHDEFORMERSTACK_H
#define __MESHDEFORMERSTACK_H

#include "../Core/Source/MCore.h"
#include "MeshDeformer.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Actor;
class Node;

/**
 * The mesh deformer stack.
 * This class represents a stack of mesh deformers, which are executed on a given mesh in
 * the order defined by the stack. The deformers will be executed from bottom to the top.
 * An example stack could be:
 * - Twist Deformer
 * - SoftSkin Deformer
 * - Morph Deformer
 * 
 * This would first perform a morph on the given mesh. After that a softskinning deformer would be applied to it, using
 * bone deformations. And finally the result of that would be deformed by a twist modifier, which would twist the mesh.
 * People who know 3D Studio Max will recognise this system as the Max Modifier Stack.
 */
class MeshDeformerStack
{
	DECLARE_CLASS(MeshDeformerStack);
	MEMORYOBJECTCATEGORY(MeshDeformer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_DEFORMERS);

	public:
		/**
		 * Constructor.
		 * @param mesh The mesh to apply this deformer on.
		 */
		MeshDeformerStack(Mesh* mesh);

		/**
		 * Destructor.
		 */
		virtual ~MeshDeformerStack();

		/**
		 * Update the stack calling the mesh deformers.
		 * @param actor The actor to use for the update. So the actor where the mesh belongs to during this update.
		 * @param node The node to use for the update, so the node where the mesh belongs to during this update.
		 * @param timeDelta The time (in seconds) passed since the last call.
		 */
		virtual void Update(Actor* actor, Node* node, const double timeDelta);

		/**
		 * Iterates through all mesh deformers in the stack and reinitializes them.
		 */
		void ReInitializeDeformers();

		/**
		 * Add a given deformer to the back of the stack.
		 * @param meshDeformer The deformer to add.
		 */
		void AddDeformer(MeshDeformer* meshDeformer);

		/**
		 * Insert a given deformer at a given position in the deformer stack.
		 * @param pos The position to insert the deformer.
		 * @param meshDeformer The deformer to store at this position.
		 */
		void InsertDeformer(const int pos, MeshDeformer* meshDeformer);

		/**
		 * Remove a given deformer.
		 * @param meshDeformer The item/element to remove.
		 */
		bool RemoveDeformer(MeshDeformer* meshDeformer);

		/**
		 * Remove all deformers from this mesh deformer stack that have a specified type ID.
		 * So you can use this to for example delete all softskin deformers.
		 * @param deformerTypeID The type ID of the deformer, which is returned by MeshDeformer::GetType().
		 * @result Returns the number of deformers that have been removed.
		 */
		int RemoveAllDeformersByType(const int deformerTypeID);

		/**
		 * Remove all deformers from this mesh deformer stack.
		 */
		void RemoveAllDeformers();

		/**
		 * Enable or disable all the deformers with the specified type ID.
		 * You can use this to for example disable all softskin deformers.
		 * @param deformerTypeID The type ID of the deformer, which is returned by MeshDeformer::GetType().
		 * @param enabled Set to true when you want to enable these deformers, or false if you want to disable them.
		 * @result Returns the number of deformers that have been enabled or disabled.
		 */
		int EnableAllDeformersByType(const int deformerTypeID, const bool enabled);

		/**
		 * Creates an exact clone (copy) of this deformer stack, including all deformers (which will also be cloned).
		 * @param mesh The mesh to apply the new stack on.
		 * @param actor The actor to apply the new stack on.
		 * @result A pointer to the cloned stack.
		 */
		MeshDeformerStack* Clone(Mesh* mesh, Actor* actor);

		/**
		 * Returns the mesh we are applying the stack on.
		 * @result A pointer to the mesh.
		 */
		Mesh* GetMesh() const;

		/**
		 * Get the number of deformers in the stack.
		 * @result The number of deformers in the stack.
		 */
		int GetNumDeformers() const;

		/**
		 * Get a given deformer.
		 * @param nr The deformer number to get.
		 * @result A pointer to the deformer.
		 */
		MeshDeformer* GetDeformer(const int nr) const;

		/**
		 * Check if the stack contains a deformer of a given type.
		 * @param deformerTypeID The type ID of the deformer you'd like to check.
		 * @result Returns true when the stack has one or more deformers of the specified type, otherwise false is returned.
		 */
		bool HasDeformerOfType(const int deformerTypeID) const;


	private:
		MCore::Array<MeshDeformer*>	mDeformers;		/**< The stack of deformers. */
		Mesh*						mMesh;			/**< Pointer to the mesh to which the modifier stack belongs to.*/
};

} // namespace EMotionFX


#endif