#pragma once

#include "NetPack.h"

class Pack_LoginCL : public NetPacket
{
public:
	Pack_LoginCL();
	virtual ~Pack_LoginCL();

	virtual short GetPacketId() { return NPI_LOGIN_CL; }

	virtual void Out(PkOStream & stream);

	void SetAccount(const char * account);
	void SetPassword(const char * password);

protected:
	char mAccount[MAX_ACCOUNT_SIZE];
	char mPassword[MAX_PASSWORD_SIZE];
};


