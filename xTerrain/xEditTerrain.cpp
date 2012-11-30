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
	: OnInit(&xEvent::OnInit, this, &xEditTerrain::_Init)
	, OnShutdown(&xEvent::OnShutdown, this, &xEditTerrain::_Shutdown)
	, OnUpdate(&xEvent::OnUpdate, this, &xEditTerrain::_Update)
	, OnRender(&RenderEvent::OnAfterDeffererShading, this, &xEditTerrain::_Render)
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
	mBrush.image = VideoBufferManager::Instance()->LoadImage_("brushes\\" + defaultBrush);
}

bool gFrame = false;

void xEditTerrain::_Update(void *)
{
	if (!mTerrain)
		return ;

	if (IKeyboard::Instance()->KeyUp(KC_F))
	{
		gFrame = !gFrame;
	}

	const Terrain::Config & config = mTerrain->GetConfig();

	for (int j = 0; j < config.zSectionCount; ++j)
	{
		for (int i = 0; i < config.xSectionCount; ++i)
		{
			TerrainSection * section = mTerrain->GetSection(i, j);
			section->GetRender()->rState.fillMode = gFrame ? FILL_FRAME : FILL_SOLID;
		}
	}


	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainBlendOp::eOp_Blend &&
		op != xTerrainTuQiOp::eOp_TuQi &&
		op != xTerrainAoXiaOp::eOp_AoXia)
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);
	mBrush.position = mTerrain->GetPosition(ray);

	if (!IMouse::Instance()->KeyPressed(MKC_BUTTON0))
		return ;

	if (op == xTerrainBlendOp::eOp_Blend)
	{
	}
	else if (op == xTerrainTuQiOp::eOp_TuQi)
	{
		_UpdateGeometry();
	}
	else if (op == xTerrainAoXiaOp::eOp_AoXia)
	{
		_UpdateGeometry();
	}
}

void xEditTerrain::_Shutdown(void *)
{
	mBrush.texture = NULL;
	mBrush.image = NULL;
}

void xEditTerrain::_Render(void *)
{
	if (!mTerrain)
		return ;

	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainBlendOp::eOp_Blend &&
		op != xTerrainTuQiOp::eOp_TuQi &&
		op != xTerrainAoXiaOp::eOp_AoXia)
		return ;

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

void xEditTerrain::_UpdateGeometry()
{
	int op = xApp::Instance()->GetOperator();

	Vec3 center = mBrush.position;
	float size = mBrush.size;
	float density = mBrush.density;

	// calculate brush box
	float sx = center.x - size * 0.5f;
	float sz = center.z + size * 0.5f;
	float ex = center.x + size * 0.5f;
	float ez = center.z - size * 0.5f;

	const Terrain::Config & config = mTerrain->GetConfig();

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

	Rect rc = { isx, isz, iex, iez };
	float * heights = mTerrain->LockHeight(rc);
	int index = 0;
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

			if (op == xTerrainTuQiOp::eOp_TuQi)
				heights[index++] += d;
			else if (op == xTerrainAoXiaOp::eOp_AoXia)
				heights[index++] -= d;
		}
	}

	mTerrain->UnlockHeight();
}

void xEditTerrain::_UpdateWeightMap()
{
	Vec3 center = mBrush.position;
	float size = mBrush.size;
	float density = mBrush.density;

	float sx = center.x - size * 0.5f;
	float sz = center.z + size * 0.5f;
	float ex = center.x + size * 0.5f;
	float ez = center.z - size * 0.5f;

	const Terrain::Config & config = mTerrain->GetConfig();

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
