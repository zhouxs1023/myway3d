/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __POSITIONCONSTRAINT_H
#define __POSITIONCONSTRAINT_H

#include "Constraint.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * A position constraint causes a node to move and follow the position of another node. 
 * This is useful for having an object match the motion of other objects.
 */
class PositionConstraint : public Constraint
{
	DECLARE_CLASS(PositionConstraint);
	MEMORYOBJECTCATEGORY(PositionConstraint, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_CONSTRAINTS);

	public:
		// the type returned by GetType()
		enum { TYPE_ID = 0x00010004 };

		/**
		 * Constructor.
		 * @param actor The actor where the node we want to constraint is part of.
		 * @param node The node to apply this contraint to.
		 * @param target The target to which the previously specified node gets linked to.
		 */
		PositionConstraint(Actor* actor, Node* node, Node* target);

		/**
		 * Destructor.
		 */
		virtual ~PositionConstraint();

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
};

} // namespace EMotionFX


#endif