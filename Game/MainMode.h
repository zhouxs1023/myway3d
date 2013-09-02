#pragma once

#include "Player.h"
#include "GMCommand.h"

class cMainMode : public GmMode
{
	DeclareRTTI();

public:
	cMainMode(const PlayerViewInfo & _initPlayerInfo);
	virtual ~cMainMode();

	virtual void Init();
	virtual void Shutdown();

	virtual void Update(float frameTime);

	virtual void ProcessMouseEvent();
	virtual void ProcessKeyEvent();

protected:
	void _InitPlayer();

protected:
	PlayerViewInfo mInitPlayerInfo;

	GmMap mMap;
	GmCameraController * mCameraController;
	cPlayer * mPlayer;

	cGMCommand * mGMCommand;
};
