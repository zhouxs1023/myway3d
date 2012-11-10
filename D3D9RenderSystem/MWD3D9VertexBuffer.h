#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWVertexBuffer.h"

namespace Myway
{

/* class: D3D9VertexBuffer
--------------------------------------------------------------------------------------
    @Remark:
        Vertex Buffer For Direct3D 9.0.
--------------------------------------------------------------------------------------
*/
class RS_ENTRY D3D9VertexBuffer : public VertexBuffer
{
    DeclareD3DResource();
    DeclareRefObj();

public:
    D3D9VertexBuffer(IDirect3DDevice9 * Device);
    ~D3D9VertexBuffer();

    void * Lock(int iOffsetBytes, int iLockSize, int LockFlags);
    void Unlock();

    IDirect3DVertexBuffer9 * GetD3DVertexBuffer() const;

    friend class D3D9VideoBufferManager;
    friend class D3D9RenderSystem;

protected:
    IDirect3DDevice9 *          mD3D9Device;
    IDirect3DVertexBuffer9 *    mD3D9VertexBuffer;
};

}