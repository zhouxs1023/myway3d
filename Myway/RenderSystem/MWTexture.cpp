#include "MWTexture.h"
#include "MWResourceManager.h"

namespace Myway
{
    Texture::Texture()
        : mName("")
        , mHeight(0)
        , mWidth(0)
        , mDepth(0)
        , mMipLevels(0)
        , mUsage(USAGE_STATIC)
        , mFormat(FMT_UNKNOWN)
        , mType(TEXTYPE_2D)

    {
    }

    Texture::~Texture()
    {
    }
}