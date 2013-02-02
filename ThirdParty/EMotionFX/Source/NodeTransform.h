/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODETRANSFORM_H
#define __NODETRANSFORM_H

#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

/**
 * A node transformation contains the orientation, position and scale of a node (used mainly internally inside EMotion FX).
 */
class NodeTransform
{
	DECLARE_CLASS(NodeTransform)
	MEMORYOBJECTCATEGORY(NodeTransform, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_MISC);

	public:
		MCore::Quaternion	mRotation;		/**< The rotation. */
		MCore::Vector3		mPosition;		/**< The position. */
		MCore::Vector3		mScale;			/**< The scale. */
};

} // namespace EMotionFX


#endif