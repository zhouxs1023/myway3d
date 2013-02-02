/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __EXPRESSIONPARTCONTROLLER_H
#define __EXPRESSIONPARTCONTROLLER_H

// include the Core system
#include "../Core/Source/MCore.h"

#include "Controller.h"
#include "ExpressionPart.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The expression part controller base class. 
 * Expression part controllers enable you to adjust the weight values of the expression parts directly.
 * Further, with controllers you can automate the facial animation processes, e.g. you can use it for animating
 * the eye brows procedurally without playing or overwriting a facial animation. You can also create a complete mood system
 * with it having expression part controllers for each mood.
 */
class ExpressionPartController : public Controller
{
	MEMORYOBJECTCATEGORY(ExpressionPartController, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_CONTROLLERS_FACIAL);

	public:
		/**
		 * Constructor.
		 * The controller is set to enabled on default.
		 * @param actor The actor to which the controller is linked to.
		 */
		ExpressionPartController(Actor* actor);

		/**
		 * Destructor.
		 */
		virtual ~ExpressionPartController();

		/**
		 * Get the unique type ID of this controller.
		 * @result The unique ID.
		 */
		virtual int GetType() const = 0;

		/**
		 * Get the type string, containing the class name of this controller.
		 * @result A pointer to the string containing the class name.
		 */
		virtual const char* GetTypeString() const = 0;

		/**
		 * Set the expression part to control.
		 * @param expressionPart The expression part which this controller controls.
		 */
		void SetExpressionPart(ExpressionPart* expressionPart);

		/**
		 * Return the controlled expression part.
		 * @return The expression part which is controlled by this controller.
		 */
		ExpressionPart* GetExpressionPart() const;

		/**
		 * Clone this controller.
		 * @param actor The actor to apply the cloned version on.
		 * @result A pointer to the cloned controller.
		 */
		Controller* Clone(Actor* actor) = 0;

		/**
		 * Update the controller (perform the calculations and apply them to the actor).
		 * @param timeDeltaInSeconds The time passed in seconds since the last call.
		 */
		virtual void Update(const float timeDeltaInSeconds) = 0;

	protected:
		ExpressionPart*	mExpressionPart;	/**< The expression part to control. */
};

} // namespace EMotionFX


#endif