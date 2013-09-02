#include "stdafx.h"

#include "LookFeelProperty.h"
#include "AllLookFeel.h"
#include "LookFeelEditor.h"

namespace UIEditor {

	LookFeelProperty::LookFeelProperty()
	{
		mLayout = new MGUI_Layout(AllLookFeel::Instance()->GetPanel());

		int top = 0;
		const int K_Space = 8;

		// Name
		//
		MGUI_Label * lb_Name = new MGUI_Label(NULL, mLayout);
		lb_Name->SetCaption(L"Name:");
		lb_Name->SetRect(MGUI_Rect(true, 0, top , 64, 24));

		mEditBox_Name = new MGUI_EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
		mEditBox_Name->SetCaption(L"");
		mEditBox_Name->SetRect(MGUI_Rect(true, 80, top, 165, 24));

		top += 24 + K_Space;

		// Skin
		//
		MGUI_Label * lb_Skin = new MGUI_Label(NULL, mLayout);
		lb_Skin->SetCaption(L"Skin:");
		lb_Skin->SetRect(MGUI_Rect(true, 0, top, 64, 24));

		mComboBox_Skin = new MGUI_ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
		mComboBox_Skin->SetRect(MGUI_Rect(true, 80, top, 165, 24));

		top += 24 + K_Space;

		// Client
		//
		MGUI_Label * lb_Client = new MGUI_Label(NULL, mLayout);
		lb_Client->SetCaption(L"Client:");
		lb_Client->SetRect(MGUI_Rect(true, 0, top, 64, 24));

		mEditBox_Client = new MGUI_EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
		mEditBox_Client->SetCaption(L"0,0,1,1");
		mEditBox_Client->SetRect(MGUI_Rect(true, 80, top, 165, 24));

		top += 24 + K_Space;

		// State
		//
		MGUI_Label * lb_State = new MGUI_Label(NULL, mLayout);
		lb_State->SetCaption(L"State:");
		lb_State->SetRect(MGUI_Rect(true, 0, top, 64, 24));

		mComboBox_State = new MGUI_ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
		mComboBox_State->SetRect(MGUI_Rect(true, 80, top, 165, 24));

		top += 24 + K_Space;

		// UV Rect
		//
		MGUI_Label * lb_UVRect = new MGUI_Label(NULL, mLayout);
		lb_UVRect->SetCaption(L"UVRect:");
		lb_UVRect->SetRect(MGUI_Rect(true, 0, top, 64, 24));

		mEditBox_UVRect = new MGUI_EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
		mEditBox_UVRect->SetCaption(L"0,0,1,1");
		mEditBox_UVRect->SetRect(MGUI_Rect(true, 80, top, 165, 24));

		top += 24 + K_Space;

		// TextColor
		//
		MGUI_Label * lb_TextColor = new MGUI_Label(NULL, mLayout);
		lb_TextColor->SetCaption(L"TColor:");
		lb_TextColor->SetRect(MGUI_Rect(true, 0, top, 64, 24));

		mWidget_TextColor = new MGUI_Widget(AllLookFeel::Instance()->GetWhite(), mLayout);
		mWidget_TextColor->SetRect(MGUI_Rect(true, 80, top, 165, 24));

		top += 24 + K_Space;

		// Init
		mComboBox_Skin->Append(L"Empty");
		for (int i = 0; i < MGUI_SkinManager::Instance()->GetSkinCount(); ++i)
		{
			mComboBox_Skin->Append(MGUI_SkinManager::Instance()->GetSkin(i)->GetName().c_wstr());
		}

		for (int i = 0; i < MGUI_WidgetState::Max; ++i)
		{
			mComboBox_State->Append(MGUI_WidgetState::GetStateString(i).c_wstr());
		}

		OnEditLookFeelChanged_(NULL);

		LookFeelEditor::Instance()->eventEditLookFeelChanged += OnEditLookFeelChanged(this, &LookFeelProperty::OnEditLookFeelChanged_);
		LookFeelEditor::Instance()->eventEditRectChanged += OnEditRectChanged(this, &LookFeelProperty::OnEditRectChanged_);

		mComboBox_Skin->eventSelectChanged += OnSkinChanged(this, &LookFeelProperty::OnSkinChanged_);
		mComboBox_State->eventSelectChanged += OnStateChanged(this, &LookFeelProperty::OnStateChanged_);

		mEditBox_Name->eventTextReturn += OnNameReturn(this, &LookFeelProperty::OnNameReturn_);
		mEditBox_Client->eventTextReturn += OnClientReturn(this, &LookFeelProperty::OnClientReturn_);
		mEditBox_UVRect->eventTextReturn += OnUVRectReturn(this, &LookFeelProperty::OnUVRectReturn_);
	}

	LookFeelProperty::~LookFeelProperty()
	{
		delete mLayout;
	}

	void LookFeelProperty::Layout()
	{
		MGUI_Rect rect;

		rect.x0 = MGUI_Engine::Instance()->GetRect().x1 - 256;
		rect.x1 = MGUI_Engine::Instance()->GetRect().x1;

		rect.y0 = 32;
		rect.y1 = MGUI_Engine::Instance()->GetRect().y1 - 32;

		mLayout->SetRect(rect);
	}

	void LookFeelProperty::OnEditLookFeelChanged_(MGUI_LookFeel * _lookfeel)
	{
		if (_lookfeel)
		{
			int state = MGUI_WidgetState::Normal;

			mEditBox_Name->SetCaption(_lookfeel->GetName().c_wstr());
			mEditBox_Client->SetCaption(_lookfeel->GetClientRect().ToString_XYWH().c_wstr());

			mEditBox_UVRect->SetCaption(_lookfeel->GetUVRect(state).ToString_XYWH().c_wstr());
			mWidget_TextColor->SetColor(_lookfeel->GetTextColor(state));

			if (_lookfeel->GetSkin() != NULL)
			{
				int index = 0;
				Texture * skin = _lookfeel->GetSkin();

				for (int i = 1; i < MGUI_SkinManager::Instance()->GetSkinCount(); ++i)
				{
					Texture * skin2 = MGUI_SkinManager::Instance()->GetSkin(i - 1).c_ptr();

					if (skin2 == skin)
					{
						index = i;
						break;
					}
				}

				d_assert (index != 0);

				mComboBox_Skin->SetSelectIndex(index);
				mComboBox_State->SetSelectIndex(state);
			}
			else
			{
				mComboBox_Skin->SetSelectIndex(0);
			}
		}
		else
		{
			mEditBox_Name->SetCaption(L"");
			mEditBox_Client->SetCaption(MGUI_Rect().ToString_XYWH().c_wstr());

			mEditBox_UVRect->SetCaption(MGUI_Rect().ToString_XYWH().c_wstr());
			mWidget_TextColor->SetColor(MGUI_Color::Black);
		}

		OnUVRectReturn_();
	}

	void LookFeelProperty::OnEditRectChanged_(MGUI_Rect _rect)
	{
		mEditBox_UVRect->SetCaption(_rect.ToString_XYWH().c_wstr());

		MGUI_LookFeel * lookfeel = LookFeelEditor::Instance()->GetEditLookFeel();

		if (lookfeel == NULL)
			return ;

		int state = mComboBox_State->GetSelectIndex();

		if (state == -1)
			return ;

		lookfeel->SetUVRect(state, _rect);
	}

	void LookFeelProperty::OnSkinChanged_(int _index)
	{
		if (_index < 1)
		{
			LookFeelEditor::Instance()->SetEditSkin(NULL);
		}
		else
		{
			LookFeelEditor::Instance()->SetEditSkin(MGUI_SkinManager::Instance()->GetSkin(_index - 1).c_ptr());
		}
	}

	void LookFeelProperty::OnStateChanged_(int _index)
	{
		if (_index == -1)
			return ;

		MGUI_LookFeel * lookfeel = LookFeelEditor::Instance()->GetEditLookFeel();

		if (lookfeel == NULL)
			return ;

		mEditBox_UVRect->SetCaption(lookfeel->GetUVRect(_index).ToString_XYWH().c_wstr());
		mWidget_TextColor->SetColor(lookfeel->GetTextColor(_index));

		OnUVRectReturn_();
	}

	void LookFeelProperty::OnNameReturn_()
	{
	}

	void LookFeelProperty::OnClientReturn_()
	{
	}

	void LookFeelProperty::OnUVRectReturn_()
	{
		const MGUI_String & text = mEditBox_UVRect->GetCaption();
		TString128 strRect = TString128::FromUnicode(text.c_str());

		MGUI_Rect rect;

		rect.FromString_XYWH(strRect);

		LookFeelEditor::Instance()->SetEditRect(rect);
	}

}