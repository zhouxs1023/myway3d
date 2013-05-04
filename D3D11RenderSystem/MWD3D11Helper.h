#pragma once

#include "MWD3D11RenderSystemEntry.h"
#include "MWRenderSystem.h"
#include <cg/cg.h>

namespace Myway {

	#define safe_release(p) if (p != NULL) { p->Release(); p = NULL; }

	class RS_ENTRY D3D11Helper
	{
	public:
		static DXGI_FORMAT  GetD3DFormat(FORMAT fmt);
		static FORMAT       GetFormat(DXGI_FORMAT fmt);

		static CGprofile    GetCgProfile(SHADER_PROFILE Profile);

		static const char * GetSemanticName(DECL_USAGE usage);
		static DXGI_FORMAT	GetSemanticFormat(DECL_TYPE type);
	};

}