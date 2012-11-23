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

    /*
        Create Vertex Declaration.
    */
    virtual VertexDeclarationPtr CreateVertexDeclaration() = 0;

    /*
        Create Vertex Buffer.
        @Param:
            @iSize: size of buffer.
            @Usage: usage of buffer. (see USAGE enumerate)
            @Pool:  memory pool of buffer. (see POOL enumerate)
    */
    virtual VertexBufferPtr CreateVertexBuffer(
        int iSize, USAGE usage = USAGE_STATIC) = 0;

    /*
        Create Index Buffer.
        @Param:
            @iSize:     size of buffer.
            @Usage:     usage of buffer. (see USAGE enumerate)
            @Pool:      memory pool of buffer. (see POOL enumerate)
            @Foramt:    format of index buffer. (must be INDEX_16 or INDEX_32)
    */
    virtual IndexBufferPtr CreateIndexBuffer(
        int iSize,  FORMAT Format = FMT_INDEX16, USAGE usage = USAGE_STATIC) = 0;

    /*
        Create Texture.
        @Param:
            @sName:     texuter name       
            @iWidth:    width by pixel.
            @iHeight:   height by pixel.
            @iMipLevel: mipmap count.
            @Usage:     usage of index buffer. (see USAGE enumerate)
            @Pool:      memory pool of index buffer. (see POOL enumerate)
            @Format:    format of index buffer. (see FORMAT enumerate)
    */
    virtual TexturePtr CreateTexture(
        const TString128 & sName, int iWidth, int iHeight, int iMipLevel,
        FORMAT Format = FMT_X8R8G8B8, USAGE usage = USAGE_STATIC) = 0;

    virtual TexturePtr CreateTextureRT(
        const TString128 & sName, int iWidth, int iHeight, FORMAT Format = FMT_X8R8G8B8) = 0;


    /*
        Create Texture.
        @Param:
            @sName:     texuter name       
            @iWidth:    width by pixel.
            @iHeight:   height by pixel.
            @iDepth:    depth by pixel.
            @iMipLevel: mipmap count.
            @Usage:     usage of index buffer. (see USAGE enumerate)
            @Pool:      memory pool of index buffer. (see POOL enumerate)
            @Format:    format of index buffer. (see FORMAT enumerate)
    */
    virtual TexturePtr CreateVolumeTexture(
        const TString128 & sName, int iWidth, int iHeight, int iDepth, int iMipLevel,
        FORMAT Format = FMT_X8R8G8B8, USAGE usage = USAGE_STATIC) = 0;



    /*
        Create Texture.
        @Param:
            @sName:     texuter name       
            @iSize:     size by pixel.
            @iMipLevel: mipmap count.
            @Usage:     usage of index buffer. (see USAGE enumerate)
            @Pool:      memory pool of index buffer. (see POOL enumerate)
            @Format:    format of index buffer. (see FORMAT enumerate)
    */
    virtual TexturePtr CreateCubeTexture(
        const TString128 & sName, int iWidth, int iMipLevel,
        FORMAT Format = FMT_X8R8G8B8, USAGE usage = USAGE_STATIC) = 0;

    /*
        Create Render Target.
            @Param:
                @sName:     texuter name       
                @iWidth:    width by pixel.
                @iHeight:   height by pixel.
                @Format:    (see FORMAT enumerate)
                @Msaa:      msaa type.
    */
    virtual RenderTargetPtr CreateRenderTarget(
        const TString128 & sName, int iWidth, int iHeight,
        FORMAT Format, MULTI_SAMPLE Msaa) = 0;

    /*
        Create Depth Stencil Buffer
            @Param:
                @iWidth:    width by pixel.
                @iHeight:   height by pixel.
                @Format:    (see FORMAT enumerate)
                @Msaa:      msaa type.
    */
    virtual DepthStencilPtr CreateDepthStencil(
        const TString128 & sName, int iWidth, int iHeight,
        FORMAT Format, MULTI_SAMPLE Msaa) = 0;

    virtual TexturePtr Load2DTexture(const TString128 & name, const TString128 & source) = 0;
    virtual TexturePtr Load3DTexture(const TString128 & name, const TString128 & source) = 0;
    virtual TexturePtr LoadCubeTexture(const TString128 & name, const TString128 & source) = 0;

    /*
        Create a image
    */
    virtual ImagePtr CreateImage(int iWidth, int iHeight, FORMAT Format) = 0;
    virtual ImagePtr LoadImage_(const TString128 & source, IMAGE_FILTER filter = IMAGE_FILTER_NONE) = 0;
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