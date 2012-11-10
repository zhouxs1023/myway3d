#include "MWImage.h"
#include "MWDynamicTTFont.h"
#include "MWResourceManager.h"
#include "MWVideoBufferManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


#define _FT_FAIED(hr) ((hr) != 0)
#define _FT_SUCCESSED(hr) ((hr) == 0)

#define _FTErrorException(func) EXCEPTION(String("FreeType2 Error: ") + #func + " failed.")

using namespace Myway;

DynamicTTFont::DynamicTTFont(const String & sName)
: m_sName(sName),
  m_sSource(""),
  m_texturesize(512),
  m_charsize(32),
  m_resolution(72),
  m_library(NULL),
  mace(NULL),
  mMaxwidth(0),
  mMaxheight(0),
  mMaxhoriBearingY(0),
  m_autoComputeInfo(TRUE),
  m_rows(0),
  m_columns(0),
  mMaxcount(0),
  m_ttfdata(NULL),
  m_ttfsize(0)
{
}

DynamicTTFont::~DynamicTTFont()
{
    Unload();
}

const String & DynamicTTFont::GetName() const
{
    return m_sName;
}

const String & DynamicTTFont::GetSourceName() const
{
    return m_sSource;
}

void DynamicTTFont::Load()
{
    Unload();

    //intitialize ft2
    if (_FT_FAIED(FT_Init_FreeType(&m_library)))
    {
       _FTErrorException(FT_Init_FreeType);
    }

    DataStreamPtr stream = ResourceManager::Instance().OpenResource(m_sSource);

    if (stream.IsNull())
        return;

    m_ttfsize = stream->Size();
    m_ttfdata = new char[m_ttfsize + 1];
    stream->Read(m_ttfdata, m_ttfsize);
    m_ttfdata[m_ttfsize] = 0;

    //load face
    if (_FT_FAIED(FT_New_Memory_Face(m_library, (const FT_Byte *)m_ttfdata, m_ttfsize, 0, &mace)))
    {
        _FTErrorException(FT_New_Memory_Face);
    }

    //set char size
    if (_FT_FAIED(FT_Set_Char_Size(mace, m_charsize << 6, 0, m_resolution, 0)))
    {
        _FTErrorException(FT_Set_Char_Size);
    }

    mMaxheight = 0;
    mMaxhoriBearingY = 0;

    if (m_autoComputeInfo)
    {
        List<ftchar_range>::Iterator iter;
        List<ftchar_range>::Iterator end;

        int horiBearingY = 0;
        int width = 0;
        int height = 0;
        int maxwidth = 0;
        int maxheight = 0;
        int maxhoriBearingY = 0;

        iter = m_charRanges.Begin();
        end = m_charRanges.End();

        while (iter != end)
        {
            for (ftchar ch = iter->lowch; ch < iter->highch; ++ch)
            {
                if (_FT_SUCCESSED(FT_Load_Char(mace, ch, FT_LOAD_RENDER)))
                {
                    horiBearingY = mace->glyph->metrics.horiBearingY >> 6;
                    width = (mace->glyph->advance.x >> 6);
                    height = mace->glyph->bitmap.rows + (mace->glyph->bitmap.rows - horiBearingY);

                    maxwidth = Math::Maximum(maxwidth, width);
                    maxheight = Math::Maximum(maxheight, height);
                    maxhoriBearingY = Math::Maximum(maxhoriBearingY, horiBearingY);
                }
            }

            ++iter; 
        }

        assert (maxwidth > 0 && maxheight > 0 && maxhoriBearingY > 0);
        
        mMaxwidth = (int)maxwidth;
        mMaxheight = (int)maxheight;
        mMaxhoriBearingY = (int)maxhoriBearingY;
    }

    assert(mMaxwidth != 0 && mMaxheight != 0 && mMaxhoriBearingY != 0);

    m_columns = m_texturesize / mMaxwidth;
    m_rows = m_texturesize / mMaxheight;
    mMaxcount = m_columns * m_rows;
}

void DynamicTTFont::Unload()
{
    if (mace)
        FT_Done_Face(mace);

    if (m_library)
        FT_Done_FreeType(m_library);

    m_library = NULL;
    mace = NULL;

    List<ftchunk*>::Iterator iter;
    List<ftchunk*>::Iterator end;

    iter = m_chunks.Begin();
    end = m_chunks.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    m_chunks.Clear();

    if (m_ttfdata)
        delete[] m_ttfdata;

    m_ttfdata = NULL;
    m_ttfsize = 0;
}

void DynamicTTFont::AddCharRange(ftchar lowch, ftchar highch)
{
    assert(lowch <= highch);

    ftchar_range * range = m_charRanges.Increase();
    range->lowch = lowch;
    range->highch = highch;
}

void DynamicTTFont::RemoveCharRange(int index)
{
    assert(index < m_charRanges.Size());

    List<ftchar_range>::Iterator iter;
    List<ftchar_range>::Iterator end;

    iter = m_charRanges.Begin();
    end = m_charRanges.End();

    while (index--)
    {
        ++iter;
    }

    m_charRanges.Erase(iter);
}

void DynamicTTFont::SetMaxWidth(int maxwidth)
{
    mMaxwidth = maxwidth;
}

void DynamicTTFont::SetMaxHeight(int maxheight)
{
    mMaxheight = maxheight;
}

void DynamicTTFont::SetMaxHoriBearingY(int maxhoriBearingY)
{
    mMaxhoriBearingY = mMaxhoriBearingY;
}

int DynamicTTFont::GetCharSize() const
{
    return m_charsize;
}

int DynamicTTFont::GetResolution() const
{
    return m_resolution;
}

int DynamicTTFont::GetTextureSize() const
{
    return m_texturesize;
}

int DynamicTTFont::GetCharRangeSize() const
{
    return m_charRanges.Size();
}

ftchar_range DynamicTTFont::GetCharRange(int index) const
{
    assert(index < m_charRanges.Size());

    List<ftchar_range>::ConstIterator iter;
    List<ftchar_range>::ConstIterator end;

    iter = m_charRanges.Begin();
    end = m_charRanges.End();

    while (index--)
    {
        ++iter;
    }

    ftchar_range fr;
    fr.lowch = iter->lowch;
    fr.highch = iter->highch;

    return fr;
}

int DynamicTTFont::GetMaxWidth() const
{
    return mMaxwidth;
}

int DynamicTTFont::GetMaxHeight() const
{
    return mMaxheight;
}

int DynamicTTFont::GetMaxHoriBearingY() const
{
    return mMaxhoriBearingY;
}

void DynamicTTFont::SetAutoComputerInfo(bool enable)
{
    m_autoComputeInfo = enable;
}

bool DynamicTTFont::IsAutoComputeInfo() const
{
    return m_autoComputeInfo;
}


//FT_HAS_HORIZONTAL
//FT_HAS_VERTICAL

void DynamicTTFont::AddRef(int id)
{
    List<ftchunk*>::Iterator iter;
    List<ftchunk*>::Iterator end;

    iter = m_chunks.Begin();
    end = m_chunks.End();

    while (iter != end && (*iter)->id != id)
    {
        ++iter;
    }

    assert(iter != end);

    ++(*iter)->refer;
}

void DynamicTTFont::DelRef(int id)
{
    List<ftchunk*>::Iterator iter;
    List<ftchunk*>::Iterator end;

    iter = m_chunks.Begin();
    end = m_chunks.End();

    while (iter != end && (*iter)->id != id)
    {
        ++iter;
    }

    assert(iter != end && (*iter)->refer);

    if (--((*iter)->refer) == 0)
    {
        (*iter)->row = 0;
        (*iter)->column = 0;
        (*iter)->size = 0;
    }
}

ftid DynamicTTFont::LoadGlyphs(const FString & str, List<ftglyph> & glyphs, TexturePtr & tex)
{
    glyphs.Clear();

    float inv_texture_size = 1.0f / m_texturesize;

    List<ftchunk*>::Iterator iter;
    List<ftchunk*>::Iterator end;

    iter = m_chunks.Begin();
    end = m_chunks.End();
  
    assert(mMaxcount >= str.Length());

    ftchunk * ck;

    while (iter != end)
    {
        ck = *iter;
        if (ck->size + str.Length() <= mMaxcount)
        {
            break;
        }

        ++iter;
    }

    if (iter == end)
    {
        ck = new ftchunk();
        ck->texture = VideoBufferManager::Instance().CreateTexture("Sys_" + m_sName + "_" + m_chunks.Size(),
                                                                   m_texturesize,
                                                                   m_texturesize,
                                                                   1, 
                                                                   USAGE_NORMAL,
                                                                   POOL_MANAGED,
                                                                   FMT_L8);
        ck->id = m_chunks.Size();
        ck->row = 0;
        ck->column = 0;
        ck->size = 0;
        ck->refer = 0;
        m_chunks.PushBack(ck);
    }

    tex = ck->texture;

    LockedBox box;
    tex->Lock(0, &box, NULL, LOCK_DISCARD);

    if (!ck->refer)
    {
        Memzero(box.pData, m_texturesize * box.RowPitch);
    }

    ftglyph glyph;

    for (int i = 0; i < str.Length(); ++i)
    {
        glyph.u0 = 0, glyph.v0 = 0;
        glyph.u1 = 0, glyph.v1 = 0;
        glyph.aspect = 1.0f;

        wchar_t ch = str[i];

        if (_FT_SUCCESSED(FT_Load_Char(mace, str[i], FT_LOAD_RENDER)))
        {
            int offsetX = mace->glyph->metrics.horiBearingX >> 6;
            int offsetY = mMaxhoriBearingY - (mace->glyph->metrics.horiBearingY >> 6);
            int needWidth = mace->glyph->advance.x >> 6;

            int row_pixel = ck->row * mMaxheight;
            int column_pixel = ck->column * mMaxwidth;

            glyph.u0 = column_pixel * inv_texture_size;
            glyph.v0 = row_pixel * inv_texture_size;
            glyph.u1 = (column_pixel + needWidth) * inv_texture_size;
            glyph.v1 = (row_pixel + mMaxheight) * inv_texture_size;
            glyph.aspect = needWidth / (float)mMaxheight;

            char * dest = (char*)box.pData + (row_pixel + offsetY) * box.RowPitch + (column_pixel + offsetX);
            char * src = (char*)mace->glyph->bitmap.buffer;

            for (int r = 0; r < mace->glyph->bitmap.rows; ++r)
            {
                Memcpy(dest, src, sizeof(char) * mace->glyph->bitmap.width);

                src += mace->glyph->bitmap.pitch;
                dest += box.RowPitch;
            }

            if (++(ck->column) >= m_columns)
            {
                ck->column = 0;
                ++(ck->row);
            }

            ++(ck->size);
        }

        glyphs.PushBack(glyph);
    }

    tex->Unlock(0);

    return ck->id;
}

void DynamicTTFont::SetSource(const String & sFileName)
{
    m_sSource = sFileName;
}

void DynamicTTFont::SetCharSize(int size)
{
    m_charsize = size;
}

void DynamicTTFont::SetResolution(int resolution)
{
    m_resolution = resolution;
}

void DynamicTTFont::SetTextureSize(int size)
{
    assert(Math::IsPowerOfTwo(m_texturesize));
    m_texturesize = size;
}
