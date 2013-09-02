#include "StdAfx.h"

#include "Pack_LoginCL.h"
#include "Pack_LoginSV.h"

Pack_LoginCL::Pack_LoginCL()
{
	mAccount[0] = 0;
	mPassword[0] = 0;
}

Pack_LoginCL::~Pack_LoginCL()
{
}

void Pack_LoginCL::Out(PkOStream & stream)
{
	stream.Write(mAccount, MAX_ACCOUNT_SIZE);
	stream.Write(mPassword, MAX_ACCOUNT_SIZE);
}

void Pack_LoginCL::SetAccount(const char * account)
{
	Strcpy(mAccount, MAX_ACCOUNT_SIZE, account);
}

void Pack_LoginCL::SetPassword(const char * password)
{
	Strcpy(mPassword, MAX_ACCOUNT_SIZE, password);
}