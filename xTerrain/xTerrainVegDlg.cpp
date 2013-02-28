#include "stdafx.h"
#include "resource.h"

#include "xTerrainVegDlg.h"
#include "xVegetationDlg.h"
#include "xAfxResourceSetup.h"

BEGIN_MESSAGE_MAP(xTerrainVegDlg, CDialog)
	ON_BN_CLICKED(IDC_TV_Add, OnAdd)
	ON_BN_CLICKED(IDC_TV_Remove, OnRemove)
	ON_BN_CLICKED(IDC_TV_Edit, OnEdit)
END_MESSAGE_MAP()

xTerrainVegDlg::xTerrainVegDlg()
{
}

xTerrainVegDlg::~xTerrainVegDlg()
{
}

void xTerrainVegDlg::_Init(Event * sender)
{
	SetDlgItemText(IDC_TV_BrushSize, "10");
	SetDlgItemText(IDC_TV_BrushDensity, "3");
	SetDlgItemText(IDC_TV_MinSize, "10");
	SetDlgItemText(IDC_TV_MaxSize, "15");
	SetDlgItemText(IDC_TV_MinLum, "1");
	SetDlgItemText(IDC_TV_MaxLum, "1");
}

void xTerrainVegDlg::_UnloadScene(Event * sender)
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_TV_List);

	while (list->GetCount())
		list->DeleteString(0);

	MForest::Instance()->RemoveAllVegetationBlock();
}

void xTerrainVegDlg::_AfterLoadScene(Event * sender)
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_TV_List);

	for (int i = 0; i < MForest::Instance()->GetVegetationCount(); ++i)
	{
		MVegetation * veg = MForest::Instance()->GetVegetation(i);

		list->InsertString(i, veg->Name.c_str());
	}

	if (Environment::Instance()->GetTerrain() && MForest::Instance()->GetVegetationBlockCount() == 0)
	{
		Terrain * terrain = Environment::Instance()->GetTerrain();

		const Terrain::Config & config = terrain->GetConfig();
		RectF rect = RectF(0, 0, config.xSize, config.zSize);

		MForest::Instance()->CreateVegetationBlocks(rect, config.xSectionCount, config.zSectionCount);
	}
}

MVegetation * xTerrainVegDlg::GetCurVeg()
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_TV_List);

	int curSel = list->GetCurSel();

	if (curSel == -1)
		return NULL;

	return MForest::Instance()->GetVegetation(curSel);
}

bool xTerrainVegDlg::IsAddMode()
{
	CButton * btn = (CButton *)GetDlgItem(IDC_TV_AddVeg);

	return btn->GetCheck() == BST_CHECKED;
}

float xTerrainVegDlg::BrushSize()
{
	CString str;

	GetDlgItemText(IDC_TV_BrushSize, str);

	return (float)atof((LPCTSTR)str);
}

int xTerrainVegDlg::BrushDensity()
{
	CString str;

	GetDlgItemText(IDC_TV_BrushDensity, str);

	return atoi((LPCTSTR)str);
}

float xTerrainVegDlg::MinSize()
{
	CString str;

	GetDlgItemText(IDC_TV_MinSize, str);

	return (float)atof((LPCTSTR)str);
}

float xTerrainVegDlg::MaxSize()
{
	CString str;

	GetDlgItemText(IDC_TV_MaxSize, str);

	return (float)atof((LPCTSTR)str);
}

float xTerrainVegDlg::MinLum()
{
	CString str;

	GetDlgItemText(IDC_TV_MinLum, str);

	return (float)atof((LPCTSTR)str);
}

float xTerrainVegDlg::MaxLum()
{
	CString str;

	GetDlgItemText(IDC_TV_MaxLum, str);

	return (float)atof((LPCTSTR)str);
}

void xTerrainVegDlg::OnAdd()
{
	afx_resource_setup();

	xVegetationDlg dlg(NULL);

	if (dlg.DoModal() == IDOK)
	{
		MForest::Instance()->AddVegetation(dlg.Name(), dlg.Type(), dlg.MeshFile(),
			dlg.DiffuseMap(), dlg.NormalMap(), dlg.SpecularMap());

		_refrushList();
	}
}

void xTerrainVegDlg::OnEdit()
{
	MVegetation * veg = GetCurVeg();

	if (veg)
	{
		afx_resource_setup();

		xVegetationDlg dlg(veg);

		if (dlg.DoModal() == IDOK)
		{
			veg->Type = dlg.Type();

			if (dlg.MeshFile() != "")
				veg->pMesh = MeshManager::Instance()->Load(dlg.MeshFile(), dlg.MeshFile());
			else
				veg->pMesh = NULL;

			veg->DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(dlg.DiffuseMap(), dlg.DiffuseMap());
			
			if (dlg.NormalMap() != "")
				veg->NormalMap = VideoBufferManager::Instance()->Load2DTexture(dlg.NormalMap(), dlg.NormalMap());
			else
				veg->NormalMap = NULL;

			if (dlg.SpecularMap() != "")
				veg->SpecularMap = VideoBufferManager::Instance()->Load2DTexture(dlg.SpecularMap(), dlg.SpecularMap());
			else
				veg->SpecularMap = NULL;

			MForest::Instance()->OnVegetationChanged(veg);
		}
	}
}

void xTerrainVegDlg::OnRemove()
{
	MVegetation * veg = GetCurVeg();

	if (veg && MessageBox("Need Delete Vegetation ?", "Warning", MB_OKCANCEL) == IDOK)
	{
		MForest::Instance()->RemoveVegetation(veg);
		_refrushList();
	}
}

void xTerrainVegDlg::_refrushList()
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_TV_List);

	while (list->GetCount())
		list->DeleteString(0);

	for (int i = 0; i < MForest::Instance()->GetVegetationCount(); ++i)
	{
		MVegetation * veg = MForest::Instance()->GetVegetation(i);

		list->InsertString(i, veg->Name.c_str());
	}
}
