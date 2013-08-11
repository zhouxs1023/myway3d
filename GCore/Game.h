#pragma once

#include "MWApp_Win32.h"
#include "GameMode.h"
#include "GUI_Util.h"

namespace Game {

	class GCORE_ENTRY Game
	{
		DECLARE_SINGLETON(Game);

	public:
		Game();
		~Game();

		void Init();
		void Shutdown();
		void Update();

		void SetMode(IGameMode * mode);
		IGameMode * GetMode();

		ShaderLib * GetShaderLib() { return mShaderLib; }

	protected:
		ShaderLib * mShaderLib;
		MGUI_Util mUIUtil;

		IGameMode * mCurrentMode;
	};

}