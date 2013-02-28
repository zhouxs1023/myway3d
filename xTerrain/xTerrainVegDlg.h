#pragma once

#include "MForest.h"

class xTerrainVegDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainVegDlg();
	~xTerrainVegDlg();

	void _Init(Event * sender);
	void _UnloadScene(Event * sender);
	void _AfterLoadScene(Event * sender);

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