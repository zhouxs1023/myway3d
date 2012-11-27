#include "stdafx.h"

#include "xApp.h"
#include "xTerrainOp.h"
#include "xEditTerrainLayer.h"
#include "MWTerrainSection.h"


xEditTerrainLayer::xEditTerrainLayer()
{
	mBrush.size = 50.0f;
	mBrush.density = 1.0f;
}

xEditTerrainLayer::~xEditTerrainLayer()
{
}

void xEditTerrainLayer::SetBrush(const TString128 & tex)
{
	mBrush.texture = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
	mBrush.image = VideoBufferManager::Instance()->LoadImage_(tex);

	d_assert (mBrush.texture != NULL && mBrush.image != NULL);
}

void xEditTerrainLayer::_Init(void *)
{
	mTech_Brush = xApp::Instance()->GetHelperShaderLib()->GetTechnique("TerrainBrush");
}

void xEditTerrainLayer::_Update(void *)
{
	Terrain * terrain = Environment::Instance()->GetTerrain();

	if (!terrain)
		return ;

	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	mBrush.position = terrain->GetPosition(ray);

	if (!IMouse::Instance()->KeyPressed(MKC_BUTTON0))
		return ;

	_UpdateWeightMap();
}

void xEditTerrainLayer::_Shutdown(void *)
{
	mBrush.texture = NULL;
	mBrush.image = NULL;
}

void xEditTerrainLayer::_Render(void *)
{
	Terrain * terrain = Environment::Instance()->GetTerrain();

	if (!terrain)
		return ;

	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	// render brush
	Aabb aabb;

	aabb.minimum = mBrush.position - Vec3(mBrush.size * 0.5f, 1, mBrush.size * 0.5f);
	aabb.maximum = mBrush.position + Vec3(mBrush.size * 0.5f, 1, mBrush.size * 0.5f);

	ShaderParam * uTransform = mTech_Brush->GetVertexShaderParamTable()->GetParam("gTransform");
	ShaderParam * uMinInvSize = mTech_Brush->GetVertexShaderParamTable()->GetParam("gMinInvSize");
	ShaderParam * uMorph = mTech_Brush->GetVertexShaderParamTable()->GetParam("gMorph");

	for (int i = 0; i < terrain->GetVisibleSectionCount(); ++i)
	{
		TerrainSection * section = terrain->GetVisibleSection(i);

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

void xEditTerrainLayer::_UpdateWeightMap()
{
	Terrain * terrain = Environment::Instance()->GetTerrain();

	if (!terrain)
		return ;

	Vec3 center = mBrush.position;
	float size = mBrush.size;
	float density = mBrush.density;

	float sx = center.x - size * 0.5f;
	float sz = center.z + size * 0.5f;
	float ex = center.x + size * 0.5f;
	float ez = center.z - size * 0.5f;

	const Terrain::Config & config = terrain->GetConfig();

	int xWeightMapSize = config.xWeightMapSize - 1;
	int zWeightMapSize = config.zWeightMapSize - 1;

	int isx = (int)Math::Ceil(sx / config.xSize * xWeightMapSize);
	int iex = (int)(ex / config.xSize) * xWeightMapSize;
	int isz = (int)Math::Ceil((1 - sz / config.zSize) * xWeightMapSize);
	int iez = (int)((1 - ez / config.zSize) * zWeightMapSize);

	isx = Math::Maximum(0, isx);
	isz = Math::Maximum(0, isz);
	iex = Math::Maximum(0, iex);
	iez = Math::Maximum(0, iez);

	isx = Math::Minimum(isx, xWeightMapSize);
	isz = Math::Minimum(isz, zWeightMapSize);
	iex = Math::Minimum(iex, xWeightMapSize);
	iez = Math::Minimum(iez, zWeightMapSize);

	for (int j = isz; j <= iez; ++j)
	{
		for (int i = isx; i <= iex; ++i)
		{
			float x = float(i) / xWeightMapSize * config.xSize;
			float z = float(j) / zWeightMapSize * config.zSize;

			if (x < 0 || x > config.xSize ||
				z < 0 || z > config.zSize)
				continue ;

			float u = (x - sx) / (sz - sx);
			float v = 1 - (z - sz) / (ez - sz);

			u = Math::Maximum(u, 0.0f);
			v = Math::Maximum(v, 0.0f);
			u = Math::Minimum(u, 1.0f);
			v = Math::Minimum(v, 1.0f);

			int w = mBrush.image->GetWidth();
			int h = mBrush.image->GetHeight();

			int iu = int(u * w);
			int iv = int(v * h);
			int iu1 = iu + 1;
			int iv1 = iv + 1;

			iu1 = Math::Maximum(iu1, w);
			iv1 = Math::Maximum(iv1, h);

			float du = u * w - iu;
			float dv = v * h - iv;

			Color4 c0 = mBrush.image->GetColor(iu,  iv);
			Color4 c1 = mBrush.image->GetColor(iu1, iv);
			Color4 c2 = mBrush.image->GetColor(iu,  iv1);
			Color4 c3 = mBrush.image->GetColor(iu1, iv1);

			Color4 cx0 = c0 + (c1 - c0) * du;
			Color4 cx1 = c2 + (c3 - c2) * du;

			Color4 cy = cx0 + (cx1 - cx0) * dv;

			float d = cy.r * density;
		}
	}
}
