#include "stdafx.h"

#include "GameAI.h"
#include "GameHelper.h"

ImplementRootRTTI(IGameAIState);

ImplementRTTI(GameComponent_AI, IGameComponent);


GameComponent_AI::GameComponent_AI()
{
	mCurrentState = NULL;
}

GameComponent_AI::~GameComponent_AI()
{
	safe_delete (mCurrentState);
}

void GameComponent_AI::Update(float frameTime)
{
	if (mCurrentState)
		mCurrentState->Think(frameTime);
}

void GameComponent_AI::ChangeState(IGameAIState * state)
{
	if (mCurrentState)
	{
		d_assert (mCurrentState->GetRTTI() != state->GetRTTI());
	}

	if (mCurrentState)
		mCurrentState->Exit();

	safe_delete(mCurrentState);

	mCurrentState = state;

	mCurrentState->SetComponent(this);
	mCurrentState->Enter();
}

IGameAIState * GameComponent_AI::GetCurrentState()
{
	return mCurrentState;
}

void GameComponent_AI::SetObject(IGameObject * obj)
{
	d_assert (RTTI_KindOf(GameActor, obj));

	IGameComponent::SetObject(obj);
}

GameActor * GameComponent_AI::GetActor()
{
	return RTTI_StaticCast(GameActor, mObject);
}





// Idle State
//
ImplementRTTI(GameAIState_Idle, IGameAIState);

GameAIState_Idle::GameAIState_Idle()
{
}

GameAIState_Idle::~GameAIState_Idle()
{
}

void GameAIState_Idle::Enter()
{
	GameActor * pActor = mComponent->GetActor();

	pActor->PlayAnimation(GameHelper::S_Anim_Idle0.c_str(), MotionBlendInfo::Default);
}

void GameAIState_Idle::Exit()
{
}

void GameAIState_Idle::Think(float frameTime)
{
}




// Move State
//
ImplementRTTI(GameAIState_Move, IGameAIState);

GameAIState_Move::GameAIState_Move(const Vec3 & pos)
	: mTargetPos(pos)
{
}

GameAIState_Move::~GameAIState_Move()
{
}

void GameAIState_Move::Enter()
{
	GameActor * pActor = mComponent->GetActor();

	pActor->PlayAnimation(GameHelper::S_Anim_Run.c_str(), MotionBlendInfo::Default);
}

void GameAIState_Move::Exit()
{
}

void GameAIState_Move::Think(float frameTime)
{
}