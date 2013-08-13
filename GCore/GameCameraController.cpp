#include "stdafx.h"

#include "GameCameraController.h"
#include "GameActor.h"

ImplementRTTI(GmCameraController, GmController);

GmCameraController::GmCameraController()
{
	mMinDist = 50;
	mMaxDist = 100;

	mMoveSpeed = 2;
	mScrollSpeed = 5;
	mRotationSpeed = 1;
}

GmCameraController::~GmCameraController()
{
}

void GmCameraController::SetObject(GmObj * obj)
{
	d_assert (RTTI_KindOf(GmActor, obj));

	GmController::SetObject(obj);

	Actor * pActor = RTTI_StaticCast(Actor, obj);

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

void GmCameraController::Update(float frameTime)
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
	if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(MKC_BUTTON1))
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

