#include "stdafx.h"
#include "GMCommand.h"

cGMCommand::cGMCommand()
{
	mLayout = new MGUI_Layout(NULL, NULL);
	mLayout->SetAlign(MGUI_Align::Stretch);

	mEditBoxLookFeel = new MGUI_LookFeel("EditBox");
	mEditBoxLookFeel->SetSkin("EditorSkin.png");
	mEditBoxLookFeel->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(169, 29, 169 + 28, 29 + 25));
	mEditBoxLookFeel->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(169, 2, 169 + 28, 2 + 25));
	mEditBoxLookFeel->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(169, 56, 169 + 28, 56 + 25));
	mEditBoxLookFeel->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(169, 164, 169 + 28, 164 + 25));
	mEditBoxLookFeel->SetClientRect(MGUI_Rect(8, 3, 8 + 11, 10 + 13));

	mEditBox = new MGUI_EditBox(mEditBoxLookFeel, mLayout);
	mEditBox->SetRect(0, 0, 256, 32);
	mEditBox->SetAlign(MGUI_Align::Center);
	mEditBox->SetCaption(L"");

	MGUI_Engine::Instance()->AddLayout(mLayout);

	//Show(false);
}

cGMCommand::~cGMCommand()
{
	MGUI_Engine::Instance()->RemoveLayout(mLayout);
	safe_delete(mEditBoxLookFeel);
}

void cGMCommand::Show(bool _show)
{
	mLayout->SetVisible(_show);
}

bool cGMCommand::IsShow()
{
	return mLayout->GetVisible();
}
