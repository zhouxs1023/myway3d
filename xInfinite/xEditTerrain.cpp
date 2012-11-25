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

	mBrush.size = 50.0f;
	mBrush.density = 1.0f;
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
	Terrain::Config config;

	Create(config);

	mTech_Brush = xApp::Instance()->GetHelperShaderLib()->GetTechnique("TerrainBrush");

	FileSystem fs("../Core/Terrain/Brushes");

	fs.Load();

	Archive::FileInfoVisitor v = fs.GetFileInfos();

	while (!v.Endof())
	{
		const Archive::FileInfo & info = v.Cursor()->second;

		if (info.type == Archive::FILE_ARCHIVE)
		{
			mBrushImages.PushBack(info.name);
		}

		++v;
	}

	d_assert(mBrushImages.Size() != 0);

	TString128 defaultBrush = mBrushImages[0];

	mBrush.texture = VideoBufferManager::Instance()->Load2DTexture(defaultBrush, "brushes\\" + defaultBrush);
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
	int op = xApp::Instance()->GetOperator();

	if (!mTerrain || (op != xTerrainBlendOp::eOp_Blend &&
		op != xTerrainTuQiOp::eOp_TuQi && op != xTerrainAoXiaOp::eOp_AoXia))
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	mBrush.position = mTerrain->GetPosition(ray);

	Ray rk = Ray(Vec3(0, 1, 0), Vec3(0, -1, 0));
	Vec3 pos = mTerrain->GetPosition(ray);

	// render brush
	Aabb aabb;

	aabb.minimum = mBrush.position - Vec3(mBrush.size * 0.5f, 1, mBrush.size * 0.5f);
	aabb.maximum = mBrush.position + Vec3(mBrush.size * 0.5f, 1, mBrush.size * 0.5f);

	ShaderParam * uTransform = mTech_Brush->GetVertexShaderParamTable()->GetParam("gTransform");
	ShaderParam * uMinInvSize = mTech_Brush->GetVertexShaderParamTable()->GetParam("gMinInvSize");
	ShaderParam * uMorph = mTech_Brush->GetVertexShaderParamTable()->GetParam("gMorph");

	for (int i = 0; i < mTerrain->GetVisibleSectionCount(); ++i)
	{
		TerrainSection * section = mTerrain->GetVisibleSection(i);

		const Aabb & bound = section->GetWorldAabb();

		float x0 = Math::Maximum(aabb.minimum.x, bound.minimum.x);
		float z0 = Math::Maximum(aabb.minimum.z, bound.minimum.z);

		float x1 = Math::Minimum(aabb.maximum.x, bound.maximum.x);
		float z1 = Math::Minimum(aabb.maximum.z, bound.maximum.z);

		if (x0 <= x1 && z0 <= z1)
		{
			float xOff = section->GetOffX();
			float zOff = section->GetOffZ();

			uTransform->SetUnifom(xOff, 0, zOff, 0);
			uMinInvSize->SetUnifom(aabb.minimum.x, aabb.minimum.z, 1 / mBrush.size, 1 / mBrush.size);

			SamplerState state;
			state.Address = TEXA_BORDER;
			state.BorderColor = Color::Black;
			RenderSystem::Instance()->SetTexture(0, state, mBrush.texture.c_ptr());

			RenderOp * rop = section->GetRender();
			RenderState oldState = rop->rState;

			rop->rState.blendMode = BM_MULTIPLY;
			rop->rState.depthWrite = false;

			RenderSystem::Instance()->Render(mTech_Brush, rop);

			rop->rState = oldState;
		}
	}

}

