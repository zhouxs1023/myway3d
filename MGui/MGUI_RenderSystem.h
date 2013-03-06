//////////////////////////////////////////////////////////////////////////
//
// MyGui Integration.
//   MyGui engine: http://mygui.info
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "MGUI_Entry.h"

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

namespace Myway {

	class MGUI_ENTRY MGUI_RenderSystem : public MyGUI::RenderManager, public MyGUI::IRenderTarget
	{
		DECLARE_ALLOC();

	public:
		MGUI_RenderSystem();
		~MGUI_RenderSystem();

		/** @see RenderManager::getViewSize */
		virtual const MyGUI::IntSize& getViewSize() const
		{
			return mViewSize;
		}

		/** @see RenderManager::getVertexFormat */
		virtual MyGUI::VertexColourType getVertexFormat()
		{
			return mVertexFormat;
		}

		/** @see RenderManager::createVertexBuffer */
		virtual MyGUI::IVertexBuffer* createVertexBuffer();
		/** @see RenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer);

		/** @see RenderManager::createTexture */
		virtual MyGUI::ITexture* createTexture(const std::string& _name);
		/** @see RenderManager::destroyTexture */
		virtual void destroyTexture(MyGUI::ITexture* _texture);
		/** @see RenderManager::getTexture */
		virtual MyGUI::ITexture* getTexture(const std::string& _name);

		/** @see RenderManager::isFormatSupported */
		virtual bool isFormatSupported(MyGUI::PixelFormat _format, MyGUI::TextureUsage _usage);

		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();

		/** @see IRenderTarget::doRender */
		virtual void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count);

		/** @see IRenderTarget::getInfo */
		virtual const MyGUI::RenderTargetInfo& getInfo()
		{
			return mInfo;
		}

		/*internal:*/
		void drawOneFrame();
		void setViewSize(int _width, int _height);

	private:
		void destroyAllResources();

		void initialise();
		void shutdown();

	private:
		MyGUI::IntSize mViewSize;
		MyGUI::VertexColourType mVertexFormat;
		MyGUI::RenderTargetInfo mInfo;
		bool mUpdate;

		typedef std::map<std::string, MyGUI::ITexture*> MapTexture;
		MapTexture mTextures;

		bool mIsInitialise;

		ShaderLib * mShaderLib;
		Technique * mDefaultTech;
		VertexDeclarationPtr mVertexDecl;
	};

}