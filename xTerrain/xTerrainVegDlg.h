#pragma once

#include "MForest.h"

class xTerrainVegDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainVegDlg();
	~xTerrainVegDlg();

	void _Init(void * param0, void * param1);
	void _UnloadScene(void * param0, void * param1);
	void _AfterLoadScene(void * param0, void * param1);

	MVegetation * GetCurVeg();
	bool IsAddMode();

	float BrushSize();
	int BrushDensity();
	float MinSize();
	float MaxSize();
	float MinLum();
	float MaxLum();

protected:
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();

protected:
	void _refrushList();
};