#pragma once

#include "MWTexture.h"

namespace Myway
{

class MW_ENTRY RenderTarget : public RefObj
{
public:
    RenderTarget();
    virtual ~RenderTarget();

    virtual void Stretch(Texture * texture) = 0;

    const TString128 & GetName() const;
    int GetWidth() const;
    int GetHeight() const;
    FORMAT GetFormat() const;
    MULTI_SAMPLE GetMultiSampleType() const;

protected:
    TString128 mName;

    int mWidth;
    int mHeight;
    FORMAT mFormat;
    MULTI_SAMPLE mMSAA;
};

DeclareSmartPtr(RenderTarget);

}