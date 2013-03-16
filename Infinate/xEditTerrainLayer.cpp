#include "stdafx.h"

#include "xApp.h"
#include "xTerrainOp.h"
#include "xEditTerrainLayer.h"
#include "MWTerrainSection.h"


xEditTerrainLayer::xEditTerrainLayer()
{
	mBrush.size = 50.0f;
	mBrush.density = 0.5f;
	mLayer = -1;
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

void xEditTerrainLayer::_Init(Event * sender)
{
	mTech_Brush = xApp::Instance()->GetHelperShaderLib()->GetTechnique("TerrainBrush");
	mTech_Layer = xApp::Instance()->GetHelperShaderLib()->GetTechnique("TerrainLayer");

	d_assert (mTech_Brush != NULL && mTech_Layer != NULL);
}

void xEditTerrainLayer::_Update(Event * sender)
{
	Terrain * terrain = Environment::Instance()->GetTerrain();

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

	if (!terrain || mLayer == -1)
		return ;

	_UpdateWeightMap();
}

void xEditTerrainLayer::_Shutdown(Event * sender)
{
	mBrush.texture = NULL;
	mBrush.image = NULL;
}

void xEditTerrainLayer::_Render(Event * sender)
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
	int iex = (int)((ex / config.xSize) * xWeightMapSize);
	int isz = (int)Math::Ceil((1 - sz / config.zSize) * zWeightMapSize);
	int iez = (int)((1 - ez / config.zSize) * zWeightMapSize);

	isx = Math::Maximum(0, isx);
	isz = Math::Maximum(0, isz);
	iex = Math::Maximum(0, iex);
	iez = Math::Maximum(0, iez);

	isx = Math::Minimum(isx, xWeightMapSize);
	isz = Math::Minimum(isz, zWeightMapSize);
	iex = Math::Minimum(iex, xWeightMapSize);
	iez = Math::Minimum(iez, zWeightMapSize);

	int index = 0;
	Rect rc = { isx, isz, iex, iez };
	float * weights = terrain->LockWeightMap(rc);

	int w = mBrush.image->GetWidth() - 1;
	int h = mBrush.image->GetHeight() - 1;

	for (int j = isz; j <= iez; ++j)
	{
		for (int i = isx; i <= iex; ++i)
		{
			float x = float(i) / xWeightMapSize * config.xSize;
			float z = (1 - float(j) / zWeightMapSize) * config.zSize;

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

			weights[index++] = d;
		}
	}

	terrain->UnlockWeightMap(mLayer);
}



void xEditTerrainLayer::_RenderSectionLayer()
{
	Terrain * terrain = Environment::Instance()->GetTerrain();

	if (!terrain || mLayer == -1)
		return ;

	int wndWidth = Engine::Instance()->GetDeviceProperty()->Width;
	int wndHeight = Engine::Instance()->GetDeviceProperty()->Height;
	const int LayerSize = 64;
	const Terrain::Config & config = terrain->GetConfig();

	int xSection = int(mBrush.position.x / config.xSectionSize);
	int zSection = int((config.zSize - mBrush.position.z) / config.zSectionSize);

	xSection = Math::Maximum(xSection, 0);
	zSection = Math::Maximum(zSection, 0);
	xSection = Math::Minimum(xSection, config.xSectionCount - 1);
	zSection = Math::Minimum(zSection, config.zSectionCount - 1);

	TerrainSection * section = terrain->GetSection(xSection, zSection);

	TexturePtr detailMap[4] = { NULL };
	TexturePtr brushMap = NULL;
	int layer[4] = { -1 };

	for (int i = 0; i < 4; ++i)
	{
		layer[i] = section->GetLayer(i);

		if (layer[i] != -1)
			detailMap[i] = terrain->_getDetailMap(layer[i]);
	}

	brushMap = terrain->_getDetailMap(mLayer);
	
	ShaderParam * uTransform = mTech_Layer->GetVertexShaderParamTable()->GetParam("gTransform");
	float scaleX = LayerSize / (float)wndWidth;
	float scaleY = LayerSize / (float)wndHeight;

	{
		uTransform->SetUnifom(scaleX, scaleY, 1 - scaleX * 2, 1 - scaleY * 2);

		SamplerState state;
		RenderSystem::Instance()->SetTexture(0, state, detailMap[0].c_ptr());
		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Layer);
	}

	{
		uTransform->SetUnifom(scaleX, scaleY, 1 - scaleX * 1, 1 - scaleY * 2);

		SamplerState state;
		RenderSystem::Instance()->SetTexture(0, state, detailMap[1].c_ptr());
		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Layer);
	}

	{
		uTransform->SetUnifom(scaleX, scaleY, 1 - scaleX * 2, 1 - scaleY * 1);

		SamplerState state;
		RenderSystem::Instance()->SetTexture(0, state, detailMap[2].c_ptr());
		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Layer);
	}

	{
		uTransform->SetUnifom(scaleX, scaleY, 1 - scaleX, 1 - scaleY);

		SamplerState state;
		RenderSystem::Instance()->SetTexture(0, state, detailMap[3].c_ptr());
		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Layer);
	}

	{
		uTransform->SetUnifom(scaleX, scaleY, 0, 1 - scaleY);

		SamplerState state;
		RenderSystem::Instance()->SetTexture(0, state, brushMap.c_ptr());
		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Layer);
	}
}