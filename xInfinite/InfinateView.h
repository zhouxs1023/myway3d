#pragma once


class CInfinateView : public CView
{
	DECLARE_DYNCREATE(CInfinateView)
	DECLARE_MESSAGE_MAP()
	DECLARE_SINGLETON(CInfinateView);

public:
	CInfinateView();
	virtual ~CInfinateView();

public:
	CInfinateDoc* GetDocument() const;

public:
	virtual INT OnCreate(LPCREATESTRUCT lpcs);
	virtual void OnDraw(CDC* pDC); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void OnDropFiles(HDROP hDropInfo);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


