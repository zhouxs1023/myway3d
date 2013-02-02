/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __CONSTRAINT_H
#define __CONSTRAINT_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Controller.h"
#include "Actor.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

/**
 * The constraint base class.
 * Constraints enable you to constrain the position, orientation, or scale of an object to other
 * objects. Further, with constraints you can impose specific limits on objects and automate
 * animation processes.
 */
class Constraint : public Controller
{
	DECLARE_CLASS(Constraint);
	MEMORYOBJECTCATEGORY(Constraint, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_CONSTRAINTS);

	public:
		/**
		 * Constructor.
		 * @param actor The actor where the node we want to constraint is part of.
		 * @param node The node to apply this contraint to.
		 * @param target The target to which the previously specified node gets linked to.
		 */
		Constraint(Actor* actor, Node* node, Node* target);

		/**
		 * Destructor.
		 */
		virtual ~Constraint();

		/**
		 * Update the controller (perform the calculations and apply them to the node).
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

		/**
		 * Set the target to which the node gets linked to.
		 * @param target The target node.
		 */
		void SetTarget(Node* target);

		/**
		 * Return the target to which the node gets linked to.
		 * @return The target node.
		 */
		Node* GetTarget() const;

		/**
		 * Get the node where we apply the constraint on.
		 * This is the node that gets modified by the constraint.
		 * @result A pointer to the node.
		 */
		Node* GetNode() const;

	protected:
		Node*	mNode;			/**< The node on which we apply the modifications. */
		Node*	mTarget;		/**< The target node. The specified node will be linked to the 'data' of this target node. */
};

} // namespace EMotionFX


#endif