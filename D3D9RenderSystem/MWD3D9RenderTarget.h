#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWRenderTarget.h"

namespace Myway
{

class RS_ENTRY D3D9RenderTarget : public RenderTarget
{
    DeclareRefObj();
    DeclareD3DResource();

public:
    D3D9RenderTarget(IDirect3DDevice9 * Device);
    virtual ~D3D9RenderTarget();

    virtual void Stretch(Texture * p);

	IDirect3DSurface9 * GetD3DSurface() { return mRenderTarget; }

    friend class    D3D9VideoBufferManager; 
    friend class    Image;
    friend class    D3D9RenderSystem;
    friend class    D3D9RenderWindow;
    friend class    D3D9SwapChain;

protected:
    IDirect3DDevice9 *      mD3D9Device;
    IDirect3DSurface9 *     mRenderTarget;
	TexturePtr				mTexture;
};

}