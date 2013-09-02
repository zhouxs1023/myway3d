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

	void Yaw(float degree);
	void Pitch(float degree);
	void Scroll(float dist);

protected:
	float mMinDist;
	float mMaxDist;

	float mMinPitchAngle;
	float mMaxPitchAngle;

	float mHeadHeight;

	float mDist;
	float mYawDegree;
	float mPitchDegree;
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

