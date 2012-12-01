#include "stdafx.h"
#include "xSceneOpenDlg.h"
#include "xScene.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(xSceneOpenDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_SceneOpen_Floder, OnFloderChanged)
END_MESSAGE_MAP()

xSceneOpenDlg::xSceneOpenDlg()
	: CDialog(IDD_Scene_Open)
{
}

xSceneOpenDlg::~xSceneOpenDlg()
{
}

BOOL xSceneOpenDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	CListBox * list = (CListBox *)GetDlgItem(IDC_SceneOpen_Floder);

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

void xSceneOpenDlg::OnOK()
{
	CString strFloder, strSceneName;

	CListBox * listFloder = (CListBox *)GetDlgItem(IDC_SceneOpen_Floder);
	CListBox * listScene = (CListBox *)GetDlgItem(IDC_SceneOpen_Scene);

	int isel = listScene->GetCurSel();

	if (isel < 0)
	{
		MessageBox("Please select scene!", "Error", MB_OK);
		return ;
	}

	listScene->GetText(isel, strSceneName);

	if (strSceneName == "")
	{
		MessageBox("Please select scene!", "Error", MB_OK);
		return ;
	}

	isel = listFloder->GetCurSel();
	listFloder->GetText(isel, strFloder);

	d_assert (isel >= 0);

	xScene::Instance()->Load((LPCTSTR)strSceneName, (LPCTSTR)strFloder);

	CDialog::OnOK();
}

void xSceneOpenDlg::OnFloderChanged()
{
	CListBox * listFloder = (CListBox *)GetDlgItem(IDC_SceneOpen_Floder);
	CListBox * listScene = (CListBox *)GetDlgItem(IDC_SceneOpen_Scene);

	while (listScene->GetCount())
		listScene->DeleteString(0);

	int isel = listFloder->GetCurSel();

	if (isel < 0)
		return ;

	CString strFloder;

	listFloder->GetText(isel, strFloder);

	ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();

	Archive * ar = rg->GetArchive((LPCTSTR)strFloder);
	
	Archive::FileInfoVisitor v = ar->GetFileInfos();

	while (!v.eof())
	{
		const TString128 & filename = v.Cursor()->second.name;

		if (File::GetExternName(filename) == "scene")
		{
			listScene->AddString(filename.c_str());
		}

		++v;
	}
}