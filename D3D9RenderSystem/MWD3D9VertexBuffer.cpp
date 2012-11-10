#include "MWD3D9VertexBuffer.h"
#include "MWVideoBufferManager.h"

namespace Myway
{

/* : D3D9VertexBuffer
--------------------------------------------------------------------------------------
    @Remark:
        Vertex Buffer for Direct3D 9.0
--------------------------------------------------------------------------------------
*/
D3D9VertexBuffer::D3D9VertexBuffer(IDirect3DDevice9 * Device)
: mD3D9Device(Device),
  mD3D9VertexBuffer(NULL)
{
}

D3D9VertexBuffer::~D3D9VertexBuffer()
{
    safe_release_com(mD3D9VertexBuffer);
}

void D3D9VertexBuffer::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyVertexBuffer(this);
}

void * D3D9VertexBuffer::Lock(int iOffsetBytes, int iLockSize, int LockFlag)
{
    void * pData = NULL;
    DWORD D3DLock = 0;

    if ((LockFlag & LOCK_DISCARD) && (mUsage == USAGE_DYNAMIC))
        D3DLock |= D3DLOCK_DISCARD;

    if (LockFlag & LOCK_NOOVERWRITE)
        D3DLock |= D3DLOCK_NOOVERWRITE;

    if (LockFlag & LOCK_READONLY)
        D3DLock |= D3DLOCK_READONLY;

    HRESULT hr = mD3D9VertexBuffer->Lock((UINT)iOffsetBytes, (UINT)iLockSize, &pData, D3DLock);
    d_assert(hr == D3D_OK && "Lock vertex buffer failed.");

    return pData;
}

void D3D9VertexBuffer::Unlock()
{
    mD3D9VertexBuffer->Unlock();
}


IDirect3DVertexBuffer9 * D3D9VertexBuffer::GetD3DVertexBuffer() const
{
    return mD3D9VertexBuffer;
}

void D3D9VertexBuffer::LostDevice()
{
    if (mUsage == USAGE_DYNAMIC)
    {
        safe_release_com(mD3D9VertexBuffer);
    }
}

void D3D9VertexBuffer::ResetDevice()
{
    if (mUsage == USAGE_DYNAMIC)
    {
        HRESULT hr = D3D_OK;
        DWORD D3DUsage = D3D9Mapping::GetD3DUsage(mUsage);

        hr = mD3D9Device->CreateVertexBuffer(mSize, D3DUsage, 0, D3DPOOL_DEFAULT, &mD3D9VertexBuffer, NULL);

        if (FAILED(hr))
        {
            EXCEPTION("D3D Error: CreateVertexBuffer failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
        }
    }
}


}
