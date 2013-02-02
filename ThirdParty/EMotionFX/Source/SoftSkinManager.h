/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SOFTSKINMANAGER_H
#define __SOFTSKINMANAGER_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Mesh.h"
#include "SoftSkinDeformer.h"


namespace EMotionFX
{

/**
 * The softskin manager.
 * This manager allows to create optimized softskin deformers, which will run as fast as possible
 * on the hardware of the user. In other words, specialised version of softskin deformers can be
 * created using this class. For example: if the hardware supports SSE, an SSE optimized softskin deformer
 * will be returned, instead of the normal C++ version.
 */
class SoftSkinManager
{
	DECLARE_CLASS(SoftSkinManager);
	MEMORYOBJECTCATEGORY(SoftSkinManager, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_DEFORMERS);
	SINGLETON(SoftSkinManager);

	public:
		/**
		 * The constructor.
		 * When constructed, the class checks if SSE is available on the hardware.
		 */
		SoftSkinManager();

		/**
		 * Creates the softskin deformer, by looking at the hardware capabilities.
		 * If SSE is present, an SSE optimized softskinner will be returned, otherwise a normal
		 * C++ version.
		 */
		SoftSkinDeformer* CreateDeformer(Mesh* mesh);


	private:
		bool	mDetectedSSE;	/**< Does the cpu support SSE instructions? */
};


} // namespace EMotionFX

#endif