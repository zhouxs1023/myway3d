#include "MGUI_System.h"
#include "MyGUI_Gui.h"

namespace Myway {

	IMP_SLN(MGUI_System);

	MGUI_System::MGUI_System()
		: OnResize(&RenderEvent::OnResize, this, &MGUI_System::_OnResize)
		, OnRender(&RenderEvent::OnRenderGUI1, this, &MGUI_System::_OnRender)
	{
		INIT_SLN;

		mLog = NULL;
		mDataManager = NULL;
		mRenderManager = NULL;
		
		mGui = NULL;
	}

	MGUI_System::~MGUI_System()
	{
		SHUT_SLN;

		d_assert (mLog == NULL);
	}

	void MGUI_System::Init()
	{
		mLog = new MyGUI::LogManager;
		mDataManager = new MGUI_DataManager;
		mRenderManager = new MGUI_RenderSystem;

		mGui = new MyGUI::Gui;
		mGui->initialise();
	}

	void MGUI_System::Shutdown()
	{
		d_assert (mLog);

		mGui->shutdown();
		delete mGui;

		delete mRenderManager;
		delete mDataManager;
		delete mLog;
	}

	void MGUI_System::Render()
	{
		mRenderManager->drawOneFrame();
	}

	void MGUI_System::_OnResize(Event * sender)
	{
		int w = Engine::Instance()->GetDeviceProperty()->Width;
		int h = Engine::Instance()->GetDeviceProperty()->Height;

		mRenderManager->setViewSize(w, h);
	}

	void MGUI_System::_OnRender(Event * sender)
	{
		mRenderManager->drawOneFrame();
	}

}