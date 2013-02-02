/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __UVVERTEXATTRIBUTELAYER_H
#define __UVVERTEXATTRIBUTELAYER_H

// include the core system
#include "../Core/Source/MCore.h"

// include the vertex attribute base class
#include "VertexAttributeLayer.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The vertex attribute layer that contains UV texture coordinates.
 * This layer represents an entire UV mapping channel.
 */
class UVVertexAttributeLayer : public VertexAttributeLayer
{
	DECLARE_CLASS(UVVertexAttributeLayer);
	MEMORYOBJECTCATEGORY(UVVertexAttributeLayer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_VERTEXATTRIBUTES);

	public:
		enum { TYPE_ID = 0x00000002 };

		/**
		 * The constructor.
		 * @param numAttributes The number of attributes to store inside this layer.
		 */
		UVVertexAttributeLayer(const int numAttributes);

		/**
		 * The destructor, which should delete all allocated attributes from memory.
		 */
		~UVVertexAttributeLayer();

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
		 * Get a pointer to the array of UV coordinates.
		 * The number of elements in the array returned is equal to the value of the 'numAttributes' parameter passed to the constructor.
		 * When you are dealing with shared vertex attributes, this will be equal to the value returned by Mesh::GetNumOrgVertices()
		 * and if you are dealing with non-shared vertex attributes this will be equal to Mesh::GetNumVertices().
		 * @result A pointer to the UV data.
		 */
		inline MCore::Vector2* GetUVs()	{ return mUVs; }

	protected:
		MCore::Vector2*	mUVs;	/**< The UV coordinates. */
};

} // namespace EMotionFX


#endif