/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __JACOBIANIKCONTROLLER_H__
#define __JACOBIANIKCONTROLLER_H__

// include the Core system
#include "../Core/Source/MCore.h"

#include "Controller.h"
#include "JacobianIKSolver.h"


namespace EMotionFX
{

/**
 * The jacobian ik controller class. 
 * The jacobian pseudo inverse controller inherits from the base controller. It must be added as a postcontroller.
 * It applies the added constraints using jacobian pseudo inverse solver.
 */
class JacobianIKController: public Controller
{
	DECLARE_CLASS(JacobianIKController)

	public:
		// the unique type ID of this controller
		enum { TYPE_ID = 0xCADA006 };

		/**
		 * Constructor.
		 * @param actor The actor to which the controller gets applied to.
		 */
		JacobianIKController(Actor* actor);

		/**
		 * Destructor.
		 */
		~JacobianIKController();

		/**
		 * Update the controller. This is the main method that performs the IK and updates the nodes.
		 * @param timeDeltaInSeconds The time passed in seconds, since the last update call.
		 */
		void Update(const float timeDeltaInSeconds);

		/**
		 * Clone this controller.
		 * @param actor The actor to apply the cloned version on.
		 * @result A pointer to the cloned controller.
		 */
		Controller* Clone(Actor* actor);

		/**
		 * Get access to the ik data.
		 * @return A pointer to the ik data.
		 */
		inline JacobianIKData* GetIKData();

		/**
		 * Return the actor instance node. It´s useful for applying constraints.
		 * @param name The name of the node to get.
		 * @return A pointer to the searched node.
		 **/
		inline Node* GetNodeByName(const char* name);

		/**
		 * Get the unique controller type ID.
		 * @return The controller type identification number.
		 */
		int GetType() const;

		/**
		 * Get the type identification string.
		 * This can be a description or the class name of the controller.
		 * @return A pointer to the string containing the name.
		 */
		const char* GetTypeString() const;

	private:
		JacobianIKData*		mData;		/**< The jacobian ik data, e.g. the constraints. */
		JacobianIKSolver*	mSolver;	/**< The jacobian pseudo inverse solver. */
		MCore::Array<float>	mSolution;	/**< The calculated solution from the last update call. */
};

// include inline code
#include "JacobianIKController.inl"

} // namespace EMotionFX


#endif