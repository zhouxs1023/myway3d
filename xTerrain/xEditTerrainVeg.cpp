#include "stdafx.h"

#include "xEditTerrainVeg.h"
#include "MVegetation.h"
#include "MForest.h"

xEditTerrainVeg::xEditTerrainVeg()
{
}

xEditTerrainVeg::~xEditTerrainVeg()
{
}

void xEditTerrainVeg::_Init(void * param0, void * param1)
{
}

void xEditTerrainVeg::_Shutdown(void * param0, void * param1)
{
}

void xEditTerrainVeg::_Update(void * param0, void * param1)
{
	if (!IMouse::Instance()->KeyUp(MKC_BUTTON0))
		return ;

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

	MVegetation * veg = MForest::Instance()->GetVegetation(0);
	
	MForest::Instance()->AddVegetationInst(veg, position, 10);
}

void xEditTerrainVeg::_Render(void * param0, void * param1)
{
}