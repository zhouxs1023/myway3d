#include "stdafx.h"
#include "MainLookFeelPanel.h"
#include "AllLookFeel.h"

namespace UIEditor {

	MainLookFeelPanel::MainLookFeelPanel()
	{
		mLayout = new MGUI_Layout(AllLookFeel::Instance()->GetPanel(), NULL);
		MGUI_Engine::Instance()->AddLayout(mLayout);

		/*MGUI_ListBox * listBox = new MGUI_ListBox(AllLookFeel::Instance()->GetListBox(), mLayout);

		listBox->SetRect(MGUI_Rect(0, 0, 128, 256));

		for (int i = 0; i < 10; ++i)
		{
			listBox->Append("ggg001");
			listBox->Append("aaa002");
		}*/

		/*MGUI_EditBox * editBox = new MGUI_EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
		editBox->SetRect(MGUI_Rect(0, 0, 128, 32));
		editBox->SetStatic(true);*/

		MGUI_ComboBox * combox = new MGUI_ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
		combox->SetRect(0, 0, 200, 24);

		for (int i = 0; i < 2; ++i)
		{
			combox->Append(L"ggg001");
			combox->Append(L"aaa002");
		}
	}

	MainLookFeelPanel::~MainLookFeelPanel()
	{
		MGUI_Engine::Instance()->RemoveLayout(mLayout);
		mLayout = NULL;
	}

	void MainLookFeelPanel::Layout()
	{
		MGUI_Rect rect;

		rect.x0 = 0;
		rect.y0 = 32;
		rect.x1 = 256;
		rect.y1 = MGUI_Engine::Instance()->GetRect().y1;

		mLayout->SetRect(rect);
	}

}

