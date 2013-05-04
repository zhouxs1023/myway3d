#include "MWD3D11RenderTarget.h"
#include "MWVideoBufferManager.h"

namespace Myway
{

D3D11RenderTarget::D3D11RenderTarget()
: mRenderTargetView(NULL)
{
}

D3D11RenderTarget::~D3D11RenderTarget()
{
    safe_release(mRenderTargetView);
}

void D3D11RenderTarget::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyRenderTarget(this);
}

void D3D11RenderTarget::Stretch(Texture * p)
{
	d_assert (0);
}




}
