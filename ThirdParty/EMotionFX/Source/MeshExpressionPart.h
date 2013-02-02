/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MESHEXPRESSIONPART_H
#define __MESHEXPRESSIONPART_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Node.h"
#include "Actor.h"
#include "ExpressionPart.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The mesh based expression part.
 * This expression part holds morph data for a given part of the character.
 * It works together with the SmartMeshMorphDeformer, which is the actual class which applies the deformations
 * to the meshes.
 */
class MeshExpressionPart : public ExpressionPart
{
	DECLARE_CLASS(MeshExpressionPart);
	MEMORYOBJECTCATEGORY(MeshExpressionPart, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_FACIALMOTIONS);

	friend class SmartMeshMorphDeformer;

	public:
		// the expression part type ID, returned by GetType()
		enum { TYPE_ID = 0x00000001	}; 

		/**
		 * The memory block ID where allocations made by this class will remain in.
		 */
		enum { MEMORYBLOCK_ID = 101 };

		/**
		 * The structure which contains the deformation data for each node which is being touched by this
		 * expression part.
		 */
		class DeformData
		{
			MEMORYOBJECTCATEGORY(DeformData, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_FACIALMOTIONS);

			public:
				/**
				 * The constructor.
				 * @param node The node on which the deformations should work.
				 * @param numVerts The number of vertices modified by this deform.
				 */
				DeformData(Node* node, int numVerts);

				/**
				 * The destructor. 
				 * Automatically releases all allocated memory by this deform.
				 */			
				~DeformData();

			public:
				MCore::Compressed16BitVector3*	mDeltas;			/**< The delta position values. */
				MCore::Compressed8BitVector3*	mDeltaNormals;		/**< The delta normal values (compressed). */
				int*							mVertexNumbers;		/**< The vertex numbers we need to modify. */
				Node*							mNode;				/**< The node which this data works on. */
				int								mNumVerts;			/**< The number of vertices in the mDeltas and mVertexNumbers arrays. */
				float							mMinValue;			/**< The compression/decompression minimum value for the delta positions. */
				float							mMaxValue;			/**< The compression/decompression maximum value for the delta positions. */
		};

		/**
		 * The transformation struct.
		 * This contains a position, rotation and scale.
		 * Please keep in mind that the rotation is stored as non-delta value, while the position and scale are
		 * stored as delta values.
		 */
		struct Transformation
		{
			MCore::Quaternion	mRotation;	/**< The rotation as a non-delta value. So not a delta value, but a target (absolute) rotation. */
			MCore::Vector3		mPosition;	/**< The position as a delta, so the difference between the original and target position. */
			MCore::Vector3		mScale;		/**< The scale as a delta, so the difference between the original and target scale. */
		};

		/**
		 * The constructor.
		 * @param captureTransforms Set this to true if you want this expression part to capture rigid transformations (changes in pos/rot/scale).
		 * @param captureMeshDeforms Set this to true if you want this expression part to capture mesh deformations (changes in vertex positions).
		 * @param parentActor The actor to apply this expression part on. So the actor which contains the face we want to modify.
		 * @param name The unique name of the expression part.
		 */
		MeshExpressionPart(const bool captureTransforms, const bool captureMeshDeforms, Actor* parentActor, const MCore::String& name);

		/**
		 * Extended constructor.
		 * @param captureTransforms Set this to true if you want this expression part to capture rigid transformations (changes in pos/rot/scale).
		 * @param captureMeshDeforms Set this to true if you want this expression part to capture mesh deformations (changes in vertex positions).
		 * @param parentActor The actor to apply this expression part on. So the actor which contains the face we want to modify.
		 * @param pose The actor representing the pose of the character when the weight value would equal 1.
		 * @param name The unique name of the expression part.
		 * @param delPoseFromMem When set to true (which is default) the actor passed as first parameter to this
		 *                       method will automatically be deleted from memory when it's no longer needed by this method anymore.
		 *                       So when set to true, you cannot use this actor (pose) anymore, because it is deleted from memory.
		 *                       When set to false, you must not forget to delete this pose actor later on yourself.
		 */
		MeshExpressionPart(const bool captureTransforms, const bool captureMeshDeforms, Actor* parentActor, Actor* pose, const MCore::String& name, const bool delPoseFromMem=true);

		/**
		 * The destructor.
		 */
		~MeshExpressionPart();

		/**
		 * Get the type of expression part.
		 * You can have different types of expression parts, such as expression parts which work
		 * with bones, or which work with morphing or other techniques.
		 * @result The unique type ID of the expression part.
		 */
		int GetType() const;

		/**
		 * Initializes this expresion part from a given actor representing the pose.
		 * The expression part will filter out all data which is changed compared to the base pose and
		 * store this information on a specific way so it can be used to accumulate multiple expression parts
		 * together and apply them to the actor to which this expression part is attached to.
		 * @param pose The actor representing the pose of the character when the weight value would equal 1.
		 * @param delPoseFromMem When set to true (which is default) the actor passed as first parameter to this
		 *                       method will automatically be deleted from memory when it's no longer needed by this method anymore.
		 *                       So when set to true, you cannot use this actor (pose) anymore, because it is deleted from memory.
		 *                       When set to false, you must not forget to delete this pose actor later on yourself.
		 */
		void InitFromPose(Actor* pose, bool delPoseFromMem=true);

		/**
		 * Restores the parts of the actor this expression part is working on to their original transformations.
		 * Only the parts of the actor which are affected by this expression part will be restored.
		 * Restoring means the resetting of for example bone transformations to their initial (neutral/base) pose.
		 * NOTE: this doesn't do anything inside this class.
		 * @param actor The actor to apply the restore on.
		 */
		void RestoreActor(Actor* actor);
        
		/**
		 * Apply the relative transformation caused by this expression part to a given node.
		 * It does not change the node itself, but it modifies the input position, rotation and scale.
		 * The node and actor pointers are only needed to retrieve some additional information from the node, needed
		 * to perform the calculations.
		 * @param actor The actor where the node belongs to.
		 * @param node The node we would like to modify (please note that the node itself will stay untouched).
		 * @param position The input position to which relative adjustments will be applied.
		 * @param rotation The input rotation to which relative adjustments will be applied.
		 * @param scale The input scale to which relative adjustments will be applied.
		 */
		void ApplyTransformation(Actor* actor, Node* node, MCore::Vector3& position, MCore::Quaternion& rotation, MCore::Vector3& scale);

		/**
		 * Checks if this expression part would influence the given node.
		 * @param node The node to perform the check with.
		 * @result Returns true if the given node will be modified by this expression part, otherwise false is returned.
		 */
		bool Influences(Node* node) const;

		/**
		 * Apply the relative deformations for this expression part to the given actor.
		 * @param actor The actor to apply the deformations on.
		 */
		void Apply(Actor* actor);

		/**
		 * Get the number of deform data objects.
		 * @result The number of deform data objects.
		 */
		int GetNumDeformDatas() const;

		/**
		 * Get a given deform data object.
		 * @param nr The deform data number, which must be in range of [0..GetNumDeformDatas()-1].
		 * @result A pointer to the deform data object.
		 */
		DeformData* GetDeformData(const int nr);

		/**
		 * Add a given deform data to the array of deform data objects.
		 * @param data The deform data object to add.
		 */
		void AddDeformData(DeformData* data);

		/**
		 * Add a new transformation to the expression part for the given node.
		 * @param nodeID The id of the node to which this transformation belongs to.
		 * @param transformation The transformation which contains the relative position, scale and the absolute rotation.
		 */
		void AddTransformation(const int nodeID, const Transformation& transform);

		/**
		 * Get the number of transformations which are part of this bones expression part.
		 * @result The number of tranformations.
		 */
		int GetNumTransformations() const;

		/**
		 * Get a given transformation and it's corresponding node id to which the transformation belongs to.
		 * @param nr The transformation number, must be in range of [0..GetNumTransformations()-1].
		 * @result A reference to a pair which contains the transformation and the corresponding node id.
		 */
		const MCore::Pair<int, Transformation>& GetTransformation(const int nr);

		/**
		 * Checks if this expression part captures the rigid transformations or not.
		 * With rigid transformations we mean the changes in position, rotation and scale.
		 * @result Returns true when it captures position/rotation/scale changes. Otherwise false is returned.
		 */
		bool GetCaptureTransforms() const;

		/**
		 * Checks if this expression part captures mesh deformations.
		 * With mesh deformations we mean changes on a per vertex level.
		 * A vertex change will be recorded only if the position of the vertex has changed compared to the base pose.
		 * Captured vertex elements are the position and normal.
		 * @result Returns true when it captures mesh deformations. Otherwise false is returned.
		 */
		bool GetCaptureMeshDeforms() const;


	protected:
		MCore::Array<DeformData*>							mDeformDatas;			/**< The deformation data objects. */
		MCore::Array< MCore::Pair<int, Transformation> >	mTransforms;			/**< The relative transformations for the given nodes, in local space. The rotation however is absolute. */
		bool												mCaptureTransforms;		/**< Does this expression part capture node rigid transformations (position/rotation/scale changes)? */
		bool												mCaptureMeshDeforms;	/**< Does this expression part capture mesh deformations (per vertex changes)? */
};

} // namespace EMotionFX

#endif