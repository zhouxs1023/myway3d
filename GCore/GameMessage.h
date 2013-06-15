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
	
	void SetSender(int id);
	int GetSender();

	void AddReceiver(int id);
	int GetReceiverCount();
	int GetReceiver(int index);
	bool HasReceiver(int id);

	virtual void Do() = 0;

protected:
	int mId;
	int mSender;
	float mTimeDelay;
	Array<int> mReceivers;
};