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

    virtual void Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags) = 0;
    virtual void Unlock(int iLevel) = 0;

	const TString128 & GetName() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetDepth() const;
	int GetMipLevels() const;
	USAGE GetUsage() const;
	FORMAT GetFormat() const;
	TEXTURE_TYPE GetTextureType() const;
	bool IsRenderTarget() const;

protected:
    TString128 mName;
    int mWidth;
    int mHeight;
    int mDepth;
    int mMipLevels;
    USAGE mUsage;
    FORMAT mFormat;
    TEXTURE_TYPE mType;
	bool mRenderTarget;
};

DeclareSmartPtr(Texture);

}