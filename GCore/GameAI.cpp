#include "stdafx.h"

#include "GameAI.h"
#include "GameHelper.h"

ImplementRootRTTI(GmAIState);

ImplementRTTI(GmCompAI, GmComponent);


GmCompAI::GmCompAI()
{
	mCurrentState = NULL;
}

GmCompAI::~GmCompAI()
{
	safe_delete (mCurrentState);
}

void GmCompAI::Update(float frameTime)
{
	if (mCurrentState)
		mCurrentState->Think(frameTime);
}

void GmCompAI::ChangeState(GmAIState * state)
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

GmAIState * GmCompAI::GetCurrentState()
{
	return mCurrentState;
}

void GmCompAI::SetObject(GmObj * obj)
{
	d_assert (RTTI_KindOf(GmActor, obj));

	GmComponent::SetObject(obj);
}

GmActor * GmCompAI::GetActor()
{
	return RTTI_StaticCast(GmActor, mObject);
}





// Idle State
//
ImplementRTTI(AIState_Idle, GmAIState);

AIState_Idle::AIState_Idle()
{
}

AIState_Idle::~AIState_Idle()
{
}

void AIState_Idle::Enter()
{
	GmActor * pActor = mComponent->GetActor();

	pActor->PlayAnimation(GmHelper::S_Anim_Idle0.c_str(), MotionBlendInfo::Default);
}

void AIState_Idle::Exit()
{
}

void AIState_Idle::Think(float frameTime)
{
}




// Move State
//
ImplementRTTI(AIState_Move, GmAIState);

AIState_Move::AIState_Move(const Vec3 & pos)
	: mTargetPos(pos)
{
}

AIState_Move::~AIState_Move()
{
}

void AIState_Move::Enter()
{
	GmActor * pActor = mComponent->GetActor();

	pActor->PlayAnimation(GmHelper::S_Anim_Run.c_str(), MotionBlendInfo::Default);
}

void AIState_Move::Exit()
{
}

void AIState_Move::Think(float frameTime)
{
}