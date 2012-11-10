#include "MWImage.h"

using namespace Myway;

Image::Image()
: mFormat(FMT_UNKNOWN),
  mWidth(0),
  mHeight(0),
  mSrcWidth(0),
  mSrcHeight(0),
  mMipmapLevel(0)
{
}

Image::~Image()
{
}

int Image::GetWidth() const
{
    return mWidth;
}

int Image::GetHeight() const
{
    return mHeight;
}

int Image::GetSrcWidth() const
{
    return mSrcWidth;
}

int Image::GetSrcHeight() const
{
    return mSrcHeight;
}

FORMAT Image::GetFormat() const
{
    return mFormat;
}

int Image::GetMipmapLevel() const
{
    return mMipmapLevel;
}

Color4 Image::GetColor(int x, int y) const
{
    Color c(0);
    Color4 c4(0);
    int i = 0;

    switch (mFormat)
    {
    case FMT_A8R8G8B8:
        c = GetA8R8G8B8(x, y);
        c4.r = c.r / 255.0f;
        c4.g = c.g / 255.0f;
        c4.b = c.b / 255.0f;
        c4.a = c.a / 255.0f;
        break;

    case FMT_X8R8G8B8:
        c = GetX8R8G8B8(x, y);
        c4.r = c.r / 255.0f;
        c4.g = c.g / 255.0f;
        c4.b = c.b / 255.0f;
        c4.a = 1;
        break;

    case FMT_R8G8B8:
        c = GetR8G8B8(x, y);
        c4.r = c.r / 255.0f;
        c4.g = c.g / 255.0f;
        c4.b = c.b / 255.0f;
        c4.a = 1;
        break;

    case FMT_A8:
        i = GetAlpha8(x, y);
        c4.r = i / 255.0f;
        c4.g = c4.r;
        c4.b = c4.g;
        c4.a = c4.b;
        break;

    case FMT_L8:
        i = GetLuminance8(x, y);
        c4.r = i / 255.0f;
        c4.g = c4.r;
        c4.b = c4.g;
        c4.a = c4.b;
        break;

    case FMT_L16:
        i = GetLuminance16(x, y);
        c4.r = i / (float)65536;
        c4.g = c4.r;
        c4.b = c4.r;
        c4.a = c4.r;
        break;

    default:
        assert (0);
        break;
    }
    
    return c4;
}

/*
void Image::GenericPerlinNoiseHightMap(float frequency)
{
    debug_assert(mD3D9Texture && mFormat == FMT_A8, "Invalid Call");

    float * pHightData = new float[mWidth * mHeight];
    assert(pHightData);
    float scale = (float)frequency / (mWidth - 1);

    int index = 0;
    float fMin, fMax, noise;
    fMin = MAX_FLOAT;
    fMax = MIN_FLOAT;

    for (int j = 0; j < mHeight; ++j)
    {
        for (int i = 0; i < mWidth; ++i)
        {
            noise = Math::PerlinNoise2((float)i, (float)j, scale);
            pHightData[index] = noise;
            if (fMin > noise)
                fMin = noise;
            if (fMax < noise)
                fMax = noise;

            ++index;
        }
    }

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    {
        unsigned char * pData;
        float span = fMax - fMin;
        float v;
        index = 0;

        for (int j = 0; j < mHeight; ++j)
        {
            pData = (unsigned char*)rc.pBits + j * rc.Pitch;
            for (int i = 0; i < mWidth; ++i)
            {
                v = 255.0f * (pHightData[index] - fMin) / span;
                *pData++ = (unsigned char)v;
                ++index;
            }
        }
    }
    mD3D9Texture->UnlockRect(0);
    
    delete[] pHightData;
}
*/