#pragma once


class xTerrainLayerDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainLayerDlg();
	~xTerrainLayerDlg();

	void _Init(void * param0, void * param1);
	void _UnloadScene(void * param0, void * param1);
	void _AfterLoadScene(void * param0, void * param1);

	int GetCurLayer();

protected:
	afx_msg void OnTypeChanged();
	afx_msg void OnAddLayer();
	afx_msg void OnRemoveLayer();
	afx_msg void OnEditLayer();
	afx_msg void OnBrushSize();
	afx_msg void OnBrushDensity();

	BOOL OnInitDialog();

protected:
	Array<int> mLayerIds;
};

