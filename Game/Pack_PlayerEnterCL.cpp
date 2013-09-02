#include "stdafx.h"

#include "Pack_PlayerEnterCL.h"

Pack_PlayerEnterCL::Pack_PlayerEnterCL()
{
	mPlayerId = -1;
}

Pack_PlayerEnterCL::~Pack_PlayerEnterCL()
{
}

void Pack_PlayerEnterCL::Out(PkOStream & stream)
{
	stream.Write(&mPlayerId, sizeof(int));
}