#include "stdafx.h"

#include "Npc.h"

svNpc::svNpc(int _actorId)
	: svActor(_actorId) 
	, mTypeId(-1)
	, mHP(1)
	, mMP(1)
{
}

svNpc::~svNpc()
{
}