#pragma once

#include "NetPack.h"
#include "GameDefine.h"

class Pack_AddNpcSV : public NetPacket
{
public:
	Pack_AddNpcSV() {}
	virtual ~Pack_AddNpcSV() {}

	virtual short GetPacketId() { return NPI_ADD_NPC_SV; }

	virtual void Out(PkOStream & stream);

	void SetViewInfo(const NpcViewInfo & _info) { mViewInfo = _info; }

protected:
	NpcViewInfo mViewInfo;
};