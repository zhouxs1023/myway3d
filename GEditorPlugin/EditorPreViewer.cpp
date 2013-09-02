#include "stdafx.h"

#include "EditorPreViewer.h"
#include "EditorGameMode.h"
#include "MainWorkSpace.h"

xPreViewerPlayer::xPreViewerPlayer()
	: GmNpc(0)
{
}

xPreViewerPlayer::~xPreViewerPlayer()
{
}




xPreViewerCameraController::xPreViewerCameraController()
{
}

xPreViewerCameraController::~xPreViewerCameraController()
{
}

void xPreViewerCameraController::SetObject(GmObj * obj)
{
	GmCameraController::SetObject(obj);

	mOrigPos = World::Instance()->MainCamera()->GetPosition();
	mOrigOrt = World::Instance()->MainCamera()->GetOrientation();
}





xPreViewerListener gPreViewerListener;

xPreViewerListener::xPreViewerListener()
	: OnGameModeChanged(xEvent::OnGameMode, this, &xPreViewerListener::_OnGameModeChanged)
	, OnUpdate(xEvent::OnUpdate, this, &xPreViewerListener::_OnUpdate)
{
	mPlayer = NULL;
	mCameraContoller = NULL;
}

xPreViewerListener::~xPreViewerListener()
{
}

void xPreViewerListener::_OnUpdate(Event * sender)
{
	if (!Editor::Instance()->GetGameMode())
		return ;

	Ray ray;

	if (IMouse::Instance()->KeyUp(MKC_BUTTON0) && RenderWindow::Instance()->GetViewRay(ray))
	{
		GmEngine::RayCheckInfo info = GmEngine::RayCheck(ray, 500);

		if (info.bPicked)
		{
			mPlayer->SetPosition(info.vPosition);
		}
	}
}

void xPreViewerListener::_OnGameModeChanged(Event * sender)
{
	if (Editor::Instance()->GetGameMode())
	{
		mPlayer = new xPreViewerPlayer;
		mPlayer->SetId(EditorGameMode::Instance()->GetUId());
		mPlayer->Init();
		EditorGameMode::Instance()->AddObject(mPlayer);

		Terrain * terrain = Environment::Instance()->GetTerrain();

		if (terrain)
		{
			Vec3 pos = World::Instance()->MainCamera()->GetPosition();
			pos.y = terrain->GetHeight(pos.x, pos.z);

			mPlayer->SetPosition(pos);
		}

		mCameraContoller = new xPreViewerCameraController;
		mCameraContoller->SetObject(mPlayer);
		EditorGameMode::Instance()->AddController(mCameraContoller);
	}
	else
	{
		EditorGameMode::Instance()->RemoveController(mCameraContoller);
		EditorGameMode::Instance()->RemoveObject(mPlayer->GetId());

		mCameraContoller = NULL;
		mPlayer = NULL;
	}
}

