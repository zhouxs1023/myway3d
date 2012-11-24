#include "stdafx.h"
#include "xEditTerrain.h"
#include "MWTerrainSection.h"

xImplementOp(xTerrainTuQiOp, eOp_TuQi);
xImplementOp(xTerrainAoXiaOp, eOp_AoXia);
xImplementOp(xTerrainBlendOp, eOp_Blend);

xTerrainTuQiOp gTerrainTuQiOp;
xTerrainAoXiaOp gTerrainAoXiaOp;
xTerrainBlendOp gTerrainBlendOp;


xEditTerrain gEditTerrain;

IMP_SLN(xEditTerrain);

xEditTerrain::xEditTerrain()
	: OnInit(xApp::OnInit, this, &xEditTerrain::_Init)
	, OnShutdown(xApp::OnShutdown, this, &xEditTerrain::_Shutdown)
	, OnUpdate(xApp::OnUpdate, this, &xEditTerrain::_Update)
	, OnRender(RenderEvent::OnAfterDeffererShading, this, &xEditTerrain::_Render)
{
	INIT_SLN;

	mTerrain = NULL;
}

xEditTerrain::~xEditTerrain()
{
	SHUT_SLN;
}

void xEditTerrain::Create(const Terrain::Config & config)
{
	Environment::Instance()->CreateTerrain(config);
	mTerrain = Environment::Instance()->GetTerrain();
}

void xEditTerrain::AddLayer(const Terrain::Layer & layer)
{
	mTerrain->AddLayer(layer);
}

void xEditTerrain::SetBrush(const TString128 & tex)
{
	mBrush.texture = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
}

void xEditTerrain::_Init(void *)
{
}

void xEditTerrain::_Update(void *)
{
	if (!IMouse::Instance()->KeyPressed(MKC_BUTTON0))
		return ;

	int op = xApp::Instance()->GetOperator();

	if (op == xTerrainBlendOp::eOp_Blend)
	{
	}
	else if (op == xTerrainTuQiOp::eOp_TuQi)
	{
	}
	else if (op == xTerrainAoXiaOp::eOp_AoXia)
	{
	}
}

void xEditTerrain::_Shutdown(void *)
{
	mBrush.texture = NULL;
}

void xEditTerrain::_Render(void *)
{
	if (!mTerrain)
		return ;

	// render brush
	Aabb aabb, tmp;

	aabb.minimum = mBrush.position - Vec3(mBrush.size, 1, mBrush.size);
	aabb.maximum = mBrush.position + Vec3(mBrush.size, 1, mBrush.size);

	ShaderParam * uTransform = mTech_Brush->GetVertexShaderParamTable()->GetParam("gTransform");
	ShaderParam * uMinInvSize = mTech_Brush->GetVertexShaderParamTable()->GetParam("gMinInvSize");
	ShaderParam * uMorph = mTech_Brush->GetVertexShaderParamTable()->GetParam("gMorph");

	for (int i = 0; i < mTerrain->GetVisibleSectionCount(); ++i)
	{
		TerrainSection * section = mTerrain->GetVisibleSection(i);

		const Aabb & bound = section->GetWorldAabb();

		if (Math::AABBIntersection(tmp, aabb, bound) == 0)
		{
			float xOff = section->GetOffX();
			float zOff = section->GetOffZ();

			uTransform->SetUnifom(xOff, 0, zOff, 0);
			uMinInvSize->SetUnifom(aabb.minimum.x, aabb.minimum.z, 1 / aabb.GetWidth(), 1 / aabb.GetDepth());

			SamplerState state;
			state.Address = TEXA_BORDER;
			state.BorderColor = Color::White;
			RenderSystem::Instance()->SetTexture(0, state, mBrush.texture.c_ptr());

			RenderSystem::Instance()->Render(mTech_Brush, section->GetRender());
		}
	}

}

