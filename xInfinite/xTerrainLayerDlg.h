#pragma once


class xTerrainLayerDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainLayerDlg();
	~xTerrainLayerDlg();

	void _Init(void *);
	int GetCurLayer();

protected:
	afx_msg void OnTypeChanged();
	afx_msg void OnAddLayer();
	afx_msg void OnRemoveLayer();
	afx_msg void OnEditLayer();

protected:
	BOOL OnInitDialog();

	Array<int> mLayerIds;
};

