/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SKINNINGINFOVERTEXATTRIBUTELAYER_H
#define __SKINNINGINFOVERTEXATTRIBUTELAYER_H

// include the core system
#include "../Core/Source/MCore.h"

// include the vertex attribute base class
#include "VertexAttributeLayer.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * A soft skinning influence between a vertex and a bone, with a given weight.
 */
class SkinInfluence
{
	DECLARE_CLASS(SkinInfluence);
	MEMORYOBJECTCATEGORY(SkinInfluence, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_VERTEXATTRIBUTES);

	public:
		/**
		 * Constructor.
		 * @param bone The bone to link the given vertex with.
		 * @param weight The weight value, which must be in range of [0..1].
		 * @param boneNr The bone number, used as optimization inside the softskin deformer.
		 */
		inline SkinInfluence(Node* bone, const float weight, const int boneNr=0);

		/**
		 * Destructor.
		 */
		~SkinInfluence();

		/**
		 * Get the bone of this influence.
		 * @result A pointer to the bone.
		 */
		inline Node* GetBone() const;

		/**
		 * Get the weight of this influence.
		 * @result The weight, which should be in range of [0..1].
		 */
		inline float GetWeight() const;

		/**
		 * Adjust the pointer to the bone.
		 * @param bone The bone to use for this influence.
		 */
		inline void SetBone(Node* bone);

		/**
		 * Adjust the weight value.
		 * @param weight The weight value, which must be in range of [0..1].
		 */
		inline void SetWeight(const float& weight);

		/**
		 * Set the bone number, used for precalculations.
		 * @param boneNr The bone number.
		 */
		inline void SetBoneNr(const int boneNr);

		/**
		 * Get the bone number, which is used for precalculations.
		 * @result The bone number.
		 */
		inline int GetBoneNr() const;
	
	private:
		Node*		mBone;		/**< The bone effecting the vertex which uses this influence. */
		float		mWeight;	/**< The weight value, between 0 and 1. */
		int			mBoneNr;	/**< A bone number, which points in an array of bone info structs used for precalculating the skinning matrices. */
};


/**
 * The vertex attribute layer that contains UV texture coordinates.
 * This layer represents an entire UV mapping channel.
 */
class SkinningInfoVertexAttributeLayer : public VertexAttributeLayer
{
	DECLARE_CLASS(SkinningInfoVertexAttributeLayer);
	MEMORYOBJECTCATEGORY(SkinningInfoVertexAttributeLayer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_VERTEXATTRIBUTES);

	public:
		enum { TYPE_ID = 0x00000003 };

		/**
		 * The constructor.
		 * @param numAttributes The number of attributes to store inside this layer.
		 */
		SkinningInfoVertexAttributeLayer(const int numAttributes);

		/**
		 * The destructor, which should delete all allocated attributes from memory.
		 */
		~SkinningInfoVertexAttributeLayer();

		/**
		 * Get the unique layer type.
		 * This identifies what type of attributes are stored internally.
		 * An example could be the type ID of an UV attribute layer, or a layer with colors or one which 
		 * identifies a layer that contains softskinning information.
		 * @result The unique type ID, which identifies what type of data is stored inside this layer. Each class inherited from 
		 *         the VertexAttributeLayer class requires a unique type ID.
		 */
		virtual int	GetType() const;

		/**
		 * Get the description of the vertex attributes or layer.
		 * You most likely want this to be the class name.
		 * @result A pointer to the string containing the name or description of the type of vertex attributes of this layer.
		 */
		virtual const char* GetTypeString() const;

		/**
		 * Relink the attributes to a given mesh that is part of a given node which again is part of a given actor.
		 * On default relinking does nothing, but you can overload this method for your vertex attribute layer type.
		 * It might be that your layer stores data which contains pointers to nodes inside some actor.
		 * Now imagine you want to copy over the vertex attribute layers of a mesh to a new actor and delete the original actor from memory.
		 * That would mean the pointers point to deleted memory. This method can be used to relink the pointers, indices or other data to the new actor and node.
		 * An example situation where this happens is when you make a call to Actor::AddLODModel(...). Here the mesh data is copied over to the actor containing all LOD levels.
		 * After this the original LOD model is deleted from memory.
		 * @param mesh The new mesh where the vertex attribute layer will be part of.
		 * @param node The node where the new mesh belongs to.
		 * @param actor The actor where the node of the second parameter belongs to.
		 */
		virtual void ReLinkToNode(Mesh* mesh, Node* node, Actor* actor);

		/**
		 * Add a given influence (using a bone and a weight).
		 * @param attributeNr The attribute/vertex number.
		 * @param bone The bone to use in the influence.
		 * @param weight The weight to use in the influence.
		 * @param boneNr The bone number, used for optimizations inside the softskin deformer.
		 */
		void AddInfluence(const int attributeNr, Node* bone, const float weight, const int boneNr = 0);

		/**
		 * Get the number of influences.
		 * @param attributeNr The attribute/vertex number.
		 * @result The number of influences.
		 */
		inline int GetNumInfluences(const int attributeNr);

		/**
		 * Get a given influence.
		 * @param attributeNr The attribute/vertex number.
		 * @param influenceNr The influence number, which must be in range of [0..GetNumInfluences()]
		 * @result The given influence.
		 */
		inline SkinInfluence& GetInfluence(const int attributeNr, const int influenceNr);

	protected:
		MCore::Array< MCore::Array<SkinInfluence> > mInfluences;	/**< The skinning influence data. */
};

// include inline code
#include "SkinningInfoVertexAttributeLayer.inl"

} // namespace EMotionFX


#endif