#include "stdafx.h"

#include "Map.h"

svMap::svMap(int _mapId)
	: mMapId(_mapId)
	, mSize(0, 0, 0)
{
}

svMap::~svMap()
{
}

void svMap::Load(const TString128 & _source)
{
	xml_doc doc;

	char * data = XmlHelper::LoadXmlFromFile(doc, _source.c_str()); d_assert (data != NULL);

	{
		xml_node * root = doc.first_node("map");

		// µØÍ¼´óÐ¡
		xml_node * nodeSize = root->first_node("size"); d_assert(nodeSize != NULL);

		mSize.x = (float)atof(nodeSize->first_attribute("x")->value());
		mSize.y = (float)atof(nodeSize->first_attribute("y")->value());
		mSize.z = (float)atof(nodeSize->first_attribute("z")->value());
	}

	eventLoaded();
}

void svMap::Enter(svActor * actor)
{
	d_assert (actor->GetMapId() == -1);

	actor->SetMapId(mMapId);
	
	mActors.PushBack(actor);

	if (actor->GetType() == ACTOR_Player)
	{
		mPlayers.PushBack((svPlayer*)actor);
	}

	eventActorEnter(actor);
}

void svMap::Leave(svActor * actor)
{
	d_assert (actor->GetMapId() == mMapId);

	for (int i = 0; i < mActors.Size(); ++i)
	{
		if (mActors[i] == actor)
		{
			mActors.Erase(i);
			break;
		}
	}

	for (int i = 0; i < mPlayers.Size(); ++i)
	{
		if (mPlayers[i] == actor)
		{
			mPlayers.Erase(i);
			break;;
		}
	}

	actor->SetMapId(-1);

	eventActorLeave(actor);
}

int svMap::GetActorCount()
{
	return mActors.Size();
}

svActor * svMap::GetActorByIndex(int _index)
{
	return mActors[_index];
}

svActor * svMap::GetActorById(int _id)
{
	for (int i = 0; i < mActors.Size(); ++i)
	{
		if (mActors[i]->GetActorId() == _id)
			return mActors[i];
	}

	return NULL;
}

void svMap::Update()
{
	for (int i = 0; i < mActors.Size(); ++i)
	{
		mActors[i]->Update();
	}

	for (int i = 0; i < mPlayers.Size(); ++i)
	{
		_updatePlayerVisibleActor(mPlayers[i]);
	}

	eventUpdate();
}

void svMap::_updatePlayerVisibleActor(svPlayer * _player)
{
	float rangeSq = _player->GetVisibleRange() * _player->GetVisibleRange();

	for (int i = 0; i < mActors.Size(); ++i)
	{
		svActor * actor = mActors[i];

		if (actor == _player)
			continue ;

		float distSq = actor->GetPosition().DistanceSq(_player->GetPosition());

		if (distSq < rangeSq && !_player->IsActorVisible(actor))
		{
			_player->AddVisibleActor(actor);
		}
		else if (distSq >= rangeSq && _player->IsActorVisible(actor))
		{
			_player->RemoveVisibleActor(actor);
		}
	}
}