#pragma once

#include "MGUI_Entry.h"

namespace Myway {

	class MGUI_RenderItem;

	class MGUI_ENTRY MGUI_Shader
	{
		DeclareRootRTTI(MGUI_Shader);

	public:
		MGUI_Shader() {}
		virtual ~MGUI_Shader() {}

		virtual void DoRender(MGUI_RenderItem * _ri) = 0;
	};
}