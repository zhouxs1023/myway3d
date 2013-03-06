#include "MGUI_VertexBuffer.h"

namespace Myway {

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	MGUI_VertexBuffer::MGUI_VertexBuffer()
		: mNeedVertexCount(0)
		, mVertexCount(RENDER_ITEM_STEEP_REALLOCK)
		, mpBuffer(NULL)
	{
	}

	MGUI_VertexBuffer::~MGUI_VertexBuffer()
	{
		destroy();
	}

	void MGUI_VertexBuffer::setVertexCount(size_t _count)
	{
		if (_count != mNeedVertexCount)
		{
			mNeedVertexCount = _count;
			resize();
		}
	}

	size_t MGUI_VertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	MyGUI::Vertex* MGUI_VertexBuffer::lock()
	{
		d_assert (mpBuffer != NULL);
		return (MyGUI::Vertex*)mpBuffer->Lock(0, 0, LOCK_DISCARD);
	}

	void MGUI_VertexBuffer::unlock()
	{
		d_assert (mpBuffer != NULL);

		mpBuffer->Unlock();
	}

	bool MGUI_VertexBuffer::create()
	{
		if (mNeedVertexCount == 0)
			return false;

		DWORD length = mNeedVertexCount * sizeof(MyGUI::Vertex);

		mpBuffer = VideoBufferManager::Instance()->CreateVertexBuffer(length);

		return mpBuffer != NULL;
	}

	void MGUI_VertexBuffer::destroy()
	{
		mpBuffer = NULL;
	}

	void MGUI_VertexBuffer::resize()
	{
		destroy();
		create();
	}

}