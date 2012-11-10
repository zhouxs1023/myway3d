#pragma once
/*
-------------------------------------------------------------------------
    
-------------------------------------------------------------------------
*/

#include "MWTexture.h"

namespace Myway
{

class MW_ENTRY TextureLinker
{
    DECLARE_ALLOC();
    DECLARE_SINGLETON(TextureLinker);

public:
    TextureLinker();
    ~TextureLinker();

    void SetTexture(int index, TexturePtr texture);
    TexturePtr GetTexture(int index);

protected:
    TexturePtr  m_pTextures[MAX_TEXTURE_LINKER];
};

}