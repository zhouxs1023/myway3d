#include "MGUI_Texture.h"
#include "MGUI_System.h"

namespace Myway
{
	MGUI_Texture::MGUI_Texture(const std::string& _name)
		: mName(_name)
		, mpTexture(NULL)
		, mNumElemBytes(0)
		, mLock(false)
		, mRenderTarget(nullptr)
	{
	}

	MGUI_Texture::~MGUI_Texture()
	{
		destroy();
	}

	const std::string& MGUI_Texture::getName() const
	{
		return mName;
	}

	void MGUI_Texture::createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format)
	{
		destroy();

		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;

		TEXTURE_TYPE type = TEXTYPE_2D;
		USAGE usage = USAGE_STATIC;
		FORMAT format = FMT_A8R8G8B8;

		if (mTextureUsage == MyGUI::TextureUsage::RenderTarget)
			type = TEXTYPE_RENDERTARGET;
		else if (mTextureUsage == MyGUI::TextureUsage::Dynamic)
			usage = USAGE_DYNAMIC;
		else if (mTextureUsage == MyGUI::TextureUsage::Stream)
			usage = USAGE_DYNAMIC;

		if (mPixelFormat == MyGUI::PixelFormat::R8G8B8A8)
		{
			format = FMT_A8R8G8B8;
			mNumElemBytes = 4;
		}
		else if (mPixelFormat == MyGUI::PixelFormat::R8G8B8)
		{
			format = FMT_R8G8B8;
			mNumElemBytes = 3;
		}
		else if (mPixelFormat == MyGUI::PixelFormat::L8A8)
		{
			format = FMT_A8L8;
			mNumElemBytes = 2;
		}
		else if (mPixelFormat == MyGUI::PixelFormat::L8)
		{
			format = FMT_L8;
			mNumElemBytes = 1;
		}
		else
		{
			d_assert (0 && "Creating texture with unknown pixel formal.");
		}

		if (type == TEXTYPE_2D)
		{
			mpTexture = VideoBufferManager::Instance()->CreateTexture(mName.c_str(), 
				mSize.width, mSize.height, 1, format, usage);
		}
		else
		{
			mpTexture = VideoBufferManager::Instance()->CreateTextureRT(mName.c_str(),
				mSize.width, mSize.height, format);
		}
		
		d_assert (mpTexture != NULL);
	}

	void MGUI_Texture::loadFromFile(const std::string& _filename)
	{
		destroy();
		mTextureUsage = MyGUI::TextureUsage::Default;
		mPixelFormat = MyGUI::PixelFormat::R8G8B8A8;
		mNumElemBytes = 4;

		mpTexture = VideoBufferManager::Instance()->Load2DTexture(mName.c_str(), _filename.c_str());

		d_assert (mpTexture != NULL);

		FORMAT format = mpTexture->GetFormat();

		if (format == FMT_A8R8G8B8 || format == FMT_X8R8G8B8)
		{
			mPixelFormat = MyGUI::PixelFormat::R8G8B8A8;
			mNumElemBytes = 4;
		}
		else if (format == FMT_R8G8B8)
		{
			mPixelFormat = MyGUI::PixelFormat::R8G8B8;
			mNumElemBytes = 3;
		}
		else if (format == FMT_A8L8)
		{
			mPixelFormat = MyGUI::PixelFormat::L8A8;
			mNumElemBytes = 2;
		}
		else if (format == FMT_L8)
		{
			mPixelFormat = MyGUI::PixelFormat::L8;
			mNumElemBytes = 1;
		}

		mSize.set(mpTexture->GetWidth(), mpTexture->GetHeight());
	}

	void MGUI_Texture::destroy()
	{
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		mpTexture = nullptr;
	}

	int MGUI_Texture::getWidth()
	{
		return mSize.width;
	}

	int MGUI_Texture::getHeight()
	{
		return mSize.height;
	}

	void* MGUI_Texture::lock(MyGUI::TextureUsage _access)
	{
		d_assert (mpTexture != NULL);

		int lockFlag = (_access == MyGUI::TextureUsage::Write) ? LOCK_DISCARD : LOCK_READONLY;

		LockedBox lb;
		mpTexture->Lock(0, &lb, NULL, lockFlag);

		mLock = true;

		return lb.pData;
	}

	void MGUI_Texture::unlock()
	{
		d_assert (mpTexture != NULL);

		mpTexture->Unlock(0);
		mLock = false;
	}

	bool MGUI_Texture::isLocked()
	{
		return mLock;
	}

	MyGUI::PixelFormat MGUI_Texture::getFormat()
	{
		return mPixelFormat;
	}

	size_t MGUI_Texture::getNumElemBytes()
	{
		return mNumElemBytes;
	}

	MyGUI::TextureUsage MGUI_Texture::getUsage()
	{
		return mTextureUsage;
	}

	MyGUI::IRenderTarget* MGUI_Texture::getRenderTarget()
	{
		if (mpTexture == nullptr)
			return nullptr;

		if (mRenderTarget == nullptr)
			mRenderTarget = new MGUI_RenderTarget(mpTexture);

		return mRenderTarget;
	}














	MGUI_RenderTarget::MGUI_RenderTarget(TexturePtr texture)
	{
		mpRenderTarget = VideoBufferManager::Instance()->CreateRenderTarget(texture);

		int width = texture->GetWidth();
		int height = texture->GetHeight();

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = -0.5f / float(width);
		mRenderTargetInfo.vOffset = -0.5f / float(height);
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);
	}

	MGUI_RenderTarget::~MGUI_RenderTarget()
	{
		mpRenderTarget = NULL;
	}

	void MGUI_RenderTarget::begin()
	{
		mpBackRT = RenderSystem::Instance()->GetRenderTarget(0);

		RenderSystem::Instance()->SetRenderTarget(0, mpRenderTarget.c_ptr());
		RenderSystem::Instance()->ClearBuffer(NULL, true, false, false);

		RenderSystem::Instance()->BeginScene();
	}

	void MGUI_RenderTarget::end()
	{
		RenderSystem::Instance()->EndScene();

		RenderSystem::Instance()->SetRenderTarget(0, mpBackRT);
		mpBackRT = NULL;
	}

	void MGUI_RenderTarget::doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count)
	{
		MGUI_System::Instance()->GetRenderManager()->doRender(_buffer, _texture, _count);
	}
}
