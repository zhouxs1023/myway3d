#pragma once

#include "MWD3D11RenderSystemEntry.h"
#include "MWRenderTarget.h"
#include "MWD3D11Helper.h"

namespace Myway
{

class RS_ENTRY D3D11RenderTarget : public RenderTarget
{
    DeclareRefObj();

public:
    D3D11RenderTarget();
    virtual ~D3D11RenderTarget();

    virtual void Stretch(Texture * p);

	friend class    Image;
    friend class    D3D11RenderSystem;
    friend class    D3D11RenderWindow;
	friend class    D3D11VideoBufferManager; 

protected:
	ID3D11RenderTargetView * mRenderTargetView;
	TexturePtr				mTexture;
};

}