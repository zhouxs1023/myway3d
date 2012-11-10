#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWDepthStencil.h"

namespace Myway
{

class RS_ENTRY D3D9DepthStencil : public DepthStencil
{
    DeclareRefObj();
    DeclareD3DResource();

public:
    D3D9DepthStencil(IDirect3DDevice9 * device);
    virtual ~D3D9DepthStencil();

    friend class    D3D9VideoBufferManager;
    friend class    D3D9RenderSystem;
    friend class    D3D9RenderWindow;
    friend class    D3D9SwapChain;

protected:
    IDirect3DDevice9 *      mD3D9Device;
    IDirect3DSurface9 *     mDepthStencil;
};

}