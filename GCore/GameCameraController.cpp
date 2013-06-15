#include "stdafx.h"

#include "GameCameraController.h"
#include "GameActor.h"

ImplementRTTI(GameCameraController, IGameController);

GameCameraController::GameCameraController()
{
	mMinDist = 50;
	mMaxDist = 100;

	mMoveSpeed = 2;
	mScrollSpeed = 5;
	mRotationSpeed = 1;
}

GameCameraController::~GameCameraController()
{
}

void GameCameraController::SetObject(IGameObject * obj)
{
	d_assert (RTTI_KindOf(GameActor, obj));

	IGameController::SetObject(obj);

	GameActor * pActor = RTTI_StaticCast(GameActor, obj);

	mCurrentPos = World::Instance()->MainCamera()->GetPosition();
	mCurrentOrt = World::Instance()->MainCamera()->GetOrientation();

	mTargetPos = pActor->GetPosition();
	mTargetOrt = pActor->GetOrientation();

	Quat quat;
	quat.FromAxis(Vec3::UnitX, Math::PI_1_4);

	mTargetOrt = quat * mTargetOrt;
	mTargetPos = mTargetPos - 0.5f * (mMinDist + mMaxDist) * quat.AxisZ();

	mCurrentOrt = mTargetOrt;

	World::Instance()->MainCamera()->SetOrientation(mCurrentOrt);
}

void GameCameraController::Update(float frameTime)
{
	float dist = mTargetPos.Distance(mCurrentPos);

	if (dist < frameTime * mMoveSpeed)
	{
		mCurrentPos = mTargetPos;
	}
	else
	{
		float speed = Math::Maximum(mMoveSpeed, (dist / mMaxDist) * mMoveSpeed);

		float k = frameTime * speed * dist;

		mCurrentPos = mCurrentPos + k * (mTargetPos - mCurrentPos).Normalize();
	}

	World::Instance()->MainCamera()->SetPosition(mCurrentPos);
}