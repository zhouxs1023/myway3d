#include "stdafx.h"
#include "resource.h"

#include "xRenderSettingPane.h"
#include "xAfxResourceSetup.h"
#include "xRenderSetting.h"
#include "xEvent.h"


BEGIN_MESSAGE_MAP(xRenderSettingPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()

	ON_WM_SETTINGCHANGE()

	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)

END_MESSAGE_MAP()

xRenderSettingPane gRenderSettingPane;

xRenderSettingPane::xRenderSettingPane()
	: IDockPane("RenderSetting")
	, OnCreatePane(&xEvent::OnCreatePane, this, &xRenderSettingPane::_Create)
	, OnAfterLoadScene(&xEvent::OnAfterLoadScene, this, &xRenderSettingPane::_AfterLoadScene)
{
}

xRenderSettingPane::~xRenderSettingPane()
{
}

void xRenderSettingPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = 0;

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int xRenderSettingPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	afx_resource_setup();

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性\n");
		return -1; 
	}

	AdjustLayout();
	return 0;
}

void xRenderSettingPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void xRenderSettingPane::Show(bool b)
{
	m_wndPropList.RemoveAll();

	if (!b)
		return ;

	IPropertyObj * obj = xRenderSetting::Instance();

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

void xRenderSettingPane::_ToCtrl(CMFCPropertyGridProperty * gp, IPropertyObj * obj, const Property * p)
{
	if (p->type == PT_Bool)
	{
		bool data = p->AsBool(obj->GetPropertyData(p));

		gp->AddSubItem(new CMFCPropertyGridProperty(p->name.c_str(), (_variant_t)(data), ""));
	}
	else if (p->type == PT_Int)
	{
		int val = p->AsInt(obj->GetPropertyData(p));

		const cEnum * e = p->enumObj->GetEnum(val);

		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(p->name.c_str(), e->name.c_str());

		for (int i = 0; i < p->enumObj->GetEnumCount(); ++i)
		{
			e = p->enumObj->GetEnum(i);
			pProp->AddOption(e->name.c_str());
		}

		pProp->AllowEdit(FALSE);

		gp->AddSubItem(pProp);
	}
}

LRESULT xRenderSettingPane::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty * prop = (CMFCPropertyGridProperty *)lParam;
	IPropertyObj * obj = xRenderSetting::Instance();

	const char * name = prop->GetName();

	const Property * p = obj->GetProperty(name);

	if (p)
	{
		char cdata[128];
		const COleVariant & strValue = prop->GetValue();
		CString val = (CString)strValue;

		Strcpy(cdata, 128, (const char *)val);

		if (p->type == PT_Bool)
		{
			bool bdata = (strValue.boolVal == -1);
			obj->SetPropertyData(p, &bdata);
		}
		else if (p->type == PT_Int && p->enumObj)
		{
			const cEnum * e = p->enumObj->GetEnum(cdata);
			obj->SetPropertyData(p, &e->val);
		}
	}


	return S_OK;
}

void xRenderSettingPane::_Create(void * param0, void * param1)
{
	CFrameWndEx * frame = (CFrameWndEx *)param0;

	if (!Create("Render Setting", frame, CRect(0, 0, 200, 200), TRUE, IDD_RenderSetting, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Can't create \"Render Setting Pane\"\n");
		return ;
	}

	EnableDocking(CBRS_ALIGN_ANY);
	frame->DockPane(this);

	ShowPane(FALSE, FALSE, TRUE);
}

void xRenderSettingPane::_AfterLoadScene(void * param0, void * param1)
{
	Show(true);
}


