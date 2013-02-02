/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __LOOKAT_CONSTRAINT_H
#define __LOOKAT_CONSTRAINT_H

// include the required files
#include "Constraint.h"


namespace EMotionFX
{

/**
 * A look at constraint constrains an node's orientation so that it aims at other nodes. Typical uses
 * of the look at constraint include aiming a light or camera at an node or group of nodes.
 * Another use of a look at constraint would be controling eyeball movement of a character.
 * Remember to add the look-at constraint as a post controller to the node since it works on the node's orientation.
 * If you add it as normal (pre) controller the calculated rotation of the node would be overwritten by the motions etc.
 */
class LookAtConstraint : public Constraint
{
	DECLARE_CLASS(LookAtConstraint)

	public:
		// the type returned by GetType()
		enum { TYPE_ID = 0x00010001 };
		
		/**
 		 * The different types of constraints that can be applied.
		 */		
		enum EConstraintType
		{
			CONSTRAINT_NONE			= 0,	/**< Do not use any constraints. */
			CONSTRAINT_ELLIPSE		= 1,	/**< Contraint using an ellipse, which is the nicest. */
			CONSTRAINT_RECTANGLE	= 2		/**< Constraint using a rectangle, which doesn't look nice, but is fast (use it on low LOD levels). */
		};

		/**
		 * Constructor.
		 * @param actor The actor where the node we want to constraint is part of.
		 * @param node The node to apply this contraint to.
		 * @param target The target to which the previously specified node gets linked to.
		 *               This target node can be NULL, in that case the goal position will be used that
		 *               you can control with SetGoal() and GetGoal().
		 */
		LookAtConstraint(Actor* actor, Node* node, Node* target);

		/**
		 * Destructor.
		 */
		~LookAtConstraint();

		/**
		 * Update the contraint (perform the calculations and apply them to the node).
		 * @param timeDeltaInSeconds The time passed in seconds since the last call.
		 */
		void Update(const float timeDeltaInSeconds);

		/**
		 * Creates a clone of the controller, which will be applied to the specified actor.
		 * @param actor The actor that will get the returned cloned controller attached to it.
		 *              This method does NOT add the controller to the specified actor. You have to do that yourself.
		 * @result A pointer to the cloned controller.
		 */
		Controller* Clone(Actor* actor);

		/**
		 * Get the unique controller type ID.
		 * @result The controller type identification number.
		 */
		int GetType() const;

		/**
		 * Get the type identification string.
		 * This can be a description or the class name of the controller.
		 * @result A pointer to the string containing the name.
		 */
		const char* GetTypeString() const;

		/**
		 * Get the goal position, this is used in case there is no target node set.
		 * @result The goal position in world space.
		 */
		const MCore::Vector3& GetGoal() const;

		/**
		 * Set the goal world space position. This position is used in case there is no target node set.
		 * @param goalWorldSpacePos The world space position where the node should look at.
		 */
		void SetGoal(const MCore::Vector3& goalWorldSpacePos);

		/**
 		 * Set the interpolation speed factor, which is how fast/smooth the node will rotate to point at the goal.
		 * On default this value is set to 1.0f.
		 * @param val The new interpolation speed factor.
		 */
		void SetInterpolationSpeed(const float factor);

		/**
 		 * Get the interpolation speed factor, which is how fast/smooth the node will rotate to point at the goal.
		 * On default this value is set to 1.0f.
		 * @result The interpolation speed factor.
		 */
		float GetInterpolationSpeed() const;

		/**
		 * The overloaded enable/disable method, which enables or disables the controller.
		 * Controllers are always enabled after they have been created.
		 * @param enable Set to true when you want to enable the controller, or false when you want to disable it.
		 */
		void SetEnabled(const bool enable);

		/**
		 * Set the pre-rotation matrix. This matrix rotates the node's up-vector, which will be used during the calculations.
		 * This is useful when you expect the up vector of the head to point upwards, while it actually points forward mathematically.
		 * You can then set a pre-rotation matrix that would rotate the forward vector into a vector that points upwards to get the desired result.
		 * If you do not do this, the head might face towards the goal with the wrong direction, for example with the top of the head instead of the face itself.
		 * @param mat The pre-rotation matrix, which is identity on default.
		 */
		void SetPreRotation(const MCore::Matrix& mat);

		/**
		 * Get the current pre-rotation matrix. For more information about what exactly the pre-rotation matrix is, please read
		 * the documentation of the SetPreRotation method.
		 * @result The matrix containing the pre-rotation which rotates the original up-vector in the desired up-vector.
		 * @see SetPreRotation
		 */
		const MCore::Matrix& GetPreRotation() const;

		/**
		 * Set the post rotation matrix. This matrix is being applied after the new orientation of the node has been calculated.
		 * If you use a pre-rotation, or if the object already was rotated, you most likely have to specify a post rotation that
		 * will rotate the object back into the desired orientation.
		 * Also sometimes you might want the head to look a bit more downwards on default. The post rotation matrix, which you specify here, can
		 * be used to add a little bit of extra rotation to the node, after the lookat has been applied. So we could rotate the head a bit, so it
		 * will look more downwards, which might give a more desired effect.
		 * @param mat The post rotation matrix. On default this is an identity matrix.
		 */
		void SetPostRotation(const MCore::Matrix& mat);

		/**
		* Get the current post rotation matrix. On default this is set to identity.
		* You can read more about what exactly the post rotation matrix is in the documentation of the SetPostRotation method.
		* @result The post rotation matrix.
		* @see SetPostRotation
		*/
		const MCore::Matrix& GetPostRotation() const;

		/**
		 * Set the orientation of the ellipse. This must be a rotation-only matrix.
		 * The ellipse is projected onto a sphere. The center of the ellipse will be projected the center of the sphere
		 * that has been rotated using the ellipse orientation matrix. The area of the ellipse marks the area of valid rotations of the look-at node.
		 * That means that the rotations will be constraint to be inside the ellipse area.
		 * @param mat The rotation matrix that marks the center of the ellipse.
		 */
		void SetEllipseOrientation(const MCore::Matrix& mat);

		/**
		 * Get the orientation matrix of the ellipse.
		 * For more information about what exactly this orientation is, please read the documentation of the SetEllipseOrientation method.
		 * @result The ellipse orientation (rotation) matrix.
		 * @see SetEllipseOrientation
		 */
		const MCore::Matrix& GetEllipseOrientation() const;

		/**
		 * Set the width and height of the ellipse that will be projected on a unit sphere.
		 * This defines the rotational limits of the look-at node.
		 * @param radii The width and height of the ellipse.
		 */
		void SetEllipseRadii(const MCore::Vector2& radii);

		/**
		 * Set the width and height of the ellipse, using two floats instead of a Vector2.
		 * @param rx The width of the ellipse.
		 * @param ry The height of the ellipse.
		 */
		void SetEllipseRadii(const float rx, const float ry);

		/**
		* Set the minimum and maximum twist angles, in radians.
		* @param minValue The minimum twist value, in radians.
		* @param maxValue The maximum twist value, in radians.
		*/
		void SetTwist(const float minValue, const float maxValue);

		/**
		 * Set the minimum and maximum twist angles, in radians, by using a Vector2.
		 * The x component must contain the minimum twist, and the y component the maximum.
		 * @param minMaxTwist The minimum and maximum twist angles, in radians.
		 */
		void SetTwist(const MCore::Vector2& minMaxTwist);

		/**
		 * Set the constraints via euler angles instead of ellipse width and height. 
		 * This converts the input into the radii for the ellipse and the twist value.
		 * The Vector3 x, y, z, stand for the rotation angles around each axis, in radians.
		 * @param minVals The minimum euler angles, in radians.
		 * @param maxVals The maximum euler angles, in radians.
		 */
		void SetEulerConstraints(const MCore::Vector3& minVals, const MCore::Vector3& maxVals);

		/**
		 * Set the constraint type. This allows you to disable constraints, or enable them, and choose the
		 * method of constraining. Available methods are using ellipse and rectangle.
		 * The rectangle method is the faster than the ellipse method, but produces less desirable results.
		 * For some types of constraints however you'd probably like to use the rectangle, especially when you
		 * want to disable the rotation on a specific axis.
		 * On default the constraints are disabled.
		 * @param constraintType The constraint type to use.
		 */
		void SetConstraintType(EConstraintType constraintType);

		/**
		 * Get the constraint type that is currently used.
		 * On default the constraints are disabled.
		 * @result The constraint type.
		 */
		EConstraintType GetConstraintType() const;


	private:
		MCore::Matrix		mPreRotation;			/**< This is used basically to tell the controller which vector to use as the up-vector. **/
		MCore::Matrix		mPostRotation;			/**< Used to make sure the node 'looks' along the new forward vector. **/
		MCore::Matrix		mEllipseOrientation;	/**< The orientation of the ellipse. */
		MCore::Quaternion	mRotationQuat;			/**< The current orientation of the node. **/
		MCore::Vector3		mGoal;					/**< The goal position in case there is no target node set. */
		MCore::Vector2		mEllipseRadii;			/**< The radius values for the ellipse, that specify its dimensions. */
		MCore::Vector2		mMinMaxTwist;			/**< The twist rotation limits. */
		float				mInterpolationSpeed;	/**< The speed factor of the interpolation (default = 1.0f). */
		EConstraintType		mConstraintType;		/**< Which type of constraint to use. */
};


} // namespace EMotionFX


#endif
