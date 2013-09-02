#include "StdAfx.h"

#include "NetPack.h"
#include "Pack_LoginCL.h"
#include "Pack_LoginSV.h"

NetPacketManager _mgr;

void RegistNetPacket()
{
	_mgr.AddPacket(new Pack_LoginSV);
}