#include "stdafx.h"
#include "resource.h"

#include "xTerrainVegDlg.h"
#include "MForest.h"

BEGIN_MESSAGE_MAP(xTerrainVegDlg, CDialog)
END_MESSAGE_MAP()

xTerrainVegDlg::xTerrainVegDlg()
{
}

xTerrainVegDlg::~xTerrainVegDlg()
{
}

void xTerrainVegDlg::_Init(void * param0, void * param1)
{
	SetDlgItemText(IDC_TV_BrushSize, "50");
	SetDlgItemText(IDC_TV_BrushDensity, "0.5");
}

void xTerrainVegDlg::_UnloadScene(void * param0, void * param1)
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_TV_List);

	while (list->GetCount())
		list->DeleteString(0);

	MForest::Instance()->RemoveAllVegetationBlock();
}

void xTerrainVegDlg::_AfterLoadScene(void * param0, void * param1)
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_TV_List);

	for (int i = 0; i < MForest::Instance()->GetVegetationCount(); ++i)
	{
		MVegetation * veg = MForest::Instance()->GetVegetation(i);

		list->InsertString(i, veg->Name.c_str());
	}

	if (Environment::Instance()->GetTerrain())
	{
		Terrain * terrain = Environment::Instance()->GetTerrain();

		const Terrain::Config & config = terrain->GetConfig();
		RectF rect = RectF(0, 0, config.xSize, config.zSize);
		
		MForest::Instance()->CreateVegetationBlocks(rect, config.xSectionCount, config.zSectionCount);
	}
}