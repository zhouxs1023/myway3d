/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "JacobianIKController.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
JacobianIKController::JacobianIKController(Actor* actor) : Controller(actor)
{
	// create the jacobian ik data which this controller uses
	mData = new JacobianIKData();

	// set the actor of the ik data
	mData->SetActor(actor);

	// create the jacobian ik solver which this controller uses
	mSolver = new JacobianIKSolver(mData);

	// clear results
	mSolution.Clear();
}


// destructor
JacobianIKController::~JacobianIKController()
{
	// get rid of the ik data
	if (mData)
		delete mData;

	// get rid of the jacobian ik solver
	if (mSolver)
		delete mSolver;
}


// return the type identification number
int JacobianIKController::GetType() const
{
	return TYPE_ID;
}


// return the type identification string
const char* JacobianIKController::GetTypeString() const
{
	return "JacobianIKController";
}


// clone this jacobian ik controller
Controller* JacobianIKController::Clone(Actor* actor)
{
	// create the jacobian ik controller
	JacobianIKController* jacobianIKController = new JacobianIKController(actor);

	// get number of constraints used by the ik data
	const int numConstraints = mData->GetNumConstraints();

	// iterate through all constraints and clone the constraints
	for (int i=0; i<numConstraints; i++)
		jacobianIKController->GetIKData()->AddConstraint(mData->GetConstraint(i)->Clone());

	// return the clone
	return jacobianIKController;
}


// update the jacobian ik controller
void JacobianIKController::Update(const float timeDeltaInSeconds)
{
	// initialize the actor instance from the actor pose
	mData->InitInstance();

	// call constraints update
	mData->UpdateConstraints(timeDeltaInSeconds);

	// solve the IK for the actor instance
	mSolver->CalcIK(mSolution);

	// apply the solution from the actor instance to the actor
	mData->Apply(mSolution);
}

}