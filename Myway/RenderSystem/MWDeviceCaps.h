#pragma once

#include "MWCore.h"

namespace Myway
{

class MW_ENTRY DeviceCaps
{
    DECLARE_SINGLETON(DeviceCaps);

public:
    DeviceCaps();
    virtual ~DeviceCaps();

    virtual int MaxTextureWidth() = 0;
    virtual int MaxTextureHeight() = 0;
    virtual int MaxVolumeExtent() = 0;
    virtual int MaxAnisotropy() = 0;
    virtual int MaxStreams() = 0;
    virtual int MaxStreamStride() = 0;
    virtual int VertexShaderVersion() = 0;
    virtual int MaxVertexShaderConst() = 0;
    virtual int PixelShaderVersion() = 0;
    virtual bool SupportStretchRectFromTexture() = 0;
};

}