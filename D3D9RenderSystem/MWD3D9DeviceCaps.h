#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWDeviceCaps.h"

namespace Myway
{

class RS_ENTRY D3D9DeviceCaps : public DeviceCaps
{
public:
    D3D9DeviceCaps();
    virtual ~D3D9DeviceCaps();

    virtual int MaxTextureWidth();
    virtual int MaxTextureHeight();
    virtual int MaxVolumeExtent();
    virtual int MaxAnisotropy();
    virtual int MaxStreams();
    virtual int MaxStreamStride();
    virtual int VertexShaderVersion();
    virtual int MaxVertexShaderConst();
    virtual int PixelShaderVersion();
    virtual bool SupportStretchRectFromTexture();

    D3DCAPS9 * GetCaps() { return &mCaps; } 

protected:
    D3DCAPS9          mCaps;
};

}