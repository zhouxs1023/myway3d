/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MOTIONPART_H
#define __MOTIONPART_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Keytrack.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * A motion part.
 * This is an animated part of a complete animation. For example an animated
 * bone. An example could be the animation of the 'upper arm'. A complete set of motion parts 
 * together form a complete motion (for example a walk motion).
 */
class MotionPart
{
	DECLARE_CLASS(MotionPart);
	MEMORYOBJECTCATEGORY(MotionPart, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_SKELETALMOTIONS);

	public:
		/**
		 * Default constructor.
		 */
		MotionPart();

		/**
		 * Constructor.
		 * @param name The name of the motion part.
		 */
		MotionPart(const MCore::String& name);

		/**
		 * Destructor.
		 */
		~MotionPart();

		/**
		 * Returns the position keyframing track.
		 * @result The position keyframing track.
		 */
		inline KeyTrack<MCore::Vector3, MCore::Vector3>& GetPosTrack();

		/**
		 * Returns the rotation keyframing track.
		 * @result The rotation keyframing track.
		 */
		inline KeyTrack<MCore::Quaternion, MCore::Compressed16BitQuaternion>& GetRotTrack();

		/**
		 * Returns the scaling keyframing track.
		 * @result The scaling keyframing track.
		 */
		inline KeyTrack<MCore::Vector3, MCore::Vector3>& GetScaleTrack();

		/**
		 * Returns the name, in form of a C/C++ character buffer.
		 * @result A pointer to the null terminated character buffer, containing the  name of this motion part.
		 */
		inline const char* GetName() const;

		/**
		 * Returns the original pose position of this part.
		 * @result The original pose position of this part.
		 */
		inline const MCore::Vector3& GetPosePos() const;

		/**
		 * Returns the original pose rotation of this part.
		 * @result The original pose rotation of this part.
		 */
		inline const MCore::Quaternion& GetPoseRot() const;

		/**
		 * Returns the original pose scaling of this part.
		 * @result The original pose scaling of this part.
		 */
		inline const MCore::Vector3& GetPoseScale() const;

		/**
		 * Set the original pose position.
		 * @param pos The original pose position.
		 */
		inline void SetPosePos(const MCore::Vector3& pos);

		/**
		 * Set the original pose rotation.
		 * @param pos The original pose rotation.
		 */
		inline void SetPoseRot(const MCore::Quaternion& rot);

		/**
		 * Set the original pose scaling.
		 * @param pos The original pose scaling.
		 */
		inline void SetPoseScale(const MCore::Vector3& scale);


	private:
		MCore::String									mName;			/**< The name of the motion part. */
		KeyTrack<MCore::Quaternion, MCore::Compressed16BitQuaternion>	mRotTrack;		/**< The rotation key track. */
		KeyTrack<MCore::Vector3,	MCore::Vector3>		mPosTrack;		/**< The position key track. */
		KeyTrack<MCore::Vector3,	MCore::Vector3>		mScaleTrack;	/**< The scale key track. */
		MCore::Quaternion								mPoseRot;		/**< The original base pose rotation. */
		MCore::Vector3									mPosePos;		/**< The original base pose position. */
		MCore::Vector3									mPoseScale;		/**< The original base pose scale. */
};

// include the inline code
#include "MotionPart.inl"

} // namespace EMotionFX


#endif