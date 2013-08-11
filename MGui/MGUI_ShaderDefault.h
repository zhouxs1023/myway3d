#pragma once

#include "MGUI_Shader.h"
#include "MGUI_Types.h"

namespace Myway {

	class MGUI_ENTRY MGUI_ShaderDefault : public MGUI_Shader
	{
		DeclareRTTI();

	public:
		MGUI_ShaderDefault();
		virtual ~MGUI_ShaderDefault();

		void DoRender(MGUI_RenderItem * _ri);

	protected:
		Technique * mTech;
	};

}