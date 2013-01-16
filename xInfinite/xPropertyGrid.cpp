
#include "stdafx.h"

#include "xPropertyGrid.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Infinate.h"
#include "xPropertyGridColorPick.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

IMP_SLN (xPropertyGrid);

xPropertyGrid::xPropertyGrid()
	: IDockPane("Property")
	, OnSelectObj(&xEvent::OnSelectObj, this, &xPropertyGrid::_OnSelect)
	, OnUnSelectObj(&xEvent::OnUnSelectObj, this, &xPropertyGrid::_OnUnSelect)
{
	INIT_SLN;
}

xPropertyGrid::~xPropertyGrid()
{
	SHUT_SLN;
}

BEGIN_MESSAGE_MAP(xPropertyGrid, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()

	ON_WM_SETTINGCHANGE()

	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序
void xPropertyGrid::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int xPropertyGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	//InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void xPropertyGrid::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

//void xPropertyGrid::InitPropList()
//{
//	SetPropListFont();
//
//	m_wndPropList.EnableHeaderCtrl(FALSE);
//	m_wndPropList.EnableDescriptionArea();
//	m_wndPropList.SetVSDotNetLook();
//	m_wndPropList.MarkModifiedProperties();
//
//	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("外观"));
//
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("三维外观"), (_variant_t) false, _T("指定窗口的字体不使用粗体，并且控件将使用三维边框")));
//
//	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一:“无”、“细”、“可调整大小”或“对话框外框”"));
//	pProp->AddOption(_T("无"));
//	pProp->AddOption(_T("细"));
//	pProp->AddOption(_T("可调整大小"));
//	pProp->AddOption(_T("对话框外框"));
//	pProp->AllowEdit(FALSE);
//
//	pGroup1->AddSubItem(pProp);
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("标题"), (_variant_t) _T("关于"), _T("指定窗口标题栏中显示的文本")));
//
//	m_wndPropList.AddProperty(pGroup1);
//
//	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("窗口大小"), 0, TRUE);
//
//	pProp = new CMFCPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定窗口的高度"));
//	pProp->EnableSpinControl(TRUE, 50, 300);
//	pSize->AddSubItem(pProp);
//
//	pProp = new CMFCPropertyGridProperty( _T("宽度"), (_variant_t) 150l, _T("指定窗口的宽度"));
//	pProp->EnableSpinControl(TRUE, 50, 200);
//	pSize->AddSubItem(pProp);
//
//	m_wndPropList.AddProperty(pSize);
//
//	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("字体"));
//
//	LOGFONT lf;
//	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
//	font->GetLogFont(&lf);
//
//	lstrcpy(lf.lfFaceName, _T("宋体, Arial"));
//
//	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定窗口的默认字体")));
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定窗口使用“MS Shell Dlg”字体")));
//
//	m_wndPropList.AddProperty(pGroup2);
//
//	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("杂项"));
//	pProp = new CMFCPropertyGridProperty(_T("(名称)"), _T("应用程序"));
//	pProp->Enable(FALSE);
//	pGroup3->AddSubItem(pProp);
//
//	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("窗口颜色"), RGB(210, 192, 254), NULL, _T("指定默认的窗口颜色"));
//	pColorProp->EnableOtherButton(_T("其他..."));
//	pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
//	pGroup3->AddSubItem(pColorProp);
//
//	static const TCHAR szFilter[] = _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||");
//	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定窗口图标")));
//
//	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("文件夹"), _T("c:\\")));
//
//	m_wndPropList.AddProperty(pGroup3);
//
//	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("层次结构"));
//
//	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("第一个子级"));
//	pGroup4->AddSubItem(pGroup41);
//
//	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("第二个子级"));
//	pGroup41->AddSubItem(pGroup411);
//
//	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 1"), (_variant_t) _T("值 1"), _T("此为说明")));
//	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 2"), (_variant_t) _T("值 2"), _T("此为说明")));
//	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 3"), (_variant_t) _T("值 3"), _T("此为说明")));
//
//	pGroup4->Expand(FALSE);
//	m_wndPropList.AddProperty(pGroup4);
//}

void xPropertyGrid::_OnSelect(void * param0, void * param1)
{
	xObj * obj = xApp::Instance()->GetSelectedObj(0);

	if (xApp::Instance()->GetSelectedObjSize() > 1)
		obj = NULL;

	Show(obj);
}

void xPropertyGrid::_OnUnSelect(void * param0, void * param1)
{
	Show(NULL);
}

void xPropertyGrid::Show(xObj * obj)
{
	m_wndPropList.RemoveAll();

	if (!obj)
		return ;

	int propSize = obj->GetPropertySize();

	MultiMap<TString128, const Property *> mmap;

	for (int i = 0; i < propSize; ++i)
	{
		const Property * p = obj->GetProperty(i);
		mmap.Insert(p->group, p);
	}

	MultiMap<TString128, const Property *>::Iterator whr = mmap.Begin();
	MultiMap<TString128, const Property *>::Iterator end = mmap.End();

	while (whr != end)
	{
		CMFCPropertyGridProperty * gp = new CMFCPropertyGridProperty(whr->first.c_str());

		List<const Property *>::Iterator w = whr->second.Begin();
		List<const Property *>::Iterator e = whr->second.End();

		while (w != e)
		{
			const Property * p = *w;

			_ToCtrl(gp, obj, p);

			++w;
		}

		m_wndPropList.AddProperty(gp);

		++whr;
	}
}

void xPropertyGrid::_ToCtrl(CMFCPropertyGridProperty * gp, xObj * obj, const Property * p)
{
	if (p->type == PT_TString)
	{
		const char * data = p->AsTString<128>(obj->GetPropertyData(p)).c_str();
		gp->AddSubItem(new CMFCPropertyGridProperty(p->name.c_str(), (_variant_t)(data), ""));
	}
	else if (p->type == PT_Vec3)
	{
		Vec3 data = p->AsVec3(obj->GetPropertyData(p));

		CMFCPropertyGridProperty* gp1 = new CMFCPropertyGridProperty(p->name.c_str());
		gp->AddSubItem(gp1);

		gp1->AddSubItem(new CMFCPropertyGridProperty("x", (_variant_t)(data.x), ""));
		gp1->AddSubItem(new CMFCPropertyGridProperty("y", (_variant_t)(data.y), ""));
		gp1->AddSubItem(new CMFCPropertyGridProperty("z", (_variant_t)(data.z), ""));
	}
	else if (p->type == PT_Vec4)
	{
		Vec4 data = p->AsVec4(obj->GetPropertyData(p));

		CMFCPropertyGridProperty* gp1 = new CMFCPropertyGridProperty(p->name.c_str());
		gp->AddSubItem(gp1);

		gp1->AddSubItem(new CMFCPropertyGridProperty("x", (_variant_t)(data.x), ""));
		gp1->AddSubItem(new CMFCPropertyGridProperty("y", (_variant_t)(data.y), ""));
		gp1->AddSubItem(new CMFCPropertyGridProperty("z", (_variant_t)(data.z), ""));
		gp1->AddSubItem(new CMFCPropertyGridProperty("w", (_variant_t)(data.w), ""));
	}
	else if (p->type == PT_Float)
	{
		float data = p->AsFloat(obj->GetPropertyData(p));

		gp->AddSubItem(new CMFCPropertyGridProperty(p->name.c_str(), (_variant_t)(data), ""));
	}
	else if (p->type == PT_Color)
	{
		Color4 data = p->AsColor(obj->GetPropertyData(p));

		int r = int(data.r * 255);
		int g = int(data.g * 255);
		int b = int(data.b * 255);

		COLORREF cr = RGB(r, g, b);

		gp->AddSubItem(new xPropertyGridColorPick(p->name.c_str(), cr));
	}
}

LRESULT xPropertyGrid::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty * prop = (CMFCPropertyGridProperty *)lParam;
	xObj * obj = xApp::Instance()->GetSelectedObj(0);

	const char * name = prop->GetName();

	const Property * p = obj->GetProperty(name);

	if (p)
	{
		char cdata[128];
		const COleVariant & strValue = prop->GetValue();
		CString val = (CString)strValue;

		Strcpy(cdata, 128, (const char *)val);

		if (p->type == PT_TString && p->size == 128)
			obj->SetPropertyData(p, cdata);
		else if (p->type == PT_Float)
		{
			float value = (float)atof(cdata);
			obj->SetPropertyData(p, &value);
		}
		else if (p->type == PT_Color)
		{
			xPropertyGridColorPick * colorPick = (xPropertyGridColorPick *)prop;
			COLORREF color = colorPick->GetColor();

			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			Color4 data = Color4(r / 255.0f, g / 255.0f, b / 255.0f);

			obj->SetPropertyData(p, &data);
		}
	}

	return S_OK;
}



