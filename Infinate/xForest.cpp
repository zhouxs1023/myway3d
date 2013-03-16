#include "stdafx.h"
#include "xForest.h"
#include "xEvent.h"
#include "xSerializer.h"
#include "xScene.h"
#include "MForest.h"

namespace Infinite {

	xForest::xForest()
		: OnNewScene(xEvent::OnNewScene, this, &xForest::_OnNewScene)
		, OnLoadScene(xEvent::OnLoadScene, this, &xForest::_OnLoadScene)
		, OnUnloadScene(xEvent::OnUnloadScene, this, &xForest::_OnUnloadScene)
		, OnSerialize(xEvent::OnSerialize, this, &xForest::_OnSerialize)
	{
	}

	xForest::~xForest()
	{
	}

	void xForest::_OnNewScene(Event * sender)
	{
	}

	void xForest::_OnLoadScene(Event * sender)
	{
	}

	void xForest::_OnUnloadScene(Event * sender)
	{
	}

	void xForest::_OnSerialize(Event * sender)
	{
		int K_ChunkId = 'xFrt';
		int K_Version = 0;

		int chunkId = *(int *)sender->GetParam(0);
		xSerializer & Serializer = *(xSerializer *)sender->GetParam(1);

		if (Serializer.IsSave())
		{
			TString128 sceneFile = xScene::Instance()->GetFileName();
			sceneFile = File::RemoveExternName(sceneFile);
			sceneFile += ".vegetation";
			sceneFile = xScene::Instance()->GetFloder() + "\\" + sceneFile;

			Serializer << K_ChunkId;
			Serializer << K_Version;

			MForest::Instance()->SaveVeg(sceneFile);
		}
		else if (K_ChunkId == chunkId)
		{
			int version;
			Serializer >> version;

			TString128 sceneFile = xScene::Instance()->GetFileName();
			sceneFile = File::RemoveExternName(sceneFile);
			sceneFile += ".vegetation";

			MForest::Instance()->LoadVeg(sceneFile);

			xScene::Instance()->DirtLoadChunk();
		}
	}


}