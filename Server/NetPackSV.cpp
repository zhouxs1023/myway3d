#include "StdAfx.h"

#include "NetPack.h"
#include "Pack_LoginCL.h"
#include "Pack_LoginSV.h"
#include "Pack_PlayerEnterCL.h"

NetPacketManager _mgr;

void RegistNetPacket()
{
	_mgr.AddPacket(new Pack_LoginCL);
	_mgr.AddPacket(new Pack_PlayerEnterCL);
}