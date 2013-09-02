#pragma once

#include "NetPack.h"

class Pack_PlayerEnterCL : public NetPacket
{
public:
	Pack_PlayerEnterCL();
	virtual ~Pack_PlayerEnterCL() {}

	virtual short GetPacketId() { return NPI_PLAYER_ENTER_CL; }

	virtual void In(PkIStream & stream);

	virtual void Execute(void * param);

protected:
	int mPlayerId;
};