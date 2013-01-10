#include "stdafx.h"
#include "resource.h"
#include "xVegetationDlg.h"

xVegetationDlg::xVegetationDlg(MVegetation * veg)
	: CDialog(IDD_Vegetation)
{
	if (veg)
	{
		mName = veg->Name;
		mType = veg->Type;
		mMesh = veg->pMesh != NULL ? veg->pMesh->GetSourceName() : "";
		mDiffuseMap = veg->DiffuseMap != NULL ? veg->DiffuseMap->GetSourceName() : "";
		mNormalMap = veg->NormalMap != NULL ? veg->NormalMap->GetSourceName() : "";
		mSpecularMap = veg->SpecularMap != NULL ? veg->SpecularMap->GetSourceName() : "";
	}
	else
	{
		mName = "";
		mType = MVegetation::GT_X2;
		mMesh = "";
		mDiffuseMap = "";
		mNormalMap = "";
		mSpecularMap = "";
	}
}

xVegetationDlg::~xVegetationDlg()
{
}

BOOL xVegetationDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	SetDlgItemText(IDC_Veg_Name, mName.c_str());
	SetDlgItemText(IDC_Veg_Mesh, mMesh.c_str());
	SetDlgItemText(IDC_Veg_DiffuseMap, mDiffuseMap.c_str());
	SetDlgItemText(IDC_Veg_SpecularMap, mSpecularMap.c_str());
	SetDlgItemText(IDC_Veg_NormalMap, mNormalMap.c_str());

	CButton * bnMesh = (CButton *)GetDlgItem(IDC_Veg_TypeMesh);
	CButton * bnBillboard = (CButton *)GetDlgItem(IDC_Veg_TypeBillboard);
	CButton * bnX2 = (CButton *)GetDlgItem(IDC_Veg_TypeX2);

	switch (mType)
	{
	case MVegetation::GT_Mesh:
		bnMesh->SetCheck(BST_CHECKED);
		break;

	case MVegetation::GT_BillBoard:
		bnBillboard->SetCheck(BST_CHECKED);
		break;
		
	case MVegetation::GT_X2:
		bnX2->SetCheck(BST_CHECKED);
		break;
	}

	if (mName != "")
	{
		CEdit * ed = (CEdit*)GetDlgItem(IDC_Veg_Name);

		ed->EnableWindow(FALSE);
	}

	return TRUE;
}

void xVegetationDlg::OnOK()
{
	CString str;

	if (mName == "")
	{
		GetDlgItemText(IDC_Veg_Name, str);
		mName = (LPCTSTR)str;
	}

	CButton * bnMesh = (CButton *)GetDlgItem(IDC_Veg_TypeMesh);
	CButton * bnBillboard = (CButton *)GetDlgItem(IDC_Veg_TypeBillboard);
	CButton * bnX2 = (CButton *)GetDlgItem(IDC_Veg_TypeX2);

	if (bnMesh->GetCheck() == BST_CHECKED)
	{
		mType = MVegetation::GT_Mesh;
	}
	else if (bnBillboard->GetCheck() == BST_CHECKED)
	{
		mType = MVegetation::GT_BillBoard;
	}
	else if (bnX2->GetCheck() == BST_CHECKED)
	{
		mType = MVegetation::GT_X2;
	}

	GetDlgItemText(IDC_Veg_Mesh, str);
	mMesh = (LPCTSTR)str;

	GetDlgItemText(IDC_Veg_DiffuseMap, str);
	mDiffuseMap = (LPCTSTR)str;

	GetDlgItemText(IDC_Veg_SpecularMap, str);
	mSpecularMap = (LPCTSTR)str;

	GetDlgItemText(IDC_Veg_NormalMap, str);
	mNormalMap = (LPCTSTR)str;

	if (mType != MVegetation::GT_Mesh)
	{
		mMesh = "";
		mSpecularMap = "";
		mNormalMap = "";
	}

	CDialog::OnOK();
}