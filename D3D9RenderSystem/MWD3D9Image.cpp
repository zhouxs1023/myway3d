#include "MWD3D9Image.h"

namespace Myway
{

D3D9Image::D3D9Image()
: mD3D9Texture(NULL)
{
}

D3D9Image::~D3D9Image()
{
    safe_release_com(mD3D9Texture);
}

void D3D9Image::DeleteSelf()
{
    delete this;
}

void D3D9Image::SetA8R8G8B8(int x, int y, Color c)
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_A8R8G8B8, "invalid call.");

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rc.pBits + y * (int)rc.Pitch + x * 4;

        *pData++ = c.b;
        *pData++ = c.g;
        *pData++ = c.r;
        *pData++ = c.a;
    }
    mD3D9Texture->UnlockRect(0);
}

void D3D9Image::SetX8R8G8B8(int x, int y, Color c)
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_X8R8G8B8, "invalid call.");

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rc.pBits + y * (int)rc.Pitch + x * 4;

        *pData++ = c.b;
        *pData++ = c.g;
        *pData++ = c.r;
        *pData++ = 255;
    }
    mD3D9Texture->UnlockRect(0);
}

void D3D9Image::SetR8G8B8(int x, int y, Color c)
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_R8G8B8, "invalid call.");

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rc.pBits + y * (int)rc.Pitch + x * 3;

        *pData++ = c.b;
        *pData++ = c.g;
        *pData++ = c.r;
    }
    mD3D9Texture->UnlockRect(0);
}

void D3D9Image::SetAlpha8(int x, int y, unsigned char a)
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_A8, "invalid call.");

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rc.pBits + y * (int)rc.Pitch + x;

        *pData++ = a;
    }
    mD3D9Texture->UnlockRect(0);
}

void D3D9Image::SetLuminance8(int x, int y, unsigned char l)
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_L8, "invalid call.");

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rc.pBits + y * (int)rc.Pitch + x;

        *pData++ = l;
    }
    mD3D9Texture->UnlockRect(0);
}

void D3D9Image::SetLuminance16(int x, int y, short l)
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_L16, "invalid call.");

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    {
        short * pData = (short*)((unsigned char*)rc.pBits + y * (int)rc.Pitch + x * 2);

        *pData++ = l;
    }
    mD3D9Texture->UnlockRect(0);
}

Color D3D9Image::GetX8R8G8B8(int x, int y) const
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_X8R8G8B8, "invalid call.");

    D3DLOCKED_RECT rect;
    Color c;

    mD3D9Texture->LockRect(0, &rect, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rect.pBits + y * rect.Pitch + x * 4;

        c.b = *pData++;
        c.g = *pData++;
        c.r = *pData++;
        c.a = *pData++;
    }
    mD3D9Texture->UnlockRect(0);

    c.a = 255;
    return c;
}

Color D3D9Image::GetA8R8G8B8(int x, int y) const
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_A8R8G8B8, "invalid call.");

    D3DLOCKED_RECT rect;
    Color c;

    mD3D9Texture->LockRect(0, &rect, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rect.pBits + y * rect.Pitch + x * 4;

        c.b = *pData++;
        c.g = *pData++;
        c.r = *pData++;
        c.a = *pData++;
    }
    mD3D9Texture->UnlockRect(0);

    return c;
}

Color D3D9Image::GetR8G8B8(int x, int y) const
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_R8G8B8, "invalid call.");

    D3DLOCKED_RECT rect;
    Color c;

    mD3D9Texture->LockRect(0, &rect, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rect.pBits + y * rect.Pitch + x * 3;

        c.b = *pData++;
        c.g = *pData++;
        c.r = *pData++;
    }
    mD3D9Texture->UnlockRect(0);

    return c;
}

unsigned char D3D9Image::GetAlpha8(int x, int y) const
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_A8, "invalid call.");

    D3DLOCKED_RECT rect;
    unsigned char c;

    mD3D9Texture->LockRect(0, &rect, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rect.pBits + y * rect.Pitch + x;

        c = *pData++;
    }
    mD3D9Texture->UnlockRect(0);

    return c;
}

unsigned char D3D9Image::GetLuminance8(int x, int y) const
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_L8, "invalid call.");

    D3DLOCKED_RECT rect;
    unsigned char c;

    mD3D9Texture->LockRect(0, &rect, NULL, 0);
    {
        unsigned char * pData = (unsigned char*)rect.pBits + y * rect.Pitch + x;

        c = *pData++;
    }
    mD3D9Texture->UnlockRect(0);

    return c;
}

short D3D9Image::GetLuminance16(int x, int y) const
{
    debug_assert(x < mWidth && y < mHeight && mD3D9Texture, "Invalid Call.");
    debug_assert(mFormat == FMT_A8, "invalid call.");

    D3DLOCKED_RECT rect;
    short c;

    mD3D9Texture->LockRect(0, &rect, NULL, 0);
    {
        short * pData = (short*)((unsigned char*)rect.pBits + y * rect.Pitch + x * 2);

         c = *pData++;
    }
    mD3D9Texture->UnlockRect(0);

    return c;
}

void D3D9Image::Lock(LockedRect & rect)
{
    assert(mD3D9Texture);

    D3DLOCKED_RECT rc;
    mD3D9Texture->LockRect(0, &rc, NULL, 0);
    rect.pData = rc.pBits;
    rect.Pitch = rc.Pitch;
}

void D3D9Image::Unlock()
{
    assert(mD3D9Texture);

    mD3D9Texture->UnlockRect(0);
}

}
