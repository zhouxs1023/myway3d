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
#include "MGUI_RenderSystem.h"
#include "MGUI_DataManager.h"

namespace Myway {

	class MGUI_ENTRY MGUI_System
	{
		DECLARE_ALLOC();
		DECLARE_SINGLETON(MGUI_System);

	public:
		MGUI_System();
		~MGUI_System();

		void Init();
		void Shutdown();

		void Render();

		MGUI_RenderSystem * GetRenderManager() { return mRenderManager; }

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