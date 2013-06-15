#include "stdafx.h"

#include "EditorGameMode.h"

	IMP_SLN(EditorGameMode);

	EditorGameMode::EditorGameMode()
	{
		INIT_SLN;
	}

	EditorGameMode::~EditorGameMode()
	{
		SHUT_SLN;
	}

	void EditorGameMode::Init()
	{
		mDataManager.Init();
	}

	void EditorGameMode::Shutdown()
	{
		mDataManager.Shutdown();
	}