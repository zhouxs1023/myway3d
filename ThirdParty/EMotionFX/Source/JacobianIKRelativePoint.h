/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __JACOBIANIKRELATIVEPOINT_H__
#define __JACOBIANIKRELATIVEPOINT_H__

// include the Core system
#include "../Core/Source/MCore.h"

#include "JacobianIKConstraint.h"

namespace EMotionFX
{

/**
 * The JacobianIKRelativePoint class.
 * Defines the constraints applied to an Actor using IK.
 * This constraint specifies a point where a Node must be.
 */
class JacobianIKRelativePoint: public JacobianIKConstraint
{
	DECLARE_CLASS(JacobianIKRelativePoint)

	public:
		/**
		 * Default constructor.
		 */
		JacobianIKRelativePoint();

		/**
		 * Constructor.
		 * @param displacement Displacement vector which the solver tries to reach.
		 */
		JacobianIKRelativePoint(const MCore::Vector3& displacement);

		/**
		 * Constructor.
		 * @param x The x component of the displacement vector which the solver tries to reach.
		 * @param y The y component of the displacement vector which the solver tries to reach.
		 * @param z The z component of the displacement vector which the solver tries to reach.
		 */
		JacobianIKRelativePoint(const float x, const float y, const float z);

		/**
		 * Destructor
		 */
		~JacobianIKRelativePoint();

		/**
		 * Clone the IK constraint.
		 * @return A pointer to a newly created exact copy of the IK constraint.
		 */
		JacobianIKConstraint* Clone();

		/**
		 * Update the jacobian ik constraint.
		 * @param timeDeltaInSeconds The time passed in seconds.
		 */
		void Update(const float timeDeltaInSeconds);

		/**
		 * Return the number of functions needed to apply the constraint.
		 * @return The number of functions.
		 */
		int GetNumFunc() const;

		/**
		 * Return the specified value for the functions.
		 * @param values An array where the values will be placed.
		 */
		void GetValues(MCore::Array<float>& values);

		/**
		 * Return the value for the functions using the actual state of the actor.
		 * @param results An array where the values will be placed.
		 */
		void GetResults(MCore::Array<float>& results);

		/**
		 * Calculate the constraint functions gradients and store them in the given matrix.
		 * @param matrix The matrix where the gradients are stored.
		 */
		 void CalcGradient(MCore::NMatrix& matrix);

		/**
		 * Sets the position where the node must be placed.
		 * @param v The vector that specifies the position.
		 */
		 inline void SetDisplacement(const MCore::Vector3& displacement);

	private:
		 MCore::Vector3 mPosition;		/**< The position where the node must be placed by IK */
		 MCore::Vector3 mDisplacement;
};

// include inline code
#include "JacobianIKRelativePoint.inl"

} // namespace EMotionFX


#endif