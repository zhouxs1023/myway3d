#include "stdafx.h"

#include "xEditTerrainVeg.h"
#include "MVegetation.h"
#include "MForest.h"
#include "xTerrainPane.h"
#include "xApp.h"
#include "MWTerrainSection.h"

xEditTerrainVeg::xEditTerrainVeg()
{
}

xEditTerrainVeg::~xEditTerrainVeg()
{
}

void xEditTerrainVeg::_Init(Event * sender)
{
	mTech_Brush = xApp::Instance()->GetHelperShaderLib()->GetTechnique("TerrainBrush");
}

void xEditTerrainVeg::_Shutdown(Event * sender)
{
}

void xEditTerrainVeg::_Update(Event * sender)
{
	float brushSize = xTerrainPane::Instance()->GetTerrainVegDlg()->BrushSize();
	int brushDensity = xTerrainPane::Instance()->GetTerrainVegDlg()->BrushDensity();
	float minSize = xTerrainPane::Instance()->GetTerrainVegDlg()->MinSize();
	float maxSize = xTerrainPane::Instance()->GetTerrainVegDlg()->MaxSize();
	float minLum = xTerrainPane::Instance()->GetTerrainVegDlg()->MinLum();
	float maxLum = xTerrainPane::Instance()->GetTerrainVegDlg()->MaxLum();
	bool addMode = xTerrainPane::Instance()->GetTerrainVegDlg()->IsAddMode();

	minSize = Math::ClampPositive(minSize);
	maxSize = Math::ClampPositive(maxSize);

	minLum = Math::ClampPositive(minLum);
	maxLum = Math::ClampPositive(maxLum);

	Terrain * terrain = Environment::Instance()->GetTerrain();

	if (!terrain)
		 return ;

	float x = IMouse::Instance()->GetPositionUnit().x;
	float y = IMouse::Instance()->GetPositionUnit().y;

	if (x <= 0 || x >= 1 ||
		y <= 0 || y >= 1)
		return ;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

	Vec3 position = terrain->GetPosition(ray);

	mBrush.position = position;
	mBrush.size = brushSize;

	if (!IMouse::Instance()->KeyUp(MKC_BUTTON0))
		return ;

	MVegetation * veg = xTerrainPane::Instance()->GetTerrainVegDlg()->GetCurVeg();

	if (!veg || brushDensity <= 0 || brushSize < 0)
		return ;

	if (brushSize == 0)
		brushDensity = 1;

	RectF rect;

	rect.x1 = position.x - brushSize * 0.5f;
	rect.y1 = position.z - brushSize * 0.5f;
	rect.x2 = position.x + brushSize * 0.5f;
	rect.y2 = position.z + brushSize * 0.5f;

	if (addMode)
	{
		float step = brushSize / brushDensity;

		for (int j = 0; j < brushDensity; ++j)
		{
			for (int i = 0; i < brushDensity; ++i)
			{
				float px = rect.x1 + i * step;
				float pz = rect.y1 + j * step;

				px += brushSize / brushDensity * 0.5f;
				pz += brushSize / brushDensity * 0.5f;

				if (px > 0 && px < terrain->GetConfig().xSize &&
					pz > 0 && pz < terrain->GetConfig().zSize)
				{
					float py = terrain->GetHeight(px, pz);

					float size = Math::RandRange(minSize, maxSize);

					if (size > 0)
						MForest::Instance()->AddVegetationInst(veg, Vec3(px, py, pz), size);
				}
			}
		}
	}
	else
	{
		MForest::Instance()->RemoveVegetationInst(rect, veg);
	}
}

void xEditTerrainVeg::_Render(Event * sender)
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
			RenderSystem::Instance()->SetTexture(0, state, RenderHelper::Instance()->GetWhiteTexture().c_ptr());

			RenderOp * rop = section->GetRender();
			RenderState oldState = rop->rState;

			rop->rState.blendMode = BM_MULTIPLY;
			rop->rState.depthWrite = false;

			RenderSystem::Instance()->Render(mTech_Brush, rop);

			rop->rState = oldState;
		}
	}
}