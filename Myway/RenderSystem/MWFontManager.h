#pragma once

#include "MWFont.h"
#include "MWFontLoader.h"

namespace Myway
{

enum FONT_TYPE
{
    FONT_DYNAMIC_TTF,
    FONT_STATIC_TTF,
    FONT_STATIC_TEXTURE,

    MW_ALIGN_ENUM(FONT_TYPE)
};

class MW_ENTRY FontManager : public Singleton<FontManager> 
{
public:
            FontManager();
            ~FontManager();

    Font *  CreateFont(const String & sName, FONT_TYPE type);

    Font *  LoadFont(const String & sFileName);
    void    Unload(const String & sName);

    Font *  GetFont(const String & sName);

protected:
    List<Font*>         m_Fonts;
    FontLoader *        m_Loader;
};

}