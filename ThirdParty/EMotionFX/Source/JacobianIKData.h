/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __JACOBIANIKDATA_H__
#define __JACOBIANIKDATA_H__

// include the Core system
#include "../Core/Source/MCore.h"

#include "JacobianIKConstraint.h"
#include "JacobianIKActorDOF.h"


namespace EMotionFX
{

/**
 * The jacobian ik data class.
 * Holds a jacobian ik constraint array and a pointer to the Actor
 * the IK is applied to. It creates an instance of the Actor for 
 * calculation issues.
 */
class JacobianIKData
{
	DECLARE_CLASS(JacobianIKData)

	public:
		/**
		 * Default constructor.
		 */
		JacobianIKData();

		/**
		 * Destructor.
		 */
		~JacobianIKData();

		/**
		 * Get the number of constraints the ik solver has to apply.
		 * @return The number of constraints.
		 */
		inline int GetNumConstraints() const;

		/**
		 * Return a pointer to the actor to which the constraints are applied to.
		 * @return A pointer to the actor to which the constraints are applied to.
		 */
		inline Actor* GetActor() const;

		inline Actor* GetActorInstance() const;

		/**
		 * Set the actor to which the constraints are applied to.
		 * @param actor A pointer to the actor to which the constraints are applied to.
		 */
		void SetActor(Actor* actor);

		/**
		 * Add a constraint which represents the goal to reach.
		 * @param constraint A pointer to the constraint. Could be a pointer to an instance of the
		 *					 JacobianIKFixedPoint or JacobianIKRelativePoint class.
		 */
		inline void AddConstraint(JacobianIKConstraint* constraint);

		/**
		 * Remove the given constraint from the constraints stack.
		 * @param constraint A pointer to the constraint.
		 */
		inline void RemoveConstraint(JacobianIKConstraint* constraint);

		/**
		 * Get a given jacobian ik constraint by index.
		 * @return A pointer to the jacobian ik constraint. NULL if the function fails.
		 */
		inline JacobianIKConstraint* GetConstraint(const int index) const;

		/**
		 * Get the node of the given constraint to which the constraint is refered to.
		 * @param index The index of the constraint to get the node from.
		 * @return A pointer to the node to which the constraint is refered to.
		 */
		inline Node* GetConstraintNode(const int index) const;

		/**
		 * Calculate the derivative of the node position respect to the given node DOF.
		 * @param node A pointer to the node.
		 * @param DOFIndex Index of the node DOF.
		 * @return A vector with the 3 coordinates derivatives.
		 */
		MCore::Vector3 CalcPosDer(Node* node, const int DOFIndex);

		/**
		 * Get the number of DOFs in the IK system.
		 * @return The number of DOFs.
		 */
		inline int GetNumDOF() const;

		/**
		 * Get the number of DOFs of the given node.
		 * @param node Pointer to the node.
		 * @return The number of DOFs.
		 */
		inline int GetNumDOF(Node* node) const;

		/**
		 * Fill in the given array with the DOF variables from the actor's actual pose.
		 * The array will be cleared before values are inserted.
		 * @param values The array where the values are going to be inserted. 
		 */
		void GetDOFValues(MCore::Array<float>& values);

		/**
		 * Return true if the given DOF has influence in the node position.
		 * @param node Pointer to the node.
		 * @param DOFIndex The DOF index.
		 * @return True if the given DOF has incluence in the node position.
		 */
		inline bool GetInfluence(Node* node, const int DOFIndex) const;

		/**
		 * Return the node index in the actor nodes array.
		 * @param node Pointer to the node.
		 * @return The node index in actor's array.
		 */
		inline int GetNodeID(Node* node) const;

		/**
		 * Apply the values in the given array to the DOF on the actor instance.
		 * @param values Array which contains the values.
		 */
		inline void SetValues(MCore::Array<float>& values);

		/**
		 * Update all constraints which are managed by the jacobian ik data.
		 * @param timeDeltaInSeconds The time delta since the last frame in seconds.
		 */
		void UpdateConstraints(const float timeDeltaInSeconds);

		/**
		 * Apply the DOF values resulted by the IK.
		 * @param solution Array with the DOF values
		 */
		void Apply(MCore::Array<float>& solution);

		/**
		 * Initialize actor instance with actor pose.
		 */
		void InitInstance();

		/**
		 * Return the index of the first DOF of the node.
		 * @param node The node where to search the first DOF.
		 * @return The index of the first DOF.
		 */
		inline int GetNode1stDOF(Node* node) const;

		/**
		 * Return max value for the DOF 'index'.
		 * @param index The DOF index
		 * @return The maximum value of the given DOF.
		 **/
		float GetMax(const int index);

		/**
		 * Return min value for the DOF 'index'.
		 * @param index The DOF index
		 * @return The minimum value of the given DOF.
		 **/
		float GetMin(const int index);

	private:
		MCore::Array<JacobianIKConstraint*> mConstraints;		/**< The constraint array. */
		JacobianIKActorDOF*					mJacobianActorDOF;	/**< The jacobian ik actor DOF of the actor to which the constraints are applied to. */
		Actor*								mActorInstance;		/**< Actor instance for calculation issues. */
		Actor*								mActor;				/**< The actor to which the jacobian ik data belongs to. */
};

// include inline code
#include "JacobianIKData.inl"

} // namespace EMotionFX


#endif