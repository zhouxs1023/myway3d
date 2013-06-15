#pragma once

#include "GameCoreEntry.h"
#include "GameObject.h"


class GCORE_ENTRY GameCameraController : public IGameController
{
	DeclareRTTI();

public:
	GameCameraController();
	virtual ~GameCameraController();

	virtual void SetObject(IGameObject * obj);
	virtual void Update(float frameTime);

protected:
	float mMinDist, mMaxDist;
	float mMoveSpeed, mScrollSpeed;
	float mRotationSpeed;

	Vec3 mTargetPos;
	Quat mTargetOrt;

	Vec3 mCurrentPos;
	Quat mCurrentOrt;
};