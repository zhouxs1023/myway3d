#include "MGUI_RenderItem.h"

namespace Myway {

	MGUI_RenderItem::MGUI_RenderItem()
	{
		Reset();
	}

	MGUI_RenderItem::~MGUI_RenderItem()
	{
	}

	void MGUI_RenderItem::Set(int _order, Texture * _texture, MGUI_Shader * _shader)
	{
		mOrder = _order;
		mTexture = _texture;
		mShader = _shader;

		mEmpty = false;
	}

	void MGUI_RenderItem::Reset()
	{
		mOrder = 0;
		mTexture = NULL;
		mShader = NULL;
		mVertices.Clear();

		mEmpty = true;
	}

	bool MGUI_RenderItem::Empty()
	{
		return mEmpty;
	}

	void MGUI_RenderItem::AddTri(const MGUI_Vertex * _tri)
	{
		mVertices.PushBack(_tri[0]);
		mVertices.PushBack(_tri[1]);
		mVertices.PushBack(_tri[2]);
	}

	void MGUI_RenderItem::AddQuad(const MGUI_Vertex * _quad)
	{
		mVertices.PushBack(_quad[0]);
		mVertices.PushBack(_quad[1]);
		mVertices.PushBack(_quad[2]);

		mVertices.PushBack(_quad[2]);
		mVertices.PushBack(_quad[1]);
		mVertices.PushBack(_quad[3]);
	}
}