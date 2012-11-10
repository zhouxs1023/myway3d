#include "MWTextureLinker.h"

namespace Myway
{

IMPLEMENT_SINGLETON(TextureLinker);

TextureLinker::TextureLinker()
{
    INITIALIZE_SINGLETON();
}

TextureLinker::~TextureLinker()
{
}

void TextureLinker::SetTexture(int index, TexturePtr texture)
{
    d_assert (index >= 0 && index < MAX_TEXTURE_LINKER);

    m_pTextures[index] = texture;
}

TexturePtr TextureLinker::GetTexture(int index)
{
    d_assert (index >= 0 && index < MAX_TEXTURE_LINKER);

    return m_pTextures[index];
}

}