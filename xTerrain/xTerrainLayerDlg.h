#pragma once


class xTerrainLayerDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainLayerDlg();
	~xTerrainLayerDlg();

	void _Init(Event * sender);
	void _UnloadScene(Event * sender);
	void _AfterLoadScene(Event * sender);

	int GetCurLayer();

protected:
	afx_msg void OnTypeChanged();
	afx_msg void OnAddLayer();
	afx_msg void OnRemoveLayer();
	afx_msg void OnEditLayer();
	afx_msg void OnBrushSize();
	afx_msg void OnBrushDensity();

	afx_msg BOOL OnInitDialog();

protected:
	Array<int> mLayerIds;
};

