#include "stdafx.h"
#include "resource.h"
#include "PropertyDlg.h"


BEGIN_MESSAGE_MAP(PropertyDlg, CDockablePane)

	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)

END_MESSAGE_MAP()

PropertyDlg::PropertyDlg()
{
}

PropertyDlg::~PropertyDlg()
{
}

void PropertyDlg::Init()
{
	/*CMFCPropertyGridCtrl * ctrl = (CMFCPropertyGridCtrl *)GetDlgItem(IDC_Property);

	ctrl->EnableHeaderCtrl(true);
	ctrl->EnableDescriptionArea();
	ctrl->SetVSDotNetLook();
	ctrl->MarkModifiedProperties();
	ctrl->SetAlphabeticMode(FALSE);
	ctrl->SetShowDragContext();

	CMFCPropertyGridProperty * pGroup1 = new CMFCPropertyGridProperty("G0");
	CMFCPropertyGridProperty * pGroup2 = new CMFCPropertyGridProperty("G2");

	pGroup1->AddSubItem(new CMFCPropertyGridProperty("p0", "2.5"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty("p1", "3.5"));

	pGroup2->AddSubItem(new CMFCPropertyGridProperty("p3", "2.5"));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty("p4", "3.5"));

	ctrl->AddProperty(pGroup1);
	ctrl->AddProperty(pGroup2);

	ctrl->ExpandAll();*/
}

LRESULT PropertyDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	/*CMFCPropertyGridProperty * prop = (CMFCPropertyGridProperty *)lParam;

	int i = (int) prop->GetData();

	LPCTSTR name = prop->GetName();
	COleVariant var = prop->GetValue();

	float v = var.fltVal;*/

	return 0;
}