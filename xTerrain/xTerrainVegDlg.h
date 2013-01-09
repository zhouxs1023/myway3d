#pragma once


class xTerrainVegDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainVegDlg();
	~xTerrainVegDlg();

	void _Init(void * param0, void * param1);
	void _UnloadScene(void * param0, void * param1);
	void _AfterLoadScene(void * param0, void * param1);
};