#include "stdafx.h"

#include "MainWorkSpace.h"

#define RENDER_WINDOW_TEXTURE "UI_RenderTexture"

namespace Infinite {

	IMP_SLN(RenderWindow);

	RenderWindow::RenderWindow(MyGUI::ImageBox * _window)
	{
		INIT_SLN;

		mRenderWindow = _window;
		mRenderTexture = (MGUI_Texture *)MGUI_System::Instance()->GetRenderManager()->createTexture(RENDER_WINDOW_TEXTURE);

		Init();
	}

	RenderWindow::~RenderWindow()
	{
		SHUT_SLN;

		MGUI_System::Instance()->GetRenderManager()->destroyTexture(mRenderTexture);
	}

	void RenderWindow::Init()
	{
		RenderScheme * scheme = Engine::Instance()->GetRenderScheme();

		int w = mRenderWindow->getSize().width;
		int h = mRenderWindow->getSize().height;

		MyGUI::IntCoord coord = mRenderWindow->getClientCoord();

		scheme->Resize(w, h);

		mRenderTexture->createFromTexture(scheme->GetColorTexture());
	}

	void RenderWindow::Shutdown()
	{
		mRenderTexture->destroy();
	}





	WorkspaceControl::WorkspaceControl(MyGUI::Widget * _parent)
		: BaseLayout("Workspace.layout", _parent)
	{
		assignWidget(mImageBox, "RenderWindow");
		mRenderWindow = new RenderWindow(mImageBox);

		mImageBox->setImageTexture(RENDER_WINDOW_TEXTURE);
	}

	WorkspaceControl::~WorkspaceControl()
	{
		delete mRenderWindow;
	}

	void WorkspaceControl::Resize()
	{
	}








	ToolControl::ToolControl(MyGUI::Widget * _parent)
		: BaseLayout("ToolControl.layout", _parent)
	{
	}

	ToolControl::~ToolControl()
	{
	}









	MainWorkSpace::MainWorkSpace(MyGUI::Widget * _parent)
		: BaseLayout("MainWorkSpace.layout", _parent)
	{
		assignBase(mToolControl, "ToolsControl");
		assignBase(mWorkspaceControl, "WorkspaceControl");
	}

	MainWorkSpace::~MainWorkSpace()
	{
	}
}