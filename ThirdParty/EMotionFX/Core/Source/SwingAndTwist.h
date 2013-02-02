/*
* EMotion FX 2 - Character Animation System
* Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
* All Rights Reserved.
*/

#ifndef __SWING_AND_TWIST_H
#define __SWING_AND_TWIST_H

// include the required files
#include "Macros.h"
#include "Config.h"
#include "SourceDeclaration.h"
#include "Quaternion.h"
#include "Vector.h"


namespace MCore
{

/**
 * This class represents an orientation via what's called the 'swing-and-twist' representation.
 * The swing-and-twist representation is most useful for imposing joint constraints.
 * This class has some implemented methods for this purpose.
 */
class SwingAndTwist
{
	public:
		/**
		 * The default constructor.
		 * The angles are initialized at 0.
		 */
		SwingAndTwist();
		
		/**
		 * Extended constructor.
		 * @param quat The quaternion from which the swing and twist will be built from.
		 */	
		SwingAndTwist(const MCore::Quaternion& quat);

		/**
		 * Extended constructor.
		 * @param mat The matrix to convert into a swing and twist representation.
		 */	
		SwingAndTwist(const MCore::Matrix& mat);

		/**
		 * The destructor.
		 */
		~SwingAndTwist();

		/**
		 * Build the swing and twist from this quaternion.
		 * @param quat The quaternion that the swing and twist will be built from.
		 */
		void FromQuaternion(const MCore::Quaternion& quat);

		/**
		 * Initialize the swing and twist from a matrix.
		 * Please note that the matrix must not contain any scaling and must be a pure rotation matrix.
		 * @param matrix The matrix to initialize the swing and twist from.
		 */
		void FromMatrix(const MCore::Matrix& mat);

		/**
		 * Convert this swing and twist to a Quaternion.
		 * @result The quaternion representing the swing and twist.
		 */
		MCore::Quaternion ToQuaternion();

		/**
		 * Convert this swing and twist to a Matrix.
		 * @result The matrix representing the swing and twist.
		 */
		MCore::Matrix SwingAndTwist::ToMatrix();

		/**
		 * Constrain the swing using a spherical ellipse.
		 * @param ellipseRadii The x and y radii of the ellipse.
		 * @result Returns true if the swing was outside of the constraints, false if it was inside.
		 */
		bool ConstrainSwingWithEllipse(const MCore::Vector2& ellipseRadii);

		/**
		 * Constrain the swing using a spherical rectangle. This is a lot 
		 * faster than the ellipse, but doesn't look as good. This should probably 
		 * only be used when models are small on screen and use a lower LOD.
		 * @param ellipseRadii The x and y radii of the ellipse
		 * @result Returns true if the swing was outside of the constraints, false if it was inside.
		 */
		bool ConstrainSwingWithRectangle(const MCore::Vector2& rectangleBounds);

		/**
		 * Constrain the twist.
		 * @param minmaxTwist The minimum and maximum values for the twist.
		 * @result Returns true if the twist was outside of the constraints, false if it was inside.
		 */
		bool ConstrainTwist(const MCore::Vector2& minmaxTwist);

	private:
		MCore::Vector2	mSwing;		/**< The swing angles. */
		float			mTwist;		/**< The twist angle. */
};


} // namespace EMotionFX

#endif