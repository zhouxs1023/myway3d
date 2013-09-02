#pragma once

#include "Actor.h"

class User;

class svPlayer : public svActor
{
public:
	svPlayer(int _actorId, User * _user);
	virtual ~svPlayer();

	virtual int GetActorType() { return ACTOR_Player; }

	void SetName(const TString32 & _name) { mName = _name; }
	const TString32 & GetName() { return mName; }

	void SetSex(int _sex) { mSex = _sex; }
	int GetSex() { return mSex; }
	
	void SetRace(int _race) { mRace = _race; }
	int GetRace() { return mRace; }

	void SetHP(int _hp) { mHP = _hp; }
	int GetHP() { return mHP; }

	void SetMP(int _mp) { mMP = _mp; }
	int GetMP() { return mMP; }

	bool IsActorVisible(svActor * _actor);
	void AddVisibleActor(svActor * _actor);
	void RemoveVisibleActor(svActor * _actor);

	void SetVisibleRange(float _range) { mVisibleRange = _range; }
	float GetVisibleRange() { return mVisibleRange; }

protected:
	User * mUser;

	TString32 mName;
	int mSex;
	int mRace;

	int mHP;
	int mMP;

	int mMaxHP;
	int mMaxMP;

	float mVisibleRange;
	Array<svActor *> mVisibleActors;
};