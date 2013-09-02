#include "stdafx.h"

#include "GameCameraController.h"
#include "GameActor.h"

ImplementRTTI(GmCameraController, GmController);

GmCameraController::GmCameraController()
{
	mMinDist = 0;
	mMaxDist = 200;

	mMinPitchAngle = 0;
	mMaxPitchAngle = 60;

	mHeadHeight = 0;

	mDist = 50;
	mYawDegree = 0;
	mPitchDegree = 45;
}

GmCameraController::~GmCameraController()
{
}

void GmCameraController::SetObject(GmObj * obj)
{
	d_assert (RTTI_KindOf(GmActor, obj));

	GmController::SetObject(obj);
}

void GmCameraController::Update(float frameTime)
{
	GmActor * pActor = RTTI_StaticCast(GmActor, mObject);

	Vec3 targetPos = pActor->GetPosition();
	Quat targetOrt = Quat::Identity;

	float dist = mDist;
	float pitch = Math::DegreeToRadian(mPitchDegree);
	float yaw = Math::DegreeToRadian(mYawDegree);

	Quat q0, q1;

	q0.FromAxis(Vec3::UnitX, pitch);
	q1.FromAxis(Vec3::UnitY, yaw);

	targetOrt = q1 * (q0 * targetOrt);
	targetPos = targetPos - mDist * targetOrt.AxisZ();

	World::Instance()->MainCamera()->SetOrientation(targetOrt);
	World::Instance()->MainCamera()->SetPosition(targetPos);
}

void GmCameraController::Yaw(float rads)
{
	mYawDegree += rads;
	mYawDegree = Math::DegreeNormalize(mYawDegree);
}

void GmCameraController::Pitch(float rads)
{
	mPitchDegree += rads;
	mPitchDegree = Math::DegreeNormalize(mPitchDegree);
}

void GmCameraController::Scroll(float dist)
{
	mDist = Math::Clamp(mDist + dist, mMinDist, mMaxDist);
}










ImplementRTTI(GmCameraControllerTest, GmController);


GmCameraControllerTest::GmCameraControllerTest()
{
}

GmCameraControllerTest::~GmCameraControllerTest()
{
}

void GmCameraControllerTest::SetObject(GmObj * obj)
{
}

void GmCameraControllerTest::Update(float frameTime)
{
	SceneNode * cam = World::Instance()->MainCameraNode();

	if (IKeyboard::Instance()->KeyPressed(KC_W))
	{
		cam->Move(5.0f);
	}
	else if (IKeyboard::Instance()->KeyPressed(KC_S))
	{
		cam->Move(-5.0f);
	}
	else if (IKeyboard::Instance()->KeyPressed(KC_A))
	{
		cam->Right(-5.0f);
	}
	else if (IKeyboard::Instance()->KeyPressed(KC_D))
	{
		cam->Right(5.0f);
	}

	// camera
	if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(MKC_BUTTON0))
	{
		Point2i pt = IMouse::Instance()->GetPositionDiff();

		if (abs(pt.y) >abs(pt.x))
		{
			cam->Pitch(pt.y * 0.005f, TS_LOCAL);
		}
		else
		{
			cam->Yaw(pt.x * 0.005f, TS_PARENT);
		}
	}

	if (IMouse::Instance()->MouseWheel())
	{
		cam->Move(0.5f * IMouse::Instance()->MouseWheel());
	}
}

