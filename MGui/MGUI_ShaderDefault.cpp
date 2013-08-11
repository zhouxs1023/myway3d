#include "MGUI_ShaderDefault.h"
#include "MGUI_RenderItem.h"
#include "MGUI_Helper.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_ShaderDefault, MGUI_Shader)

	MGUI_ShaderDefault::MGUI_ShaderDefault()
	{
		mTech = MGUI_Engine::Instance()->GetShaderLib()->GetTechnique("MGUI_Default");
		d_assert (mTech != NULL);
	}

	MGUI_ShaderDefault::~MGUI_ShaderDefault()
	{
	}

	void MGUI_ShaderDefault::DoRender(MGUI_RenderItem * _ri)
	{
		const MGUI_Vertex * vb = _ri->GetVertices();
		int primCount = _ri->GetVertexCount() / 3;
		Texture * texture = _ri->GetTexture();

		SamplerState state;
		RenderSystem::Instance()->SetTexture(0, state, texture);

		RenderSystem::Instance()->RenderUI(mTech, MGUI_Helper::Instance()->GetVertexDeclaration(), vb, primCount);
	}

}