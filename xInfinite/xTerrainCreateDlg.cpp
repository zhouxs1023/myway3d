#include "stdafx.h"
#include "xTerrainCreateDlg.h"
#include "resource.h"

xTerrainCreateDlg::xTerrainCreateDlg()
	: CDialog(IDD_Terrain_Create)
{
}

xTerrainCreateDlg::~xTerrainCreateDlg()
{
}


BOOL xTerrainCreateDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	SetDlgItemText(IDC_Edit_TC_SizeX, "2048");
	SetDlgItemText(IDC_Edit_TC_SizeZ, "2048");

	CComboBox * cbVertX = (CComboBox *)GetDlgItem(IDC_Combo_TC_VertexX);
	CComboBox * cbVertZ = (CComboBox *)GetDlgItem(IDC_Combo_TC_VertexZ);

	cbVertX->InsertString(0, "256");
	cbVertX->InsertString(1, "512");
	cbVertX->InsertString(2, "1024");
	cbVertX->InsertString(3, "2048");
	cbVertX->InsertString(4, "4096");
	cbVertX->SetCurSel(2);

	cbVertZ->InsertString(0, "256");
	cbVertZ->InsertString(1, "512");
	cbVertZ->InsertString(2, "1024");
	cbVertZ->InsertString(3, "2048");
	cbVertZ->InsertString(4, "4096");
	cbVertZ->SetCurSel(2);


	return TRUE;
}

void xTerrainCreateDlg::OnOK()
{
	CString strSizeX, strSizeZ, strVertX, strVertZ;
		
	GetDlgItemText(IDC_Edit_TC_SizeX, strSizeX);
	GetDlgItemText(IDC_Edit_TC_SizeZ, strSizeZ);
	GetDlgItemText(IDC_Combo_TC_VertexX, strVertX);
	GetDlgItemText(IDC_Combo_TC_VertexZ, strVertZ);

	mSizeX = (float)atof((LPCTSTR)strSizeX);
	mSizeZ = (float)atof((LPCTSTR)strSizeZ);

	mVertX = atoi((LPCTSTR)strVertX);
	mVertZ = atoi((LPCTSTR)strVertZ);

	CDialog::OnOK();
}
