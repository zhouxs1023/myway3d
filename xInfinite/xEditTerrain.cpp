#include "stdafx.h"
#include "xEditTerrain.h"

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

