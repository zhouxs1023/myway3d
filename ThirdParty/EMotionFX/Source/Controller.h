/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// include the Core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
class Actor;

/**
 * The controller base class. 
 * Controllers enable you to modify the position, orientation, scale etc. of a nodes within a specified actor.
 * Further, with controllers you can automate animation processes.
 */
class Controller
{
	DECLARE_CLASS(Controller);
	MEMORYOBJECTCATEGORY(Controller, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_MISC);

	public:
		/**
		 * Constructor.
		 * The controller is set to enabled on default.
		 * @param actor The actor to apply the node to.
		 */
		Controller(Actor* actor);

		/**
		 * Destructor.
		 */
		virtual ~Controller();

		/**
		 * Update the controller (perform the calculations and apply them to the node).
		 * @param timeDeltaInSeconds The time passed in seconds since the last call.
		 */
		virtual void Update(const float timeDeltaInSeconds) = 0;

		/**
		 * Returns the actor this controller is working on.
		 * @return A pointer to the actor.
		 */
		Actor* GetActor() const;

		/**
		 * Creates a clone of the controller, which will be applied to the specified actor.
		 * @param actor The actor that will get the returned cloned controller attached to it.
		 *              This method does NOT add the controller to the specified actor. You have to do that yourself.
		 * @result A pointer to the cloned controller.
		 */
		virtual Controller* Clone(Actor* actor) = 0;

		/**
		 * Get the unique controller type ID.
		 * @result The controller type identification number.
		 */
		virtual int GetType() const = 0;

		/**
		 * Get the type identification string.
		 * This can be a description or the class name of the controller.
		 * @result A pointer to the string containing the name.
		 */
		virtual const char* GetTypeString() const = 0;

		/**
		 * Check if the controller is enabled or not.
		 * @result Returns true when the controller is active (enabled) or false when the controller is inactive (disabled).
		 */
		bool IsEnabled() const;

		/**
		 * Enable or disable the controller.
		 * Disabling a controller just results in the Update method of the controller not being called during the Actor::Update() call.
		 * @param enabled Set to true when you want to enable the controller or to false when you want to disable the controller.
		 */
		virtual void SetEnabled(const bool enabled);


	protected:
		Actor*	mActor;		/**< The actor where this controller works on. */
		bool	mIsEnabled;	/**< Is this controller enabled? */
};

} // namespace EMotionFX


#endif