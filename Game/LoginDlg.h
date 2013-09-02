#pragma once

class cLoginDlg
{
public:
	cLoginDlg();
	~cLoginDlg();

protected:
	void OnUpdate_();
	void OnEnter_();

protected:
	MGUI_LookFeel * mLoginBackImage;
	MGUI_Layout * mLayout;

	MGUI_LookFeel * mBnLookFeel;

	tListener0<cLoginDlg> OnUpdate;
	tListener0<cLoginDlg> OnEnter;
};