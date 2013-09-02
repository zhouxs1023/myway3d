#include "stdafx.h"

#include "MainMode.h"
#include "Client.h"

#include "Pack_PlayerEnterCL.h"

ImplementRTTI(cMainMode, GmMode);

cMainMode::cMainMode(const PlayerViewInfo & _initPlayerInfo)
{
	mInitPlayerInfo = _initPlayerInfo;
}

cMainMode::~cMainMode()
{
}

void cMainMode::Init()
{
	mMap.Load("test003.gmap");

	Environment::Instance()->SetHDREnable(true);
	Environment::Instance()->SetGodRayEnable(true);
	//Environment::Instance()->SetShadowEnable(true);
	Environment::Instance()->SetSSAOEnable(true);
	RenderSystem::Instance()->SetSMAAType(eSmaaType::SMAA_HIGH);

	Pack_PlayerEnterCL pack;

	pack.SetPlayerId(mInitPlayerInfo.ActorId);

	Client::Instance()->SendPacket(&pack);

	_InitPlayer();

	mGMCommand = new cGMCommand;
}

void cMainMode::_InitPlayer()
{
	mPlayer = new cPlayer(mInitPlayerInfo);

	AddObject(mPlayer);

	mCameraController = new GmCameraController;
	mCameraController->SetObject(mPlayer);
	AddController(mCameraController);
}

void cMainMode::Shutdown()
{
	safe_delete(mGMCommand);
}

void cMainMode::Update(float frameTime)
{
	if (IKeyboard::Instance()->KeyUp(KC_GRAVE))
		mGMCommand->Show(!mGMCommand->IsShow());

	GmMode::Update(frameTime);
}

void cMainMode::ProcessMouseEvent()
{
	// camera
	if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(MKC_BUTTON1))
	{
		Point2i pt = IMouse::Instance()->GetPositionDiff();

		if (abs(pt.y) >abs(pt.x))
		{
			mCameraController->Pitch(pt.y * 0.1f);
		}
		else
		{
			mCameraController->Yaw(pt.x * 0.1f);
		}
	}

	if (IMouse::Instance()->KeyPressed(MKC_BUTTON0))
	{
		Point2f pt = IMouse::Instance()->GetPositionUnit();
		Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);

		GmEngine::RayCheckInfo info = GmEngine::RayCheck(ray, 500);

		if (info.bPicked)
		{
			mPlayer->MoveTo(info.vPosition);
		}
	}

	if (IMouse::Instance()->MouseWheel())
	{
		mCameraController->Scroll(-0.1f * IMouse::Instance()->MouseWheel());
	}
}

void cMainMode::ProcessKeyEvent()
{
}