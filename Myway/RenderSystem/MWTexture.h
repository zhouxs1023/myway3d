#pragma once

#include "MWRenderDefine.h"
#include "MWResource.h"

namespace Myway
{

class MW_ENTRY Texture : public Resource, public RefObj
{
public:
    Texture();
    virtual ~Texture();

    virtual void    Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags) = 0;
    virtual void    Unlock(int iLevel) = 0;

    const TString128 & GetName() const      { return mName; }
    int             GetWidth() const        { return mWidth; }
    int             GetHeight() const       { return mHeight; }
    int             GetDepth() const        { return mDepth; }
    int             GetMipLevels() const    { return mMipLevels; } 
    USAGE           GetUsage() const        { return mUsage; }
    FORMAT          GetFormat() const       { return mFormat; }
    TEXTURE_TYPE    GetTextureType() const  { return mType; }

protected:
    TString128 mName;
    int mWidth;
    int mHeight;
    int mDepth;
    int mMipLevels;
    USAGE mUsage;
    FORMAT mFormat;
    TEXTURE_TYPE mType;
};

DeclareSmartPtr(Texture);

}