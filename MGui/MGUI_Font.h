#pragma once

#include "MGUI_Types.h"

namespace Myway {

	struct MGUI_Glyph
	{
		MGUI_RectF uv;
		float aspect;

		MGUI_Glyph() : aspect(1) {}
	};

	class MGUI_ENTRY MGUI_Font
	{
		struct MGUI_CharRange
		{
			MGUI_Char low_ch;
			MGUI_Char high_ch;
		};

		DECLARE_SINGLETON (MGUI_Font);

	public:
		static const MGUI_Char kSelector = 0xFFFFFFFE;
		static const MGUI_Char kSelectorBack = 0xFFFFFFFF;

	public:
		MGUI_Font();
		~MGUI_Font();

		bool Load(const char * source);
		void Unload();

		int GetResolution() { return mResolution; }
		int GetCharSize() { return mCharSize; }

		TexturePtr GetTexture() { return mTexture; }

		const MGUI_Glyph * GetGlyph(MGUI_Char _char);

	protected:
		void LoadFromXml(const char * source);

	protected:
		TString128 mFontSource;

		int mResolution;
		int mCharSize;

		Array<MGUI_CharRange> mCharRanges;
		TexturePtr mTexture;

		HashMap<MGUI_Char, MGUI_Glyph> mGlyphMap;
	};


}

