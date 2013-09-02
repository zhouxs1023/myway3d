#pragma once

#include "NetPack.h"

class Pack_LoginSV : public NetPacket
{
public:
	Pack_LoginSV();
	virtual ~Pack_LoginSV();

	virtual short GetPacketId() { return NPI_LOGIN_SV; }

	virtual void In(PkIStream & stream);

	virtual void Execute(void * param);

protected:
	int mResult;
};
