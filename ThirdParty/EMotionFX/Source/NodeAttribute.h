/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODEATTRIBUTE_H
#define __NODEATTRIBUTE_H

// include the Core system
#include "../Core/Source/MCore.h"

#include "MemoryCategories.h"

namespace EMotionFX
{

/**
 * The node attribute base class.
 * Custom attributes can be attached to every node.
 * An example of an attribute could be physics properties.
 * In order to create your own node attribute, simply inherit a class from this base class.
 */
class NodeAttribute
{
	MEMORYOBJECTCATEGORY(NodeAttribute, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_NODEATTRIBUTES);

	public:
		/**
		 * The constructor.
		 */
		NodeAttribute() {}

		/**
		 * The destructor.
		 */
		virtual ~NodeAttribute() {}

		/**
		 * Get the attribute type.
		 * @result The attribute ID.
		 */
		virtual int	GetType() const = 0;

		/**
		 * Get the attribute type as a string.
		 * This string should contain the name of the class.
		 * @result The string containing the type name.
		 */
		virtual const char* GetTypeString() const = 0;

		/**
		 * Clone the node attribute.
		 * @result Returns a pointer to a newly created exact copy of the node attribute.
		 */
		virtual NodeAttribute* Clone() = 0;
};


} // namespace EMotionFX


#endif