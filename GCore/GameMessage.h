#pragma once

#include "GameCoreEntry.h"

class IGameObject;

class GCORE_ENTRY IGameMessage 
{
public:
	IGameMessage(int id);
	virtual ~IGameMessage();

	int GetId() const { return mId; }

	void SetTimeDelay(float time);
	float GetTimeDelay();
	
	void SetSenderId(int id);
	int GetSenderId();

	void SetReceiverId(int id);
	int GetReceiverId();

	virtual void Do() = 0;

protected:
	float mTimeDelay;

	int mId;

	int mSenderId;
	int mReceiverId;
};