
// InfinateView.cpp : CInfinateView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Infinate.h"
#endif

#include "InfinateDoc.h"
#include "InfinateView.h"
#include "xApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInfinateView

IMPLEMENT_DYNCREATE(CInfinateView, CView)

BEGIN_MESSAGE_MAP(CInfinateView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CInfinateView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()

END_MESSAGE_MAP()


IMP_SLN(CInfinateView);
// CInfinateView 构造/析构

CInfinateView::CInfinateView()
{
	// TODO: 在此处添加构造代码
	INIT_SLN;
}

CInfinateView::~CInfinateView()
{
	SHUT_SLN;
}

BOOL CInfinateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CInfinateView 绘制

void CInfinateView::OnDraw(CDC* /*pDC*/)
{
	CInfinateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	static bool bInited = false;
	if (!bInited)
	{
		xApp::Instance()->Init(m_hWnd);
		bInited = true;
	}
}


// CInfinateView 打印


void CInfinateView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CInfinateView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CInfinateView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CInfinateView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CInfinateView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	/*ClientToScreen(&point);
	OnContextMenu(this, point);*/
}

void CInfinateView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


void CInfinateView::OnSize(UINT nType, int cx, int cy)
{
	if (nType != SIZE_MINIMIZED)
		xApp::Instance()->Resize(cx, cy);
}

// CInfinateView 诊断

#ifdef _DEBUG
void CInfinateView::AssertValid() const
{
	CView::AssertValid();
}

void CInfinateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInfinateDoc* CInfinateView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInfinateDoc)));
	return (CInfinateDoc*)m_pDocument;
}
#endif //_DEBUG


// CInfinateView 消息处理程序
