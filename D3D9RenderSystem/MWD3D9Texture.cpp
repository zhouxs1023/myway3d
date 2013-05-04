#include "MWD3D9Texture.h"
#include "MWD3D9RenderSystem.h"
#include "MWVideoBufferManager.h"
#include "MWResourceManager.h"
#include "Engine.h"

namespace Myway
{


/* : D3D9Texture
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
*/
D3D9Texture::D3D9Texture(IDirect3DDevice9 * Device)
: mD3D9Device(Device)
{
}

D3D9Texture::~D3D9Texture()
{
    safe_release_com(mD3D9Texture);
}

void D3D9Texture::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyTexture(this);
}

void D3D9Texture::Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags)
{
	d_assert(!mRenderTarget);

    HRESULT hr = D3D_OK;

    DWORD D3DLock = 0;

    if ((LockFlags & LOCK_DISCARD) && (mUsage & USAGE_DYNAMIC))
        D3DLock |= D3DLOCK_DISCARD;

    if (LockFlags & LOCK_NOOVERWRITE)
        D3DLock |= D3DLOCK_NOOVERWRITE;

    if (LockFlags & LOCK_READONLY)
        D3DLock |= D3DLOCK_READONLY;

    D3DLOCKED_RECT D3DLockRect;
    RECT D3DRect; 

    if (pBox)
    {
        D3DRect.left = pBox->Left;
        D3DRect.top = pBox->Top;
        D3DRect.right = pBox->Right;
        D3DRect.bottom = pBox->Bottom;
        hr = mD3D9Texture->LockRect(iLevel, &D3DLockRect, &D3DRect, D3DLock);
    }
    else 
    {
        hr = mD3D9Texture->LockRect(iLevel, &D3DLockRect, NULL, D3DLock);
    }

    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: LockRect failed. desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }
    
    pLockedBox->RowPitch = D3DLockRect.Pitch;
    pLockedBox->SlicePitch = 1;
    pLockedBox->pData = D3DLockRect.pBits;
}

void D3D9Texture::Unlock(int iLevel)
{
    IDirect3DTexture9 * pD3DTexture = static_cast<IDirect3DTexture9*>(mD3D9Texture);
    pD3DTexture->UnlockRect(iLevel);
}
//
//void D3D9Texture::UpdateMipmap()
//{
//	d_assert (Math::IsPowerOfTwo(mWidth) && Math::IsPowerOfTwo(mHeight));
//	d_assert(mWidth == mHeight)
//	d_assert (mUsage == USAGE_STATIC);
//
//	int level0 = 0;
//	int leveln = 1;
//	int size0 = mWidth;
//	int sizen = mWidth / 2;
//
//	int count = mD3D9Texture->GetLevelCount();
//
//	while (leveln < count)
//	{
//		IDirect3DSurface9 * surf0, * surfn;
//		
//		mD3D9Texture->GetSurfaceLevel(level0, &surf0);
//		mD3D9Texture->GetSurfaceLevel(leveln, &surfn);
//
//		D3DLOCKED_RECT rc0, rcn;
//		surf0->LockRect(&rc0, NULL, D3DLOCK_READONLY);
//		surfn->LockRect(&rcn, NULL, 0);
//
//		
//
//		for (int j = 0; j < sizen; ++j)
//		{
//			for (int i = 0; i < sizen; ++i)
//			{
//
//			}
//		}
//
//		surfn->UnlockRect();
//		surf0->UnlockRect();
//
//		surf0->Release();
//		surfn->Release();
//
//
//		level0 += 1;
//		leveln += 1;
//
//		size0 = size0 / 2;
//		sizen = sizen / 2;
//	}
//}

void D3D9Texture::LostDevice()
{
    if (mUsage == USAGE_DYNAMIC)
        safe_release_com(mD3D9Texture);
}

void D3D9Texture::ResetDevice()
{
    if (mUsage == USAGE_DYNAMIC)
    {
        assert(!mD3D9Texture);

        HRESULT hr = D3D_OK;
        DWORD D3DUsage = D3D9Mapping::GetD3DUsage(mUsage);
        D3DPOOL D3DPool = D3D9Mapping::GetD3DPool(mUsage);
        D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(mFormat);

        if (mRenderTarget)
		{
            D3DUsage = D3DUSAGE_RENDERTARGET;
			D3DPool = D3DPOOL_DEFAULT;
		}

        int rWidth = mWidth, rHeight = mHeight;

        if (rWidth == -1 || rHeight == -1)
        {
            const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

            rWidth = dp->Width;
            rHeight = dp->Height;
        }

        hr = mD3D9Device->CreateTexture(rWidth, 
                                        rHeight,
                                        mMipLevels,
                                        D3DUsage,
                                        D3DFormat,
                                        D3DPool,
                                        &mD3D9Texture,
                                        NULL);

        d_assert (hr == S_OK);
    }
}

void D3D9Texture::Load()
{
    if (mLoadState == Resource::LOADED)
        return ;

    ResourceManager::Instance()->GetResourceLoader()->Load(this);
}

void D3D9Texture::Reload()
{
    if (mLoadState == Resource::LOADED)
        Unload();

    ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
}

void D3D9Texture::Unload()
{
    safe_release_com(mD3D9Texture);

    mHeight = 0;
    mWidth = 0;
    mDepth = 0;
    mMipLevels = 0;
    mUsage = USAGE_STATIC;
    mFormat = FMT_UNKNOWN;
    mType = TEXTYPE_2D;

    mLoadState = Resource::UNLOADED;
}

void D3D9Texture::LoadImp(DataStreamPtr stream)
{
    D3DXIMAGE_INFO ImgInfo;

    IDirect3DTexture9 * pD3D9Texture = NULL;
    HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(mD3D9Device,
        stream->GetData(),
        stream->Size(),
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        D3DFMT_UNKNOWN,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        &ImgInfo,
        NULL,
        &pD3D9Texture);

    D3DErrorExceptionFunction(D3DXCreateTextureFromFileEx, hr);

    D3DSURFACE_DESC desc;

    pD3D9Texture->GetLevelDesc(0, &desc);

    mD3D9Texture = pD3D9Texture;
    mWidth = desc.Width;
    mHeight = desc.Height;
    mDepth = 1;
    mFormat = D3D9Mapping::GetFormat(desc.Format);
    mMipLevels = pD3D9Texture->GetLevelCount();

    mLoadState = Resource::LOADED;
}



/* : D3D9VolumeTexture
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
*/
D3D9VolumeTexture::D3D9VolumeTexture(IDirect3DDevice9 * Device)
: mD3D9Device(Device)
{
}

void D3D9VolumeTexture::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyTexture(this);
}

D3D9VolumeTexture::~D3D9VolumeTexture()
{
    safe_release_com(mD3D9VolumeTexture);
}

void D3D9VolumeTexture::Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags)
{
    HRESULT hr = D3D_OK;

    DWORD D3DLock = 0;

    if ((LockFlags & LOCK_DISCARD) && (mUsage & USAGE_DYNAMIC))
        D3DLock |= D3DLOCK_DISCARD;

    if (LockFlags & LOCK_NOOVERWRITE)
        D3DLock |= D3DLOCK_NOOVERWRITE;

    if (LockFlags & LOCK_READONLY)
        D3DLock |= D3DLOCK_READONLY;

    D3DLOCKED_BOX D3DLockBox;
    D3DBOX D3DBox;

    if (pBox)
    {
        D3DBox.Left = (UINT)pBox->Left;
        D3DBox.Top = (UINT)pBox->Top;
        D3DBox.Right = (UINT)pBox->Right;
        D3DBox.Bottom = (UINT)pBox->Bottom;
        D3DBox.Front = (UINT)pBox->Front;
        D3DBox.Back = (UINT)pBox->Back;
        hr = mD3D9VolumeTexture->LockBox(iLevel, &D3DLockBox, &D3DBox, D3DLock);
    }
    else
    {
        hr = mD3D9VolumeTexture->LockBox(iLevel, &D3DLockBox, NULL, D3DLock);
    }

    if (FAILED(hr))
    {
        EXCEPTION("D3D Error: LockBox failed. desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
    }

    pLockedBox->RowPitch = D3DLockBox.RowPitch;
    pLockedBox->SlicePitch = D3DLockBox.SlicePitch;
    pLockedBox->pData = D3DLockBox.pBits;
}

void D3D9VolumeTexture::Unlock(int iLevel)
{
    mD3D9VolumeTexture->UnlockBox(iLevel);
}

void D3D9VolumeTexture::LostDevice()
{
    if (mUsage == USAGE_DYNAMIC)
        safe_release_com(mD3D9VolumeTexture);
}

void D3D9VolumeTexture::ResetDevice()
{
    if (mUsage == USAGE_DYNAMIC)
    {
        assert(!mD3D9VolumeTexture);

        HRESULT hr = D3D_OK;
        DWORD D3DUsage = D3D9Mapping::GetD3DUsage(mUsage);
        D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(mFormat);

        hr = mD3D9Device->CreateVolumeTexture(mWidth, 
                                                mHeight,
                                                mDepth,
                                                mMipLevels,
                                                D3DUsage,
                                                D3DFormat,
                                                D3DPOOL_DEFAULT,
                                                &mD3D9VolumeTexture,
                                                NULL);
        if (FAILED(hr))
        {
            EXCEPTION("D3D Error: CreateVolumeTexture faield. desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
        }
    }
}

void D3D9VolumeTexture::Load()
{
    if (mLoadState == Resource::LOADED)
        return ;

    ResourceManager::Instance()->GetResourceLoader()->Load(this);
}

void D3D9VolumeTexture::Reload()
{
    if (mLoadState == Resource::LOADED)
        Unload();

    ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
}

void D3D9VolumeTexture::Unload()
{
    safe_release_com(mD3D9VolumeTexture);

    mHeight = 0;
    mWidth = 0;
    mDepth = 0;
    mMipLevels = 0;
    mUsage = USAGE_STATIC;
    mFormat = FMT_UNKNOWN;
    mType = TEXTYPE_3D;

    mLoadState = Resource::UNLOADED;
}

void D3D9VolumeTexture::LoadImp(DataStreamPtr stream)
{
    D3DXIMAGE_INFO ImgInfo;

    IDirect3DVolumeTexture9 * pD3D9VolumeTexture;
    HRESULT hr = D3DXCreateVolumeTextureFromFileInMemoryEx(mD3D9Device,
        stream->GetData(),
        stream->Size(),
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        D3DFMT_UNKNOWN,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        &ImgInfo,
        NULL,
        &pD3D9VolumeTexture);

    D3DErrorExceptionFunction(D3DXCreateVolumeTextureFromFileInMemoryEx, hr);

    D3DVOLUME_DESC desc;
    pD3D9VolumeTexture->GetLevelDesc(0, &desc);

    mD3D9VolumeTexture = pD3D9VolumeTexture;
    mWidth = desc.Width;
    mHeight = desc.Height;
    mDepth = desc.Depth;
    mFormat = D3D9Mapping::GetFormat(desc.Format);
    mMipLevels = pD3D9VolumeTexture->GetLevelCount();
}







/* : D3D9CubeTexture
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
*/
D3D9CubeTexture::D3D9CubeTexture(IDirect3DDevice9 * Device)
: mD3D9Device(Device)
{
}

D3D9CubeTexture::~D3D9CubeTexture()
{
}

void D3D9CubeTexture::DeleteSelf()
{
    VideoBufferManager::Instance()->DestroyTexture(this);
}

void D3D9CubeTexture::Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags)
{
    EXCEPTION("Cube texture can't lock able");
}

void D3D9CubeTexture::Unlock(int iLevel)
{
    EXCEPTION("Cube texture can't lock able");
}


void D3D9CubeTexture::LostDevice()
{
    if (mUsage == USAGE_DYNAMIC)
        safe_release_com(mD3D9CubeTexture);
}

void D3D9CubeTexture::ResetDevice()
{
    if (mUsage == USAGE_DYNAMIC)
    {
        assert(!mD3D9CubeTexture);

        HRESULT hr = D3D_OK;
        DWORD D3DUsage = D3D9Mapping::GetD3DUsage(mUsage);
        D3DFORMAT D3DFormat = D3D9Mapping::GetD3DFormat(mFormat);
        IDirect3DCubeTexture9 * pD3D9CubeTexture = NULL;

        hr = mD3D9Device->CreateCubeTexture(mWidth, 
                                              mMipLevels,
                                              D3DUsage,
                                              D3DFormat,
                                              D3DPOOL_DEFAULT,
                                              &pD3D9CubeTexture,
                                              NULL);
        if (FAILED(hr))
        {
            EXCEPTION("D3D Error: CreateCubeTexture faield. desc: " + D3D9Mapping::GetD3DErrorDescription(hr));
        }
    }
}

void D3D9CubeTexture::Load()
{
    if (mLoadState == Resource::LOADED)
        return ;

    ResourceManager::Instance()->GetResourceLoader()->Load(this);
}

void D3D9CubeTexture::Unload()
{
    safe_release_com(mD3D9CubeTexture);

    mHeight = 0;
    mWidth = 0;
    mDepth = 0;
    mMipLevels = 0;
    mUsage = USAGE_STATIC;
    mFormat = FMT_UNKNOWN;
    mType = TEXTYPE_3D;

    mLoadState = Resource::UNLOADED;
}

void D3D9CubeTexture::Reload()
{
    if (mLoadState == Resource::LOADED)
        Unload();

    ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
}

void D3D9CubeTexture::LoadImp(DataStreamPtr stream)
{
    D3DXIMAGE_INFO ImgInfo;

    IDirect3DCubeTexture9 * pD3D9CubeTexture = NULL;
    HRESULT hr = D3DXCreateCubeTextureFromFileInMemoryEx(mD3D9Device,
        stream->GetData(),
        stream->Size(),
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        D3DFMT_UNKNOWN,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        &ImgInfo,
        NULL,
        &pD3D9CubeTexture);

    D3DErrorExceptionFunction(D3DXCreateCubeTextureFromFileEx, hr);

    D3DSURFACE_DESC desc;

    pD3D9CubeTexture->GetLevelDesc(0, &desc);

    mD3D9CubeTexture = pD3D9CubeTexture;
    mWidth = desc.Width;
    mHeight = desc.Height;
    mDepth = 1;
    mFormat = D3D9Mapping::GetFormat(desc.Format);
    mMipLevels = pD3D9CubeTexture->GetLevelCount();
}

}
