#pragma once



class xSceneOpenDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xSceneOpenDlg();
	~xSceneOpenDlg();

protected:
	BOOL OnInitDialog();
	void OnOK();
	void OnFloderChanged();
};
