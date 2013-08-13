#pragma once

#include "GameCore.h"

class EditorGameMode : public GmMode
{
	DECLARE_SINGLETON(EditorGameMode);

public:
	EditorGameMode();
	virtual ~EditorGameMode();

	void Init();
	void Shutdown();

protected:
	GmDataManager mDataManager;
};