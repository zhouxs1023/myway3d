#pragma once

#include "MWTexture.h"

namespace Myway
{

class RenderTargetListener
{
public:
    RenderTargetListener() {}
    virtual ~RenderTargetListener() {}

    virtual void BeginRendering() {}
    virtual void EndRendering() {}
};

class MW_ENTRY RenderTarget : public RefObj
{
public:
    RenderTarget();
    virtual ~RenderTarget();

    virtual void Stretch(Texture * texture) = 0;

    const TString128 & GetName() const { return mName; }
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    FORMAT GetFormat() const { return mFormat; }
    MULTI_SAMPLE GetMultiSampleType() const { return mMSAA; }

protected:
    TString128 mName;

    int mWidth;
    int mHeight;
    FORMAT mFormat;
    MULTI_SAMPLE mMSAA;
};

DeclareSmartPtr(RenderTarget);

}