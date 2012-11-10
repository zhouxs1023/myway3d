// WorldBuildView.cpp : MyView 类的实现
//

#include "stdafx.h"

#include "MyDoc.h"
#include "MyView.h"
#include "Controller.h"


// MyView

IMPLEMENT_DYNCREATE(MyView, CView)

BEGIN_MESSAGE_MAP(MyView, CView)

    ON_WM_SIZE()

END_MESSAGE_MAP()

// MyView 构造/析构

MyView::MyView()
{
	// TODO: 在此处添加构造代码

}

MyView::~MyView()
{
}

BOOL MyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// MyView 绘制

void MyView::OnDraw(CDC* /*pDC*/)
{
	MyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    static bool bInit = true;
    if (bInit)
    {
        xInfi::Controller::Instance()->Init(m_hWnd);
        bInit = false;
    }
}

MyDoc* MyView::GetDocument() const // 非调试版本是内联的
{
    return (MyDoc*)m_pDocument;
}


void MyView::OnSize(UINT nType, int cx, int cy)
{
    xInfi::Controller::Instance()->Resize(cx, cy);
}