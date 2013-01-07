#include "stdafx.h"
#include "resource.h"
#include "xPaneDlg.h"
#include "xApp.h"

BEGIN_MESSAGE_MAP(xPaneDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_PaneList, OnSelChanged)
	ON_BN_CLICKED(IDC_Pane_Visible, OnVisible)
END_MESSAGE_MAP()

xPaneDlg::xPaneDlg()
	: CDialog(IDD_Pane)
{
}

xPaneDlg::~xPaneDlg()
{
}

BOOL xPaneDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	CListBox * list = (CListBox *)GetDlgItem(IDC_PaneList);

	for (int i = 0; i < xApp::Instance()->GetPaneCount(); ++i)
	{
		IDockPane * pane = xApp::Instance()->GetPane(i);

		list->InsertString(i, pane->GetName().c_str());
	}

	return TRUE;
}

void xPaneDlg::OnOK()
{
	CDialog::OnOK();
}

void xPaneDlg::OnSelChanged()
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_PaneList);
	CButton * btn = (CButton *)GetDlgItem(IDC_Pane_Visible);

	int iSel = list->GetCurSel();

	if (iSel != -1)
	{
		IDockPane * pane = xApp::Instance()->GetPane(iSel);
		
		BOOL visible =  pane->IsWindowVisible();

		btn->SetCheck(visible ? BST_CHECKED : BST_UNCHECKED);
	}
}


void xPaneDlg::OnVisible()
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_PaneList);
	CButton * btn = (CButton *)GetDlgItem(IDC_Pane_Visible);

	int iSel = list->GetCurSel();

	if (iSel != -1)
	{
		IDockPane * pane = xApp::Instance()->GetPane(iSel);

		if (btn->GetCheck() == BST_CHECKED)
			pane->ShowPane(TRUE, FALSE, TRUE);
		else
			pane->ShowPane(FALSE, FALSE, TRUE);
	}
}
