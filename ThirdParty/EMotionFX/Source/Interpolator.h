/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __INTERPOLATOR_H
#define __INTERPOLATOR_H

// include the Core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
template <class ReturnType, class StorageType> class KeyTrack;

/**
 * The keyframe interpolator class.
 * This class can return values at a given time for a given key track.
 * The interpolation scheme used to calculate this value at the given time
 * is implemented inside the templates inherited from this base class.
 */
template <class ReturnType, class StorageType>
class Interpolator
{
	DECLARE_CLASS(Interpolator);
	MEMORYOBJECTCATEGORY(Interpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		Interpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		Interpolator(KeyTrack<ReturnType, StorageType>* interpolationSource);

		/**
		 * Destructor.
		 */
		virtual ~Interpolator();

		/**
		 * Interpolates between keyframe number startKey and the next one.
		 * Keep in mind that startKey must be in range of [0..GetInterpolationSource()->GetNumKeys()-2].
		 * @param startKey The keyframe to start interpolating at. Interpolation will most likely be done between
		 *                 the startKey and the one coming after that.
		 * @param timeValue A value in range of [0..1].
		 * @result Returns the interpolated value.
		 */
		virtual ReturnType Interpolate(const int startKey, const double timeValue) const = 0;

		/**
		 * Initialize the interpolator.
		 * This method should in most curves precalculate the tangents.
		 */
		virtual void Init() = 0;

		/**
		 * Returns the unique interpolator type ID.
		 * This can be used to determine what interpolator is used.
		 * @result The unique type identification number of this interpolator class.
		 */
		virtual int GetType() const = 0;

		/**
		 * Returns the type string of this interpolator class.
		 * This can be the class name, or a description of the interpolator.
		 * @result The string containing the description or class name of this interpolator.
		 */
		virtual const char* GetTypeString() const = 0;

		/**
		 * Get the key track where this interpolator is working on.
		 * @result A pointer to the key track.
		 */
		inline KeyTrack<ReturnType, StorageType>* GetKeyTrack() const;

		/**
		 * Set the key track where this interpolator is working on.
		 * @param interpolationSource A pointer to the key track to use.
		 */
		inline void SetKeyTrack(KeyTrack<ReturnType, StorageType>* interpolationSource);


	protected:
		KeyTrack<ReturnType, StorageType>*		mInterpolationSource;	/**< The keytrack which will be used by the interpolator. */
};

// include the inline code
#include "Interpolator.inl"

} // namespace EMotionFX


#endif