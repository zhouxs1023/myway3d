/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SOFTSKINDEFORMER_H
#define __SOFTSKINDEFORMER_H

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
 * The soft skinning mesh deformer 'base' class.
 * The calculations will be done on the CPU, however there will also be
 * specialized versions of this class, so which are inherited from this class.
 * These other classes will be a special soft skinning deformer for the Intel Pentium 4 and one for the AMD Athlon.
 * However, this class will work on all CPUs, but will be slower as the natively optimized ones.
 */
class SoftSkinDeformer : public MeshDeformer
{
	DECLARE_CLASS(SoftSkinDeformer);
	MEMORYOBJECTCATEGORY(SoftSkinDeformer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_DEFORMERS);

	public:
		// the type id of the deformer, returned by GetType()
		enum { TYPE_ID    = 0x00000001 };

		// the subtype id, returned by GetSubType()
		enum { SUBTYPE_ID = 0x00000001 };

		/**
		 * Default constructor.
		 * @param mesh A pointer to the mesh to deform.
		 * @param subType The subtype of the deformer. In case you have specialisations of a specific deformer for specific CPUs,
		 *				  you should give every specilization a unique subtype. The C++ version could have subtype 0, while the 
		 *				  SSE optimized version would have subtype 1 and the 3D Now! version would for example have subtype 2.
		 */
		SoftSkinDeformer(Mesh* mesh);

		/**
		 * Destructor.
		 */
		virtual ~SoftSkinDeformer();

		/**
		 * Update the deformer.
		 * @param actor The actor to use for the update. So the actor where the mesh belongs to during this update.
		 * @param node The node to use for the update, so the node where the mesh belongs to during this update.
		 * @param timeDelta The time (in seconds) passed since the last call.
		 */
		virtual void Update(Actor* actor, Node* node, const double timeDelta);

		/**
		 * Reinitialize the mesh deformer.
		 * Updates the the array of bone information used for precalculation.
		 */
		void ReInitialize();

		/**
		 * Creates an exact clone (copy) of this deformer, and returns a pointer to it.
		 * @param mesh The mesh to apply the deformer on.
		 * @param actor The actor to apply the deformer on.
		 * @result A pointer to the newly created clone of this deformer.
		 */
		virtual MeshDeformer* Clone(Mesh *mesh, Actor* actor);

		/**
		 * Returns the unique type ID of the deformer.
		 * @result The type ID of the deformer.
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


	protected:
		/**
		 * Structure used for precalculating the skinning matrices.
		 */
		struct BoneInfo
		{
			Node*			mNode;		// 4 bytes
			MCore::Matrix	mMatrix;	// 16 bytes

			inline BoneInfo() : mNode(NULL) {}
		};

		MCore::Array<BoneInfo>	mBones;	/**< The array of bone information used for precalculation. */
};


} // namespace EMotionFX


#endif