#include "MGUI_Layout.h"
#include "MGUI_Helper.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_Layout, MGUI_Widget);

	MGUI_Layout::MGUI_Layout(const MGUI_LookFeel * _lookfeel)
		: MGUI_Widget(_lookfeel, NULL)
	{
		MGUI_Engine::Instance()->AddLayout(this);
	}

	MGUI_Layout::~MGUI_Layout()
	{
		MGUI_Engine::Instance()->RemoveLayout(this);
	}

	void MGUI_Layout::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		MGUI_LookFeel * _lookfeel = (MGUI_LookFeel *)mLookFeel;
		const MGUI_Rect & clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);
		int state = MGUI_Helper::Instance()->GetWidgetState(this);

		if (_lookfeel)
		{
			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());

			const MGUI_Rect & myRect = this->GetAbsRect();
			const MGUI_Rect & clRect = this->GetClientRect();
			const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(state), _lookfeel->GetSkin());
			const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(state), _lookfeel->GetSkin());
			const Color4 & color = mColor;

			MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, color, clipRect);
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	MGUI_RenderItem * MGUI_Layout::GetRenderItem(int _order,  MGUI_Shader * _shader, Texture * _skin)
	{
		List<MGUI_RenderItem *>::Iterator whr = mRenderItems.Begin();
		List<MGUI_RenderItem *>::Iterator end = mRenderItems.End();

		while (whr != end)
		{
			MGUI_RenderItem * item = *whr;

			if (item->GetOrder() == _order
				&& item->GetShader() == _shader
				&& item->GetTexture() == _skin)
				return item;

			++whr;
		}

		whr = mRenderItems.Begin();
		while (whr != end)
		{
			MGUI_RenderItem * item = *whr;

			if (item->GetOrder() > _order)
			{
				item = MGUI_Helper::Instance()->GetEmptyRenderItem(_order, _skin, _shader);

				mRenderItems.Insert(whr, item);

				return item;
			}

			++whr;
		}

		{
			MGUI_RenderItem * item = MGUI_Helper::Instance()->GetEmptyRenderItem(_order, _skin, _shader);

			mRenderItems.PushBack(item);

			return item;
		}
	}

	void MGUI_Layout::UpdateRenderItem()
	{
		List<MGUI_RenderItem *>::Iterator whr = mRenderItems.Begin();
		List<MGUI_RenderItem *>::Iterator end = mRenderItems.End();

		while (whr != end)
		{
			(*whr)->Reset();

			++whr;
		}

		mRenderItems.Clear();

		_AddRenderItem(this);
	}

	void MGUI_Layout::DoRender()
	{
		List<MGUI_RenderItem *>::Iterator whr = mRenderItems.Begin();
		List<MGUI_RenderItem *>::Iterator end = mRenderItems.End();

		while (whr != end)
		{
			MGUI_RenderItem * item = *whr;

			item->GetShader()->DoRender(item);

			++whr;
		}
	}

}

