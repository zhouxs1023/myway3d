#include "stdafx.h"

#include "GameMessage.h"


IGameMessage::IGameMessage(int id)
	: mId(id)
	, mSenderId(-1)
	, mReceiverId(-1)
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

void IGameMessage::SetSenderId(int id)
{
	mSenderId = id;
}

int IGameMessage::GetSenderId()
{
	return mSenderId;
}

void IGameMessage::SetReceiverId(int id)
{
	mReceiverId = id;
}

int IGameMessage::GetReceiverId()
{
	return mReceiverId;
}