#pragma once

#include "MWTexture.h"
#include "MWFString.h"

/*
--------------------------------------------------------------------------
                    high                        low
    GB2312      0xB0-0xF7(176-247)          0xA0-0xFE£¨160-254£© 
    GBK         0x81-0xFE£¨129-254£©        0x40-0xFE£¨64-254£© 
    Big5        0x81-0xFE£¨129-255£©        0x40-0x7E£¨64-126£©
                                            0xA1£­0xFE£¨161-254£© 
*/

/*
    
*/


namespace Myway
{

struct MW_ENTRY ftglyph
{
    union
    {
        struct
        {
            float           texcoord[4];
        };

        struct
        {
            float           u0, v0;
            float           u1, v1;
        };
    };
    
    float  aspect;
};

typedef int ftid;

#define INVALID_FTID            0xFFFFFFFF
#define IS_INVALID_FTID(id)     ((id) == INVALID_FTID)

#define FONT_IS_CHINESE(c)  (c > 0x8000)

class MW_ENTRY Font : public AllocObj
{
public: 
    Font() {}
    virtual ~Font() {}

    virtual const String & GetName() const = 0;
    virtual const String & GetSourceName() const = 0;

    _private virtual void AddRef(int id) {}
    _private virtual void DelRef(int id) {}

    _private virtual void Load() = 0;
    _private virtual void Unload() = 0;

    virtual ftid LoadGlyphs(const FString & str, List<ftglyph> & glyphs, TexturePtr & tex) = 0;
};

}