/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __ROTATIONCONSTRAINT_H
#define __ROTATIONCONSTRAINT_H

#include "Constraint.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * A rotation constraint matches the orientation of one the node to another node. This constraint
 * is useful for making several nodes orient simultaneously. For example, you can make a group of
 * characters all look in the same direction at the same time by animating one character's head
 * and then constraining all the other character's heads to the head you've just animated.
 */
class RotationConstraint : public Constraint
{
	DECLARE_CLASS(RotationConstraint);
	MEMORYOBJECTCATEGORY(RotationConstraint, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_CONSTRAINTS);

	public:
		// the type returned by GetType()
		enum { TYPE_ID = 0x00010002 };

		/**
		 * Constructor.
		 * @param actor The actor where the node we want to constraint is part of.
		 * @param node The node to apply this contraint to.
		 * @param target The target to which the previously specified node gets linked to.
		 */
		RotationConstraint(Actor* actor, Node* node, Node* target);

		/**
		 * Destructor.
		 */
		virtual ~RotationConstraint();

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
		 * Set how strongly the motion of the node is diminished by the constraint.
		 * @param damping The damping value.
		 */
		void SetDamping(const float damping);

		/**
		 * Return how strongly the motion of the node is diminished by the constraint.
		 * @return The damping value.
		 */
		float GetDamping() const;

	protected:
		float	mDamping;		/**< Specifies how strongly the motion of the node is diminished by the constraint. */
};

} // namespace EMotionFX


#endif