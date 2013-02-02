/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __JACOBIANIKCONSTRAINT_H__
#define __JACOBIANIKCONSTRAINT_H__

// include the Core system
#include "../Core/Source/MCore.h"

#include "Node.h"


namespace EMotionFX
{

// forward declarations.
class JacobianIKData;

/**
 * The JacobianIKConstraint class.
 * Base class of a specific jacobian ik constraint which is applied to an actor using the jacobian IK controller.
 */
class JacobianIKConstraint
{
	DECLARE_CLASS(JacobianIKConstraint)

	public:
		/**
		 * Default constructor.
		 */
		JacobianIKConstraint(){}

		/**
		 * Destructor.
		 */
		~JacobianIKConstraint(){}

		/**
		 * Clone the IK constraint.
		 * @return A pointer to a newly created exact copy of the IK constraint.
		 */
		virtual JacobianIKConstraint* Clone() = 0;

		/**
		 * Update the jacobian ik constraint.
		 * @param timeDeltaInSeconds The time passed in seconds.
		 */
		virtual void Update(const float timeDeltaInSeconds) = 0;

		/**
		 * Return the number of functions needed to apply the constraint.
		 * @return The number of functions.
		 */
		virtual int GetNumFunc() const = 0;

		/**
		 * Return the specified value for the functions.
		 * @param values An array where the values will be placed.
		 */
		virtual void GetValues(MCore::Array<float>& values) = 0;

		/**
		 * Return the value for the functions using the actual state of the actor.
		 * @param results An array where the values will be placed.
		 */
		virtual void GetResults(MCore::Array<float>& results) = 0;

		/**
		 * Calculate the constraint functions gradients and store them in the given matrix.
		 * @param matrix The matrix where the gradients are stored.
		 */
		 virtual void CalcGradient(MCore::NMatrix& matrix) = 0;

		/**
		 * Return the node the constraint is applied to.
		 * @return The pointer to the node to which the constraint is applied to.
		 */
		inline Node* GetNode();

		/**
		 * Return a pointer to the node the IK is applied from.
		 * @return A pointer to the base node.
		 */
		inline Node* GetBaseNode();

		/**
		 * Set the node the constraint is applied to.
		 * @param node A pointer to the node to which the constraint is applied to.
		 */
		inline void SetNode(Node* node);

		/**
		 * Set the pointer to a node the IK is applied from.
		 * @param node A pointer to the base node.
		 */
		inline void SetBaseNode(Node* node);

		/**
		 * Set the jacobian ik data to work with.
		 * @param data A Pointer to the jacobian ik data.
		 */
		inline void SetIKData(JacobianIKData* data);

	protected:
		Node*			mBaseNode;		/**< Node from which IK is applied. */
		Node*			mNode;			/**< Constrained node. */
		JacobianIKData*	mIKData;		/**< Pointer to JacobianIKData class. It´s needed to get information about nodes parameters and derivatives. */
};

// include inline code
#include "JacobianIKConstraint.inl"

} // namespace EMotionFX


#endif