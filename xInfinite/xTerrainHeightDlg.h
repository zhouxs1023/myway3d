#pragma once


class xTerrainHeightDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainHeightDlg();
	~xTerrainHeightDlg();

	void _Init(void *);

protected:
	BOOL OnInitDialog();
};
