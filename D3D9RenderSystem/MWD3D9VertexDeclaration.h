#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWVertexDeclaration.h"

namespace Myway
{

/* class: D3D9VertexDeclaration
--------------------------------------------------------------------------------------
    @Remark:
        Vertex Declaration For Direct3D 9.0.
--------------------------------------------------------------------------------------
*/

class RS_ENTRY D3D9VertexDeclaration : public VertexDeclaration
{
    DeclareRefObj();

   friend class D3D9VideoBufferManager;
   friend class D3D9RenderSystem;

public: 
    D3D9VertexDeclaration(IDirect3DDevice9 * Device);
    ~D3D9VertexDeclaration();

    void Init();

    IDirect3DVertexDeclaration9 * GetD3DVertexDeclaration() const { return mD3D9VertexDecl; }

protected:
    IDirect3DDevice9 *                      mD3D9Device;
    IDirect3DVertexDeclaration9 *           mD3D9VertexDecl;
};

}
