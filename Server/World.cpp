#include "stdafx.h"

#include "World.h"

svWorld * svWorld::msInstance = NULL;

svWorld::svWorld()
{
	msInstance = this;

	mDataDir = "..\\svdata";
	mElapsedTime = 0;
}

svWorld::~svWorld()
{
	msInstance = NULL;
}

void svWorld::Init()
{
	// 初始化世界
	xml_doc doc;

	TString128 worldFile = mDataDir + "\\world\\game.world";

	char * data = XmlHelper::LoadXmlFromFile(doc, worldFile.c_str());

	d_assert (data != NULL);

	xml_node * root = doc.first_node("world");

	// load map
	xml_node * nodeMaps = root->first_node("maps");
	xml_node * nodeMap = nodeMaps->first_node("map");

	while (nodeMap != NULL)
	{
		int id = atoi(nodeMap->first_attribute("id")->value());
		const char * source = nodeMap->first_attribute("source")->value();

		svMap * mp = new svMap(id);

		mp->Load(mDataDir + "\\world\\" + source);

		AddMap(mp);

		nodeMap = nodeMap->next_sibling();
	}

	XmlHelper::XmlFree(data);
}

void svWorld::Shutdown()
{
	RemoveAllActor();
	RemoveAllMap();
}

void svWorld::Update(float elapsedTime)
{
	mElapsedTime = elapsedTime;

	for (int i = 0; i < mMaps.Size(); ++i)
	{
		mMaps[i]->Update();
	}
}

void svWorld::AddMap(svMap * _map)
{
	d_assert (GetMapById(_map->GetMapId()) == NULL);

	mMaps.PushBack(_map);
}

void svWorld::RemoveAllMap()
{
	for (int i = 0; i < mMaps.Size(); ++i)
		delete mMaps[i];

	mMaps.Clear();
}

int svWorld::GetMapCount()
{
	return mMaps.Size();
}

svMap * svWorld::GetMapByIndex(int _index)
{
	return mMaps[_index];
}

svMap * svWorld::GetMapById(int _mapId)
{
	for (int i = 0; i < mMaps.Size(); ++i)
	{
		if (mMaps[i]->GetMapId() == _mapId)
			return mMaps[i];
	}

	return NULL;
}


void svWorld::AddActor(svActor * _actor)
{
	d_assert (GetActorById(_actor->GetActorId()) == NULL);

	mActors.PushBack(_actor);
}

void svWorld::RemoveActor(svActor * _actor)
{
	RemoveActor(_actor->GetActorId());
}

void svWorld::RemoveActor(int _actorId)
{
	for (int i = 0; i < mActors.Size(); ++i)
	{
		if (mActors[i]->GetActorId() == _actorId)
		{
			delete mActors[i];
			mActors.Erase(i);
			return ;
		}
	}

	d_assert (0);
}

void svWorld::RemoveAllActor()
{
	for (int i = 0; i < mActors.Size(); ++i)
		delete mActors[i];

	mActors.Clear();
}

int svWorld::GetActorCount()
{
	return mActors.Size();
}

svActor * svWorld::GetActorById(int _actorId)
{
	for (int i = 0; i < mActors.Size(); ++i)
	{
		if (mActors[i]->GetActorId() == _actorId)
			return mActors[i];
	}

	return NULL;
}

svActor * svWorld::GetActorByIndex(int _index)
{
	return mActors[_index];
}