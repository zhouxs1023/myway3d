#include "stdafx.h"
#include "LookFeelPanel.h"
#include "LookFeelEditor.h"
#include "AllLookFeel.h"

namespace UIEditor {

	LookFeelPanel::LookFeelPanel()
	{
		mLayout = new MGUI_Layout(AllLookFeel::Instance()->GetPanel());

		mComboBox_WidgetType = new MGUI_ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
		mComboBox_WidgetType->SetRect(0, 0, 200, 24);
		mComboBox_WidgetType->SetAlign(MGUI_Align::HStretch);

		for (int i = 0; i < MGUI_WidgetType::Max; ++i)
		{
			mComboBox_WidgetType->Append(MGUI_WidgetType::TypeString[i].c_wstr());
		}

		mListBox_LookFeel = new MGUI_ListBox(AllLookFeel::Instance()->GetListBox(), mLayout);
		mListBox_LookFeel->SetAlign(MGUI_Align::HStretch);
		mListBox_LookFeel->SetRect(0, 24, 0, 24 + 256);
		
		MGUI_Button * bnCreate = new MGUI_Button(AllLookFeel::Instance()->GetButton(), mLayout);
		bnCreate->SetAlign(MGUI_Align::HStretch);
		bnCreate->SetRect(MGUI_Rect(true, 0, 24 + 256 + 5, 0, 30));
		bnCreate->SetCaption(L"Create");

		MGUI_Button * bnDelete = new MGUI_Button(AllLookFeel::Instance()->GetButton(), mLayout);
		bnDelete->SetAlign(MGUI_Align::HStretch);
		bnDelete->SetRect(MGUI_Rect(true, 0, 24 + 256 + 5 + 30 + 5, 0, 30));
		bnDelete->SetCaption(L"Delete");

		MGUI_Button * bnRename = new MGUI_Button(AllLookFeel::Instance()->GetButton(), mLayout);
		bnRename->SetAlign(MGUI_Align::HStretch);
		bnRename->SetRect(MGUI_Rect(true, 0, 24 + 256 + 5 + 30 + 5 + 30 + 5, 0, 30));
		bnRename->SetCaption(L"Rename");

		mComboBox_WidgetType->eventSelectChanged += OnWidgetTypeChanged(this, &LookFeelPanel::OnWidgetTypeChanged_);
		mListBox_LookFeel->eventSelectChanged += OnLookFeelChanged(this, &LookFeelPanel::OnLookFeelChanged_);

		bnCreate->eventMouseClick += OnCreate(this, &LookFeelPanel::OnCreate_);
		bnDelete->eventMouseClick += OnDelete(this, &LookFeelPanel::OnDelete_);
	}

	LookFeelPanel::~LookFeelPanel()
	{
		delete mLayout;
	}

	void LookFeelPanel::Layout()
	{
		MGUI_Rect rect;

		rect.x0 = 0;
		rect.y0 = 32;
		rect.x1 = 256;
		rect.y1 = MGUI_Engine::Instance()->GetRect().y1 - 32;

		mLayout->SetRect(rect);
	}

	void LookFeelPanel::OnWidgetTypeChanged_(int _index)
	{
		mListBox_LookFeel->Clear();

		if (_index == -1)
			return ;

		for (int i = 0; i < MGUI_LookFeelManager::Instance()->GetLookFeelCount(_index); ++i)
		{
			MGUI_LookFeel * lookfeel = MGUI_LookFeelManager::Instance()->GetLookFeel(_index, i);

			mListBox_LookFeel->Append(lookfeel->GetName().c_wstr(), lookfeel);
		}
	}

	void LookFeelPanel::OnLookFeelChanged_(int _index)
	{
		if (_index == -1)
			return ;

		int type = mComboBox_WidgetType->GetSelectIndex();

		if (type == -1)
			return ;

		MGUI_LookFeel * lookfeel = MGUI_LookFeelManager::Instance()->GetLookFeel(type, _index);

		LookFeelEditor::Instance()->SetEditLookFeel(lookfeel);
	}

	void LookFeelPanel::OnCreate_()
	{
		int type = mComboBox_WidgetType->GetSelectIndex();

		if (type == -1)
			return ;

		int index = 0;
		TString128 name_ = MGUI_WidgetType::GetTypeString(type);

		while (MGUI_LookFeelManager::Instance()->GetLookFeel(type, name_ + index) != NULL)
			++index;

		MGUI_LookFeel * lookfeel = new MGUI_LookFeel(name_ + index);
		
		MGUI_LookFeelManager::Instance()->AddLookFeel(type, lookfeel);

		mListBox_LookFeel->Append(lookfeel->GetName().c_wstr(), lookfeel);
	}

	void LookFeelPanel::OnDelete_()
	{
		int type = mComboBox_WidgetType->GetSelectIndex();

		if (type == -1)
			return ;

		int index = mListBox_LookFeel->GetSelectIndex();

		if (index == -1)
			return ;

		MGUI_LookFeel * lookfeel = (MGUI_LookFeel *)mListBox_LookFeel->GetUserData(index);

		MGUI_LookFeelManager::Instance()->RemoveLookFeel(type, lookfeel);

		mListBox_LookFeel->Remove(index);
	}
}

