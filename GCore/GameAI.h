#pragma once

#include "GameObject.h"
#include "GameActor.h"

class GmCompAI;

class GCORE_ENTRY GmAIState
{
	DeclareRootRTTI(GmAIState);

public:
	GmAIState() : mComponent(NULL) {};
	virtual ~GmAIState() {}

	virtual void SetComponent(GmCompAI * cp) { mComponent = cp; }
	GmCompAI * GetComponent() { return mComponent; }

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Think(float frameTime) = 0;

protected:
	GmCompAI * mComponent;
};


class GCORE_ENTRY GmCompAI : public GmComponent
{
	DeclareRTTI();

public:
	GmCompAI();
	virtual ~GmCompAI();

	virtual void Update(float frameTime);

	virtual void ChangeState(GmAIState * state);
	virtual GmAIState * GetCurrentState();

	virtual void SetObject(GmObj * obj);
	GmActor * GetActor();

protected:
	GmAIState * mCurrentState; 
};



// Idle State
//
class GCORE_ENTRY AIState_Idle : public GmAIState
{
	DeclareRTTI();

public:
	AIState_Idle();
	virtual ~AIState_Idle();

	virtual void Enter();
	virtual void Exit();

	virtual void Think(float frameTime);
};

// Move State
//
class GCORE_ENTRY AIState_Move : public GmAIState
{
	DeclareRTTI();

public:
	AIState_Move(const Vec3 & pos);
	virtual ~AIState_Move();

	virtual void Enter();
	virtual void Exit();

	virtual void Think(float frameTime);

protected:
	Vec3 mTargetPos;
};