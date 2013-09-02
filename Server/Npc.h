#pragma once

#include "Actor.h"

class svNpc : public svActor
{
public:
	svNpc(int _actorId);
	virtual ~svNpc();

	virtual int GetActorType() { return ACTOR_Npc; }

	void SetTypeId(int _typeId) { mTypeId = _typeId; }
	int GetTypeId() { return mTypeId; }

	void SetHP(int _hp) { mHP = _hp; }
	int GetHP() { return mHP; }

	void SetMP(int _mp) { mMP = _mp; }
	int GetMP() { return mMP; }

protected:
	int mTypeId;

	int mHP;
	int mMP;
};