#pragma once

#include "NetPack.h"

class Pack_AddNpcSV : public NetPacket
{
public:
	Pack_AddNpcSV();
	virtual ~Pack_AddNpcSV();

	virtual short GetPacketId() { return NPI_LOGIN_SV; }

	virtual void In(PkIStream & stream);

	virtual void Execute(void * param);

protected:
	int mNpcId;
	int mTypeId;

	int mHP;
	int mMP;

	Vec3 mPosition;
	float mDirection;

	int mState;
};