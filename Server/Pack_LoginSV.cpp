#include "StdAfx.h"

#include "Pack_LoginSV.h"


Pack_LoginSV::Pack_LoginSV()
{
	mResult = eLoginFail;
}

Pack_LoginSV::~Pack_LoginSV()
{
}

void Pack_LoginSV::Out(PkOStream & stream)
{
	stream.Write(&mResult, sizeof(int));
}

void Pack_LoginSV::SetResult(int x)
{
	mResult = x;
}

int Pack_LoginSV::GetResult() const
{
	return mResult;
}