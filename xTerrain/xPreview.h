#pragma once

#include "MActor.h"

class xPreViewer
{
	enum State {
		K_Idle,
		K_Run,
		K_Walk,
		K_Jump,
		K_XiaLuo,
	};

public:
	xPreViewer();
	~xPreViewer();

	void Init();
	void Shutdown();

	void Update();

protected:
	void OnXailuo();
	void OnIdle();
	void OnWalk();
	void OnRun();
	void OnJump();
	void OnWaveArm();

protected:
	MActor * mActor;
	SceneNode * mNode;
	State mState;
};

class xPreViewerListener
{
public:
	xPreViewerListener();
	~xPreViewerListener();

	void _OnAppModeChanged(void *, void *);
	void _OnUpdate(void *, void *);

protected:
	tEventListener<xPreViewerListener> OnAppModeChanged;
	tEventListener<xPreViewerListener> OnUpdate;

	xPreViewer mPreviewer;
};