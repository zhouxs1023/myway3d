#pragma once

#include "MGUI_Types.h"

namespace Myway {

	class MGUI_ENTRY MGUI_SkinManager
	{
		DECLARE_SINGLETON(MGUI_SkinManager);

	public:
		MGUI_SkinManager();
		~MGUI_SkinManager();

		void Load(const char * _source);
		void Unload();

		int GetSkinCount() { return mSkins.Size(); }
		TexturePtr GetSkin(int _index) { return mSkins[_index]; }

	protected:
		Array<TexturePtr> mSkins;
	};

}