#pragma once

#include "xBaseOperator.h"

class xTerrainOp : public xBaseOp
{
public:
	static const int eOp_Terrain;

	virtual void Process();

	virtual const char * GetIcon() { return "TuQi.jpg"; }
};

