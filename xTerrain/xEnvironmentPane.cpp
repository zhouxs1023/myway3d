#include "stdafx.h"

#include "xEnvironmentPane.h"
#include "xEvent.h"
#include "xAfxResourceSetup.h"
#include "resource.h"
#include "xPropertyGridColorPick.h"
#include "xScene.h"

BEGIN_MESSAGE_MAP(xKeyFrameTab, CMFCTabCtrl)

	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)

END_MESSAGE_MAP()

LRESULT xKeyFrameTab::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	int i = GetActiveTab();

	IPropertyObj * obj = NULL;

	switch (i)
	{
	case 0:
		obj = Environment::Instance()->GetKeyFrame(0);
		break;

	case 1:
		obj = Environment::Instance()->GetKeyFrame(1);
		break;

	case 2:
		obj = Environment::Instance()->GetKeyFrame(2);
		break;

	case 3:
		obj = Environment::Instance()->GetKeyFrame(3);
		break;

	case 4:
		obj = Environment::Instance()->GetGlobalParam();
		break;
	}

	if (!obj)
		return S_OK;

	CMFCPropertyGridProperty * prop = (CMFCPropertyGridProperty *)lParam;

	const char * name = prop->GetName();
	const Property * p = obj->GetProperty(name);

	if (p)
	{
		if (p->type == PT_TString && p->size == 128)
		{
			const COleVariant & strValue = prop->GetValue();
			CString val = (CString)strValue;

			TString128 data = (LPCTSTR)val;

			obj->SetPropertyData(p, &data);
		}
		else if (p->type == PT_Float)
		{
			const COleVariant & strValue = prop->GetValue();
			CString val = (CString)strValue;

			float data = (float)atof((LPCTSTR)val);

			obj->SetPropertyData(p, &data);
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

//////////////////////////////////////////////////////////////////////////
//
// Environment Pane
//
xEnvironmentPane gEnvironmentPane;

BEGIN_MESSAGE_MAP(xEnvironmentPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()

END_MESSAGE_MAP()



IMP_SLN(xEnvironmentPane);

xEnvironmentPane::xEnvironmentPane()
	: OnInit(&xEvent::OnInitUI, this, &xEnvironmentPane::_Init)
	, OnShutdown(&xEvent::OnShutdown, this, &xEnvironmentPane::_Shutdown)
	, OnCreatePane(&xEvent::OnCreatePane, this, &xEnvironmentPane::_Create)
	, OnUpdate(&xEvent::OnUpdate, this, &xEnvironmentPane::_Update)
{
	INIT_SLN;
}

xEnvironmentPane::~xEnvironmentPane()
{
	SHUT_SLN;
}

void xEnvironmentPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = 0;

	mTab.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);

	CRect rc;   
	mTab.GetClientRect(rc);   
	rc.top += 20;   
	rc.bottom -= 5;   
	rc.left += 5;   
	rc.right -= 5;

	mNight.MoveWindow(&rc);
	mMorning.MoveWindow(&rc);
	mNoon.MoveWindow(&rc);
	mEvening.MoveWindow(&rc);
	mGlobal.MoveWindow(&rc);
}

void xEnvironmentPane::_Frush()
{
	EvKeyFrame * kNight = Environment::Instance()->GetKeyFrame(EVKT_Night);
	EvKeyFrame * kMorning = Environment::Instance()->GetKeyFrame(EVKT_Morning);
	EvKeyFrame * kNoon = Environment::Instance()->GetKeyFrame(EVKT_Noon);
	EvKeyFrame * kEvening = Environment::Instance()->GetKeyFrame(EVKT_Evening);
	EvGlobalParam * kGlobal = Environment::Instance()->GetGlobalParam();

	_Frush(mNight, kNight);
	_Frush(mMorning, kMorning);
	_Frush(mNoon, kNoon);
	_Frush(mEvening, kEvening);
	//_Frush(&mGlobal, kGlobal);
}

void xEnvironmentPane::_Frush(CMFCPropertyGridCtrl & PropertyGrid, IPropertyObj * obj)
{
	PropertyGrid.RemoveAll();

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

		PropertyGrid.AddProperty(gp);

		++whr;
	}
}

void xEnvironmentPane::_ToCtrl(CMFCPropertyGridProperty * gp, IPropertyObj * obj, const Property * p)
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

int xEnvironmentPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	afx_resource_setup();

	// create tab control
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!mTab.Create(CMFCTabCtrl::STYLE_3D_VS2005, rectDummy, this, IDC_Environment_Tab))
	{
		TRACE0("Create \"Environment Tab\" failed!\n");
		return -1;
	}

	mTab.SetLocation(CMFCBaseTabCtrl::LOCATION_TOP);

	mNight.Create(WS_VISIBLE | WS_CHILD, rectDummy, &mTab, 2);
	mMorning.Create(WS_VISIBLE | WS_CHILD, rectDummy, &mTab, 2);
	mNoon.Create(WS_VISIBLE | WS_CHILD, rectDummy, &mTab, 2);
	mEvening.Create(WS_VISIBLE | WS_CHILD, rectDummy, &mTab, 2);
	mGlobal.Create(WS_VISIBLE | WS_CHILD, rectDummy, &mTab, 2);

	mTab.AddTab(&mNight, "Night");
	mTab.AddTab(&mMorning, "Morning");
	mTab.AddTab(&mNoon, "Noon");
	mTab.AddTab(&mEvening, "Evening");
	mTab.AddTab(&mGlobal, "Global");

	AdjustLayout();

	return 0;
}

void xEnvironmentPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void xEnvironmentPane::_Create(void * param0, void * param1)
{
	CFrameWndEx * frame = (CFrameWndEx *)param0;

	if (!Create("Environment", frame, CRect(0, 0, 200, 200), TRUE, IDD_Terrain, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("can't create \"Environment Pane\"\n");
		return ;
	}

	EnableDocking(CBRS_ALIGN_ANY);
	frame->DockPane(this);
}

void xEnvironmentPane::_Init(void * param0, void * param1)
{
}

void xEnvironmentPane::_Shutdown(void * param0, void * param1)
{
}

void xEnvironmentPane::_Update(void * param0, void * param1)
{
	if (!xScene::Instance()->IsInited())
		return ;

	int i = mTab.GetActiveTab();

	if (i >= EVKT_Night && i <= EVKT_Evening)
		Environment::Instance()->SetKey((EvKeyType)i);
}