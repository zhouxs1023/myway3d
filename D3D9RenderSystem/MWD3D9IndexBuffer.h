#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9Mapping.h"
#include "MWIndexBuffer.h"

namespace Myway
{

/* class: D3D9IndexBuffer
--------------------------------------------------------------------------------------
    @Remark:
        Index Buffer For Direct3D 9.0.
--------------------------------------------------------------------------------------
*/
class RS_ENTRY D3D9IndexBuffer : public IndexBuffer
{
    DeclareRefObj();
    DeclareD3DResource();

    friend class D3D9VideoBufferManager;
    friend class D3D9RenderSystem;

public:
    D3D9IndexBuffer(IDirect3DDevice9 * Device);
    ~D3D9IndexBuffer();

    void * Lock(int iOffsetBytes, int uiLockSize, int LockFlags);
    void Unlock();

    IDirect3DIndexBuffer9 * GetD3DIndexBuffer() const;

protected:
    IDirect3DDevice9 *          mD3D9Device;
    IDirect3DIndexBuffer9 *     mD3D9IndexBuffer;
};

}