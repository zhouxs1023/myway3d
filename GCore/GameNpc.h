#pragma once

#include "GameActor.h"
#include "GameData.h"
#include "GameHelper.h"

class GCORE_ENTRY GmNpc : public GmActor
{
	DECLARE_ALLOC();
	DeclareRTTI();

public:
	GmNpc(int templateId);
	virtual ~GmNpc();

protected:
	void _init(int templateId);

protected:
	const GmNpcInfo * mNpcInfo;
};

