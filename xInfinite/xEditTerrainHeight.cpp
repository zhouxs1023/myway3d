#include "stdafx.h"
#include "xEditTerrainHeight.h"
#include "xTerrainOp.h"
#include "MWTerrainSection.h"


xEditTerrainHeight::xEditTerrainHeight()
{
	mBrush.size = 50.0f;
	mBrush.density = 1.0f;
}

xEditTerrainHeight::~xEditTerrainHeight()
{
}

void xEditTerrainHeight::SetBrush(const TString128 & tex)
{
	mBrush.texture = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
	mBrush.image = VideoBufferManager::Instance()->LoadImage_(tex);

	d_assert (mBrush.texture != NULL && mBrush.image != NULL);
}

void xEditTerrainHeight::_Init(void *)
{
	mTech_Brush = xApp::Instance()->GetHelperShaderLib()->GetTechnique("TerrainBrush");
}

void xEditTerrainHeight::_Update(void *)
{
	Terrain * terrain = Environment::Instance()->GetTerrain();

	if (!terrain)
		return ;

	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	if (pt.x < 0 || pt.y < 0 || pt.x > 1 || pt.y > 1)
		return ;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	mBrush.position = terrain->GetPosition(ray);

	if (!IMouse::Instance()->KeyPressed(MKC_BUTTON0))
		return ;

	_UpdateGeometry();
}

void xEditTerrainHeight::_Shutdown(void *)
{
	mBrush.texture = NULL;
	mBrush.image = NULL;
}

void xEditTerrainHeight::_Render(void *)
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

void xEditTerrainHeight::_UpdateGeometry()
{
	Terrain * terrain = Environment::Instance()->GetTerrain();

	if (!terrain)
		return ;

	int op = xApp::Instance()->GetOperator();

	Vec3 center = mBrush.position;
	float size = mBrush.size;
	float density = mBrush.density;

	// calculate brush box
	float sx = center.x - size * 0.5f;
	float sz = center.z + size * 0.5f;
	float ex = center.x + size * 0.5f;
	float ez = center.z - size * 0.5f;

	const Terrain::Config & config = terrain->GetConfig();

	int xVertexCount = config.xVertexCount - 1;
	int zVertexCount = config.zVertexCount - 1;

	int isx = (int)Math::Ceil(sx / config.xSize * xVertexCount);
	int iex = (int)(ex / config.xSize * xVertexCount);
	int isz = (int)Math::Ceil((1 - sz / config.zSize) * zVertexCount);
	int iez = (int)((1 - ez / config.zSize) * zVertexCount);

	isx = Math::Maximum(0, isx);
	isz = Math::Maximum(0, isz);
	iex = Math::Maximum(0, iex);
	iez = Math::Maximum(0, iez);

	isx = Math::Minimum(isx, xVertexCount);
	isz = Math::Minimum(isz, zVertexCount);
	iex = Math::Minimum(iex, xVertexCount);
	iez = Math::Minimum(iez, zVertexCount);

	int index = 0;
	Rect rc = { isx, isz, iex, iez };
	float * heights = terrain->LockHeight(rc);

	int w = mBrush.image->GetWidth() - 1;
	int h = mBrush.image->GetHeight() - 1;

	for (int j = isz; j <= iez; ++j)
	{
		for (int i = isx; i <= iex; ++i)
		{
			float x = float(i) / xVertexCount * config.xSize;
			float z = (1 - float(j) / zVertexCount) * config.zSize;

			if (x < 0 || x > config.xSize ||
				z < 0 || z > config.zSize)
				continue ;

			float u = (x - sx) / (ex - sx);
			float v = (z - sz) / (ez - sz);

			u = Math::Maximum(u, 0.0f);
			v = Math::Maximum(v, 0.0f);
			u = Math::Minimum(u, 1.0f);
			v = Math::Minimum(v, 1.0f);

			int iu = int(u * w);
			int iv = int(v * h);
			int iu1 = iu + 1;
			int iv1 = iv + 1;

			iu1 = Math::Minimum(iu1, w);
			iv1 = Math::Minimum(iv1, h);

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

			if (mOp == eUp)
				heights[index++] += d;
			else if (mOp == eDown)
				heights[index++] -= d;
		}
	}

	terrain->UnlockHeight();
}
