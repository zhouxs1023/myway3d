#pragma once

#include "NetPack.h"

class Pack_PlayerEnterCL : public NetPacket
{
public:
	Pack_PlayerEnterCL();
	virtual ~Pack_PlayerEnterCL();

	virtual short GetPacketId() { return NPI_PLAYER_ENTER_CL; }

	virtual void Out(PkOStream & stream);

	void SetPlayerId(int _id) { mPlayerId = _id; }
	int GetPlayerId() { return mPlayerId; }

protected:
	int mPlayerId;
};