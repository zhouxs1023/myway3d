#include "stdafx.h"

#include "GameMessage.h"


IGameMessage::IGameMessage(int id)
	: mId(id)
	, mSender(-1)
	, mTimeDelay(0)
{
}

IGameMessage::~IGameMessage()
{
}

void IGameMessage::SetTimeDelay(float time)
{
	mTimeDelay = time;
}

float IGameMessage::GetTimeDelay()
{
	return mTimeDelay;
}

void IGameMessage::SetSender(int id)
{
	mSender = id;
}

int IGameMessage::GetSender()
{
	return mSender;
}

void IGameMessage::AddReceiver(int id)
{
	d_assert (id != -1);

	if (!HasReceiver(id))
		mReceivers.PushBack(id);
}

int IGameMessage::GetReceiverCount()
{
	return mReceivers.Size();
}

int IGameMessage::GetReceiver(int index)
{
	return mReceivers[index];
}

bool IGameMessage::HasReceiver(int id)
{
	for (int i = 0; i < mReceivers.Size(); ++i)
	{
		if (mReceivers[i] == id)
			return true;
	}

	return false;
}
