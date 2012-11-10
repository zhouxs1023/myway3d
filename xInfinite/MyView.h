// WorldBuildView.h : MyView 类的接口
//
#pragma once


class MyView : public CView
{
    DECLARE_DYNCREATE(MyView)
    DECLARE_MESSAGE_MAP()

public:
	MyView();
    virtual ~MyView();

	MyDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


protected:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

