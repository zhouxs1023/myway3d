/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODEIDGENERATOR_H
#define __NODEIDGENERATOR_H

#include "../Core/Source/MCore.h"

#include "MemoryCategories.h"

namespace EMotionFX
{

/**
 * The node ID generator.
 * Every node gets a unique ID based on its name. This singleton converts names into ID values on such a way 
 * that every name results in the same ID. This can be used to replace node name compares with compares based on
 * the node ID.
 */
class NodeIDGenerator
{
	DECLARE_CLASS(NodeIDGenerator)
	MEMORYOBJECTCATEGORY(NodeIDGenerator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_MISC);
	SINGLETON(NodeIDGenerator)

	private:
		/**
		 * Default constructor.
		 */
		NodeIDGenerator();

		/**
		 * Destructor.
		 */
		virtual ~NodeIDGenerator();


	public:
		/**
		 * Generate a unique node id for the given string which contains the node name.
		 * @param nodeName The name of the node to generate an id for.
		 * @return The unique id of the given node.
		 */
		int GenerateIDForString(const MCore::String& nodeName);

		/**
		 * Return the name of the given node id.
		 * @param id The unique node id to search for the name.
		 * @return The name of the given node.
		 */
		const MCore::String& GetNodeName(const int id);

	private:
		MCore::Array< MCore::String >		mNodeNames;		/**< String array which contains the names of the nodes. */
};

} // namespace EMotionFX


#endif