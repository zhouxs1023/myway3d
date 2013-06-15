#pragma once

#include "GameCore.h"

class EditorGameMode : public IGameMode
{
	DECLARE_SINGLETON(EditorGameMode);

public:
	EditorGameMode();
	virtual ~EditorGameMode();

	void Init();
	void Shutdown();

protected:
	GameDataManager mDataManager;
};