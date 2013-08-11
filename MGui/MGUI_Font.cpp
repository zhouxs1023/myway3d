#include "MGUI_Font.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

namespace Myway {

	MGUI_Font * MGUI_Font::msInstance = NULL;

	MGUI_Font::MGUI_Font()
	: mCharSize(24),
	  mResolution(72)
	{
		msInstance = this;
	}

	MGUI_Font::~MGUI_Font()
	{
		Unload();
		msInstance = NULL;
	}

	bool MGUI_Font::Load(const char * source)
	{
		Unload();

		FT_LibraryRec_ * ftLibrary = NULL;
		FT_FaceRec_ * ftFace = NULL;

		if (FT_Init_FreeType(&ftLibrary) != 0)
		{
			return false;
		}

		LoadFromXml(source);
		
		if (mFontSource == "" && mCharRanges.Empty())
			return false;

		//load face
		TString128 fontFile = "Font\\" + mFontSource;
		if (FT_New_Face(ftLibrary, fontFile.c_str(), 0, &ftFace) != 0)
			return false;

		//set char size
		if (FT_Set_Char_Size(ftFace, mCharSize << 6, 0, mResolution, 0) != 0)
			return false;

		int mMaxWidth = 0, mMaxHeight = 0;
		int mMaxHoriBearingY = 0;

		mGlyphMap.Insert(kSelector, MGUI_Glyph());
		mGlyphMap.Insert(kSelectorBack, MGUI_Glyph());

		for (int i = 0; i < mCharRanges.Size(); ++i)
		{
			const MGUI_CharRange & cr = mCharRanges[i];
			int horiBearingY = 0;
			int width = 0;
			int height = 0;

			for (MGUI_Char ch = cr.low_ch; ch <= cr.high_ch; ++ch)
			{
				if (FT_Load_Char(ftFace, ch, FT_LOAD_RENDER) != 0)
					continue ;

				horiBearingY = ftFace->glyph->metrics.horiBearingY >> 6;
				width = (ftFace->glyph->advance.x >> 6);
				height = ftFace->glyph->bitmap.rows + (ftFace->glyph->bitmap.rows - horiBearingY);

				mGlyphMap.Insert(ch, MGUI_Glyph());

				mMaxWidth = Math::Maximum(mMaxWidth, width);
				mMaxHeight = Math::Maximum(mMaxHeight, height);
				mMaxHoriBearingY = Math::Maximum(mMaxHoriBearingY, horiBearingY);
			}
		}

		d_assert(mMaxWidth != 0 && mMaxHeight != 0 && mMaxHoriBearingY != 0);

		int numPixels = (mMaxWidth) * (mMaxHeight + 1) * mGlyphMap.Size();

		int texWidth = 512, texHeight = 512;

		while (texWidth * texHeight < numPixels)
		{
			if (texWidth > texHeight)
				texHeight *= 2;
			else
				texWidth *= 2;
		}

		mTexture = VideoBufferManager::Instance()->CreateTexture(
			"MGUI_FontTexure", texWidth, texHeight, 1, FMT_A8L8);

		int r_pixel = 0;
		int c_pixel = 0;
		float invTexWidth = 1.0f / texWidth;
		float invTexHeight = 1.0f / texHeight;

		LockedBox lockBox;
		mTexture->Lock(0, &lockBox, NULL, LOCK_NORMAL);

		Memset16(lockBox.pData, 0x00FF, texWidth * texHeight);

		// selected char
		{
			int offsetX = mMaxWidth / 2 - 1;

			int dOffset = r_pixel * texWidth + (c_pixel + offsetX);
			unsigned short * dest = (unsigned short *)lockBox.pData + dOffset;

			MGUI_Glyph glyph;

			glyph.uv.x0 = c_pixel * invTexWidth;
			glyph.uv.y0 = r_pixel * invTexHeight;
			glyph.uv.x1 = (c_pixel + mMaxWidth) * invTexWidth;
			glyph.uv.y1 = (r_pixel + mMaxHeight) * invTexHeight;
			glyph.aspect = mMaxWidth / (float)mMaxHeight;

			mGlyphMap[kSelector] = glyph;

			for (int i = 0; i < mMaxHeight; ++i)
			{
				dest[0] = 0x50FF;
				dest[1] = 0xFFFF;
				dest[2] = 0x50FF;

				dest += texWidth;
			}

			c_pixel += mMaxWidth + 1;
		}

		// selected Back
		{
			int dOffset = r_pixel * texWidth + (c_pixel);
			unsigned short * dest = (unsigned short *)lockBox.pData + dOffset;

			MGUI_Glyph glyph;

			d_assert (mMaxWidth > 3 && mMaxHeight > 3);

			glyph.uv.x0 = (c_pixel + 1) * invTexWidth;
			glyph.uv.y0 = (r_pixel + 1) * invTexHeight;
			glyph.uv.x1 = (c_pixel + mMaxWidth - 1) * invTexWidth;
			glyph.uv.y1 = (r_pixel + mMaxHeight - 1) * invTexHeight;
			glyph.aspect = (mMaxWidth - 1) / (float)(mMaxHeight - 1);

			mGlyphMap[kSelectorBack] = glyph;

			for (int i = 0; i < mMaxHeight; ++i)
			{
				for (int j = 0; j < mMaxWidth; ++j)
				{
					dest[j] = 0xFFFF;
				}

				dest += texWidth;
			}

			c_pixel += mMaxWidth + 1;
		}

		for (int i = 0; i < mCharRanges.Size(); ++i)
		{
			const MGUI_CharRange & cr = mCharRanges[i];

			for (MGUI_Char ch = cr.low_ch; ch <= cr.high_ch; ++ch)
			{
				if (ch == kSelector && ch == kSelectorBack)
					continue;

				if (FT_Load_Char(ftFace, ch, FT_LOAD_RENDER) != 0)
					 continue;

				MGUI_Glyph glyph;

				int offsetX = ftFace->glyph->metrics.horiBearingX >> 6;
				int offsetY = mMaxHoriBearingY - (ftFace->glyph->metrics.horiBearingY >> 6);
				int needWidth = ftFace->glyph->advance.x >> 6;
				int needHeight = mMaxHeight;

				if (c_pixel + mMaxWidth + 1 >= texWidth)
				{
					c_pixel = 0;
					r_pixel += mMaxHeight + 1;

					d_assert (r_pixel < texWidth);
				}

				glyph.uv.x0 = c_pixel * invTexWidth;
				glyph.uv.y0 = r_pixel * invTexHeight;
				glyph.uv.x1 = (c_pixel + needWidth) * invTexWidth;
				glyph.uv.y1 = (r_pixel + needHeight) * invTexHeight;
				glyph.aspect = needWidth / (float)needHeight;

				mGlyphMap[ch] = glyph;

				int dOffset = (r_pixel + offsetY) * texWidth + (c_pixel + offsetX);
				unsigned short * dest = (unsigned short *)lockBox.pData + dOffset;
				unsigned char * src = (unsigned char*)ftFace->glyph->bitmap.buffer;

				for (int r = 0; r < ftFace->glyph->bitmap.rows; ++r)
				{
					for (int b = 0; b < ftFace->glyph->bitmap.width; ++b)
					{
						dest[b] = src[b] * unsigned short(256) + 0xFF;
					}
					
					src += ftFace->glyph->bitmap.pitch;
					dest += texWidth;
				}

				c_pixel += mMaxWidth + 1;
			}
		}

		mTexture->Unlock(0);

		FT_Done_Face(ftFace);
		FT_Done_FreeType(ftLibrary);

		return true;
	}

	void MGUI_Font::Unload()
	{
		mFontSource = "";
		mResolution = 0;
		mCharSize = 0;
		mCharRanges.Clear();

		mTexture = NULL;

		mGlyphMap.Clear();
	}

	void MGUI_Font::LoadFromXml(const char * source)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);

		if (stream.IsNull())
			return;

		xml_doc doc;
		XmlHelper::LoadXmlFromMem(doc, (char *)stream->GetData());

		xml_node * root = doc.first_node("Font");

		xml_node * node = root->first_node("FontSource");
		d_assert (node != NULL);
		mFontSource = node->first_attribute("value")->value();

		node = root->first_node("Resolution");
		if (node)
			mResolution = atoi(node->first_attribute("value")->value());

		node = root->first_node("CharSize");
		if (node)
			mCharSize = atoi(node->first_attribute("value")->value());

		node = root->first_node("CharRange");
		while (node)
		{
			MGUI_Char low_ch = (MGUI_Char)atoi(node->first_attribute("low")->value());
			MGUI_Char high_ch = (MGUI_Char)atoi(node->first_attribute("high")->value());

			MGUI_CharRange cr = { low_ch, high_ch };

			mCharRanges.PushBack(cr);

			node = node->next_sibling("CharRange");
		}
	}

	const MGUI_Glyph * MGUI_Font::GetGlyph(MGUI_Char _char)
	{
		HashMap<MGUI_Char, MGUI_Glyph>::Iterator whr = mGlyphMap.Find(_char);

		if (whr != mGlyphMap.End())
			return &(whr->second);

		return NULL;
	}
}