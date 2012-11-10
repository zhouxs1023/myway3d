#pragma once

#include "MWFont.h"

namespace Myway
{

class DynamicTTFont;

class MW_ENTRY FontLoader : public AllocObj
{
public:
    FontLoader() {}
    virtual ~FontLoader() {}

    virtual Font * Load(const String & filename) = 0;
};

class MW_ENTRY DefaultFontLoader : public FontLoader
{
public:
    DefaultFontLoader();
    virtual ~DefaultFontLoader();

    virtual Font * Load(const String & filename);
    
protected:
    void ParseDynamicTTFont(DynamicTTFont * font, CommandScript & cst);
};

}