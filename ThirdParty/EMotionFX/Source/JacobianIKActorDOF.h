/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __JACOBIANACTORDOF_H__
#define __JACOBIANACTORDOF_H__

// include the Core system
#include "../Core/Source/MCore.h"

#include "NodeDOF.h"
#include "Actor.h"


namespace EMotionFX
{

/**
 * The JacobianIKActorDOF class.
 * Contains information about the DOFs applied on the nodes of the actor. It obtains the total number of DOFs, which
 * DOF is from each node and which of them gets influence over each node. This attribute will be stored only once per actor
 * and not on node level.
 */
class JacobianIKActorDOF
{
	DECLARE_CLASS(JacobianIKActorDOF)

	public:
		/**
		 * Default constructor.
		 */
		JacobianIKActorDOF();

		/**
		 * Destructor.
		 */
		~JacobianIKActorDOF();

		/**
		 * Update the attributes of the actor. The actor must point to the owner of the attribute instance.
		 * @param actor Pointer to the actor which owns this attribute.
		 */
		inline void Update(Actor* actor);

		/**
		 * Update the attributes of the actor data.
		 */
		void Update();

		/**
		 * Get the number of DOFs applied to the actor.
		 * @return The number of DOFs.
		 */
		inline int GetNumDOF() const;

		/**
		 * Get an array which contains the actual DOF values.
		 * @return The array which includes the values.
		 */
		MCore::Array<float>& GetValues();

		/**
		 * Return true if the given DOF has influence in the position.
		 * @param node A pointer to the node to check.
		 * @param DOFIndex The index of the DOF.
		 */
		inline bool GetInfluence(Node* node, const int DOFIndex);

		/**
		 * Calculate the angle values using the actual actor pose.
		 */
		void CalcAnglesValues();

		/**
		 * Calculate the rotation axis for each DOF.
		 */
		void CalcDOFAxis();

		/**
		 * Get the node that includes the given node DOF.
		 * @param DOFIndex The index of the node DOF.
		 * @return A pointer to the node which includes the given DOF attribute. NULL if the node hasn't been found.
		 */
		Node* GetDOFNode(const int DOFIndex);

		/**
		 * Return a pointer to the actor to which this actor DOF belongs to.
		 * @return A pointer to the actor to which this actor DOF belongs to.
		 */
		inline Actor* GetActor();

		/**
		 * Get the rotation axis for the given node DOF.
		 * @param DOFIndex The index of the node DOF.
		 * @return The axis vector for the given node DOF.
		 */
		MCore::Vector3 GetDOFAxis(const int DOFIndex);

		/**
		 * Apply the given DOF values to the actor.
		 * @param values An array with the values.
		 */
		void SetValues(const MCore::Array<float>& values);

		/**
		 * Return the index of the first DOF referred to any of the node's rotations.
		 * @param index The index of the node.
		 * @return The index to the first node DOF.
		 */
		inline int GetNode1stDOF(const int index);

	private:
		/**
		 * Calculate the influence of each DOF in each node position and store it in the influence matrix.
		 */
		void CalcInfluence();

		MCore::TNMatrix<bool>	mInfluence;		/**< The influence matrix. */
		MCore::Array<int>		mNode1stDOF;	/**< Array containing the index for the 1st DOF of each node. */
		int						mNumDOF;		/**< The total number of DOFs. */
		Actor*					mActor;			/**< The pointer to the Actor the DOFs are applied to. */
};

// include inline code
#include "JacobianIKActorDOF.inl"

} // namespace EMotionFX


#endif