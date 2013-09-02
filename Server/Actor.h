#pragma once

#include "GameDefine.h"

class svActor
{
public:
	svActor(int _actorId);
	virtual ~svActor();

	virtual int GetActorType() = 0;

	int GetActorId() { return mActorId; };

	void SetPosition(float x, float y, float z);
	void SetPosition(const Vec3 & position);
	const Vec3 & GetPosition() { return mPosition; }

	void SetDirection(float _rot);
	float GetDirection() { return mDirection; }

	void SetMapId(int _mapId) { mMapId = _mapId; } 
	int GetMapId() { return mMapId; }

	virtual void Update() {};

protected:
	int mActorId;
	int mMapId;

	Vec3 mPosition;
	float mDirection;
};