#pragma once

#include "MGUI_Widget.h"
#include "MGUI_LookFeel.h"
#include "MGUI_RenderItem.h"
#include "MGUI_Serializer.h"

namespace Myway {

	class MGUI_ENTRY MGUI_Layout : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		MGUI_Layout(const MGUI_LookFeel * _lookfeel);
		virtual ~MGUI_Layout();

		MGUI_RenderItem * GetRenderItem(int _order, MGUI_Shader * _shader, Texture * _skin);

		virtual void _AddRenderItem(MGUI_Layout * _layout);

		void UpdateRenderItem();
		void DoRender();

	protected:
		List<MGUI_RenderItem *> mRenderItems;
	};

}