/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "ExpressionPartController.h"


namespace EMotionFX
{

// default constructor
ExpressionPartController::ExpressionPartController(Actor* actor) : Controller(actor)
{
	mExpressionPart = NULL;
}


// destructor
ExpressionPartController::~ExpressionPartController()
{
	// disable manual weight control again
	mExpressionPart->SetEnableManualMode( false );
}


// set the expression part to control
void ExpressionPartController::SetExpressionPart(ExpressionPart* expressionPart)
{
	// disable manual weight control for the last controlled expression part if the controller is already assigned to one
	if (mExpressionPart)
		expressionPart->SetEnableManualMode( false );

	// assign the new expression part to the controller
	mExpressionPart = expressionPart;

	// enable manual weight control for the given expression part if it is valid
	if (mExpressionPart)
		mExpressionPart->SetEnableManualMode( true );
}


// get the controlled expression part
ExpressionPart* ExpressionPartController::GetExpressionPart() const
{
	return mExpressionPart;
}

} // namespace EMotionFX