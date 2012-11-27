#pragma once

#include "xBaseOperator.h"

class xTerrainOp : public xBaseOp
{
public:
	static const int eOp_Terrain;

	virtual void Process()
	{
		xApp::Instance()->SetOperator(eOp_Terrain);
	}

	virtual const char * GetIcon()
	{
		return "TuQi.jpg";
	}
};

