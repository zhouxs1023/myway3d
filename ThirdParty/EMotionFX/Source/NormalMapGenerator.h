/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NORMALMAPGENERATOR_H
#define __NORMALMAPGENERATOR_H

// include the Core system
#include "../Core/Source/MCore.h"

#include "Actor.h"
#include "NormalMap.h"
#include "ImageResChecker.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The normal map generator base class.
 * For more information about what normal maps exactly are, read the description of the NormalMap class.
 * The normal map generators are responsible for generating normal maps for a low detail model, from a high detail model.
 * So they take a low-poly and high-poly version of the model as input, and they output normal maps for the low-poly model
 * which will make this low-poly version look as the high-poly version.
 */
class NormalMapGenerator
{
	MEMORYOBJECTCATEGORY(NormalMapGenerator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NORMALMAPGENERATOR);

	public:
		/**
		 * Constructor.
		 */
		NormalMapGenerator();

		/**
		 * Destructor.
		 */
		virtual ~NormalMapGenerator();

		/**
		 * The main method, which generates a set of normalmaps from a low and high detail model.
		 * Both actors will not be deleted from memory after execution of this method.
		 * @param lowDetail The low detail version of the actor.
		 * @param highDetail The high detail version of the actor.
		 * @param tangentSpace When set to true, the normal map will be generated in tangent space, otherwise in object space.
		 * @param postFilter When set to true, a post filter process will be applied after the normal map has been generated.
		 *                   This post filter will try to increase the quality of the normal map by removing potential noticable errors and
		 *                   applying some filtering in areas where the algorithm thinks it is needed.
		 */
		virtual void Generate(Actor* lowDetail, Actor* highDetail, bool tangentSpace=true, bool postFilter=true) = 0;

		/**
		 * Get the unique type identification number of this generator.
		 * @result The unique type ID.
		 */
		virtual int GetType() const = 0;

		/**
		 * Get the description of this generator.
		 * @result A string containing the description.
		 */
		virtual const char* GetTypeString() = 0;

		/**
		 * Get the progress state, which means how far the generation process is completed.
		 * This is represented in a percentage ranging from 0 to 100.
		 * @result The progress state, as a value ranging from 0 to 100.
		 */
		virtual float GetProgress() const = 0;

		/**
		 * Get rid of normal maps, and remove them from memory.
		 */
		void ReleaseNormalMaps();

		/**
		 * Get number of normal maps which have been generated, or which are currently being generated.
		 * The number of normal maps will not increase or decrease during the generation process.
		 * @result The number of normal maps.
		 */
		int GetNumNormalMaps() const;

		/**
		 * Get access to the given normal map.
		 * @param nr The index of the normal map, which must be in range of [0..GetNumNormalMaps()-1]
		 * @result A pointer to the normal map.
		 */
		NormalMap* GetNormalMap(const int nr) const;

		/**
		 * Get the current normal map.
		 * @result The index pointing to the current normalmap that is being generated. The same normalmap
		 *         can be touched several times during the generation process.
		 */
		int GetCurrentNormalMap() const;

		/**
		 * Set the resolution ratio. This ratio defines the ratio between the original texture map and the 
		 * normal map for this texture. The normal map resolution is calculated using the following equation:
		 *
		 * <pre>
		 * normalMapRes = ratio * textureRes
		 * </pre>
		 *
		 * @param ratio The ratio between the texture resolutions and the normal map resolutions.
		 */
		void SetRatio(const float ratio);

		/**
		 * Set the fallback resolution of the normal maps. The specified default (fallback) resolution will be used as widht and height
		 * for the generated normal maps when the original texture resolutions cannot be extracted, due to unsupported texture formats
		 * or missing texture files. On default this value is set to 256, which means a normal map resolution of 256x256.
		 * @param resolution Width and Height of the images.
		 */
		void SetDefaultRes(const int resolution);


	protected:
		MCore::Array<NormalMap*>	mNormalMaps;		/**< The normal maps which are being generated. */
		float						mRatio;				/**< The texture/normalmap resolution ratio. */
		int							mCurNormalMap;		/**< The current normal map being generated. */
		int							mDefaultRes;		/**< The default fallback resolution. */
};

} // namespace EMotionFX

#endif