#include "stdafx.h"
#include "xSceneNewDlg.h"
#include "xScene.h"
#include "resource.h"

xSceneNewDlg::xSceneNewDlg()
	: CDialog(IDD_Scene_New)
{
}

xSceneNewDlg::~xSceneNewDlg()
{
}

BOOL xSceneNewDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	CListBox * list = (CListBox *)GetDlgItem(IDC_SceneNew_Floder);

	ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();

	ResourceGroup::ArchiveVisitor v = rg->GetArchives();

	int index = 0;
	while (!v.Endof())
	{
		const TString128 & floder = (*v.Cursor())->GetName();

		list->InsertString(index, floder.c_str());

		++v;
	}

	return TRUE;
}

void xSceneNewDlg::OnOK()
{
	CString strSceneName, strFloder;

	GetDlgItemText(IDC_SceneNew_Name, strSceneName);

	CListBox * list = (CListBox *)GetDlgItem(IDC_SceneNew_Floder);
	int isel = list->GetCurSel();

	if (isel < 0)
	{
		MessageBox("Please select floder!", "Error", MB_OK);
		return ;
	}

	list->GetText(isel, strFloder);

	if (strSceneName == "")
	{
		MessageBox("Scene name empty!", "Error", MB_OK);
		return ;
	}

	strSceneName += ".scene";

	ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();

	Archive * ar = rg->GetArchive((LPCTSTR)strFloder);

	d_assert (ar != NULL);

	if (ar->Exist((LPCTSTR)strSceneName))
	{
		MessageBox("Scene has already exist!", "Error", MB_OK);

		return ;
	}

	xScene::Instance()->New((LPCTSTR)strSceneName, (LPCTSTR)strFloder);

	CDialog::OnOK();
}