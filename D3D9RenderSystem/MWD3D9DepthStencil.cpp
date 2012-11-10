#include "MWD3D9DepthStencil.h"
#include "MWVideoBufferManager.h"
#include "MWRenderSystem.h"
#include "Engine.h"


namespace Myway
{

D3D9DepthStencil::D3D9DepthStencil(IDirect3DDevice9 * device)
: mD3D9Device(device),
  mDepthStencil(NULL)
{
}

D3D9DepthStencil::~D3D9DepthStencil()
{
    safe_release_com(mDepthStencil);
}

void D3D9DepthStencil::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyDepthStencil(this);
}

void D3D9DepthStencil::LostDevice()
{
    safe_release_com(mDepthStencil);
}

void D3D9DepthStencil::ResetDevice()
{
    assert(!mDepthStencil);

    const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

    HRESULT hr = D3D_OK;

    int rWidth = mWidth, rHeight = mHeight;
    if (rWidth == -1 || rHeight == -1)
    {
        rWidth = dp->Width;
        rHeight = dp->Height;
    }

    D3DMULTISAMPLE_TYPE Msaa = D3D9Mapping::GetD3DMultiSampleType(mMSAA);
    D3DFORMAT Format = D3D9Mapping::GetD3DFormat(mFormat);

    hr = mD3D9Device->CreateDepthStencilSurface(rWidth, rHeight, Format, Msaa, 0, TRUE, &mDepthStencil, NULL);

    D3DErrorExceptionFunction(CreateDepthStencilSurface, hr);
}

}
