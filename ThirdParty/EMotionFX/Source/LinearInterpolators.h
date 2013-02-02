/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __LINEARINTERPOLATORS_H
#define __LINEARINTERPOLATORS_H

// include the Core system
#include "../Core/Source/MCore.h"

// include the interpolator
#include "Interpolator.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The linear interpolator class.
 * This class interpolates value on a linear way, so things are not curved.
 */
template <class ReturnType, class StorageType>
class LinearInterpolator : public Interpolator<ReturnType, StorageType>
{
	DECLARE_CLASS(LinearInterpolator);
	MEMORYOBJECTCATEGORY(LinearInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		// the type ID, returned by GetType()
		enum { TYPE_ID = 0x00000001 };

		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		LinearInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		LinearInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource);

		/**
		 * Destructor.
		 */
		~LinearInterpolator();

		/**
		 * Interpolates between keyframe number startKey and the next one.
		 * Keep in mind that startKey must be in range of [0..GetInterpolationSource()->GetNumKeys()-2].
		 * @param startKey The keyframe to start interpolating at. Interpolation will most likely be done between
		 *                 the startKey and the one coming after that.
		 * @param timeValue A value in range of [0..1].
		 * @result Returns the interpolated value.
		 */
		virtual ReturnType Interpolate(const int startKey, const double timeValue) const;

		/**
		 * Initialize the interpolator. 
		 * This will calculate the delta values to speed up interpolation.
		 */
		void Init();

		/**
		 * Returns the unique interpolator type ID.
		 * This can be used to determine what interpolator is used.
		 * @result The unique type identification number of this interpolator class.
		 */
		virtual int GetType() const					{ return TYPE_ID; }

		/**
		 * Returns the type string of this interpolator class.
		 * This can be the class name, or a description of the interpolator.
		 * @result The string containing the description or class name of this interpolator.
		 */
		virtual const char* GetTypeString() const	{ return "LinearInterpolator"; }
};


/**
 * The linear interpolator class used for quaternions.
 * The reason why there is a special class for quaternions is that they
 * need some additional checks in order to perform the interpolation correctly.
 */
template <class StorageType>
class LinearQuaternionInterpolator : public LinearInterpolator<MCore::Quaternion, StorageType>
{
	MEMORYOBJECTCATEGORY(LinearQuaternionInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		LinearQuaternionInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		LinearQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource);

		/**
		 * Interpolates between keyframe number startKey and the next one.
		 * Keep in mind that startKey must be in range of [0..GetInterpolationSource()->GetNumKeys()-2].
		 * @param startKey The keyframe to start interpolating at. Interpolation will most likely be done between
		 *                 the startKey and the one coming after that.
		 * @param timeValue A value in range of [0..1].
		 * @result Returns the interpolated value.
		 */
		MCore::Quaternion Interpolate(const int startKey, const double timeValue) const;
};

// include inline code
#include "LinearInterpolators.inl"

} // namespace EMotionFX

#endif