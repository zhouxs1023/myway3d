#include "MGUI_ImageBox.h"
#include "MGUI_Layout.h"
#include "MGUI_Engine.h"

namespace Myway {

	MGUI_ImageBox::MGUI_ImageBox(MGUI_Widget * _parent)
		: MGUI_Widget(NULL, _parent)
	{
		mShader = MGUI_Engine::Instance()->GetDefaultShader();
	}

	MGUI_ImageBox::~MGUI_ImageBox()
	{
	}

	void MGUI_ImageBox::SetSkin(TexturePtr _skin, const MGUI_RectF & _rect)
	{
		mSkin = _skin;
		mUVRect = _rect;
	}

	void MGUI_ImageBox::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		if (mSkin == NULL)
			return ;

		MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mShader, GetSkin().c_ptr());

		const MGUI_Rect & clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);

		MGUI_Rect _rect; MGUI_RectF _uv;
		
		if (MGUI_Helper::Instance()->Clip(_rect, _uv, GetAbsRect(), mUVRect, clipRect))
		{
			ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv,  mColor));
		}
	}

}
