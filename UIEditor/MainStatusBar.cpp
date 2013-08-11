#include "stdafx.h"

#include "MainStatusBar.h"
#include "AllLookFeel.h"

namespace UIEditor {

	MainStatusBar::MainStatusBar()
	{
		mLayout = new MGUI_Layout(AllLookFeel::Instance()->GetPanel(), NULL);

		MGUI_Engine::Instance()->AddLayout(mLayout);
	}

	MainStatusBar::~MainStatusBar()
	{
		MGUI_Engine::Instance()->RemoveLayout(mLayout);
		mLayout = NULL;
	}

	void MainStatusBar::Layout()
	{
		MGUI_Rect rect;

		rect.x0 = 256;
		rect.y0 = MGUI_Engine::Instance()->GetRect().y1 - 32;
		rect.x1 = MGUI_Engine::Instance()->GetRect().x1;
		rect.y1 = MGUI_Engine::Instance()->GetRect().y1;

		mLayout->SetRect(rect);
	}

}