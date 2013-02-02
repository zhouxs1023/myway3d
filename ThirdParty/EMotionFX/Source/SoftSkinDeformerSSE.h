/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SOFTSKINDEFORMERSSE_H
#define __SOFTSKINDEFORMERSSE_H

// only include this code when we use microsoft visual c++, because of the assembler stuff
#if COMPILER == COMPILER_MSVC

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "SoftSkinDeformer.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * SSE and ASM optimized version of the SoftSkinDeformer class.
 * It is currently about 20% faster compared to the C++ version.
 */
class SoftSkinDeformerSSE : public SoftSkinDeformer
{
	DECLARE_CLASS(SoftSkinDeformerSSE);
	MEMORYOBJECTCATEGORY(SoftSkinDeformerSSE, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_DEFORMERS);

	public:
		// the unique subtype id, returned by GetSubType()
		enum { SUBTYPE_ID = 0x00000002 };

		/**
		 * Default constructor.
		 * @param mesh A pointer to the mesh to deform.
		 */
		SoftSkinDeformerSSE(Mesh* mesh) : SoftSkinDeformer(mesh) {}

		/**
		 * Update the deformer.
		 * @param actor The actor to use for the update. So the actor where the mesh belongs to during this update.
		 * @param node The node to use for the update, so the node where the mesh belongs to during this update.
		 * @param timeDelta The time (in seconds) passed since the last call.
		 */
		void Update(Actor* actor, Node* node, const double timeDelta);

		/**
		 * Creates an exact clone (copy) of this deformer, and returns a pointer to it.
		 * @param mesh The mesh to apply the deformer on.
		 * @param actor The actor to apply the deformer on.
		 * @result A pointer to the newly created clone of this deformer.
		 */
		MeshDeformer* Clone(Mesh *mesh, Actor* actor);

		/**
		 * Returns the unique subtype ID of this deformer.
		 * A subtype identifies the specialization type of the given deformer.
		 * A cplus plus version of a deformer could have a sub type of 0, while the SSE assembly optimized
		 * version would have a sub type of 1 for example.
		 * @result The sub type identification number.
		 */
		int GetSubType() const;
};



} // namespace EMotionFX

#endif // #if COMPILER == COMPILER_MSVC

#endif