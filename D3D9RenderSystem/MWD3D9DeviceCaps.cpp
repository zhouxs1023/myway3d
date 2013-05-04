#include "MWD3D9DeviceCaps.h"

namespace Myway
{

D3D9DeviceCaps::D3D9DeviceCaps()
{
    ZeroMemory(&mCaps, sizeof(D3DCAPS9));
}

D3D9DeviceCaps::~D3D9DeviceCaps()
{
}

int D3D9DeviceCaps::MaxTextureWidth()
{
    return (int)mCaps.MaxTextureWidth;
}

int D3D9DeviceCaps::MaxTextureHeight()
{
    return (int)mCaps.MaxTextureHeight;
}

int D3D9DeviceCaps::MaxVolumeExtent()
{
    return (int)mCaps.MaxVolumeExtent;
}

int D3D9DeviceCaps::MaxAnisotropy()
{
    return (int)mCaps.MaxAnisotropy;
}

int D3D9DeviceCaps::MaxStreams()
{
    return (int)mCaps.MaxStreams;
}

int D3D9DeviceCaps::MaxStreamStride()
{
    return (int)mCaps.MaxStreamStride;
}

int D3D9DeviceCaps::VertexShaderVersion()
{
    switch (mCaps.VertexShaderVersion)
    {
    case D3DVS_VERSION(3, 0):
        return (int)SP_VS;

    default:
        return 0;
    }
}

int D3D9DeviceCaps::MaxVertexShaderConst()
{
    return (int)mCaps.MaxVertexShaderConst;
}

int D3D9DeviceCaps::PixelShaderVersion()
{
    switch (mCaps.PixelShaderVersion)
    {
    case D3DPS_VERSION(3, 0):
        return (int)SP_PS;

    default:
        return 0;
    }
}

bool D3D9DeviceCaps::SupportStretchRectFromTexture()
{
    if (mCaps.Caps2 & D3DDEVCAPS2_CAN_STRETCHRECT_FROM_TEXTURES)
        return true;
    else
        return false;
}

}
