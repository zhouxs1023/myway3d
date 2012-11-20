#pragma once

class xToolBar : public CMFCToolBar
{
	static const int MAX_BUTTONS = 32;

	DECLARE_SINGLETON(xToolBar);

public:
	static Event OnCommand;

public:
	xToolBar();
	virtual ~xToolBar();

	/*BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT, DWORD dwStyle = AFX_DEFAULT_TOOLBAR_STYLE,
		          CRect rcBorders = CRect(1, 1, 1, 1), UINT nID = AFX_IDW_TOOLBAR);

	virtual void DoPaint(CDC* pDC);
*/
	void Add(const char * icon, UINT btn);
	

protected:
	void Init(void * data);

	virtual BOOL PreTranslateMessage(MSG * msg);

protected:
	CMFCToolBarImages mImageList;
	int mNumButtons;
	UINT mButtons[MAX_BUTTONS];

	tEventListener<xToolBar> mOnInitUI;
};