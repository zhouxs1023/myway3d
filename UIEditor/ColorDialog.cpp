#include "stdafx.h"
#include "ColorDialog.h"
#include "AllLookFeel.h"

namespace Myway {

	ColorDialog::ColorDialog()
	{
		mLayout = new MGUI_Layout(AllLookFeel::Instance()->GetDialog());
		mLayout->SetAlign(MGUI_Align::Center);

		mLayout->SetRect(MGUI_Rect(true, 496, 400, 427, 311));

		mLabel_Caption = new MGUI_Label(AllLookFeel::Instance()->GetDialogCaption(), mLayout);
		mLabel_Caption->SetCaption(L"Color Picker");
		mLabel_Caption->SetAlign(MGUI_Align::Top | MGUI_Align::HStretch);

		mLabel_R = new MGUI_Label(NULL, mLayout);
		mLabel_G = new MGUI_Label(NULL, mLayout);
		mLabel_B = new MGUI_Label(NULL, mLayout);
		mLabel_R->SetCaption(L"R:");
		mLabel_G->SetCaption(L"G:");
		mLabel_B->SetCaption(L"B:");
		mLabel_R->SetRect(MGUI_Rect(true, 304, 187, 48, 24));
		mLabel_G->SetRect(MGUI_Rect(true, 304, 213, 48, 26));
		mLabel_B->SetRect(MGUI_Rect(true, 304, 241, 48, 26));
		
		mEditBox_R = new MGUI_EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
		mEditBox_G = new MGUI_EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
		mEditBox_B = new MGUI_EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
		mEditBox_R->SetRect(MGUI_Rect(true, 356, 188, 56, 26));
		mEditBox_G->SetRect(MGUI_Rect(true, 356, 215, 56, 26));
		mEditBox_B->SetRect(MGUI_Rect(true, 356, 242, 56, 26));
	}

	ColorDialog::~ColorDialog()
	{
		delete mLayout;
	}

}