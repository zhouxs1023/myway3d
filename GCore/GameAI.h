#pragma once

#include "GameObject.h"
#include "GameActor.h"

class GameComponent_AI;

class GCORE_ENTRY IGameAIState
{
	DeclareRootRTTI(IGameAIState);

public:
	IGameAIState() : mComponent(NULL) {};
	virtual ~IGameAIState() {}

	virtual void SetComponent(GameComponent_AI * cp) { mComponent = cp; }
	GameComponent_AI * GetComponent() { return mComponent; }

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Think(float frameTime) = 0;

protected:
	GameComponent_AI * mComponent;
};


class GCORE_ENTRY GameComponent_AI : public IGameComponent
{
	DeclareRTTI();

public:
	GameComponent_AI();
	virtual ~GameComponent_AI();

	virtual void Update(float frameTime);

	virtual void ChangeState(IGameAIState * state);
	virtual IGameAIState * GetCurrentState();

	virtual void SetObject(IGameObject * obj);
	GameActor * GetActor();

protected:
	IGameAIState * mCurrentState; 
};



// Idle State
//
class GCORE_ENTRY GameAIState_Idle : public IGameAIState
{
	DeclareRTTI();

public:
	GameAIState_Idle();
	virtual ~GameAIState_Idle();

	virtual void Enter();
	virtual void Exit();

	virtual void Think(float frameTime);
};

// Move State
//
class GCORE_ENTRY GameAIState_Move : public IGameAIState
{
	DeclareRTTI();

public:
	GameAIState_Move(const Vec3 & pos);
	virtual ~GameAIState_Move();

	virtual void Enter();
	virtual void Exit();

	virtual void Think(float frameTime);

protected:
	Vec3 mTargetPos;
};