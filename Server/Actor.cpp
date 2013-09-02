#include "stdafx.h"

#include "Actor.h"

svActor::svActor(int _actorId)
	: mActorId(_actorId)
	, mPosition(0, 0, 0)
	, mDirection(0)
	, mMapId(-1)
{
}

svActor::~svActor()
{
}

void svActor::SetPosition(float x, float y, float z)
{
	SetPosition(Vec3(x, y, z));
}

void svActor::SetPosition(const Vec3 & position)
{
	mPosition = position;
}

void svActor::SetDirection(float _rot)
{
	mDirection = _rot;
}
