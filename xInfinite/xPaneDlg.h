#pragma once

class xPaneDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xPaneDlg();
	virtual ~xPaneDlg();

protected:
	BOOL OnInitDialog();
	void OnOK();
	void OnSelChanged();
	void OnVisible();
};