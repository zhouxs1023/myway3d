#pragma once

#include "Actor.h"
#include "Player.h"


class svMap
{
public:
	tEvent1<svActor *> eventActorEnter;
	tEvent1<svActor *> eventActorLeave;

	tEvent0 eventLoaded;
	tEvent0 eventUpdate;

public:
	svMap(int _mapId);
	~svMap();

	void Load(const TString128 & _source);

	int GetMapId() { return mMapId; }

	void Enter(svActor * actor);
	void Leave(svActor * actor);

	int GetActorCount();
	svActor * GetActorByIndex(int _index);
	svActor * GetActorById(int _id);

	void Update();

protected:
	void _updatePlayerVisibleActor(svPlayer * _player);

protected:
	int mMapId;
	Vec3 mSize;

	Array<svActor *> mActors;
	Array<svPlayer *> mPlayers;
};