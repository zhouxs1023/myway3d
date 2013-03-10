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
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Types.h"
#include "MyGUI_IRenderTarget.h"


namespace Myway
{
	class MGUI_ENTRY MGUI_Texture : public MyGUI::ITexture
	{
		DECLARE_ALLOC();

	public:
		MGUI_Texture(const std::string& _name);
		virtual ~MGUI_Texture();

		virtual const std::string& getName() const;

		virtual void createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format);
		virtual void createFromTexture(TexturePtr tex);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename) { }

		virtual void destroy();

		virtual void* lock(MyGUI::TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked();

		virtual int getWidth();
		virtual int getHeight();

		virtual MyGUI::PixelFormat getFormat();
		virtual MyGUI::TextureUsage getUsage();
		virtual size_t getNumElemBytes();

		virtual MyGUI::IRenderTarget* getRenderTarget();

		TexturePtr _getTexture() { return mpTexture; }

	private:
		std::string mName;
		TexturePtr mpTexture;
		MyGUI::IntSize mSize;
		MyGUI::TextureUsage mTextureUsage;
		MyGUI::PixelFormat mPixelFormat;
		size_t mNumElemBytes;
		bool mLock;
		bool mNoLock;
		MyGUI::IRenderTarget* mRenderTarget;
	};


	class MGUI_RenderTarget : public MyGUI::IRenderTarget
	{
		DECLARE_ALLOC();

	public:
		MGUI_RenderTarget(TexturePtr texture);
		virtual ~MGUI_RenderTarget();

		virtual void begin();
		virtual void end();

		virtual void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count);

		virtual const MyGUI::RenderTargetInfo& getInfo()
		{
			return mRenderTargetInfo;
		}

	private:
		RenderTarget * mpBackRT;
		RenderTargetPtr mpRenderTarget;
		MyGUI::RenderTargetInfo mRenderTargetInfo;
	};
}

