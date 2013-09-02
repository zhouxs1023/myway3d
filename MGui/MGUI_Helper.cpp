#include "MGUI_Helper.h"
#include "MGUI_Widget.h"
#include "MGUI_RenderItem.h"
#include "MGUI_Engine.h"

#pragma warning(push)
#pragma warning(disable : 4244)

namespace Myway {

	MGUI_Helper * MGUI_Helper::msInstance;

	MGUI_Helper::MGUI_Helper()
	{
		msInstance = this;

		mUnknownChar = L'*';
	}

	MGUI_Helper::~MGUI_Helper()
	{
		mVertexDecl = NULL;
		msInstance = NULL;

		for (int i = 0; i < mRenderItems.Size(); ++i)
			delete mRenderItems[i];

		mRenderItems.Clear();
	}

	MGUI_RectF MGUI_Helper::MapUVRect(const MGUI_Rect & _rect, Texture * _skin)
	{
		MGUI_RectF uv;

		if (_skin != NULL)
		{
			float invW = 1.0f / (_skin->GetWidth() - 1);
			float invH = 1.0f / (_skin->GetHeight() - 1);

			uv.x0 = (float)_rect.x0 * invW;
			uv.x1 = (float)_rect.x1 * invW;
			uv.y0 = (float)_rect.y0 * invH;
			uv.y1 = (float)_rect.y1 * invH;
		}

		return uv;
	}

	bool MGUI_Helper::Clip(MGUI_Rect & _orect, MGUI_RectF & _ouv, const MGUI_Rect & _rect, const MGUI_RectF & _uv, const MGUI_Rect & _clipRect)
	{
		if (_rect.DX() <= 0 || _rect.DY() <= 0)
			return false;

		if (_rect.x0 >= _clipRect.x1 || _rect.x1 <= _clipRect.x0 ||
			_rect.y0 >= _clipRect.y1 || _rect.y1 <= _clipRect.y0)
			return false;

		if (_rect.x0 >= _clipRect.x0 && _rect.x1 <= _clipRect.x1 &&
			_rect.y0 >= _clipRect.y0 && _rect.y1 <= _clipRect.y1)
		{
			_orect = _rect;
			_ouv = _uv;
		}
		else
		{
			float uStep = _uv.DX() / _rect.DX();
			float vStep = _uv.DY() / _rect.DY();
			float u = _uv.x0, v = _uv.y0;
			float x = _rect.x0, y = _rect.y0;

			_orect.x0 = Math::Maximum(_clipRect.x0, _rect.x0);
			_orect.y0 = Math::Maximum(_clipRect.y0, _rect.y0);
			_orect.x1 = Math::Minimum(_clipRect.x1, _rect.x1);
			_orect.y1 = Math::Minimum(_clipRect.y1, _rect.y1);

			_ouv.x0 = u + (_orect.x0 - x) * uStep;
			_ouv.y0 = v + (_orect.y0 - y) * vStep;
			_ouv.x1 = u + (_orect.x1 - x) * uStep;
			_ouv.y1 = v + (_orect.y1 - y) * vStep;
		}

		return true;
	}

	bool MGUI_Helper::Clip(MGUI_RectF & _orect, MGUI_RectF & _ouv, const MGUI_RectF & _rect, const MGUI_RectF & _uv, const MGUI_Rect & _clipRect)
	{
		if (_rect.DX() <= 0 || _rect.DY() <= 0)
			return false;

		if (_rect.x0 >= _clipRect.x1 || _rect.x1 <= _clipRect.x0 ||
			_rect.y0 >= _clipRect.y1 || _rect.y1 <= _clipRect.y0)
			return false;

		if (_rect.x0 >= _clipRect.x0 && _rect.x1 <= _clipRect.x1 &&
			_rect.y0 >= _clipRect.y0 && _rect.y1 <= _clipRect.y1)
		{
			_orect = _rect;
			_ouv = _uv;
		}
		else
		{
			MGUI_RectF clipRect;

			clipRect.x0 = (float)_clipRect.x0;
			clipRect.y0 = (float)_clipRect.y0;
			clipRect.x1 = (float)_clipRect.x1;
			clipRect.y1 = (float)_clipRect.y1;

			float uStep = _uv.DX() / _rect.DX();
			float vStep = _uv.DY() / _rect.DY();
			float u = _uv.x0, v = _uv.y0;
			float x = _rect.x0, y = _rect.y0;

			_orect.x0 = Math::Maximum(clipRect.x0, _rect.x0);
			_orect.y0 = Math::Maximum(clipRect.y0, _rect.y0);
			_orect.x1 = Math::Minimum(clipRect.x1, _rect.x1);
			_orect.y1 = Math::Minimum(clipRect.y1, _rect.y1);

			_ouv.x0 = u + (_orect.x0 - x) * uStep;
			_ouv.y0 = v + (_orect.y0 - y) * vStep;
			_ouv.x1 = u + (_orect.x1 - x) * uStep;
			_ouv.y1 = v + (_orect.y1 - y) * vStep;
		}

		return true;
	}

	const MGUI_Vertex * MGUI_Helper::GetQuad(const MGUI_Rect & _r, const MGUI_RectF & _uv, const MGUI_Color & _c)
	{
		static MGUI_Vertex q[4];

		q[0].x = _r.x0; q[0].y = _r.y0; q[0].u = _uv.x0; q[0].v = _uv.y0; q[0].color = _c;
		q[1].x = _r.x1; q[1].y = _r.y0; q[1].u = _uv.x1; q[1].v = _uv.y0; q[1].color = _c;
		q[2].x = _r.x0; q[2].y = _r.y1; q[2].u = _uv.x0; q[2].v = _uv.y1; q[2].color = _c;
		q[3].x = _r.x1; q[3].y = _r.y1; q[3].u = _uv.x1; q[3].v = _uv.y1; q[3].color = _c;

		const MGUI_Rect & rect = MGUI_Engine::Instance()->GetRect();

		float invWidth = 1.0f / (rect.DX());
		float invHeight = 1.0f / (rect.DY());

		for (int i = 0; i < 4; ++i)
		{
			q[i].x *= invWidth;
			q[i].y *= invHeight;

			q[i].x = q[i].x * 2 - 1;
			q[i].y = 1 - q[i].y * 2;
		}

		return q;
	}

	void MGUI_Helper::AddRenderItem(MGUI_RenderItem * ri, const MGUI_Rect & myRect, const MGUI_Rect & clRect,
		const MGUI_RectF & uvRect, const MGUI_RectF & uvClientRect, const Color4 & color, const MGUI_Rect & clipRect)
	{
		MGUI_Rect rect, _rect;
		MGUI_RectF uv, _uv;

		// top
		if (clRect.y0 > 0)
		{
			// left
			if (clRect.x0 > 0)
			{
				rect.x0 = myRect.x0;
				rect.x1 = myRect.x0 + clRect.x0;

				rect.y0 = myRect.y0;
				rect.y1 = myRect.y0 + clRect.y0;

				uv.x0 = uvRect.x0;
				uv.x1 = uvClientRect.x0;

				uv.y0 = uvRect.y0;
				uv.y1 = uvClientRect.y0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			// center
			if (1)
			{
				rect.x0 = myRect.x0 + clRect.x0;
				rect.x1 = myRect.x0 + clRect.x1;

				rect.y0 = myRect.y0;
				rect.y1 = myRect.y0 + clRect.y0;

				uv.x0 = uvClientRect.x0;
				uv.x1 = uvClientRect.x1;

				uv.y0 = uvRect.y0;
				uv.y1 = uvClientRect.y0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			// right
			if (clRect.x1 < myRect.DX())
			{
				rect.x0 = myRect.x0 + clRect.x1;
				rect.x1 = myRect.x1;

				rect.y0 = myRect.y0;
				rect.y1 = myRect.y0 + clRect.y0;

				uv.x0 = uvClientRect.x1;
				uv.x1 = uvRect.x1;

				uv.y0 = uvRect.y0;
				uv.y1 = uvClientRect.y0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}
		}

		// center
		if (1)
		{
			// left
			if (clRect.x0 > 0)
			{
				rect.x0 = myRect.x0;
				rect.x1 = myRect.x0 + clRect.x0;

				rect.y0 = myRect.y0 + clRect.y0;
				rect.y1 = myRect.y0 + clRect.y1;

				uv.x0 = uvRect.x0;
				uv.x1 = uvClientRect.x0;

				uv.y0 = uvClientRect.y0;
				uv.y1 = uvClientRect.y1;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			// center
			if (1)
			{
				rect.x0 = myRect.x0 + clRect.x0;
				rect.x1 = myRect.x0 + clRect.x1;

				rect.y0 = myRect.y0 + clRect.y0;
				rect.y1 = myRect.y0 + clRect.y1;

				uv.x0 = uvClientRect.x0;
				uv.x1 = uvClientRect.x1;

				uv.y0 = uvClientRect.y0;
				uv.y1 = uvClientRect.y1;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			// right
			if (clRect.x1 < myRect.DX())
			{
				rect.x0 = myRect.x0 + clRect.x1;
				rect.x1 = myRect.x1;

				rect.y0 = myRect.y0 + clRect.y0;
				rect.y1 = myRect.y0 + clRect.y1;

				uv.x0 = uvClientRect.x1;
				uv.x1 = uvRect.x1;

				uv.y0 = uvClientRect.y0;
				uv.y1 = uvClientRect.y1;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}
		}

		// right
		if (clRect.y1 < myRect.DY())
		{
			// left
			if (clRect.x0 > 0)
			{
				rect.x0 = myRect.x0;
				rect.x1 = myRect.x0 + clRect.x0;

				rect.y0 = myRect.y0 + clRect.y1;
				rect.y1 = myRect.y1;

				uv.x0 = uvRect.x0;
				uv.x1 = uvClientRect.x0;

				uv.y0 = uvClientRect.y1;
				uv.y1 = uvRect.y1;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			// center
			if (1)
			{
				rect.x0 = myRect.x0 + clRect.x0;
				rect.x1 = myRect.x0 + clRect.x1;

				rect.y0 = myRect.y0 + clRect.y1;
				rect.y1 = myRect.y1;

				uv.x0 = uvClientRect.x0;
				uv.x1 = uvClientRect.x1;

				uv.y0 = uvClientRect.y1;
				uv.y1 = uvRect.y1;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			// right
			if (clRect.x1 < myRect.DX())
			{
				rect.x0 = myRect.x0 + clRect.x1;
				rect.x1 = myRect.x1;

				rect.y0 = myRect.y0 + clRect.y1;
				rect.y1 = myRect.y1;

				uv.x0 = uvClientRect.x1;
				uv.x1 = uvRect.x1;

				uv.y0 = uvClientRect.y1;
				uv.y1 = uvRect.y1;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}
		}
	}

	const MGUI_Vertex * MGUI_Helper::GetQuad(const MGUI_RectF & _r, const MGUI_RectF & _uv, const MGUI_Color & _c)
	{
		static MGUI_Vertex q[4];

		q[0].x = _r.x0; q[0].y = _r.y0; q[0].u = _uv.x0; q[0].v = _uv.y0; q[0].color = _c;
		q[1].x = _r.x1; q[1].y = _r.y0; q[1].u = _uv.x1; q[1].v = _uv.y0; q[1].color = _c;
		q[2].x = _r.x0; q[2].y = _r.y1; q[2].u = _uv.x0; q[2].v = _uv.y1; q[2].color = _c;
		q[3].x = _r.x1; q[3].y = _r.y1; q[3].u = _uv.x1; q[3].v = _uv.y1; q[3].color = _c;

		const MGUI_Rect & rect = MGUI_Engine::Instance()->GetRect();

		float invWidth = 1.0f / (rect.DX());
		float invHeight = 1.0f / (rect.DY());

		for (int i = 0; i < 4; ++i)
		{
			q[i].x *= invWidth;
			q[i].y *= invHeight;

			q[i].x = q[i].x * 2 - 1;
			q[i].y = 1 - q[i].y * 2;
		}

		return q;
	}

	const MGUI_Rect & MGUI_Helper::GetParentRect(MGUI_Widget * _parent)
	{
		if (_parent)
			return _parent->GetRect();

		return MGUI_Engine::Instance()->GetRect();
	}

	const MGUI_Rect & MGUI_Helper::GetClipRect(MGUI_Widget * _parent)
	{
		if (_parent)
			return _parent->GetClipRect();

		return MGUI_Engine::Instance()->GetRect();
	}


	MGUI_RenderItem * MGUI_Helper::GetEmptyRenderItem(int _order, Texture * _texture, MGUI_Shader * _shader)
	{
		for (int i = 0; i < mRenderItems.Size(); ++i)
		{
			MGUI_RenderItem * item = mRenderItems[i];
			if (item->Empty())
			{
				item->Set(_order, _texture, _shader);
				return item;
			}
		}

		MGUI_RenderItem * item = new MGUI_RenderItem();

		item->Set(_order, _texture, _shader);

		mRenderItems.PushBack(item);

		return item;
	}

	VertexDeclarationPtr MGUI_Helper::GetVertexDeclaration()
	{
		if (mVertexDecl != NULL)
			return mVertexDecl;

		mVertexDecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		mVertexDecl->AddElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
		mVertexDecl->AddElement(0, 8, DT_FLOAT4, DU_COLOR, 0);
		mVertexDecl->AddElement(0, 24, DT_FLOAT2, DU_TEXCOORD, 0);
		mVertexDecl->Init();

		return mVertexDecl;
	}

	int MGUI_Helper::GetWidgetState(MGUI_Widget * _widget)
	{
		int state = MGUI_WidgetState::Normal;

		if (!_widget->GetEnable())
			state = MGUI_WidgetState::Disabled;
		else if (_widget == MGUI_InputManager::Instance()->GetKeyFocusedWidget())
			state = MGUI_WidgetState::Selected;
		else if (_widget == MGUI_InputManager::Instance()->GetMouseFocusedWidget())
		{
			if (MGUI_InputManager::Instance()->IsMouseCaptured())
				state = MGUI_WidgetState::Pressed;
			else
				state = MGUI_WidgetState::Focused;
		}

		return state;
	}
}

#pragma warning(pop)
