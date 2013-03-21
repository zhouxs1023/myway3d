//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "MWApp_Win32.h"
#include "Infinite.h"
#include "BackgroundPane.h"
#include "MainPane.h"
#include "Common\\PointerManager.h"
#include "Editor.h"

namespace Infinite {

	class INFI_ENTRY App : public App_Win32
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
		input::PointerManager mPointManager;
		BackgroundPane * mBackgroundPane;
		MainPane * mMainPane;
		RenderScheme * mRenderer;

		Editor mEditor;
	};

}