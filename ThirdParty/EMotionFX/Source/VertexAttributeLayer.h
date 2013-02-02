/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __VERTEXATTRIBUTELAYER_H
#define __VERTEXATTRIBUTELAYER_H

// include the core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Node;
class Mesh;
class Actor;

/**
 * The vertex attribute layer base class.
 * Each mesh can have a set of custom vertex attribute layers.
 * Each layer must be inherited from this base class and must store its custom data, such as texture coordinates
 * or vertex color or softskinning information.
 */
class VertexAttributeLayer
{
	DECLARE_CLASS(VertexAttributeLayer);
	MEMORYOBJECTCATEGORY(VertexAttributeLayer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_VERTEXATTRIBUTES);

	public:
		/**
		 * The constructor.
		 * @param numAttributes The number of attributes to store inside this layer.
		 */
		VertexAttributeLayer(const int numAttributes);

		/**
		 * The destructor, which should delete all allocated attributes from memory.
		 */
		virtual ~VertexAttributeLayer();

		/**
		 * Get the unique layer type.
		 * This identifies what type of attributes are stored internally.
		 * An example could be the type ID of an UV attribute layer, or a layer with colors or one which 
		 * identifies a layer that contains softskinning information.
		 * @result The unique type ID, which identifies what type of data is stored inside this layer. Each class inherited from 
		 *         the VertexAttributeLayer class requires a unique type ID.
		 */
		virtual int GetType() const = 0;

		/**
		 * Get the description of the vertex attributes or layer.
		 * You most likely want this to be the class name.
		 * @result A pointer to the string containing the name or description of the type of vertex attributes of this layer.
		 */
		virtual const char* GetTypeString() const = 0;

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
};

} // namespace EMotionFX


#endif