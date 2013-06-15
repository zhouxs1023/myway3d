#pragma once

#include "GameActor.h"
#include "GameData.h"
#include "GameHelper.h"

class GCORE_ENTRY GameNpc : public GameActor
{
	DECLARE_ALLOC();
	DeclareRTTI();

public:
	GameNpc(int templateId);
	virtual ~GameNpc();

protected:
	void _init(int templateId);

protected:
	const GameNpcInfo * mNpcInfo;
};

