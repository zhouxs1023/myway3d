/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __HERMITEINTERPOLATORS_H
#define __HERMITEINTERPOLATORS_H

// include the Core system
#include "../Core/Source/MCore.h"

// include the interpolator base class
#include "Interpolator.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The Hermite interpolator class.
 * A hermite is a spline which goes through its control points.
 * This means that you will get smooth motion, going through all keys you specified.
 */
template <class ReturnType, class StorageType>
class HermiteInterpolator : public Interpolator<ReturnType, StorageType>
{
	DECLARE_CLASS(HermiteInterpolator);
	MEMORYOBJECTCATEGORY(HermiteInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		// the type ID, returned by GetType()
		enum { TYPE_ID = 0x00000003 };

		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		HermiteInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		HermiteInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource);

		/**
		 * Destructor.
		 */
		~HermiteInterpolator();

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
		 * In this case this will calculate the tangents for the keys.
		 */
		virtual void Init();

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
		virtual const char* GetTypeString() const	{ return "HermiteInterpolator"; }

		/**
		 * Get the tangent for a given key, with both read and write access.
		 * Be sure to call this only AFTER the Init method has been called.
		 * Otherwise the tangent data won't yet be allocated, or the number of tangents might
		 * be different from the number of keys inside the keytrack.
		 * @param keyNr The key number to get the tangent from.
		 * @result A reference to the tangent for the given key, including write access.
		 */
		inline ReturnType& GetTangent(const int keyNr);


	protected:
		ReturnType*		mTangents;	/**< The tangent vectors, one for each key. */
};



/**
 * The hermite interpolator class used for quaternions.
 * The reason why there is a special class for quaternions is that they
 * need some additional checks in order to perform the interpolation correctly.
 */
template <class StorageType>
class HermiteQuaternionInterpolator : public HermiteInterpolator<MCore::Quaternion, StorageType>
{
	MEMORYOBJECTCATEGORY(HermiteQuaternionInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		HermiteQuaternionInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		HermiteQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource);

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
#include "HermiteInterpolators.inl"

} // namespace EMotionFX

#endif