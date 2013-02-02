/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __PHYSICSCONTROLLER_H
#define __PHYSICSCONTROLLER_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Controller.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
class Actor;
class NodePhysicsAttribute;
class NodeLimitAttribute;

/**
 * The physics controller base class.
 * This controller acts as a base class for implementations with several physics systems.
 * The purpose of the controller is the ability to apply rigid body dynamics and ragdolls to individual nodes
 * and complete actors.
 */
class PhysicsController : public Controller
{
	DECLARE_CLASS(PhysicsController);
	MEMORYOBJECTCATEGORY(PhysicsController, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_PHYSICS);

	public:
		/**
		 * Constructor.
		 * @param actor The actor to apply this physics controller to.
		 */
		PhysicsController(Actor* actor);

		/**
		 * Destructor.
		 */
		virtual ~PhysicsController();

		/**
		 * Update the physics controller (perform the calculations and apply them to the nodes).
		 * @param timeDeltaInSeconds The time passed in seconds since the last call.
		 */
		virtual void Update(const float timeDeltaInSeconds) = 0;

		/**
		 * Creates a clone of the controller, which will be applied to the specified actor.
		 * @param actor The actor that will get the returned cloned controller attached to it.
		 *              This method does NOT add the controller to the specified actor. You have to do that yourself.
		 * @result A pointer to the cloned controller.
		 */
		virtual Controller* Clone(Actor* actor) = 0;


	protected:
		/**
		 * Retrieves the physics attribute from the specific node. If the function fails, NULL will be returned.
		 * @param index Index of the node in the actor.
		 * @return A pointer to the physics attribute of the given node.
		 */
		NodePhysicsAttribute* GetNodePhysicsAttribute(const int index);

		/**
		 * Retrieves the limit attribute from the specific node. If the function fails, NULL will be returned.
		 * @param index Index of the node in the actor.
		 * @return A pointer to the physics attribute of the given node.
		 */
		NodeLimitAttribute* GetNodeLimitAttribute(const int index);
};

} // namespace EMotionFX


#endif