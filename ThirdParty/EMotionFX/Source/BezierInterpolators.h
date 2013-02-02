/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __BEZIERINTERPOLATORS_H
#define __BEZIERINTERPOLATORS_H

// include the Core system
#include "../Core/Source/MCore.h"

// includ the interpolator
#include "Interpolator.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The Bezier interpolator class.
 * This class will interpolate on a bezier curve, which does not go
 * through the control points. The advantage of this is that it will result in very smooth results.
 */
template <class ReturnType, class StorageType>
class BezierInterpolator : public Interpolator<ReturnType, StorageType>
{
	DECLARE_CLASS(BezierInterpolator);
	MEMORYOBJECTCATEGORY(BezierInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		// the type ID, returned by GetType()
		enum { TYPE_ID = 0x00000002 };

		/**
		 * Default Constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		BezierInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		BezierInterpolator(KeyTrack<ReturnType, StorageType>* interpolationSource);

		/**
		 * Destructor.
		 */
		~BezierInterpolator();

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
		virtual const char* GetTypeString() const	{ return "BezierInterpolator"; }

		/**
		 * Returns the given control point for a given key with both read and write access.
		 * When this method detects that there isn't any control point information allocated yet, it will
		 * automatically allocate new memory. So you do not have to bother about memory management.
		 * The number of control points is always equal to the number which is returned by 
		 * the method KeyTrack<ReturnType, StorageType>::GetNumKeys().
		 * @param keyNr The key number to get the control point for.
		 */
		inline ReturnType& GetControlPoint(const int keyNr);


	protected:
		ReturnType*	mControlPoints;		/**< The control points. */
		int			mNumControlPoints;	/**< The total number of control points. */
};


/**
 * The bezier interpolator class used for quaternions.
 * The reason why there is a special class for quaternions is that they
 * need some additional checks in order to perform the interpolation correctly.
 */
template <class StorageType>
class BezierQuaternionInterpolator : public BezierInterpolator<MCore::Quaternion, StorageType>
{
	MEMORYOBJECTCATEGORY(BezierQuaternionInterpolator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_MOTIONS_INTERPOLATORS);

	public:
		/**
		 * Default constructor.
		 * Do not forget to call SetKeyTrack() if you are not using the KeyTrack::SetInterpolator() method...
		 */
		BezierQuaternionInterpolator();

		/**
		 * Constructor.
		 * @param interpolationSource The keytrack which will be used by the interpolator.
		 */
		BezierQuaternionInterpolator(KeyTrack<MCore::Quaternion, StorageType>* interpolationSource);

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
#include "BezierInterpolators.inl"

} // namespace EMotionFX


#endif