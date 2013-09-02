#pragma once

#include "MGUI_Entry.h"
#include "MGUI_Types.h"
#include "MGUI_Font.h"

namespace Myway {

	class MGUI_Widget;
	class MGUI_RenderItem;
	class MGUI_Shader;

	class MGUI_ENTRY MGUI_Helper
	{
		static MGUI_Helper * msInstance;

	public:
		static MGUI_Helper * Instance() { return msInstance; }

	public:
		MGUI_Helper();
		~MGUI_Helper();

		MGUI_RectF MapUVRect(const MGUI_Rect & _rect, Texture * _skin);

		bool Clip(MGUI_Rect & _orect, MGUI_RectF & _ouv, const MGUI_Rect & _rect, const MGUI_RectF & _uv, const MGUI_Rect & _clipRect);
		bool Clip(MGUI_RectF & _orect, MGUI_RectF & _ouv, const MGUI_RectF & _rect, const MGUI_RectF & _uv, const MGUI_Rect & _clipRect);

		const MGUI_Vertex * GetQuad(const MGUI_Rect & _rect, const MGUI_RectF & _uv, const MGUI_Color & _c);
		const MGUI_Vertex * GetQuad(const MGUI_RectF & _rect, const MGUI_RectF & _uv, const MGUI_Color & _c);

		void AddRenderItem(MGUI_RenderItem * ri, const MGUI_Rect & myRect, const MGUI_Rect & clRect,
			const MGUI_RectF & uvRect, const MGUI_RectF & uvClientRect, const Color4 & color, const MGUI_Rect & clipRect);

		const MGUI_Rect & GetParentRect(MGUI_Widget * _parent);
		const MGUI_Rect & GetClipRect(MGUI_Widget * _parent);

		MGUI_RenderItem * GetEmptyRenderItem(int _order, Texture * _texture, MGUI_Shader * _shader);

		VertexDeclarationPtr GetVertexDeclaration();

		void SetUnknownChar(MGUI_Char _char) { mUnknownChar = _char; }
		MGUI_Char GetUnknownChar() { return mUnknownChar; }

		int GetWidgetState(MGUI_Widget * _widget);

	protected:
		Array<MGUI_RenderItem *> mRenderItems;
		VertexDeclarationPtr mVertexDecl;
		MGUI_Char mUnknownChar;
	};

}