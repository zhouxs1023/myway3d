#pragma once

#include "MWD3D11RenderSystemEntry.h"
#include "MWD3D11Helper.h"
#include "MWDepthStencil.h"

namespace Myway
{

class RS_ENTRY D3D11DepthStencil : public DepthStencil
{
    DeclareRefObj();

public:
    D3D11DepthStencil();
    virtual ~D3D11DepthStencil();

    friend class    D3D11VideoBufferManager;
    friend class    D3D11RenderSystem;
    friend class    D3D11RenderWindow;

protected:
    ID3D11DepthStencilView * mDepthStencilView;
};

}