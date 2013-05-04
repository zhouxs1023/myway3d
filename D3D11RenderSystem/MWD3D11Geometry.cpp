#include "MWD3D11Geometry.h"
#include "MWD3D11Driver.h"

namespace Myway {

	D3D11_VertexBuffer::D3D11_VertexBuffer()
		: mD3D11Buffer(NULL)
	{
	}

	D3D11_VertexBuffer::~D3D11_VertexBuffer()
	{
	}

	void D3D11_VertexBuffer::DeleteSelf()
	{
		VideoBufferManager::Instance()->DestroyVertexBuffer(this);
	}

	void * D3D11_VertexBuffer::Lock(int offset, int iLockSize, int LockFlags)
	{
		d_assert (mD3D11BufferDesc.Usage != D3D11_USAGE_IMMUTABLE &&
			mD3D11BufferDesc.CPUAccessFlags != 0);

		ID3D11DeviceContext * context = D3D11Driver::Instance()->Context();

		D3D11_MAP D3DMapType = D3D11_MAP_WRITE;

		if ((LockFlags = LOCK_DISCARD) && (mUsage == USAGE_DYNAMIC))
			D3DMapType = D3D11_MAP_WRITE_DISCARD;

		if (LockFlags = LOCK_NOOVERWRITE)
			D3DMapType = D3D11_MAP_WRITE_NO_OVERWRITE;

		if (LockFlags = LOCK_READONLY)
			D3DMapType = D3D11_MAP_READ;

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		mappedSubResource.pData = NULL;

		HRESULT hr = context->Map(mD3D11Buffer, 0, D3DMapType, 0, &mappedSubResource);

		d_assert (SUCCEEDED(hr));

		return static_cast<char*>(mappedSubResource.pData) + offset;
	}

	void D3D11_VertexBuffer::Unlock()
	{
		ID3D11DeviceContext * context = D3D11Driver::Instance()->Context();

		context->Unmap(mD3D11Buffer, 0);
	}








	D3D11_IndexBuffer::D3D11_IndexBuffer()
		: mD3D11Buffer(NULL)
	{
	}

	D3D11_IndexBuffer::~D3D11_IndexBuffer()
	{
	}

	void D3D11_IndexBuffer::DeleteSelf()
	{
		VideoBufferManager::Instance()->DestroyIndexBuffer(this);
	}

	void * D3D11_IndexBuffer::Lock(int offset, int iLockSize, int LockFlags)
	{
		d_assert (mD3D11BufferDesc.Usage != D3D11_USAGE_IMMUTABLE &&
			mD3D11BufferDesc.CPUAccessFlags != 0);

		ID3D11DeviceContext * context = D3D11Driver::Instance()->Context();

		D3D11_MAP D3DMapType = D3D11_MAP_WRITE;

		if (LockFlags = LOCK_DISCARD)
			D3DMapType = D3D11_MAP_WRITE_DISCARD;

		if (LockFlags = LOCK_NOOVERWRITE)
			D3DMapType = D3D11_MAP_WRITE_NO_OVERWRITE;

		if (LockFlags = LOCK_READONLY)
			D3DMapType = D3D11_MAP_READ;

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		mappedSubResource.pData = NULL;

		HRESULT hr = context->Map(mD3D11Buffer, 0, D3DMapType, 0, &mappedSubResource);

		d_assert (SUCCEEDED(hr));

		return static_cast<char*>(mappedSubResource.pData) + offset;
	}

	void D3D11_IndexBuffer::Unlock()
	{
		ID3D11DeviceContext * context = D3D11Driver::Instance()->Context();

		context->Unmap(mD3D11Buffer, 0);
	}
}