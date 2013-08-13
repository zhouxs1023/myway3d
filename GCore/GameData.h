#pragma once

#include "GameStructer.h"

class GCORE_ENTRY GmDataManager
{
	DECLARE_SINGLETON(GmDataManager);

public:
	GmDataManager();
	~GmDataManager();

	void Init();
	void Shutdown();

	const GmNpcInfo * GetNpcInfo(int id);

protected:
	void _loadNpcInfo();

protected:
	Array<GmNpcInfo> mNpcInfos;
};

