#pragma once

#include "Actor.h"
#include "Map.h"

class svWorld
{
	DECLARE_SINGLETON (svWorld);

public:
	svWorld();
	~svWorld();

	void Init();
	void Shutdown();

	void Update(float elapsedTime);

	// map
	void AddMap(svMap * _map);
	void RemoveAllMap();
	int GetMapCount();
	svMap * GetMapByIndex(int _index);
	svMap * GetMapById(int _mapId);

	// actor
	void AddActor(svActor * _actor);
	void RemoveActor(svActor * _actor);
	void RemoveActor(int _actorId);
	void RemoveAllActor();
	int GetActorCount();
	svActor * GetActorByIndex(int _index);
	svActor * GetActorById(int _actorId);

	float GetElapsedTime() { return mElapsedTime; }

protected:
	TString128 mDataDir;
	float mElapsedTime;
	Array<svActor *> mActors;
	Array<svMap *> mMaps;
};