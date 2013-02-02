/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __TCBSPLINEINTERPOLATORS_H
#define __TCBSPLINEINTERPOLATORS_H

// include the Core system
#include "../Core/Source/MCore.h"

// include the interpolator
#include "Interpolator.h"
#include "HermiteInterpolators.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The TCB spline interpolator class.
 * TCB stands for Tension, Continuity and Bias. These are properties used to describe
 * the behavior of the spline. It basically is a specialized form of the Hermite spline.
 * When there is no TCB information set (when there never has been a call to GetTCB())
 * the interpolation will just be the same as the HermiteInterpolator method.
 * In other words, in that case the TCB values will all be equal to zero.
 */
template <class ReturnType, class StorageType>
class TCBSplineInterpolator : public HermiteInterpolator<ReturnType, StorageType>
{
	DECLARE_CLASS(TCBSplineInterpolator<>);
	MEMORYOBJECTCATEGORY(TCBSplineInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		// the type ID, returned by GetType()
		enum { TYPE_ID = 0x00000004 };

		/**
		 * The TCB information for each key.
		 * All attributes inside the struct must be in range of [0..1].
		 */
		struct TCBInfo
		{
			float	mTension;		/**< The tension. */
			float	mContinuity;	/**< The continuity. */
			float	mBias;			/**< The bias. */

			TCBInfo() : mTension(0), mContinuity(0), mBias(0) {}
		};

	
		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		TCBSplineInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		TCBSplineInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource);

		/**
		 * Destructor.
		 */
		~TCBSplineInterpolator();

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
		virtual const char* GetTypeString() const	{ return "TCBSplineInterpolator"; }

		/**
		 * Returns the given TCB information for a given key with both read and write access.
		 * When this method detects that there isn't any TCB information allocated yet, it will
		 * automatically allocate new memory. So you do not have to bother about memory management.
		 * The number of TCBInfo structs is always equal to the number which is returned by 
		 * the method KeyTrack<ReturnType, StorageType>::GetNumKeys().
		 * @param keyNr The key number to get the TCB information from.
		 */
		inline TCBInfo&	GetTCB(const int keyNr);


	protected:
		int			mNumTCBs;
		TCBInfo*	mTCBs;
};


/**
 * The TCB spline interpolator class used for quaternions.
 * The reason why there is a special class for quaternions is that they
 * need some additional checks in order to perform the interpolation correctly.
 */
template <class StorageType>
class TCBSplineQuaternionInterpolator : public TCBSplineInterpolator<MCore::Quaternion, StorageType>
{
	MEMORYOBJECTCATEGORY(TCBSplineQuaternionInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		TCBSplineQuaternionInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		TCBSplineQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource);

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

// include the inline code
#include "TCBSplineInterpolators.inl"

} // namespace EMotionFX


#endif