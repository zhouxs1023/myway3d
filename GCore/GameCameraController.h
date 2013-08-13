#pragma once

#include "GameCoreEntry.h"
#include "GameObject.h"

class GCORE_ENTRY GmCameraController : public GmController
{
	DeclareRTTI();

public:
	GmCameraController();
	virtual ~GmCameraController();

	virtual void SetObject(GmObj * obj);
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


class GCORE_ENTRY GmCameraControllerTest : public GmController
{
	DeclareRTTI();

public:
	GmCameraControllerTest();
	virtual ~GmCameraControllerTest();

	virtual void SetObject(GmObj * obj);

	virtual void Update(float frameTime);
};

