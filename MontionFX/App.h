//////////////////////////////////////////////////////////////////////////
//
// MotionFx: Motion Tool For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "MotionFX.h"
#include "MWApp_Win32.h"
#include "Common\\PointerManager.h"
#include "BackgroundPane.h"
#include "MainPane.h"
#include "Editor.h"

namespace MotionFX {

	class MOTIONFX_ENTRY App : public App_Win32
	{
	public:
		App();
		virtual ~App();

		virtual bool Init();
		virtual void Shutdown();
		virtual void Update();
		virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

	protected:
		MGUI_System mUISystem;
		RenderScheme * mRenderer;

		input::PointerManager mPointManager;
		BackgroundPane * mBackgroundPane;
		MainPane * mMainPane;

		Editor mEditor;
	};

}