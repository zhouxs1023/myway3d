#include "StdAfx.h"

#include "Pack_LoginSV.h"
#include "Game.h"
#include "SelectRoleMode.h"

Pack_LoginSV::Pack_LoginSV()
{
	mResult = eLoginFail;
}

Pack_LoginSV::~Pack_LoginSV()
{
}

void Pack_LoginSV::In(PkIStream & stream)
{
	stream.Read(&mResult, sizeof(int));
}

void Pack_LoginSV::Execute(void * param)
{
	GmRoot::Instance()->SetMode(new cSelectRoleMode);
}