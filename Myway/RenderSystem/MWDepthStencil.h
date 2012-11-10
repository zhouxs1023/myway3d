#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

class MW_ENTRY DepthStencil : public RefObj
{
public:
    DepthStencil();
    virtual ~DepthStencil();

    const TString128 & GetName();
    int GetWidth();
    int GetHeight();
    FORMAT GetFormat();
    MULTI_SAMPLE GetMultiSampleType();

protected:
    TString128      mName;
    MULTI_SAMPLE    mMSAA;
    int             mWidth;
    int             mHeight;
    FORMAT          mFormat;
};

DeclareSmartPtr(DepthStencil);

#include "MWDepthStencil.inl"

}