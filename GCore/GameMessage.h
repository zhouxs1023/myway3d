#pragma once

#include "GameCoreEntry.h"

class GmObj;

class GCORE_ENTRY GmMsg 
{
public:
	GmMsg(int id);
	virtual ~GmMsg();

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