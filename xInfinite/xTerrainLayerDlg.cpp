#include "stdafx.h"

#include "xTerrainLayerDlg.h"
#include "xTerrainLayerInfoDlg.h"
#include "xTerrainPane.h"
#include "xApp.h"
#include "resource.h"


//////////////////////////////////////////////////////////////////////////
//
// Terrain Layer
//

BEGIN_MESSAGE_MAP(xTerrainLayerDlg, CDialog)
	
	ON_BN_CLICKED(IDC_Button_TL_AddLayer, OnAddLayer)
	ON_BN_CLICKED(IDC_Button_TL_RemoveLayer, OnRemoveLayer)
	ON_BN_CLICKED(IDC_Button_TL_EditLayer, OnEditLayer)

	ON_CBN_SELCHANGE(IDC_Combo_TL_BrushType, OnTypeChanged)

END_MESSAGE_MAP()

xTerrainLayerDlg::xTerrainLayerDlg()
{
}

xTerrainLayerDlg::~xTerrainLayerDlg()
{
}

BOOL xTerrainLayerDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	return TRUE;
}

int xTerrainLayerDlg::GetCurLayer()
{
	CListBox * list = (CListBox *)GetDlgItem(IDC_List_TL_Layers);

	int isel = list->GetCurSel();

	if (isel != -1)
		return mLayerIds[isel];

	return -1;
}

void xTerrainLayerDlg::_Init(void *)
{
	CComboBox * cbBrushType = (CComboBox *)GetDlgItem(IDC_Combo_TL_BrushType);

	FileSystem fs("../Core/Terrain/Brushes");

	fs.Load();

	Archive::FileInfoVisitor v = fs.GetFileInfos();

	while (!v.Endof())
	{
		const Archive::FileInfo & info = v.Cursor()->second;

		if (info.type == Archive::FILE_ARCHIVE)
		{
			TString128 base = File::RemoveExternName(info.name);
			TString128 ext = File::GetExternName(info.name);

			if (ext == "png")
				cbBrushType->AddString(base.c_str());
		}

		++v;
	}

	cbBrushType->SetCurSel(0);

	OnTypeChanged();

	CSliderCtrl * sdSize = (CSliderCtrl *)GetDlgItem(IDC_Slider_TL_BrushSize);
	CSliderCtrl * sdDensity = (CSliderCtrl *)GetDlgItem(IDC_Slider_TL_BrushDensity);

	sdSize->SetRange(xSliderMin, xSliderMax);
	sdDensity->SetRange(xSliderMin, xSliderMax);

	sdSize->SetPos(50);
	sdDensity->SetPos(50);

	// init layer list
	Terrain * terrain = Environment::Instance()->GetTerrain();
	CListBox * list = (CListBox *)GetDlgItem(IDC_List_TL_Layers);

	if (terrain)
	{
		int index = 0;
		for (int i = 0; i < Terrain::kMaxLayers; ++i)
		{
			const Terrain::Layer * layer = terrain->GetLayer(i);

			if (layer->detail != "")
			{
				list->InsertString(index, layer->detail.c_str());
				mLayerIds.PushBack(i);
			}
		}
	}
}

void xTerrainLayerDlg::OnTypeChanged()
{
	CString str;

	GetDlgItemText(IDC_Combo_TL_BrushType, str);

	xEditTerrainLayer * edit = xTerrainPane::Instance()->GetTerrainLayer();

	str = "brushes\\" + str + ".png";

	edit->SetBrush((LPCTSTR)str);
}

void xTerrainLayerDlg::OnAddLayer()
{
	Terrain * tn = Environment::Instance()->GetTerrain();

	if (tn == NULL)
		return ;

	xTerrainLayerInfoDlg dlg(NULL);

	if (dlg.DoModal() == IDOK)
	{
		Terrain::Layer layer;

		layer.detail = dlg.GetDetailMap();
		layer.normal = dlg.GetNormalMap();
		layer.specular = dlg.GetSpecularMap();
		layer.scale = dlg.GetUVScale();
		layer.material = -1;

		int id = tn->AddLayer(layer);

		CListBox * list = (CListBox *)GetDlgItem(IDC_List_TL_Layers);

		list->InsertString(list->GetCount(), layer.detail.c_str());

		mLayerIds.PushBack(id);
	}
}

void xTerrainLayerDlg::OnRemoveLayer()
{
	Terrain * tn = Environment::Instance()->GetTerrain();

	if (tn == NULL)
		return ;
}

void xTerrainLayerDlg::OnEditLayer()
{
	Terrain * tn = Environment::Instance()->GetTerrain();

	if (tn == NULL)
		return ;

	CListBox * list = (CListBox *)GetDlgItem(IDC_List_TL_Layers);

	int isel = list->GetCurSel();

	if (isel != -1)
	{
		int layerId = mLayerIds[isel];
		const Terrain::Layer * layer = tn->GetLayer(layerId);

		xTerrainLayerInfoDlg dlg(layer);

		if (dlg.DoModal() == IDOK)
		{
			Terrain::Layer layer;

			layer.detail = dlg.GetDetailMap();
			layer.normal = dlg.GetNormalMap();
			layer.specular = dlg.GetSpecularMap();
			layer.scale = dlg.GetUVScale();
			layer.material = -1;

			tn->SetLayer(layerId, layer);
		}
	}
}