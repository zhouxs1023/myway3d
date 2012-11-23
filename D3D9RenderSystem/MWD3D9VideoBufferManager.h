#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "MWD3D9VertexDeclaration.h"
#include "MWD3D9VertexBuffer.h"
#include "MWD3D9IndexBuffer.h"
#include "MWD3D9Texture.h"
#include "MWD3D9Image.h"
#include "MWD3D9RenderTarget.h"
#include "MWD3D9DepthStencil.h"
#include "MWVideoBufferManager.h"


namespace Myway
{

/* class: D3D9VideoBufferManager
--------------------------------------------------------------------------------------
@Remark:
    Hardware Buffer Manager For Direct3D 9.0.
--------------------------------------------------------------------------------------
*/

class D3D9VideoBufferManager : public VideoBufferManager
{
    typedef HashMap<TString128, D3D9Texture*, TString128::hash_t>       Texture2DMap;
    typedef HashMap<TString128, D3D9CubeTexture*, TString128::hash_t>   TextureCubeMap;
    typedef HashMap<TString128, D3D9VolumeTexture*, TString128::hash_t> TextureVolMap;
    typedef HashMap<TString128, D3D9RenderTarget*, TString128::hash_t>  TextureRTMap;
    typedef HashMap<TString128, D3D9DepthStencil*, TString128::hash_t>  TextureDSMap;

public:
    D3D9VideoBufferManager(IDirect3DDevice9 * Device);
    ~D3D9VideoBufferManager();

    
    VertexDeclarationPtr CreateVertexDeclaration();

    VertexBufferPtr CreateVertexBuffer(
        int iSize, USAGE usage);

    IndexBufferPtr CreateIndexBuffer(
        int iSize, FORMAT Format, USAGE usage);

    TexturePtr CreateTexture(
        const TString128 & sName, int iWidth, int iHeight,
        int iMipLevel, FORMAT Format, USAGE usage);

    virtual TexturePtr CreateTextureRT(
        const TString128 & sName, int iWidth, int iHeight, FORMAT Format);

    TexturePtr CreateVolumeTexture(
        const TString128 & sName, int iWidth, int iHeight, int iDepth,
        int iMipLevel, FORMAT Format, USAGE usage);

    TexturePtr CreateCubeTexture(
        const TString128 & sName, int iWidth, int iMipLevel,
        FORMAT Format, USAGE usage);

    RenderTargetPtr CreateRenderTarget(
        const TString128 & sName, int iWidth, int iHeight,
        FORMAT Format, MULTI_SAMPLE Msaa);

    DepthStencilPtr CreateDepthStencil(
        const TString128 & sName, int iWidth, int iHeight,
        FORMAT Format, MULTI_SAMPLE Msaa);
   
    TexturePtr Load2DTexture(const TString128 & name, const TString128 & source);
    TexturePtr Load3DTexture(const TString128 & name, const TString128 & source);
    TexturePtr LoadCubeTexture(const TString128 & name, const TString128 & source);

   virtual ImagePtr CreateImage(int iWidth, int iHeight, FORMAT Format);
   virtual ImagePtr LoadImage_(const TString128 & source, IMAGE_FILTER filter);
   virtual void SaveImage(ImagePtr image, const TString128 & sImageFile, IMAGE_FILE_FORMAT Format);

   virtual void BitBlt(ImagePtr imageDest, TexturePtr texSrc, const Rect * pDest, const Rect * pSrc);
   virtual void BitBlt(TexturePtr texDest, ImagePtr imageSrc, const Rect * pDest, const Rect * pSrc);

   TexturePtr FindTexture(const TString128 & sName);
   DepthStencilPtr FindDepthStencil(const TString128 & sName);

   void DestroyVertexDeclaration(VertexDeclaration * pDecl);
   void DestroyVertexBuffer(VertexBuffer * pVertexBuffer);
   void DestroyIndexBuffer(IndexBuffer * pIndexBuffer);
   void DestroyTexture(Texture * pTexture);
   void DestroyRenderTarget(RenderTarget * pRenderTarget);
   void DestroyDepthStencil(DepthStencil * pDepthStencil);

   void LostDevice();
   void ResetDevice();

protected:
    IDirect3DDevice9 *                              mD3D9Device;
    List<D3D9VertexBuffer*>                         mVertexBuffers;
    List<D3D9IndexBuffer*>                          mIndexBuffers;
    Texture2DMap                                    mTextures;
    TextureVolMap                                   mVolumeTextures;
    TextureCubeMap                                  mCubeTextures;
    TextureRTMap                                    mRenderTargets;
    TextureDSMap                                    mDepthStencils;
};



}