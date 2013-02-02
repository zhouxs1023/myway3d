/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MESHDEFORMER_H
#define __MESHDEFORMER_H

// include the Core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Mesh;
class Actor;
class Node;


/**
 * The mesh deformer base class.
 * A mesh deformer can deform (apply modifications) on a given mesh.
 * Examples of deformers could be a TwistDeformer, SoftSkinDeformer, MorphDeformer, etc.
 * Every deformer has its own unique type ID number and the MeshDeformerStack contains a list of 
 * deformers which are executed in the specified order.
 */
class MeshDeformer
{
	DECLARE_CLASS(MeshDeformer);
	MEMORYOBJECTCATEGORY(MeshDeformer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_DEFORMERS);

	public:
		/**
		 * Default constructor.
		 * @param mesh A pointer to the mesh to deform.
		 */
		MeshDeformer(Mesh* mesh);

		/**
		 * Destructor.
		 */
		virtual ~MeshDeformer();

		/**
		 * Update the mesh deformer.
		 * @param actor The actor to use for the update. So the actor where the mesh belongs to during this update.
		 * @param node The node to use for the update, so the node where the mesh belongs to during this update.
		 * @param timeDelta The time (in seconds) passed since the last call.
		 */
		virtual void Update(Actor* actor, Node* node, const double timeDelta) = 0;

		/**
		 * Reinitialize the mesh deformer.
		 */
		virtual void ReInitialize();

		/**
		 * Creates an exact clone (copy) of this deformer, and returns a pointer to it.
		 * @param mesh The mesh to apply the cloned deformer on.
		 * @param actor The actor to apply the cloned deformer on.
		 * @result A pointer to the newly created clone of this deformer.
		 */
		virtual MeshDeformer* Clone(Mesh* mesh, Actor* actor) = 0;

		/**
		 * Returns the type identification number of the deformer class.
		 * @result The type identification number.
		 */
		virtual int GetType() const = 0;

		/**
		 * Returns the sub type identification number.
		 * This number is used to identify special specilizations of a given deformer, like the same type of deformer, but with P4 optimizations.
		 * In that case the type will be the same, but the subtype will be different for each specialization.
		 * @result The unique subtype identification number.
		 */
		virtual int GetSubType() const = 0;

		/**
		 * Check if the controller is enabled or not.
		 * @result Returns true when the controller is active (enabled) or false when the controller is inactive (disabled).
		 */
		bool IsEnabled() const;

		/**
		 * Enable or disable the controller.
		 * Disabling a controller just results in the Update method of the controller not being called during the Actor::Update() call.
		 * @param enabled Set to true when you want to enable the controller or to false when you want to disable the controller.
		 */
		void SetEnabled(const bool enabled);


	protected:
		Mesh*	mMesh;		/**< Pointer to the mesh to which the deformer belongs to.*/
		bool	mIsEnabled;	/**< When set to true, this mesh deformer will be processed, otherwise it will be skipped during update. */
};

} // namespace EMotionFX


#endif