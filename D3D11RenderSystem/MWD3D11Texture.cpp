#include "MWD3D11Texture.h"
#include "MWVideoBufferManager.h"
#include "MWResourceManager.h"
#include "Engine.h"
#include "MWD3D11Driver.h"

namespace Myway
{


	/* : D3D11Texture
	--------------------------------------------------------------------------------------
	--------------------------------------------------------------------------------------
	*/
	D3D11Texture::D3D11Texture()
		: mD3D11Texture(NULL)
	{
	}

	D3D11Texture::~D3D11Texture()
	{
		safe_release(mD3D11Texture);
	}

	void D3D11Texture::DeleteSelf()
	{
		VideoBufferManager::Instance()->DestroyTexture(this);
	}

	void D3D11Texture::Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags)
	{
	}

	void D3D11Texture::Unlock(int iLevel)
	{
	}

	void D3D11Texture::LostDevice()
	{
		if (mUsage == USAGE_DYNAMIC)
			safe_release_com(mD3D11Texture);
	}

	void D3D11Texture::Load()
	{
		if (mLoadState == Resource::LOADED)
			return ;

		ResourceManager::Instance()->GetResourceLoader()->Load(this);
	}

	void D3D11Texture::Reload()
	{
		if (mLoadState == Resource::LOADED)
			Unload();

		ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
	}

	void D3D11Texture::Unload()
	{
		safe_release(mD3D11Texture);

		mHeight = 0;
		mWidth = 0;
		mDepth = 0;
		mMipLevels = 0;
		mUsage = USAGE_STATIC;
		mFormat = FMT_UNKNOWN;
		mType = TEXTYPE_2D;

		mLoadState = Resource::UNLOADED;
	}

	void D3D11Texture::LoadImp(DataStreamPtr stream)
	{
		D3DX11_IMAGE_LOAD_INFO ImgInfo;

		ID3D11Resource * pD3D11Texture = NULL;
		HRESULT hr = D3DX11CreateTextureFromMemory(
			D3D11Driver::Instance()->Device(),
			stream->GetData(),
			stream->Size(),
			&ImgInfo,
			NULL,
			&pD3D11Texture,
			NULL);

		d_assert (hr == S_OK);

		//mD3D11Texture = pD3D11Texture;
		mWidth = ImgInfo.Width;
		mHeight = ImgInfo.Height;
		mDepth = 1;
		mFormat = D3D11Helper::GetFormat(ImgInfo.Format);
		mMipLevels = pD3D11Texture->GetLevelCount();

		mLoadState = Resource::LOADED;
	}



	/* : D3D11VolumeTexture
	--------------------------------------------------------------------------------------
	--------------------------------------------------------------------------------------
	*/
	D3D11VolumeTexture::D3D11VolumeTexture(IDirect3DDevice9 * Device)
		: mD3D11Device(Device)
	{
	}

	void D3D11VolumeTexture::DeleteSelf()
	{
		VideoBufferManager::Instance()->DestroyTexture(this);
	}

	D3D11VolumeTexture::~D3D11VolumeTexture()
	{
		safe_release_com(mD3D11VolumeTexture);
	}

	void D3D11VolumeTexture::Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags)
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
			hr = mD3D11VolumeTexture->LockBox(iLevel, &D3DLockBox, &D3DBox, D3DLock);
		}
		else
		{
			hr = mD3D11VolumeTexture->LockBox(iLevel, &D3DLockBox, NULL, D3DLock);
		}

		if (FAILED(hr))
		{
			EXCEPTION("D3D Error: LockBox failed. desc: " + D3D11Mapping::GetD3DErrorDescription(hr));
		}

		pLockedBox->RowPitch = D3DLockBox.RowPitch;
		pLockedBox->SlicePitch = D3DLockBox.SlicePitch;
		pLockedBox->pData = D3DLockBox.pBits;
	}

	void D3D11VolumeTexture::Unlock(int iLevel)
	{
		mD3D11VolumeTexture->UnlockBox(iLevel);
	}

	void D3D11VolumeTexture::LostDevice()
	{
		if (mUsage == USAGE_DYNAMIC)
			safe_release_com(mD3D11VolumeTexture);
	}

	void D3D11VolumeTexture::ResetDevice()
	{
		if (mUsage == USAGE_DYNAMIC)
		{
			assert(!mD3D11VolumeTexture);

			HRESULT hr = D3D_OK;
			DWORD D3DUsage = D3D11Mapping::GetD3DUsage(mUsage);
			D3DFORMAT D3DFormat = D3D11Mapping::GetD3DFormat(mFormat);

			hr = mD3D11Device->CreateVolumeTexture(mWidth, 
				mHeight,
				mDepth,
				mMipLevels,
				D3DUsage,
				D3DFormat,
				D3DPOOL_DEFAULT,
				&mD3D11VolumeTexture,
				NULL);
			if (FAILED(hr))
			{
				EXCEPTION("D3D Error: CreateVolumeTexture faield. desc: " + D3D11Mapping::GetD3DErrorDescription(hr));
			}
		}
	}

	void D3D11VolumeTexture::Load()
	{
		if (mLoadState == Resource::LOADED)
			return ;

		ResourceManager::Instance()->GetResourceLoader()->Load(this);
	}

	void D3D11VolumeTexture::Reload()
	{
		if (mLoadState == Resource::LOADED)
			Unload();

		ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
	}

	void D3D11VolumeTexture::Unload()
	{
		safe_release_com(mD3D11VolumeTexture);

		mHeight = 0;
		mWidth = 0;
		mDepth = 0;
		mMipLevels = 0;
		mUsage = USAGE_STATIC;
		mFormat = FMT_UNKNOWN;
		mType = TEXTYPE_3D;

		mLoadState = Resource::UNLOADED;
	}

	void D3D11VolumeTexture::LoadImp(DataStreamPtr stream)
	{
		D3DXIMAGE_INFO ImgInfo;

		IDirect3DVolumeTexture9 * pD3D11VolumeTexture;
		HRESULT hr = D3DXCreateVolumeTextureFromFileInMemoryEx(mD3D11Device,
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
			&pD3D11VolumeTexture);

		D3DErrorExceptionFunction(D3DXCreateVolumeTextureFromFileInMemoryEx, hr);

		D3DVOLUME_DESC desc;
		pD3D11VolumeTexture->GetLevelDesc(0, &desc);

		mD3D11VolumeTexture = pD3D11VolumeTexture;
		mWidth = desc.Width;
		mHeight = desc.Height;
		mDepth = desc.Depth;
		mFormat = D3D11Mapping::GetFormat(desc.Format);
		mMipLevels = pD3D11VolumeTexture->GetLevelCount();
	}







	/* : D3D11CubeTexture
	--------------------------------------------------------------------------------------
	--------------------------------------------------------------------------------------
	*/
	D3D11CubeTexture::D3D11CubeTexture(IDirect3DDevice9 * Device)
		: mD3D11Device(Device)
	{
	}

	D3D11CubeTexture::~D3D11CubeTexture()
	{
	}

	void D3D11CubeTexture::DeleteSelf()
	{
		VideoBufferManager::Instance()->DestroyTexture(this);
	}

	void D3D11CubeTexture::Lock(int iLevel, LockedBox * pLockedBox, const Boxi * pBox, int LockFlags)
	{
		EXCEPTION("Cube texture can't lock able");
	}

	void D3D11CubeTexture::Unlock(int iLevel)
	{
		EXCEPTION("Cube texture can't lock able");
	}


	void D3D11CubeTexture::LostDevice()
	{
		if (mUsage == USAGE_DYNAMIC)
			safe_release_com(mD3D11CubeTexture);
	}

	void D3D11CubeTexture::ResetDevice()
	{
		if (mUsage == USAGE_DYNAMIC)
		{
			assert(!mD3D11CubeTexture);

			HRESULT hr = D3D_OK;
			DWORD D3DUsage = D3D11Mapping::GetD3DUsage(mUsage);
			D3DFORMAT D3DFormat = D3D11Mapping::GetD3DFormat(mFormat);
			IDirect3DCubeTexture9 * pD3D11CubeTexture = NULL;

			hr = mD3D11Device->CreateCubeTexture(mWidth, 
				mMipLevels,
				D3DUsage,
				D3DFormat,
				D3DPOOL_DEFAULT,
				&pD3D11CubeTexture,
				NULL);
			if (FAILED(hr))
			{
				EXCEPTION("D3D Error: CreateCubeTexture faield. desc: " + D3D11Mapping::GetD3DErrorDescription(hr));
			}
		}
	}

	void D3D11CubeTexture::Load()
	{
		if (mLoadState == Resource::LOADED)
			return ;

		ResourceManager::Instance()->GetResourceLoader()->Load(this);
	}

	void D3D11CubeTexture::Unload()
	{
		safe_release_com(mD3D11CubeTexture);

		mHeight = 0;
		mWidth = 0;
		mDepth = 0;
		mMipLevels = 0;
		mUsage = USAGE_STATIC;
		mFormat = FMT_UNKNOWN;
		mType = TEXTYPE_3D;

		mLoadState = Resource::UNLOADED;
	}

	void D3D11CubeTexture::Reload()
	{
		if (mLoadState == Resource::LOADED)
			Unload();

		ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
	}

	void D3D11CubeTexture::LoadImp(DataStreamPtr stream)
	{
		D3DXIMAGE_INFO ImgInfo;

		IDirect3DCubeTexture9 * pD3D11CubeTexture = NULL;
		HRESULT hr = D3DXCreateCubeTextureFromFileInMemoryEx(mD3D11Device,
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
			&pD3D11CubeTexture);

		D3DErrorExceptionFunction(D3DXCreateCubeTextureFromFileEx, hr);

		D3DSURFACE_DESC desc;

		pD3D11CubeTexture->GetLevelDesc(0, &desc);

		mD3D11CubeTexture = pD3D11CubeTexture;
		mWidth = desc.Width;
		mHeight = desc.Height;
		mDepth = 1;
		mFormat = D3D11Mapping::GetFormat(desc.Format);
		mMipLevels = pD3D11CubeTexture->GetLevelCount();
	}

}
