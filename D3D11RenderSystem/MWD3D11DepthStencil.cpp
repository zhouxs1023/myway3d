#include "MWD3D11DepthStencil.h"
#include "MWVideoBufferManager.h"


namespace Myway {

D3D11DepthStencil::D3D11DepthStencil()
: mDepthStencilView(NULL)
{
}

D3D11DepthStencil::~D3D11DepthStencil()
{
    safe_release(mDepthStencilView);
}

void D3D11DepthStencil::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyDepthStencil(this);
}


}
