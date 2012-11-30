#include "stdafx.h"

#include "xTerrain.h"
#include "xTerrainCreateDlg.h"

xTerrain::xTerrain(const Terrain::Config & config)
	: xObj("Terrain")
{
	Environment::Instance()->CreateTerrain(config);
	mTerrain = Environment::Instance()->GetTerrain();
}

xTerrain::~xTerrain()
{
}

bool xTerrain::OnPropertyChanged(const Property * p)
{
	return false;
}





xObj * xTerrainFactory::Create(const char * name)
{
	if (Environment::Instance()->GetTerrain())
	{
		MessageBox(NULL, "Terrain has created!", "Error", MB_OK);

		return NULL;
	}

	xTerrainCreateDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		Terrain::Config config;

		config.xSize = dlg.GetSizeX();
		config.zSize = dlg.GetSizeZ();

		config.xVertexCount = dlg.GetVertX();
		config.zVertexCount = dlg.GetVertZ();

		return new xTerrain(config);
	}

	return NULL;
}




xTerrainFactoryListener gListener;