#include "MGUI_RenderSystem.h"

#include "MGUI_VertexBuffer.h"
#include "MGUI_Texture.h"
#include "MGUI_DataManager.h"

#include "MyGUI_Timer.h"
#include "MyGUI_Gui.h"

typedef MyGUI::Timer MGUI_Timer;

namespace Myway {

	MGUI_RenderSystem::MGUI_RenderSystem()
		: mIsInitialise(false)
		, mUpdate(false)
	{
		initialise();
	}

	MGUI_RenderSystem::~MGUI_RenderSystem()
	{
		shutdown();
	}


	void MGUI_RenderSystem::initialise()
	{
		d_assert (!mIsInitialise);

		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = MyGUI::VertexColourType::ColourARGB;

		memset(&mInfo, 0, sizeof(mInfo));

		setViewSize(Engine::Instance()->GetDeviceProperty()->Width,
					Engine::Instance()->GetDeviceProperty()->Height);
	
		mUpdate = false;

		mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("Shaders\\MGUI.ShaderLib", "Shaders\\MGUI.ShaderLib");
		d_assert (mShaderLib);
		mDefaultTech = mShaderLib->GetTechnique("MGUI_Default");
		d_assert (mDefaultTech);

		mVertexDecl = VideoBufferManager::Instance()->CreateVertexDeclaration();

		mVertexDecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		mVertexDecl->AddElement(0, 12, DT_COLOR, DU_COLOR, 0);
		mVertexDecl->AddElement(0, 16, DT_FLOAT2, DU_TEXCOORD, 0);
		mVertexDecl->Init();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void MGUI_RenderSystem::shutdown()
	{
		d_assert (mIsInitialise);

		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		mVertexDecl = NULL;
		destroyAllResources();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	MyGUI::IVertexBuffer* MGUI_RenderSystem::createVertexBuffer()
	{
		return new MGUI_VertexBuffer();
	}

	void MGUI_RenderSystem::destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void MGUI_RenderSystem::doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count)
	{
		SamplerState state;
		MGUI_Texture* dxTex = static_cast<MGUI_Texture*>(_texture);
		RenderSystem::Instance()->SetTexture(0, state, dxTex->_getTexture().c_ptr());


		MGUI_VertexBuffer * vb = static_cast<MGUI_VertexBuffer *>(_buffer);
		RenderSystem::Instance()->RenderUI(mDefaultTech, mVertexDecl, vb->_getVertexBuffer(), _count / 3);
	}

	void MGUI_RenderSystem::drawOneFrame()
	{
		MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		RS_RenderEvent(RenderUI);

		static MGUI_Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void MGUI_RenderSystem::begin()
	{
		RenderState rs;

		rs.cullMode = CULL_NONE;
		rs.depthCheck = DCM_ALWAYS;
		rs.depthWrite = false;
		rs.blendMode = BM_ALPHA_BLEND;

		RenderSystem::Instance()->SetRenderState(rs);
	}

	void MGUI_RenderSystem::end()
	{
	}

	MyGUI::ITexture* MGUI_RenderSystem::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		d_assert(item == mTextures.end());

		MGUI_Texture * texture = new MGUI_Texture(_name);
		mTextures[_name] = texture;
		return texture;
	}

	void MGUI_RenderSystem::destroyTexture(MyGUI::ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		d_assert(item != mTextures.end());

		mTextures.erase(item);

		delete _texture;
	}

	MyGUI::ITexture* MGUI_RenderSystem::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	bool MGUI_RenderSystem::isFormatSupported(MyGUI::PixelFormat _format, MyGUI::TextureUsage _usage)
	{
		if (_usage == MyGUI::TextureUsage::RenderTarget)
		{
			if (_format == MyGUI::PixelFormat::R8G8B8A8 ||
				_format == MyGUI::PixelFormat::R8G8B8)
				return true;

			return false;
		}

		return true;
	}

	void MGUI_RenderSystem::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
			delete item->second;
	
		mTextures.clear();
	}

	void MGUI_RenderSystem::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 0.0f;
		mInfo.hOffset = -0.5f / float(mViewSize.width);
		mInfo.vOffset = -0.5f / float(mViewSize.height);
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);

		mUpdate = true;
	}

}