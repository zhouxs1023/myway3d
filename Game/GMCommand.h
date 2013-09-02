#pragma once

class cGMCommand
{
public:
	cGMCommand();
	~cGMCommand();

	void Show(bool _show);
	bool IsShow();

protected:
	MGUI_Layout * mLayout;
	MGUI_EditBox * mEditBox;
	MGUI_LookFeel * mEditBoxLookFeel;
};