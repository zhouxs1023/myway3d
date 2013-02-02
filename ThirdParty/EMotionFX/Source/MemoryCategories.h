/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MEMORYCATEGORIES_H
#define __MEMORYCATEGORIES_H

// include the Core system
#include "../Core/Source/MCore.h"

namespace EMotionFX
{

/**
 * The default alignment of returned memory addresses, in bytes.
 */
#define EMFX_DEFAULT_ALIGNMENT	4


/**
 * The memory categories of the EMotion FX classes.
 * Each EMotion FX class or template is linked to a given memory category.
 * The memory manager can be used to request memory usage statistics per category.
 * This is useful to see how much memory is allocated in each category.
 */
enum
{
	MEMCATEGORY_UNKNOWN						= 0,	// all allocations that are done inside Core, and other unknowns

	MEMCATEGORY_CONTROLLERS_CONSTRAINTS		= 100,
	MEMCATEGORY_CONTROLLERS_PHYSICS			= 101,
	MEMCATEGORY_CONTROLLERS_IK				= 102,
	MEMCATEGORY_CONTROLLERS_FACIAL			= 103,
	MEMCATEGORY_CONTROLLERS_MISC			= 104,

	MEMCATEGORY_GEOMETRY_HWSHADERBUFFERS	= 200,
	MEMCATEGORY_GEOMETRY_MATERIALS			= 201,
	MEMCATEGORY_GEOMETRY_MESHES				= 202,
	MEMCATEGORY_GEOMETRY_DEFORMERS			= 203,
	MEMCATEGORY_GEOMETRY_VERTEXATTRIBUTES	= 204,

	MEMCATEGORY_MOTIONS_MOTIONINSTANCES		= 300,
	MEMCATEGORY_MOTIONS_MOTIONSYSTEMS		= 301,
	MEMCATEGORY_MOTIONS_FACIALMOTIONS		= 302,
	MEMCATEGORY_MOTIONS_SKELETALMOTIONS		= 303,
	MEMCATEGORY_MOTIONS_INTERPOLATORS		= 304,
	MEMCATEGORY_MOTIONS_KEYTRACKS			= 305,
	MEMCATEGORY_MOTIONS_MOTIONLINKS			= 306,
	MEMCATEGORY_MOTIONS_MISC				= 308,

	MEMCATEGORY_NODES_NODES					= 400,
	MEMCATEGORY_NODES_ACTORS				= 401,
	MEMCATEGORY_NODES_NODEATTRIBUTES		= 402,
	MEMCATEGORY_NODES_COLLISIONDETECTION	= 403,
	MEMCATEGORY_NODES_MISC					= 404,

	MEMCATEGORY_IMPORTER					= 500,	

	MEMCATEGORY_NORMALMAPGENERATOR			= 600
};


}	// namespace EMotionFX

#endif