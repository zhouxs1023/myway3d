#include "MWD3D11VideoBufferManager.h"
#include "MWD3D11Driver.h"

namespace Myway {

	D3D11VideoBufferManager::D3D11VideoBufferManager()
	{
	}

	D3D11VideoBufferManager::~D3D11VideoBufferManager()
	{
	}

	VertexBufferPtr D3D11VideoBufferManager::CreateVertexBuffer(int size, int stride, USAGE usage, bool cpuAccess, const void * initData)
	{
		d_assert (size > 0 && stride > 0);

		if (!cpuAccess)
		{
			d_assert(initData != NULL && usage == USAGE_STATIC);
		}

		ID3D11Device * device = D3D11Driver::Instance()->Device();
		ID3D11Buffer * pD3DBuffer = NULL;
		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		D3D11_USAGE D3DUsage = (usage == USAGE_STATIC ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);
		if (!cpuAccess)
			D3DUsage = D3D11_USAGE_IMMUTABLE;

		DWORD cpuAccessFlags = cpuAccess ? 0 : (D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE);
		if (usage == USAGE_DYNAMIC)
			cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;

		memset(&desc, 0, sizeof(desc));
		desc.ByteWidth = size;
		desc.StructureByteStride = stride;
		desc.Usage = D3DUsage;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = cpuAccessFlags;
		desc.MiscFlags = 0;

		data.pSysMem = initData;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		hr = device->CreateBuffer(&desc, initData ? &data : NULL, &pD3DBuffer);

		d_assert (SUCCEEDED(hr));

		D3D11_VertexBuffer * pVB = new D3D11_VertexBuffer();

		pVB->mD3D11BufferDesc = desc;
		pVB->mD3D11Buffer = pD3DBuffer;
		pVB->mSize = size;
		pVB->mUsage = usage;
		pVB->mStride = stride;

		mVertexBuffers.PushBack(pVB);

		return VertexBufferPtr(pVB);
	}

	IndexBufferPtr D3D11VideoBufferManager::CreateIndexBuffer(int size, bool index16, USAGE usage, bool cpuAccess, const void * initData)
	{
		d_assert (size > 0);

		if (!cpuAccess)
		{
			d_assert(initData != NULL && usage == USAGE_STATIC);
		}

		ID3D11Device * device = D3D11Driver::Instance()->Device();
		ID3D11Buffer * pD3DBuffer = NULL;
		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		D3D11_USAGE D3DUsage = (usage == USAGE_STATIC ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);
		if (!cpuAccess)
			D3DUsage = D3D11_USAGE_IMMUTABLE;

		DWORD cpuAccessFlags = cpuAccess ? 0 : (D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE);
		if (usage == USAGE_DYNAMIC)
			cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;

		memset(&desc, 0, sizeof(desc));
		desc.ByteWidth = size;
		desc.StructureByteStride = index16 ? 2 : 4;
		desc.Usage = D3DUsage;
		desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = cpuAccessFlags;
		desc.MiscFlags = 0;

		data.pSysMem = initData;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		hr = device->CreateBuffer(&desc, initData ? &data : NULL, &pD3DBuffer);

		d_assert (SUCCEEDED(hr));

		D3D11_IndexBuffer * pIb = new D3D11_IndexBuffer();

		pIb->mD3D11BufferDesc = desc;
		pIb->mD3D11Buffer = pD3DBuffer;
		pIb->mSize = size;
		pIb->mUsage = usage;
		pIb->mIndex16 = index16;

		mIndexBuffers.PushBack(pIb);

		return IndexBufferPtr(pIb);
	}

	void D3D11VideoBufferManager::DestroyVertexBuffer(VertexBuffer * pVertexBuffer)
	{
		List<D3D11_VertexBuffer*>::Iterator iter = mVertexBuffers.Begin();
		List<D3D11_VertexBuffer*>::Iterator end = mVertexBuffers.End();

		while (iter != end && (*iter) != pVertexBuffer)
		{
			++iter;
		}

		d_assert(iter != end);

		delete (*iter);
		mVertexBuffers.Erase(iter);
	}

	void D3D11VideoBufferManager::DestroyIndexBuffer(IndexBuffer * pIndexBuffer)
	{
		List<D3D11_IndexBuffer*>::Iterator iter;
		List<D3D11_IndexBuffer*>::Iterator end;

		iter = mIndexBuffers.Begin();
		end = mIndexBuffers.End();

		while (iter != end && (*iter) != pIndexBuffer)
		{
			++iter;
		}

		assert(iter != end);

		delete (*iter);
		mIndexBuffers.Erase(iter);
	}





	TexturePtr D3D11VideoBufferManager::CreateTexture(
		const TString128 & name, int width, int height,
		int mipLevel, FORMAT format, USAGE usage, bool bRenderTarget)
	{
		ID3D11Device * device = D3D11Driver::Instance()->Device();

		DWORD cpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		if (usage == USAGE_DYNAMIC && !bRenderTarget)
			cpuAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (bRenderTarget)
			cpuAccessFlags = 0;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = mipLevel;
		desc.ArraySize = 1;
		desc.Format = D3D11Helper::GetD3DFormat(format);

		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;


		desc.Usage = (usage == USAGE_STATIC) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 
	}
}