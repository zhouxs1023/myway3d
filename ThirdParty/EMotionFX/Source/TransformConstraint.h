/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __TRANSFORMCONSTRAINT_H
#define __TRANSFORMCONSTRAINT_H

#include "Constraint.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * A transform constraint matches the position, orientation and scaling of one node to another node.
 * This constraint is useful for making several nodes transform, rotate and scale simultaneously.
 */
class TransformConstraint : public Constraint
{
	DECLARE_CLASS(TransformConstraint);
	MEMORYOBJECTCATEGORY(TransformConstraint, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_CONSTRAINTS);

	public:
		// the type returned by GetType()
		enum { TYPE_ID = 0x00010005 };

		/**
		 * Constructor.
		 * @param actor The actor where the node we want to constraint is part of.
		 * @param node The node to apply this contraint to.
		 * @param target The target to which the previously specified node gets linked to.
		 */
		TransformConstraint(Actor* actor, Node* node, Node* target);

		/**
		 * Destructor.
		 */
		virtual ~TransformConstraint();

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