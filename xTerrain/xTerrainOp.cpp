#include "stdafx.h"
#include "xTerrainOp.h"
#include "xApp.h"

xImplementOp(xTerrainOp, eOp_Terrain);

xTerrainOp gTerrainOp;

void xTerrainOp::Process()
{
	xApp::Instance()->SetOperator(eOp_Terrain);
}