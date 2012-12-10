#include "MWD3D9VideoBufferManager.h"
#include "MWD3D9RenderSystem.h"
#include "MWImage.h"
#include "MWResourceManager.h"
#include "Engine.h"

namespace Myway
{

D3D9VideoBufferManager::D3D9VideoBufferManager(IDirect3DDevice9 * Device)
: mD3D9Device(Device)
{
}

D3D9VideoBufferManager::~D3D9VideoBufferManager()
{
    assert(mVertexBuffers.Size() == 0 && 
           mIndexBuffers.Size() == 0 &&
           mTextures.Size() == 0 &&
           mVolumeTextures.Size() == 0 &&
           mCubeTextures.Size() == 0 &&
           mRenderTargets.Size() == 0 &&
           mDepthStencils.Size() == 0);
}

VertexDeclarationPtr D3D9VideoBufferManager::CreateVertexDeclaration()
{
    VertexDeclaration * pVertexDecl = new D3D9VertexDeclaration(mD3D9Device);
    return VertexDeclarationPtr(pVertexDecl);
}

VertexBufferPtr D3D9VideoBufferManager::CreateVertexBuffer(int iSize, USAGE Usage)
{
    assert (iSize);

    HRESULT hr;
    DWORD D3DUsage = D3D9Mapping::GetD3DUsage(Usage);
    D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(Usage);
    IDirect3DVertexBuffer9 * pD3DVB;

    hr = mD3D9Device->CreateVertexBuffer(iSize, D3DUsage, 0, D3DPool, &pD3DVB, NULL);

    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: CreateVertexBuffer failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }

    D3D9VertexBuffer * pVB = new D3D9VertexBuffer(mD3D9Device);

    pVB->mD3D9VertexBuffer = pD3DVB;
    pVB->mSize = iSize;
    pVB->mUsage = Usage;

    mVertexBuffers.PushBack(pVB);

    return VertexBufferPtr(pVB);
}

IndexBufferPtr D3D9VideoBufferManager::CreateIndexBuffer(int iSize, FORMAT Format, USAGE Usage)
{
    assert (iSize);

    HRESULT hr;
    DWORD D3DUsage = D3D9Mapping::GetD3DUsage(Usage);
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
    D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(Usage);
    IDirect3DIndexBuffer9 * pD3DIB;

    hr = mD3D9Device->CreateIndexBuffer(iSize, D3DUsage, D3DFormat, D3DPool, &pD3DIB, NULL);
                                         
    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: CreateIndexBuffer failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }

   D3D9IndexBuffer * pIB = new D3D9IndexBuffer(mD3D9Device);

   pIB->mD3D9IndexBuffer = pD3DIB;
   pIB->mSize = iSize;
   pIB->mUsage = Usage;
   pIB->mFormat = Format;

   mIndexBuffers.PushBack(pIB);

   return IndexBufferPtr(pIB);
}

TexturePtr D3D9VideoBufferManager::CreateTexture(const TString128 & sName,
                                                 int iWidth,
                                                 int iHeight,
                                                 int iMipLevels,
                                                 FORMAT Format,
                                                 USAGE Usage)
{
    d_assert (iWidth && iHeight);
    d_assert (FindTexture(sName).IsNull());

    if (!RenderSystem::Instance()->CheckTextureFormat(Format, Usage))
    {
        EXCEPTION("Your device can't support texture format '" + D3D9Mapping::GetFormatString(Format) + 
                  "' for usage '" + D3D9Mapping::GetUsageString(Usage));
    }

    HRESULT hr = D3D_OK;
    DWORD D3DUsage = D3D9Mapping::GetD3DUsage(Usage);
    D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(Usage);
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
    IDirect3DTexture9 * pD3D9Texture = NULL;

	if (iMipLevels == 0)
	{
		if (!RenderSystem::Instance()->CheckTextureFormat(Format, Usage, true))
		{
			EXCEPTION("Your driver can't support texture format '" + D3D9Mapping::GetFormatString(Format) + 
				"' for usage '" + D3D9Mapping::GetUsageString(Usage) + "' for auto generater mipmap");
		}

		D3DUsage |= D3DUSAGE_AUTOGENMIPMAP;
	}
   
    hr = mD3D9Device->CreateTexture(iWidth, 
                                    iHeight,
                                    iMipLevels,
                                    D3DUsage,
                                    D3DFormat,
                                    D3DPool,
                                    &pD3D9Texture,
                                    NULL);

    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: CreateTexture failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }

    D3D9Texture * pTexture = new D3D9Texture(mD3D9Device);

    pTexture->mName = sName;
    pTexture->mD3D9Texture = pD3D9Texture;
    pTexture->mWidth = iWidth;
    pTexture->mHeight = iHeight;
    pTexture->mDepth = 1;
    pTexture->mUsage = Usage;
    pTexture->mFormat = Format;
    pTexture->mType = TEXTYPE_2D;
    pTexture->mMipLevels = iMipLevels;

    mTextures.Insert(pTexture->GetName(), pTexture);

    return TexturePtr(pTexture);
}


TexturePtr D3D9VideoBufferManager::CreateTextureRT(const TString128 & sName, int iWidth, int iHeight, FORMAT Format)
{
    int rWidth = iWidth, rHeight = iHeight;

    if (rWidth == -1 && rHeight == -1)
    {
        const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();
        rWidth = dp->Width;
        rHeight = dp->Height;
    }

    d_assert (rWidth && rHeight);
    d_assert (FindTexture(sName).IsNull());

    if (!RenderSystem::Instance()->CheckRenderTargetFormat(Format))
    {
        EXCEPTION("Your device can't support texture format '" + D3D9Mapping::GetFormatString(Format) + 
            "' for render target");
    }

    HRESULT hr = D3D_OK;
    DWORD D3DUsage = D3DUSAGE_RENDERTARGET;
    D3DPOOL D3DPool = D3DPOOL_DEFAULT;
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
    IDirect3DTexture9 * pD3D9Texture = NULL;


    hr = mD3D9Device->CreateTexture(
        rWidth, rHeight, 1,
        D3DUsage, D3DFormat, D3DPool,
        &pD3D9Texture, NULL);

    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: CreateTexture failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }

    D3D9Texture * pTexture = new D3D9Texture(mD3D9Device);

    pTexture->mName = sName;
    pTexture->mD3D9Texture = pD3D9Texture;
    pTexture->mWidth = iWidth;
    pTexture->mHeight = iHeight;
    pTexture->mDepth = 1;
    pTexture->mUsage = USAGE_DYNAMIC;
    pTexture->mFormat = Format;
    pTexture->mType = TEXTYPE_RENDERTARGET;
    pTexture->mMipLevels = 1;

    mTextures.Insert(pTexture->GetName(), pTexture);

    return TexturePtr(pTexture);
}


TexturePtr D3D9VideoBufferManager::CreateVolumeTexture(const TString128 & sName,
                                                       int iWidth,
                                                       int iHeight,
                                                       int iDepth,
                                                       int iMipLevel,
                                                       FORMAT Format,
                                                       USAGE Usage)
{
    d_assert(FindTexture(sName).IsNull());
    
    if (!RenderSystem::Instance()->CheckTextureFormat(Format, Usage))
    {
        EXCEPTION("Your device can't support texture format '" + D3D9Mapping::GetFormatString(Format) + 
                  "' for usage '" + D3D9Mapping::GetUsageString(Usage));
    }

    HRESULT hr = D3D_OK;
    DWORD D3DUsage = D3D9Mapping::GetD3DUsage(Usage);
    D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(Usage);
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
    IDirect3DVolumeTexture9 * pD3D9VolumeTexture = NULL;

    hr = mD3D9Device->CreateVolumeTexture(iWidth, iHeight, iDepth, iMipLevel, 
                                          D3DUsage, D3DFormat, D3DPool,
                                          &pD3D9VolumeTexture, NULL);
    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: CreateVolumeTexture failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }

    D3D9VolumeTexture * pTexture = new D3D9VolumeTexture(mD3D9Device);

    pTexture->mName = sName;
    pTexture->mD3D9VolumeTexture = pD3D9VolumeTexture;
    pTexture->mWidth = iWidth;
    pTexture->mHeight = iHeight;
    pTexture->mDepth = iDepth;
    pTexture->mUsage = Usage;
    pTexture->mFormat = Format;
    pTexture->mType = TEXTYPE_3D;
    pTexture->mMipLevels = iMipLevel;

    mVolumeTextures.Insert(pTexture->GetName(), pTexture);

    return TexturePtr(pTexture);
}

TexturePtr D3D9VideoBufferManager::CreateCubeTexture(const TString128 & sName,
                                                     int iWidth,
                                                     int iMipLevel,
                                                     FORMAT Format,
                                                     USAGE Usage)
{
    d_assert(FindTexture(sName).IsNull());
    
    if (!RenderSystem::Instance()->CheckTextureFormat(Format, Usage))
    {
        EXCEPTION("Your device can't support texture format '" + D3D9Mapping::GetFormatString(Format) + 
                  "' for usage '" + D3D9Mapping::GetUsageString(Usage));
    }

    HRESULT hr = D3D_OK;
    DWORD D3DUsage = D3D9Mapping::GetD3DUsage(Usage);
    D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(Usage);
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(Format);
    IDirect3DCubeTexture9 * pD3D9CubeTexture = NULL;

    hr = mD3D9Device->CreateCubeTexture(
        iWidth, iMipLevel, D3DUsage, D3DFormat,
        D3DPool, &pD3D9CubeTexture, NULL);

    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: CreateCubeTexture failed, desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }

    D3D9CubeTexture * pTexture = new D3D9CubeTexture(mD3D9Device);

    pTexture->mName = sName;
    pTexture->mD3D9CubeTexture = pD3D9CubeTexture;
    pTexture->mWidth = iWidth;
    pTexture->mHeight = iWidth;
    pTexture->mDepth = 1;
    pTexture->mUsage = Usage;
    pTexture->mFormat = Format;
    pTexture->mType = TEXTYPE_CUBE;
    pTexture->mMipLevels = iMipLevel;

    mCubeTextures.Insert(pTexture->GetName(), pTexture);

    return TexturePtr(pTexture);
}

RenderTargetPtr D3D9VideoBufferManager::CreateRenderTarget(const TString128 & sName,
                                                           int iWidth,
                                                           int iHeight,
                                                           FORMAT RenderTargetFormat,
                                                           MULTI_SAMPLE MSAA)
{
    d_assert(FindTexture(sName).IsNull());

    if (!RenderSystem::Instance()->CheckRenderTargetFormat(RenderTargetFormat))
    {
        EXCEPTION("Your device can't support texture format '" + 
                  D3D9Mapping::GetFormatString(RenderTargetFormat) + "' for render target.");
    }

    if (!RenderSystem::Instance()->CheckMSAAFormat(RenderTargetFormat, MSAA))
    {
        EXCEPTION("Your device can't support render target format '" + 
                  D3D9Mapping::GetFormatString(RenderTargetFormat) + "' for msaa '" +
                  D3D9Mapping::GetMSAAString(MSAA) + "'.");
    }

    int rWidth = iWidth, rHeight = iHeight; 

    if (rWidth == -1 || rHeight == -1)
    {
        rWidth = Engine::Instance()->GetDeviceProperty()->Width;
        rHeight = Engine::Instance()->GetDeviceProperty()->Height;
    }

    HRESULT hr = D3D_OK;
    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(RenderTargetFormat);
    D3DMULTISAMPLE_TYPE MultiSampleType = D3D9Mapping::GetD3DMultiSampleType(MSAA);
    DWORD MultiSampleQuality = 0;
    IDirect3DTexture9 * pD3D9Texture = NULL;
    IDirect3DSurface9 * pD3D9RenderTarget = NULL;

   
       
    hr = mD3D9Device->CreateRenderTarget(rWidth, rHeight, D3DFormat, MultiSampleType,
                                         MultiSampleQuality, FALSE, &pD3D9RenderTarget, NULL);

    D3DErrorExceptionFunction(CreateRenderTarget, hr);
   

    D3D9RenderTarget * pTexture = new D3D9RenderTarget(mD3D9Device);

    pTexture->mName = sName;
    pTexture->mRenderTarget = pD3D9RenderTarget;
    pTexture->mWidth = iWidth;
    pTexture->mHeight = iHeight;
    pTexture->mFormat = RenderTargetFormat;
    pTexture->mMSAA= MSAA;

    mRenderTargets.Insert(pTexture->GetName(), pTexture);;

    return RenderTargetPtr(pTexture);
}

DepthStencilPtr D3D9VideoBufferManager::CreateDepthStencil(const TString128 & sName,
                                                           int iWidth,
                                                           int iHeight,
                                                           FORMAT DepthStencilFormat,
                                                           MULTI_SAMPLE MSAA)
{
    if (!RenderSystem::Instance()->CheckMSAAFormat(DepthStencilFormat, MSAA))
    {
        EXCEPTION("Your device can't support depth buffer format '" + 
                  D3D9Mapping::GetFormatString(DepthStencilFormat) + "' for msaa '" +
                  D3D9Mapping::GetMSAAString(MSAA) + "'.");
    }

    int rWidth = iWidth, rHeight = iHeight; 

    if (rWidth == -1 || rHeight == -1)
    {
        rWidth = Engine::Instance()->GetDeviceProperty()->Width;
        rHeight = Engine::Instance()->GetDeviceProperty()->Height;
    }

    HRESULT hr = D3D_OK;

    D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(DepthStencilFormat);
    D3DMULTISAMPLE_TYPE MultiSampleType = D3D9Mapping::GetD3DMultiSampleType(MSAA);
    DWORD MultiSampleQuality = 0;
    IDirect3DSurface9 * pD3D9DepthStencil = NULL;

    D3D9DepthStencil * pDepthStencil = new D3D9DepthStencil(mD3D9Device);

    hr = mD3D9Device->CreateDepthStencilSurface(rWidth, rHeight, D3DFormat,
                                                MultiSampleType, MultiSampleQuality, TRUE,
                                                &pD3D9DepthStencil, NULL);

    D3DErrorExceptionFunction(CreateDepthStencilSurface, hr);

    pDepthStencil->mName = sName;
    pDepthStencil->mWidth = iWidth;
    pDepthStencil->mHeight = iHeight;
    pDepthStencil->mFormat = DepthStencilFormat;
    pDepthStencil->mMSAA = MSAA;
    pDepthStencil->mDepthStencil = pD3D9DepthStencil;

    mDepthStencils.Insert(pDepthStencil->GetName(), pDepthStencil);

    return DepthStencilPtr(pDepthStencil);
}

TexturePtr D3D9VideoBufferManager::Load2DTexture(const TString128 & name, const TString128 & source)
{
    TexturePtr t = FindTexture(name);

    if (t != NULL)
        return t;

    D3D9Texture * p = new D3D9Texture(mD3D9Device);

    p->mName = name;
    p->mWidth = 0;
    p->mHeight = 0;
    p->mDepth = 1;
    p->mMipLevels = 0;
    p->mUsage = USAGE_STATIC;
    p->mFormat = FMT_UNKNOWN;
    p->mType = TEXTYPE_2D;
    p->mSourceName = source;
    p->mGroupName = "";

    mTextures.Insert(name, p);

    p->Load();

    return p;
}

TexturePtr D3D9VideoBufferManager::Load3DTexture(const TString128 & name, const TString128 & source)
{
    TexturePtr t = FindTexture(name);

    if (t != NULL)
        return t;

    D3D9VolumeTexture * p = new D3D9VolumeTexture(mD3D9Device);

    p->mName = name;
    p->mWidth = 0;
    p->mHeight = 0;
    p->mDepth = 1;
    p->mMipLevels = 0;
    p->mUsage = USAGE_STATIC;
    p->mFormat = FMT_UNKNOWN;
    p->mType = TEXTYPE_3D;
    p->mSourceName = source;
    p->mGroupName = "";

    mVolumeTextures.Insert(name, p);

    p->Load();

    return p;
}

TexturePtr D3D9VideoBufferManager::LoadCubeTexture(const TString128 & name, const TString128 & source)
{
    TexturePtr t = FindTexture(name);

    if (t != NULL)
        return t;

    D3D9CubeTexture * p = new D3D9CubeTexture(mD3D9Device);

    p->mName = name;
    p->mWidth = 0;
    p->mHeight = 0;
    p->mDepth = 1;
    p->mMipLevels = 0;
    p->mUsage = USAGE_STATIC;
    p->mFormat = FMT_UNKNOWN;
    p->mType = TEXTYPE_CUBE;
    p->mSourceName = source;
    p->mGroupName = "";

    mCubeTextures.Insert(name, p);

    p->Load();

    return p;
}


ImagePtr D3D9VideoBufferManager::CreateImage(int iWidth, int iHeight, FORMAT Format)
{
    IDirect3DTexture9 * texture;
    HRESULT hr = D3DXCreateTexture(mD3D9Device, 
                                   iWidth,
                                   iHeight,
                                   1,
                                   0,
                                   D3D9Mapping::GetD3DFormat(Format),
                                   D3DPOOL_SYSTEMMEM,
                                   &texture);

    D3DErrorExceptionFunction(D3DXCreateTexture, hr);

    IDirect3DSurface9 * surface;
    texture->GetSurfaceLevel(0, &surface);
    D3DSURFACE_DESC desc;
    surface->GetDesc(&desc);

    D3D9Image * image = new D3D9Image();

    image->mWidth = desc.Width;
    image->mHeight = desc.Height;
    image->mSrcWidth = iWidth;
    image->mSrcHeight = iHeight;
    image->mFormat = D3D9Mapping::GetFormat(desc.Format);
    image->mMipmapLevel = texture->GetLevelCount();
    image->mD3D9Texture = texture;

    surface->Release();

    return ImagePtr(image);
}

ImagePtr D3D9VideoBufferManager::LoadImage_(const TString128 & source, IMAGE_FILTER filter)
{
    DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source.c_str());

    if (stream == NULL)
    {
        LOG_PRINT_FORMAT("image '%s' can't find\n", source.c_str());
        return NULL;
    }

    D3DXIMAGE_INFO ImgInfo;

    IDirect3DTexture9 * texture;
    HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(mD3D9Device,
                                                     stream->GetData(),
                                                     stream->Size(),
                                                     D3DX_DEFAULT,
                                                     D3DX_DEFAULT,
                                                     1,
                                                     0,
                                                     D3DFMT_UNKNOWN,
                                                     D3DPOOL_SCRATCH,
                                                     D3D9Mapping::GetD3DXFilter(filter),
                                                     D3DX_DEFAULT,
                                                     0,
                                                     &ImgInfo,
                                                     NULL,
                                                     &texture);

    D3DErrorExceptionFunction(D3DXCreateTextureFromInMemoryEx, hr);

    D3DSURFACE_DESC desc;
    texture->GetLevelDesc(0, &desc);

    D3D9Image * image = new D3D9Image();

    image->mWidth = desc.Width;
    image->mHeight = desc.Height;
    image->mSrcWidth = ImgInfo.Width;
    image->mSrcHeight = ImgInfo.Height;
    image->mFormat = D3D9Mapping::GetFormat(desc.Format);
    image->mMipmapLevel = texture->GetLevelCount();
    image->mD3D9Texture = texture;

    return ImagePtr(image);
}

ImagePtr D3D9VideoBufferManager::LoadImageFromFile(const TString128 & filename, int w, int h, IMAGE_FILTER filter)
{
	D3DXIMAGE_INFO ImgInfo;

	IDirect3DTexture9 * texture;
	HRESULT hr = D3DXCreateTextureFromFileEx(mD3D9Device,
		filename.c_str(),
		w,
		h,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_SCRATCH,
		D3D9Mapping::GetD3DXFilter(filter),
		D3DX_DEFAULT,
		0,
		&ImgInfo,
		NULL,
		&texture);

	D3DErrorExceptionFunction(D3DXCreateTextureFromFileEx, hr);

	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);

	D3D9Image * image = new D3D9Image();

	image->mWidth = desc.Width;
	image->mHeight = desc.Height;
	image->mSrcWidth = ImgInfo.Width;
	image->mSrcHeight = ImgInfo.Height;
	image->mFormat = D3D9Mapping::GetFormat(desc.Format);
	image->mMipmapLevel = texture->GetLevelCount();
	image->mD3D9Texture = texture;

	return ImagePtr(image);
}

void D3D9VideoBufferManager::SaveImage(ImagePtr image, const TString128 & sImageFile, IMAGE_FILE_FORMAT Format)
{
    D3D9Image * img = (D3D9Image*)image.c_ptr();

    D3DXSaveTextureToFile(sImageFile.c_str(), 
                          D3D9Mapping::GetD3DXImageFileFormat(Format),
                          img->_MyTexture(),
                          NULL);
}

void D3D9VideoBufferManager::BitBlt(ImagePtr imageDest, TexturePtr texSrc, const Rect * pDest, const Rect * pSrc)
{
    IDirect3DTexture9 * pD3DDestTexture = NULL;
    IDirect3DTexture9 * pD3DSrcTexture = NULL;
    IDirect3DSurface9 * pDestSurface;
    IDirect3DSurface9 * pSrcSurface;

    D3D9Image * img = (D3D9Image*)imageDest.c_ptr();
    pD3DDestTexture = img->_MyTexture();

    if (texSrc->GetTextureType() == TEXTYPE_2D || texSrc->GetTextureType() == TEXTYPE_RENDERTARGET)
    {
        pD3DSrcTexture = static_cast<D3D9Texture*>(texSrc.c_ptr())->mD3D9Texture;
    }

    assert(pD3DSrcTexture);

    const RECT * pDestRect = (const RECT *)(pDest);
    const RECT * pSrcRect = (const RECT *)(pSrc);

    pD3DDestTexture->GetSurfaceLevel(0, &pDestSurface);
    pD3DSrcTexture->GetSurfaceLevel(0, &pSrcSurface);

    D3DXLoadSurfaceFromSurface(pDestSurface, 
                               NULL,
                               pDestRect,
                               pSrcSurface,
                               NULL,
                               pSrcRect,
                               D3DX_DEFAULT,
                               0);
}

void D3D9VideoBufferManager::BitBlt(TexturePtr texDest, ImagePtr imageSrc, const Rect * pDest, const Rect * pSrc)
{
    IDirect3DTexture9 * pD3DDestTexture = NULL;
    IDirect3DTexture9 * pD3DSrcTexture = NULL;

    if (texDest->GetTextureType() == TEXTYPE_2D)
    {
        pD3DDestTexture = static_cast<D3D9Texture*>(texDest.c_ptr())->mD3D9Texture;
    }

    D3D9Image * img = (D3D9Image*)imageSrc.c_ptr();
    pD3DSrcTexture = img->_MyTexture();
    
    assert(pD3DDestTexture && pD3DSrcTexture);

    IDirect3DSurface9 * pDestSurface;
    IDirect3DSurface9 * pSrcSurface;
    const RECT * pDestRect = reinterpret_cast<const RECT *>(pDest);
    const RECT * pSrcRect = reinterpret_cast<const RECT *>(pSrc);

    pD3DDestTexture->GetSurfaceLevel(0, &pDestSurface);
    pD3DSrcTexture->GetSurfaceLevel(0, &pSrcSurface);

    D3DXLoadSurfaceFromSurface(pDestSurface, 
                               NULL,
                               pDestRect,
                               pSrcSurface,
                               NULL,
                               pSrcRect,
                               D3DX_DEFAULT,
                               0);
}

TexturePtr D3D9VideoBufferManager::FindTexture(const TString128 & sName)
{
    // 1. find in 2d texture map
    Texture2DMap::Iterator iter0 = mTextures.Find(sName);
    Texture2DMap::Iterator end0 = mTextures.End();

    if (iter0 != end0)
        return TexturePtr(iter0->second);

    // 2. find in cube texture map
    TextureCubeMap::Iterator iter1 = mCubeTextures.Find(sName);
    TextureCubeMap::Iterator end1 = mCubeTextures.End();

    if (iter1 != end1)
        return TexturePtr(iter1->second);

    // 3. find in volume texture map
    TextureVolMap::Iterator iter2 = mVolumeTextures.Find(sName);
    TextureVolMap::Iterator end2 = mVolumeTextures.End();

    if (iter2 != end2)
        return TexturePtr(iter2->second);

    return TexturePtr();
}

DepthStencilPtr D3D9VideoBufferManager::FindDepthStencil(const TString128 & sName)
{
    TextureDSMap::Iterator iter = mDepthStencils.Find(sName);
    TextureDSMap::Iterator end = mDepthStencils.End();

    return iter != end ? DepthStencilPtr(iter->second) : DepthStencilPtr();
}

void D3D9VideoBufferManager::DestroyVertexDeclaration(VertexDeclaration * pDecl)
{
    delete pDecl;
}

void D3D9VideoBufferManager::DestroyVertexBuffer(VertexBuffer * pVertexBuffer)
{
    List<D3D9VertexBuffer*>::Iterator iter;
    List<D3D9VertexBuffer*>::Iterator end;

    iter = mVertexBuffers.Begin();
    end = mVertexBuffers.End();

    while (iter != end && (*iter) != pVertexBuffer)
    {
        ++iter;
    }

    assert(iter != end);

    delete (*iter);
    mVertexBuffers.Erase(iter);
}

void D3D9VideoBufferManager::DestroyIndexBuffer(IndexBuffer * pIndexBuffer)
{
    List<D3D9IndexBuffer*>::Iterator iter;
    List<D3D9IndexBuffer*>::Iterator end;

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

void D3D9VideoBufferManager::DestroyTexture(Texture * texture)
{
    TEXTURE_TYPE type = texture->GetTextureType();

    if (type == TEXTYPE_2D || type == TEXTYPE_RENDERTARGET)
    {
        Texture2DMap::Iterator iter = mTextures.Find(texture->GetName());
        Texture2DMap::Iterator end = mTextures.End();

        assert(iter != end);

        delete iter->second;
        mTextures.Erase(iter);
    }

    else if (type == TEXTYPE_CUBE)
    {
        TextureCubeMap::Iterator iter = mCubeTextures.Find(texture->GetName());
        TextureCubeMap::Iterator end = mCubeTextures.End();

        assert(iter != end);

        delete iter->second;
        mCubeTextures.Erase(iter);
    }

    else if (type == TEXTYPE_3D)
    {
        TextureVolMap::Iterator iter = mVolumeTextures.Find(texture->GetName());
        TextureVolMap::Iterator end = mVolumeTextures.End();

        assert(iter != end);

        delete iter->second;
        mVolumeTextures.Erase(iter);
    }

    else
        d_assert (0);
}

void D3D9VideoBufferManager::DestroyRenderTarget(RenderTarget * pRenderTarget)
{
    TextureRTMap::Iterator iter = mRenderTargets.Find(pRenderTarget->GetName());
    TextureRTMap::Iterator end = mRenderTargets.End();

    assert(iter != end);

    delete iter->second;
    mRenderTargets.Erase(iter);
}

void D3D9VideoBufferManager::DestroyDepthStencil(DepthStencil * pDepthStencil)
{
    TextureDSMap::Iterator iter = mDepthStencils.Find(pDepthStencil->GetName());
    TextureDSMap::Iterator end = mDepthStencils.End();

    assert(iter != end);

    delete iter->second;
    mDepthStencils.Erase(iter);
}

void D3D9VideoBufferManager::LostDevice()
{
    List<D3D9VertexBuffer*>::Iterator vb_iter;
    List<D3D9VertexBuffer*>::Iterator vb_end;

    List<D3D9IndexBuffer*>::Iterator i_iter;
    List<D3D9IndexBuffer*>::Iterator i_end;

    Texture2DMap::Iterator n_iter;
    Texture2DMap::Iterator n_end;

    TextureCubeMap::Iterator c_iter;
    TextureCubeMap::Iterator c_end;

    TextureVolMap::Iterator v_iter;
    TextureVolMap::Iterator v_end;

    TextureRTMap::Iterator r_iter;
    TextureRTMap::Iterator r_end;

    TextureDSMap::Iterator d_iter;
    TextureDSMap::Iterator d_end;

    vb_iter = mVertexBuffers.Begin();
    vb_end = mVertexBuffers.End();

    i_iter = mIndexBuffers.Begin();
    i_end = mIndexBuffers.End();

    n_iter = mTextures.Begin();
    n_end = mTextures.End();

    c_iter = mCubeTextures.Begin();
    c_end = mCubeTextures.End();

    v_iter = mVolumeTextures.Begin();
    v_end = mVolumeTextures.End();

    r_iter = mRenderTargets.Begin();
    r_end = mRenderTargets.End();

    d_iter = mDepthStencils.Begin();
    d_end = mDepthStencils.End();

    while (vb_iter != vb_end)
    {
        (*vb_iter)->LostDevice();

        ++vb_iter;
    }

    while (i_iter != i_end)
    {
        (*i_iter)->LostDevice();

        ++i_iter;
    }


    while (n_iter != n_end)
    {
        n_iter->second->LostDevice();

        ++n_iter;
    }

    while (c_iter != c_end)
    {
        c_iter->second->LostDevice();

        ++c_iter;
    }

    while (v_iter != v_end)
    {
        v_iter->second->LostDevice();

        ++v_iter;
    }

    while (r_iter != r_end)
    {
        r_iter->second->LostDevice();

        ++r_iter;
    }

    while (d_iter != d_end)
    {
        d_iter->second->LostDevice();

        ++d_iter;
    }
}

void D3D9VideoBufferManager::ResetDevice()
{
    List<D3D9VertexBuffer*>::Iterator vb_iter;
    List<D3D9VertexBuffer*>::Iterator vb_end;

    List<D3D9IndexBuffer*>::Iterator i_iter;
    List<D3D9IndexBuffer*>::Iterator i_end;

    Texture2DMap::Iterator n_iter;
    Texture2DMap::Iterator n_end;

    TextureCubeMap::Iterator c_iter;
    TextureCubeMap::Iterator c_end;

    TextureVolMap::Iterator v_iter;
    TextureVolMap::Iterator v_end;

    TextureRTMap::Iterator r_iter;
    TextureRTMap::Iterator r_end;

    TextureDSMap::Iterator d_iter;
    TextureDSMap::Iterator d_end;

    vb_iter = mVertexBuffers.Begin();
    vb_end = mVertexBuffers.End();

    i_iter = mIndexBuffers.Begin();
    i_end = mIndexBuffers.End();

    n_iter = mTextures.Begin();
    n_end = mTextures.End();

    c_iter = mCubeTextures.Begin();
    c_end = mCubeTextures.End();

    v_iter = mVolumeTextures.Begin();
    v_end = mVolumeTextures.End();

    r_iter = mRenderTargets.Begin();
    r_end = mRenderTargets.End();

    d_iter = mDepthStencils.Begin();
    d_end = mDepthStencils.End();

    while (vb_iter != vb_end)
    {
        (*vb_iter)->ResetDevice();

        ++vb_iter;
    }

    while (i_iter != i_end)
    {
        (*i_iter)->ResetDevice();

        ++i_iter;
    }


    while (n_iter != n_end)
    {
        n_iter->second->ResetDevice();

        ++n_iter;
    }

    while (c_iter != c_end)
    {
        c_iter->second->ResetDevice();

        ++c_iter;
    }

    while (v_iter != v_end)
    {
        v_iter->second->ResetDevice();

        ++v_iter;
    }

    while (r_iter != r_end)
    {
        r_iter->second->ResetDevice();

        ++r_iter;
    }

    while (d_iter != d_end)
    {
        d_iter->second->ResetDevice();

        ++d_iter;
    }
}

}