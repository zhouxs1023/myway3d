#include "MWD3D9VertexDeclaration.h"
#include "MWD3D9RenderSystem.h"

namespace Myway
{

D3D9VertexDeclaration::D3D9VertexDeclaration(IDirect3DDevice9 * Device)
: mD3D9Device(Device),
  mD3D9VertexDecl(NULL)
{
}

D3D9VertexDeclaration::~D3D9VertexDeclaration()
{
    safe_release_com(mD3D9VertexDecl);
}

void D3D9VertexDeclaration::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyVertexDeclaration(this);
}


void D3D9VertexDeclaration::Init()
{

    D3DVERTEXELEMENT9 d3dve[MAX_VERTEX_ELEMENT];

    for (int i = 0; i < mElementCount; ++i)
    {
        d3dve[i].Stream = mElements[i].Stream;
        d3dve[i].Offset = mElements[i].Offset;
        d3dve[i].Type   = D3D9Mapping::GetD3DDeclType(mElements[i].Type);
        d3dve[i].Method = D3DDECLMETHOD_DEFAULT;
        d3dve[i].Usage  = D3D9Mapping::GetD3DDeclUsage(mElements[i].Usage);
        d3dve[i].UsageIndex = mElements[i].UsageIndex;
    }
    
    d3dve[mElementCount].Stream = 0xFF;
    d3dve[mElementCount].Offset = 0;
    d3dve[mElementCount].Type   = D3DDECLTYPE_UNUSED;
    d3dve[mElementCount].Method = 0;
    d3dve[mElementCount].Usage  = 0;
    d3dve[mElementCount].UsageIndex = 0;

    HRESULT hr = mD3D9Device->CreateVertexDeclaration(d3dve, &mD3D9VertexDecl);

    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: CreateVertexDeclaration failed. desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }
}

}
