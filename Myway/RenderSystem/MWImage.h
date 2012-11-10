#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

class MW_ENTRY Image : public RefObj
{
public:
    Image();
    virtual ~Image();

    virtual void SetA8R8G8B8(int x, int y, Color c) = 0;
    virtual void SetX8R8G8B8(int x, int y, Color c) = 0;
    virtual void SetR8G8B8(int x, int y, Color c) = 0;
    virtual void SetAlpha8(int x, int y, unsigned char a) = 0;
    virtual void SetLuminance8(int x, int y, unsigned char l) = 0;
    virtual void SetLuminance16(int x, int y, short l) = 0;

    virtual Color GetA8R8G8B8(int x, int y) const = 0;
    virtual Color GetX8R8G8B8(int x, int y) const = 0;
    virtual Color GetR8G8B8(int x, int y) const = 0;
    virtual unsigned char GetAlpha8(int x, int y) const = 0;
    virtual unsigned char GetLuminance8(int x, int y) const = 0;
    virtual short GetLuminance16(int x, int y) const = 0;

    virtual void Lock(LockedRect & rect) = 0;
    virtual void Unlock() = 0;

    Color4 GetColor(int x, int y) const;

    int GetWidth() const;
    int GetHeight() const;
    int GetSrcWidth() const;
    int GetSrcHeight() const;
    FORMAT GetFormat() const;
    int GetMipmapLevel() const;

protected:
    FORMAT           mFormat;
    int              mSrcWidth;
    int              mSrcHeight;
    int              mWidth;
    int              mHeight;
    int              mMipmapLevel;
};

DeclareSmartPtr(Image);

}