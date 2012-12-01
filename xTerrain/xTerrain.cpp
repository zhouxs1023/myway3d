#include "stdafx.h"

#include "xTerrain.h"
#include "xTerrainCreateDlg.h"
#include "xAfxResourceSetup.h"
#include "xScene.h"


xTerrain::xTerrain()
	: xObj("Terrain")
{
	mTerrain = NULL;
}

xTerrain::~xTerrain()
{
	Environment::Instance()->UnloadTerrain();
}

void xTerrain::_create(const Terrain::Config & config)
{
	d_assert (mTerrain == NULL);

	Environment::Instance()->CreateTerrain(config);
	mTerrain = Environment::Instance()->GetTerrain();
}

void xTerrain::_load(const char * source)
{
	d_assert (mTerrain == NULL);

	Environment::Instance()->LoadTerrain(source);
	mTerrain = Environment::Instance()->GetTerrain();
}

void xTerrain::Serialize(xSerializer & Serializer)
{
	xObj::Serialize(Serializer);

	int version = 0;

	if (Serializer.IsSave())
	{
		Serializer << version;

		TString128 sceneFile = xScene::Instance()->GetFileName();
		sceneFile = File::RemoveExternName(sceneFile);
		sceneFile += ".terrain";

		sceneFile = xScene::Instance()->GetFloder() + "\\" + sceneFile;

		mTerrain->Save(sceneFile.c_str());
	}
	else
	{
		Serializer >> version;

		TString128 sceneFile = xScene::Instance()->GetFileName();
		sceneFile = File::RemoveExternName(sceneFile);
		sceneFile += ".terrain";

		_load(sceneFile.c_str());
	}
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


	if (xScene::Instance()->IsLoading())
	{
		xTerrain * obj = new xTerrain();
		return obj;
	}
	else
	{
		afx_resource_setup();

		xTerrainCreateDlg dlg;

		if (dlg.DoModal() == IDOK)
		{
			Terrain::Config config;

			config.xSize = dlg.GetSizeX();
			config.zSize = dlg.GetSizeZ();

			config.xVertexCount = dlg.GetVertX();
			config.zVertexCount = dlg.GetVertZ();

			xTerrain * obj = new xTerrain();

			obj->_create(config);

			return obj;
		}
	}
	
	return NULL;
}




xTerrainFactoryListener gListener;