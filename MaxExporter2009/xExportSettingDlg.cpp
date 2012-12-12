#include "stdafx.h"
#include "resource.h"

#include "xExportSettingDlg.h"
#include "xExportConfig.h"

BEGIN_MESSAGE_MAP(xExportSettingDlg, CDialog)
END_MESSAGE_MAP()

xExportSettingDlg::xExportSettingDlg()
	: CDialog(IDD_ExportSetting)
{
}

xExportSettingDlg::~xExportSettingDlg()
{
}

BOOL xExportSettingDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	xExportConfig::Instance()->Load();

	bool expSelected = xExportConfig::Instance()->IsExportSelected();
	bool expNormal = xExportConfig::Instance()->IsExportNormal();
	bool expTexcoord = xExportConfig::Instance()->IsExportTexcoord();
	bool expColor = xExportConfig::Instance()->IsExportColor();
	bool expTangent = xExportConfig::Instance()->IsExportTangent();
	bool expLightmapUV = xExportConfig::Instance()->IsExportLightmapUV();

	CButton * bnSelected = (CButton *)GetDlgItem(IDC_CK_Selected);
	CButton * bnNormal = (CButton *)GetDlgItem(IDC_CK_Normal);
	CButton * bnTexcoord = (CButton *)GetDlgItem(IDC_CK_Texcoord);
	CButton * bnColor = (CButton *)GetDlgItem(IDC_CK_Color);
	CButton * bnTangent = (CButton *)GetDlgItem(IDC_CK_Tangent);
	CButton * bnLightmapUV = (CButton *)GetDlgItem(IDC_CK_LightmapUV);

	bnSelected->SetCheck(expSelected ? BST_CHECKED : BST_UNCHECKED);
	bnNormal->SetCheck(expNormal ? BST_CHECKED : BST_UNCHECKED);
	bnTexcoord->SetCheck(expTexcoord ? BST_CHECKED : BST_UNCHECKED);
	bnColor->SetCheck(expColor ? BST_CHECKED : BST_UNCHECKED);
	bnTangent->SetCheck(expTangent ? BST_CHECKED : BST_UNCHECKED);
	bnLightmapUV->SetCheck(expLightmapUV ? BST_CHECKED : BST_UNCHECKED);


	return TRUE;
}

INT_PTR xExportSettingDlg::DoModal()
{
	return CDialog::DoModal();
}

void xExportSettingDlg::OnOK()
{
	CButton * bnSelected = (CButton *)GetDlgItem(IDC_CK_Selected);
	CButton * bnNormal = (CButton *)GetDlgItem(IDC_CK_Normal);
	CButton * bnTexcoord = (CButton *)GetDlgItem(IDC_CK_Texcoord);
	CButton * bnColor = (CButton *)GetDlgItem(IDC_CK_Color);
	CButton * bnTangent = (CButton *)GetDlgItem(IDC_CK_Tangent);
	CButton * bnLightmapUV = (CButton *)GetDlgItem(IDC_CK_LightmapUV);

	bool expSelected = (bnSelected->GetCheck() == BST_CHECKED);
	bool expNormal = (bnNormal->GetCheck() == BST_CHECKED);
	bool expTexcoord = (bnTexcoord->GetCheck() == BST_CHECKED);
	bool expColor = (bnColor->GetCheck() == BST_CHECKED);
	bool expTangent = (bnTangent->GetCheck() == BST_CHECKED);
	bool expLightmapUV = (bnLightmapUV->GetCheck() == BST_CHECKED);

	xExportConfig::Instance()->SetExportSelected(expSelected);
	xExportConfig::Instance()->SetExportNormal(expNormal);
	xExportConfig::Instance()->SetExportTexcoord(expTexcoord);
	xExportConfig::Instance()->SetExportColor(expColor);
	xExportConfig::Instance()->SetExportTangent(expTangent);
	xExportConfig::Instance()->SetExportLightmapUV(expLightmapUV);

	xExportConfig::Instance()->Save();

	CDialog::OnOK();
}

