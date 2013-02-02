/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SMARTMESHMORPHDEFORMER_H
#define __SMARTMESHMORPHDEFORMER_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Mesh.h"
#include "MeshDeformer.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Actor;
class Node;


/**
 * The smart mesh morphing deformer.
 * This deformer applies relative morphs on specific parts of the mesh.
 * The deformation data is retrieved from a mesh expression part (MeshExpressionPart) object.
 */
class SmartMeshMorphDeformer : public MeshDeformer
{
	DECLARE_CLASS(SmartMeshMorphDeformer);
	MEMORYOBJECTCATEGORY(SmartMeshMorphDeformer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_DEFORMERS);

	public:
		// the unique type ID of this deformer, returned by GetType()
		enum { TYPE_ID    = 0x00000002 };

		// the subtype id, returned by GetSubType()
		enum { SUBTYPE_ID = 0x00000001 };

		/**
		 * Default constructor.
		 * @param part The mesh expression part where this deformer is linked to, so which contains the data.
		 * @param deformDataNr The index into the deform data array inside the mesh expression part.
		 * @param mesh A pointer to the mesh to deform.
		 */
		SmartMeshMorphDeformer(class MeshExpressionPart* part, const int deformDataNr, Mesh* mesh);

		/**
		 * Destructor.
		 */
		virtual ~SmartMeshMorphDeformer();

		/**
		 * Get the unique deformer type ID.
		 * @result The unique ID of this deformer type.
		 */
		int GetType() const;

		/**
		 * Returns the unique subtype ID of this deformer.
		 * A subtype identifies the specialization type of the given deformer.
		 * A cplus plus version of a deformer could have a sub type of 0, while the SSE assembly optimized
		 * version would have a sub type of 1 for example.
		 * @result The sub type identification number.
		 */
		virtual int GetSubType() const;

		/**
		 * Update the deformer.
		 * @param actor The actor to use for the update. So the actor where the mesh belongs to during this update.
		 * @param node The node to use for the update, so the node where the mesh belongs to during this update.
		 * @param timeDelta The time (in seconds) passed since the last call.
		 */
		virtual void Update(Actor* actor, Node* node, const double timeDelta);

		/**
		 * Creates an exact clone (copy) of this deformer, and returns a pointer to it.
		 * @param mesh The mesh to apply the deformer on.
		 * @param actor The actor to apply the deformer on.
		 * @result A pointer to the newly created clone of this deformer.
		 */
		virtual MeshDeformer* Clone(Mesh *mesh, Actor* actor);

		/**
		 * Get the mesh expression part where this deformer will retrieve its deformation data from.
		 * @result A pointer to the expression part.
		 */
		class MeshExpressionPart* GetExpressionPart() const;


	protected:
		class MeshExpressionPart*	mMeshExpressionPart;	/**< The mesh expression part where this deformer will get the deformation data from. */
		int							mDeformDataNr;			/**< An index inside the mesh expression part's list with deformation data objects. */
		bool						mLastNearZero;			/**< Used for optimization reasons. It specifies if the last update was at a weight near zero or not. */
};


} // namespace EMotionFX


#endif