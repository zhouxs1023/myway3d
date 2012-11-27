#pragma once


class xTerrainLayerDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainLayerDlg();
	~xTerrainLayerDlg();

	void _Init(void *);

protected:
	BOOL OnInitDialog();
};

