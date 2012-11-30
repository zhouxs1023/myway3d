#include "stdafx.h"
#include "xTerrainLayerInfoDlg.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(xTerrainLayerInfoDlg, CDialog)

END_MESSAGE_MAP()

xTerrainLayerInfoDlg::xTerrainLayerInfoDlg(const Terrain::Layer * layer)
	: CDialog(IDD_Layer)
	, mLayer(layer)
{
}

xTerrainLayerInfoDlg::~xTerrainLayerInfoDlg()
{
}

void xTerrainLayerInfoDlg::OnOK()
{
	CString strDetailMap, strNormalMap, strSpecularMap, strUVScale;

	GetDlgItemText(IDC_Layer_DetailMap, strDetailMap);
	GetDlgItemText(IDC_Layer_NormalMap, strNormalMap);
	GetDlgItemText(IDC_Layer_SpecularMap, strSpecularMap);
	GetDlgItemText(IDC_Layer_UVScale, strUVScale);

	mDetailMap = (LPCTSTR)strDetailMap;
	mNormalMap = (LPCTSTR)strNormalMap;
	mSpecularMap = (LPCTSTR)strSpecularMap;
	mUVScale = (float)atof((LPCTSTR)strUVScale);

	if (mDetailMap == "")
	{
		MessageBox("DetailMap is empty !", "Error", MB_OK);
		return ;
	}

	if (mUVScale <= 0)
	{
		MessageBox("UVScale must greater 0!", "Error", MB_OK);
		return ;
	}

	CDialog::OnOK();
}

BOOL xTerrainLayerInfoDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	if (mLayer)
	{
		SetDlgItemText(IDC_Layer_DetailMap, mLayer->detail.c_str());
		SetDlgItemText(IDC_Layer_NormalMap, mLayer->normal.c_str());
		SetDlgItemText(IDC_Layer_SpecularMap, mLayer->specular.c_str());
		SetDlgItemText(IDC_Layer_UVScale, TString128(mLayer->scale).c_str());
	}
	else
	{
		SetDlgItemText(IDC_Layer_UVScale, "100");
	}

	return TRUE;
}