//////////////////////////////////////////////////////////////////////////
//
// MyGui Integration.
//   MyGui engine: http://mygui.info
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "MGUI_Entry.h"
#include "MGUI_Texture.h"
#include "MGUI_RenderSystem.h"
#include "MGUI_DataManager.h"
#include "MGUI_Input.h"

namespace Myway {

	class MGUI_ENTRY MGUI_System
	{
		DECLARE_ALLOC();
		DECLARE_SINGLETON(MGUI_System);

	public:
		Event OnMouseMove;
		Event OnMousePress;
		Event OnMouseRelease;
		Event OnKeyPress;
		Event OnKeyRelease;

	public:
		MGUI_System();
		~MGUI_System();

		void Init();
		void Shutdown();

		void Render();

		MGUI_RenderSystem * GetRenderManager() { return mRenderManager; }

		void InjectMouseEvent();
		void InjectKeyEvent(DWORD uMsg, WPARAM wParam,LPARAM lParam);

	protected:
		void injectMouseMove(int _absx, int _absy, int _absz);
		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		void injectKeyRelease(MyGUI::KeyCode _key);

	protected:
		void _OnResize(Event * sender);
		void _OnRender(Event * sender);

	protected:
		tEventListener<MGUI_System> OnResize;
		tEventListener<MGUI_System> OnRender;

		MyGUI::LogManager * mLog;
		MGUI_RenderSystem * mRenderManager;
		MGUI_DataManager * mDataManager;

		MyGUI::Gui * mGui;
	};

}