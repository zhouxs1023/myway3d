#pragma once

#include "MGUI_Types.h"
#include "MGUI_Widget.h"

namespace Myway {

	class MGUI_ENTRY MGUI_ImageBox : public MGUI_Widget
	{
	public:
		MGUI_ImageBox(MGUI_Widget * _parent);
		virtual ~MGUI_ImageBox();

		void SetSkin(TexturePtr _skin, const MGUI_RectF & _rect = MGUI_RectF(0, 0, 1, 1));

		TexturePtr GetSkin() { return mSkin; }
		const MGUI_RectF & GetUVRect() { return mUVRect; }

		void SetShader(MGUI_Shader * _shader) { mShader = _shader; }
		MGUI_Shader * GetShader() { return mShader; }

		virtual void _AddRenderItem(MGUI_Layout * _layout);

	protected:
		TexturePtr mSkin;
		MGUI_RectF mUVRect;
		MGUI_Shader * mShader;
	};

}