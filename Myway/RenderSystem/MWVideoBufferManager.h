#pragma once

#include "MWVertexDeclaration.h"
#include "MWVertexBuffer.h"
#include "MWIndexBuffer.h"
#include "MWTexture.h"
#include "MWImage.h"
#include "MWRenderTarget.h"
#include "MWDepthStencil.h"

namespace Myway
{

class MW_ENTRY VideoBufferManager
{
    DECLARE_SINGLETON(VideoBufferManager);

public:
    VideoBufferManager();
    virtual ~VideoBufferManager();

    virtual VertexDeclarationPtr CreateVertexDeclaration() = 0;

    
    virtual VertexBufferPtr CreateVertexBuffer(
        int size, int stride, USAGE usage = USAGE_STATIC,
		bool cpuAccess = true, const void * initData = NULL) = 0;

    virtual IndexBufferPtr CreateIndexBuffer(
        int size, bool index16 = true, USAGE usage = USAGE_STATIC,
		bool cpuAccess = true, const void * initData = NULL) = 0;



    virtual TexturePtr CreateTexture(
        const TString128 & sName, int iWidth, int iHeight, int iMipLevel,
        FORMAT Format = FMT_X8R8G8B8, USAGE usage = USAGE_STATIC, bool bRenderTarget = false) = 0;

    inline TexturePtr CreateTextureRT(
        const TString128 & sName, int iWidth, int iHeight, FORMAT Format = FMT_X8R8G8B8)
	{
		return CreateTexture(sName, iWidth, iHeight, 1, Format, USAGE_DYNAMIC, true);
	}

    virtual TexturePtr CreateVolumeTexture(
        const TString128 & sName, int iWidth, int iHeight, int iDepth, int iMipLevel,
        FORMAT Format = FMT_X8R8G8B8, USAGE usage = USAGE_STATIC, bool bRenderTarget = false) = 0;

	virtual TexturePtr CreateCubeTexture(
        const TString128 & sName, int iWidth, int iMipLevel,
        FORMAT Format = FMT_X8R8G8B8, USAGE usage = USAGE_STATIC, bool bRenderTarget = false) = 0;

    virtual RenderTargetPtr CreateRenderTarget(
        const TString128 & sName, int iWidth, int iHeight,
        FORMAT Format, MULTI_SAMPLE Msaa) = 0;

	virtual RenderTargetPtr CreateRenderTarget(TexturePtr rtTex) = 0;

    virtual DepthStencilPtr CreateDepthStencil(
        const TString128 & sName, int iWidth, int iHeight,
        FORMAT Format, MULTI_SAMPLE Msaa) = 0;



    virtual TexturePtr Load2DTexture(const TString128 & name, const TString128 & source) = 0;
    virtual TexturePtr Load3DTexture(const TString128 & name, const TString128 & source) = 0;
    virtual TexturePtr LoadCubeTexture(const TString128 & name, const TString128 & source) = 0;

   

    virtual ImagePtr CreateImage(int iWidth, int iHeight, FORMAT Format) = 0;
    virtual ImagePtr LoadImage_(const TString128 & source, IMAGE_FILTER filter = IMAGE_FILTER_NONE) = 0;
    virtual ImagePtr LoadImageFromFile(const TString128 & filename, int w = -1, int h = -1, IMAGE_FILTER filter = IMAGE_FILTER_NONE) = 0;
    virtual void SaveImage(ImagePtr image, const TString128 & sImageFile, IMAGE_FILE_FORMAT Format) = 0;

    virtual void BitBlt(ImagePtr imageDest, TexturePtr texSrc, const Rect * pDest, const Rect * pSrc) = 0;
    virtual void BitBlt(TexturePtr texDest, ImagePtr imageSrc, const Rect * pDest, const Rect * pSrc) = 0;

    //

    /*
        Find
    */
    virtual TexturePtr FindTexture(const TString128 & sName) = 0;
    virtual DepthStencilPtr FindDepthStencil(const TString128 & sName) = 0;

    /*
        Destroy
    */
    virtual void DestroyVertexDeclaration(VertexDeclaration * pDecl) = 0;
    virtual void DestroyVertexBuffer(VertexBuffer * pVertexBuffer) = 0;
    virtual void DestroyIndexBuffer(IndexBuffer * pIndexBuffer) = 0;
    virtual void DestroyTexture(Texture * pTexture) = 0;
    virtual void DestroyRenderTarget(RenderTarget * pRenderTarget) = 0;
    virtual void DestroyDepthStencil(DepthStencil * pDepthStencil) = 0;
};

}