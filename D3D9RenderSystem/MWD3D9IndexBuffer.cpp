#include "MWD3D9IndexBuffer.h"
#include "MWVideoBufferManager.h"

namespace Myway
{


D3D9IndexBuffer::D3D9IndexBuffer(IDirect3DDevice9 * Device)
: mD3D9Device(Device)
{
}

D3D9IndexBuffer::~D3D9IndexBuffer()
{
    safe_release_com(mD3D9IndexBuffer);
}

void D3D9IndexBuffer::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyIndexBuffer(this);
}

void * D3D9IndexBuffer::Lock(int iOffsetBytes, int iLockSize, int LockFlag)
{
    void * pData = NULL;
    DWORD D3DLock = 0;

    if ((LockFlag & LOCK_DISCARD) && (mUsage & USAGE_DYNAMIC))
        D3DLock |= D3DLOCK_DISCARD;

    if (LockFlag & LOCK_NOOVERWRITE)
        D3DLock |= D3DLOCK_NOOVERWRITE;

    if (LockFlag & LOCK_READONLY)
        D3DLock |= D3DLOCK_READONLY;

    HRESULT hr = mD3D9IndexBuffer->Lock(iOffsetBytes, iLockSize, &pData, D3DLock);
    d_assert(hr == D3D_OK && "Lock index buffer failed.");

    return pData;
}

void D3D9IndexBuffer::Unlock()
{
    mD3D9IndexBuffer->Unlock();
}

void D3D9IndexBuffer::LostDevice()
{
    if (mUsage == USAGE_DYNAMIC)
        safe_release_com(mD3D9IndexBuffer);
}

void D3D9IndexBuffer::ResetDevice()
{
    if (mUsage == USAGE_DYNAMIC)
    {
        d_assert(mD3D9IndexBuffer == NULL);

        HRESULT hr;
        DWORD D3DUsage = D3D9Mapping::GetD3DUsage(mUsage);
        D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(mFormat);

        hr = mD3D9Device->CreateIndexBuffer(mSize,
                                              D3DUsage,
                                              D3DFormat,
                                              D3DPOOL_DEFAULT,
                                              &mD3D9IndexBuffer,
                                              NULL);
        if (FAILED(hr))
        {
            EXCEPTION("D3D Error: CreateIndexBuffer failed. desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
        }
    }
}

IDirect3DIndexBuffer9 * D3D9IndexBuffer::GetD3DIndexBuffer() const
{
    return mD3D9IndexBuffer;
}

}
