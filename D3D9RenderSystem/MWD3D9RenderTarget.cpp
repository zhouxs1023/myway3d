#include "MWD3D9RenderTarget.h"
#include "MWD3D9Texture.h"
#include "MWRenderSystem.h"
#include "Engine.h"

namespace Myway
{

D3D9RenderTarget::D3D9RenderTarget(IDirect3DDevice9 * Device)
: mD3D9Device(Device)
, mRenderTarget(NULL)
{
}

D3D9RenderTarget::~D3D9RenderTarget()
{
    safe_release_com(mRenderTarget);
}

void D3D9RenderTarget::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyRenderTarget(this);
}

void D3D9RenderTarget::Stretch(Texture * p)
{
    d_assert (p->GetTextureType() == TEXTYPE_RENDERTARGET);

    HRESULT hr = D3D_OK;
    IDirect3DSurface9 * surface;
    D3D9Texture * d3dTex = (D3D9Texture *)p;
    IDirect3DTexture9 * t = d3dTex->GetD3DTexture();

    t->GetSurfaceLevel(0, &surface);

    hr = mD3D9Device->StretchRect(mRenderTarget, NULL, surface, NULL, D3DTEXF_POINT);

    surface->Release();

    D3DErrorExceptionFunction(StretchRect, hr);
}

void D3D9RenderTarget::LostDevice()
{
    safe_release_com(mRenderTarget);
}

void D3D9RenderTarget::ResetDevice()
{
    assert(!mRenderTarget);

    const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

    HRESULT hr = D3D_OK;

    D3DMULTISAMPLE_TYPE Msaa = D3D9Mapping::GetD3DMultiSampleType(mMSAA);
    D3DFORMAT Format = D3D9Mapping::GetD3DFormat(mFormat);

    int rWidth = mWidth, rHeight = mHeight;
    if (rWidth == -1 || rHeight == -1)
    {
        rWidth = dp->Width;
        rHeight = dp->Height;
    }

    hr = mD3D9Device->CreateRenderTarget(rWidth, rHeight, Format, Msaa, 0, FALSE, &mRenderTarget, NULL);
    D3DErrorExceptionFunction(CreateRenderTarget, hr);
}


}
