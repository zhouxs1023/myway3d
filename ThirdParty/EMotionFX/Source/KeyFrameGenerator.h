/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __KEYFRAMEGENERATOR_H
#define __KEYFRAMEGENERATOR_H

#include "../Core/Source/MCore.h"
#include "KeyTrack.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

/**
 * The keyframe generator template base class.
 * Keyframe generators can generate keyframes in a given keytrack.
 * This can sometimes automate behaviour of specific things. An example is an eyeblink generator, which 
 * can automatically generate keyframes which simulate eye blinks.
 * The main advantage of keyframe generators over procedural generators is that the values are editable afterwards,
 * while using procedural generators don't allow any fine tuning to the results.
 */
template <class ReturnType, class StorageType>
class KeyFrameGenerator
{
	MEMORYOBJECTCATEGORY(KeyFrameGenerator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_MISC);

	public:
		/**
		 * The constructor.
		 */
		KeyFrameGenerator()	{}

		/**
		 * The destructor.
		 */
		virtual ~KeyFrameGenerator()	{}

		/**
		 * The main method, which applies the generator on a given keytrack.
		 * Before you call the generate method you might want to set the properties of the generator, if it has any.
		 * @param outTrack A pointer to the keytrack to write the keyframes in.
		 */
		virtual void Generate(KeyTrack<ReturnType, StorageType>* outTrack) = 0;

		/**
		 * Get the description of the generator.
		 * This should be the class name as a string.
		 * @result The string containing the description of the generator.
		 */
		virtual const char* GetDescription() const = 0;

		/**
		 * Get the unique type identification number of this class.
		 * It can be used to detect the keyframe generator types.
		 * @result The unique ID of this class.
		 */
		virtual int GetType() const = 0;
};

} // namespace EMotionFX

#endif