#pragma once

#include "MWApp_Win32.h"
#include "GameMode.h"
#include "GameData.h"
#include "GUI_Util.h"


class GCORE_ENTRY GmRoot
{
	DECLARE_SINGLETON(GmRoot);

public:
	GmRoot();
	~GmRoot();

	void Init();
	void Shutdown();
	void Update();

	void SetMode(GmMode * mode);
	GmMode * GetMode();

	ShaderLib * GetShaderLib() { return mShaderLib; }

protected:
	ShaderLib * mShaderLib;
	MGUI_Util mUIUtil;
	GmDataManager mDataManager;
	GmMode * mCurrentMode;
};

