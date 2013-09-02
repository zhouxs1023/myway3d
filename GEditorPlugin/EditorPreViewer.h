#pragma once

#include "Editor.h"
#include "GameActor.h"
#include "GameNpc.h"
#include "GameCameraController.h"

class xPreViewerPlayer : public GmNpc
{
public:
	xPreViewerPlayer();
	virtual ~xPreViewerPlayer();

protected:
};


class xPreViewerCameraController : public GmCameraController
{
public:
	xPreViewerCameraController();
	~xPreViewerCameraController();

	virtual void SetObject(GmObj * obj);

protected:
	Vec3 mOrigPos;
	Quat mOrigOrt;
};






	class xPreViewerListener
	{
	public:
		xPreViewerListener();
		~xPreViewerListener();

		void _OnGameModeChanged(Event * sender);
		void _OnUpdate(Event * sender);

	protected:
		tEventListener<xPreViewerListener> OnGameModeChanged;
		tEventListener<xPreViewerListener> OnUpdate;

		xPreViewerPlayer * mPlayer;
		xPreViewerCameraController * mCameraContoller;
	};

