#include "stdafx.h"
#include "xEditTerrain.h"

xImplementOp(xTerrainTuQiOp, eOp_TuQi);
xImplementOp(xTerrainAoXiaOp, eOp_AoXia);
xImplementOp(xTerrainBlendOp, eOp_Blend);

//xTerrainTuQiOp gTerrainTuQiOp;
//xTerrainAoXiaOp gTerrainAoXiaOp;
//xTerrainBlendOp gTerrainBlendOp;


xEditTerrain::xEditTerrain()
{
}

xEditTerrain::~xEditTerrain()
{
}

void xEditTerrain::Create(const Terrain::Config & config)
{
	Environment::Instance()->CreateTerrain(config);
}

void xEditTerrain::AddLayer(const Terrain::Layer & layer)
{
}

void xEditTerrain::SetBrush(const TString128 & tex)
{
	mBrush.texture = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
}

void xEditTerrain::_Update(void *)
{
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
}

void xEditTerrain::_Render(void *)
{
}

