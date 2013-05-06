#pragma once

#include "MWD3D11RenderSystemEntry.h"
#include "MWD3D11Texture.h"
#include "MWD3D11Geometry.h"
#include "MWD3D11Helper.h"


namespace Myway {

	class D3D11VideoBufferManager : public VideoBufferManager
	{
	public:
		D3D11VideoBufferManager();
		virtual ~D3D11VideoBufferManager();

		VertexBufferPtr CreateVertexBuffer(
			int size, int stride, USAGE usage, bool cpuAccess, const void * initData);

		IndexBufferPtr CreateIndexBuffer(
			int size, bool index16, USAGE usage, bool cpuAccess, const void * initData);

		void DestroyVertexBuffer(VertexBuffer * pVertexBuffer);
		void DestroyIndexBuffer(IndexBuffer * pIndexBuffer);


		TexturePtr CreateTexture(
			const TString128 & sName, int iWidth, int iHeight,
			int iMipLevel, FORMAT Format, USAGE usage, bool bRenderTarget);

		TexturePtr CreateVolumeTexture(
			const TString128 & sName, int iWidth, int iHeight, int iDepth,
			int iMipLevel, FORMAT Format, USAGE usage, bool bRenderTarget);

		TexturePtr CreateCubeTexture(
			const TString128 & sName, int iWidth,
			int iMipLevel, FORMAT Format, USAGE usage, bool bRenderTarget);

	protected:
		List<D3D11_VertexBuffer*> mVertexBuffers;
		List<D3D11_IndexBuffer*> mIndexBuffers;
	};

}