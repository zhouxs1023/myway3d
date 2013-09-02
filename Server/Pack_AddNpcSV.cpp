#include "stdafx.h"

#include "Pack_AddNpcSV.h"


void Pack_AddNpcSV::Out(PkOStream & stream)
{
	stream.Write(&mViewInfo, sizeof(NpcViewInfo));
}