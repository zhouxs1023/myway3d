#pragma once

#include "MGUI_Types.h"
#include "MGUI_Shader.h"

namespace Myway {

	class MGUI_ENTRY MGUI_RenderItem
	{
	public:
		MGUI_RenderItem();
		~MGUI_RenderItem();

		void Set(int _order, Texture * _texture, MGUI_Shader * _shader);
		void Reset();
		bool Empty();

		int GetOrder() const { return mOrder; }
		Texture * GetTexture() const { return mTexture; }
		MGUI_Shader * GetShader() const { return mShader; }
		const MGUI_Vertex * GetVertices() const { return &mVertices[0]; }
		int GetVertexCount() const { return mVertices.Size(); }

		void AddTri(const MGUI_Vertex * _tri);
		void AddQuad(const MGUI_Vertex * _quad);

	protected:
		bool mEmpty;

		int mOrder;
		Texture * mTexture;
		MGUI_Shader * mShader;

		Array<MGUI_Vertex> mVertices;
	};

}