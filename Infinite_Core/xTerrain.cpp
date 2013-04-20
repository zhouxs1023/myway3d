#include "stdafx.h"

#include "xTerrain.h"
#include "xScene.h"

namespace Infinite {


xTerrain::xTerrain()
	: Shape("Terrain")
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

	World::Instance()->Resize(config.xSize, 1024, config.zSize);

	Environment::Instance()->GetWaterManager()->GetWater()->Init();
}

void xTerrain::_load(const char * source)
{
	d_assert (mTerrain == NULL);

	Environment::Instance()->LoadTerrain(source);
	mTerrain = Environment::Instance()->GetTerrain();

	World::Instance()->Resize(mTerrain->GetConfig().xSize, 2048, mTerrain->GetConfig().zSize);

	Environment::Instance()->GetWaterManager()->GetWater()->Init();
}

void xTerrain::Serialize(xSerializer & Serializer)
{
	Shape::Serialize(Serializer);

	int version = 0;

	if (Serializer.IsSave())
	{
		Serializer << version;

		TString128 sceneFile = xScene::Instance()->GetFileName();
		sceneFile = File::RemoveExternName(sceneFile);
		sceneFile += ".terrain";

		sceneFile = xScene::Instance()->GetFloder() + "\\" + sceneFile;

		mTerrain->Save(sceneFile.c_str());

		// water
		TString128 waterFile = xScene::Instance()->GetFileName();
		waterFile = File::RemoveExternName(waterFile);
		waterFile += ".water";
		waterFile = xScene::Instance()->GetFloder() + "\\" + waterFile;

		WaterManager::Instance()->GetWater()->Save(waterFile.c_str());
	}
	else
	{
		Serializer >> version;

		TString128 sceneFile = xScene::Instance()->GetFileName();
		sceneFile = File::RemoveExternName(sceneFile);
		sceneFile += ".terrain";

		_load(sceneFile.c_str());

		// water
		TString128 waterFile = sceneFile;
		waterFile = File::RemoveExternName(waterFile);
		waterFile += ".water";

		WaterManager::Instance()->GetWater()->Load(waterFile.c_str());
	}
}

bool xTerrain::OnPropertyChanged(const Property * p)
{
	return false;
}



xTerrainFactory::xTerrainFactory()
	: OnOK(this, &xTerrainFactory::_OnOK)
{
	mTerrainCreateDlg = NULL;
}

xTerrainFactory::~xTerrainFactory()
{
	if (mTerrainCreateDlg)
		mTerrainCreateDlg->Event_OnOK -= &OnOK;

	safe_delete (mTerrainCreateDlg);
}

Shape * xTerrainFactory::Create(const char * name)
{
	if (Environment::Instance()->GetTerrain())
	{
		HWND hWnd = Engine::Instance()->GetDeviceProperty()->hWnd;

		MessageBox(hWnd, "Terrain has created!", "Error", MB_OK);

		return NULL;
	}


	if (xScene::Instance()->IsLoading())
	{
		xTerrain * obj = new xTerrain();
		return obj;
	}
	else
	{
		if (mTerrainCreateDlg == NULL)
		{
			mTerrainCreateDlg = new TerrainCreateDlg;

			mTerrainCreateDlg->Event_OnOK += &OnOK;
		}

		mTerrainCreateDlg->DoModal();
	}
	
	return NULL;
}

void xTerrainFactory::_OnOK(Event * sender)
{
	Terrain::Config config;

	config.xSize = mTerrainCreateDlg->GetXSize();
	config.zSize = mTerrainCreateDlg->GetZSize();

	config.xVertexCount = mTerrainCreateDlg->GetXVertSize();
	config.zVertexCount = mTerrainCreateDlg->GetZVertSize();

	xTerrain * obj = new xTerrain();

	obj->_create(config);
}


}
