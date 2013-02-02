/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __KEYFRAME_H
#define __KEYFRAME_H

// include core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The keyframe class.
 * Each keyframe holds a value at a given time.
 * Interpolators can then calculate interpolated values between a set of keys, which are stored
 * inside a key track. This makes it possible to do keyframed animations.
 */
template <class ReturnType, class StorageType>
class KeyFrame
{
	DECLARE_CLASS(KeyFrame<>);
	MEMORYOBJECTCATEGORY(KeyFrame, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_KEYTRACKS);

	public:
		/**
		 * Default constructor.
		 */
		KeyFrame();

		/**
		 * Constructor which sets the time and value.
		 * @param time The time of the keyframe, in seconds.
		 * @param value The value at this time.
		 */
		KeyFrame(const float time, const ReturnType& value);

		/**
		 * Destructor.
		 */
		~KeyFrame();

		/**
		 * Return the time of the keyframe.
		 * @return The time, in seconds.
		 */
		inline float GetTime() const;

		/**
		 * Return the value of the keyframe.
		 * @return The value of the keyframe.
		 */
		inline ReturnType GetValue() const;

		/**
		 * Set the time of the keyframe.
		 * @param time The time of the keyframe, in seconds.
		 */
		inline void SetTime(const float time);

		/**
		 * Set the value.
		 * @param value The value.
		 */
		inline void SetValue(const ReturnType& value);


	protected:
		StorageType	mValue;	/**< The key value. */
		float		mTime;	/**< Time in seconds. */
};

// include inline code
#include "KeyFrame.inl"

} // namespace EMotionFX


#endif