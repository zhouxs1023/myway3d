#pragma once

#include "MWFont.h"

struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace Myway
{

struct MW_ENTRY  ftdesc : public AllocObj
{
    ftchar          ch;
    ftglyph         glyph;
};

struct MW_ENTRY  ftchunk : public AllocObj
{
    ftid            id;
    int          refer;
    int          row;
    int          column;
    int          size;
    TexturePtr      texture;
};

struct MW_ENTRY  ftchar_range : public AllocObj
{
    ftchar          lowch;
    ftchar          highch;
};

class MW_ENTRY DynamicTTFont : public Font
{
public:
    DynamicTTFont(const String & sName);
    virtual ~DynamicTTFont();

    virtual const String & GetName() const;
    virtual const String & GetSourceName() const;

    _private virtual void AddRef(int id);
    _private virtual void DelRef(int id);

    virtual ftid LoadGlyphs(const FString & str, List<ftglyph> & glyphs, TexturePtr & tex);

    virtual void Load();
    virtual void Unload();

    void SetSource(const String & sFileName);

    void SetCharSize(int size);
    void SetResolution(int resolution);

    void SetTextureSize(int size);

    void AddCharRange(ftchar lowch, ftchar highch);
    void RemoveCharRange(int index);

    void SetMaxWidth(int maxwidth);
    void SetMaxHeight(int maxheight);
    void SetMaxHoriBearingY(int maxhoriBearingY);

    int GetCharSize() const;
    int GetResolution() const;
    int GetTextureSize() const;

    int GetCharRangeSize() const;
    ftchar_range GetCharRange(int index) const;

    int GetMaxWidth() const;
    int GetMaxHeight() const;
    int GetMaxHoriBearingY() const;

    void SetAutoComputerInfo(bool enable);
    bool IsAutoComputeInfo() const;

protected:
    String              m_sName;
    String              m_sSource;

    int              m_charsize;
    int              m_resolution;
    int              m_texturesize;

    List<ftchunk*>      m_chunks;

    FT_LibraryRec_ *    m_library;
    FT_FaceRec_ *       mace;

    int              mMaxwidth;
    int              mMaxheight;
    int              mMaxhoriBearingY;
    
    int              m_rows;
    int              m_columns;
    int              mMaxcount;

    bool                m_autoComputeInfo;
    List<ftchar_range>  m_charRanges;

    int              m_ttfsize;
    char *              m_ttfdata;
};


}

