#pragma once


class xTerrainHeightDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	xTerrainHeightDlg();
	~xTerrainHeightDlg();

	void _Init(void *);

protected:
	afx_msg void OnOpChanged();
	afx_msg void OnTypeChanged();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBrushSize();
	afx_msg void OnBrushDensity();

protected:
	BOOL OnInitDialog();
};
