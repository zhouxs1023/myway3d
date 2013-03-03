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
#include "MyGUI_IVertexBuffer.h"

namespace Myway {

	class MGUI_ENTRY MGUI_VertexBuffer : public MyGUI::IVertexBuffer
	{
		DECLARE_ALLOC();

	public:
		MGUI_VertexBuffer();
		virtual ~MGUI_VertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual MyGUI::Vertex * lock();
		virtual void unlock();

		VertexBufferPtr _getVertexBuffer() { return mpBuffer; }

	private:
		bool create();
		void destroy();
		void resize();

	private:
		VertexBufferPtr mpBuffer;

		size_t mVertexCount;
		size_t mNeedVertexCount;
	};

}