#pragma once

class xExportSettingDlg : public CDialog
{
	DECLARE_MESSAGE_MAP();

public:
	xExportSettingDlg();
	~xExportSettingDlg();

	BOOL OnInitDialog();

	INT_PTR DoModal();
	void OnOK();
};