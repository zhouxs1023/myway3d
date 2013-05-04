#pragma once

#include "MWD3D11RenderSystemEntry.h"
#include "MWD3D11Helper.h"
#include "MWVertexBuffer.h"
#include "MWIndexBuffer.h"

namespace Myway
{

class RS_ENTRY D3D11_VertexBuffer : public VertexBuffer
{
    DeclareRefObj();

	friend class D3D11VideoBufferManager;

public:
    D3D11_VertexBuffer();
    virtual ~D3D11_VertexBuffer();

	void * Lock(int iOffsetBytes, int iLockSize, int LockFlags);
	void Unlock();

	ID3D11Buffer * GetD3D1Buffer() const { return mD3D11Buffer; }
	const D3D11_BUFFER_DESC & GetD3D11BufferDesc() const { return mD3D11BufferDesc; }

protected:
	D3D11_BUFFER_DESC mD3D11BufferDesc;
	ID3D11Buffer * mD3D11Buffer;
};


class RS_ENTRY D3D11_IndexBuffer : public IndexBuffer 
{
	DeclareRefObj();

	friend class D3D11VideoBufferManager;

public:
	D3D11_IndexBuffer();
	virtual ~D3D11_IndexBuffer();

	void * Lock(int iOffsetBytes, int iLockSize, int LockFlags);
	void Unlock();

	ID3D11Buffer * GetD3D1Buffer() const { return mD3D11Buffer; }
	const D3D11_BUFFER_DESC & GetD3D11BufferDesc() const { return mD3D11BufferDesc; }

protected:
	D3D11_BUFFER_DESC mD3D11BufferDesc;
	ID3D11Buffer * mD3D11Buffer;
};

}