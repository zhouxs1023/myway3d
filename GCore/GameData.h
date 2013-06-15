#pragma once

#include "GameStructer.h"

class GCORE_ENTRY GameDataManager
{
	DECLARE_SINGLETON(GameDataManager);

public:
	GameDataManager();
	~GameDataManager();

	void Init();
	void Shutdown();

	const GameNpcInfo * GetNpcInfo(int id);

protected:
	void _loadNpcInfo();

protected:
	Array<GameNpcInfo> mNpcInfos;
};

