#include "stdafx.h"

#include "GameMessage.h"


GmMsg::GmMsg(int id)
	: mId(id)
	, mSenderId(-1)
	, mReceiverId(-1)
	, mTimeDelay(0)
{
}

GmMsg::~GmMsg()
{
}

void GmMsg::SetTimeDelay(float time)
{
	mTimeDelay = time;
}

float GmMsg::GetTimeDelay()
{
	return mTimeDelay;
}

void GmMsg::SetSenderId(int id)
{
	mSenderId = id;
}

int GmMsg::GetSenderId()
{
	return mSenderId;
}

void GmMsg::SetReceiverId(int id)
{
	mReceiverId = id;
}

int GmMsg::GetReceiverId()
{
	return mReceiverId;
}