#pragma once

#include "GameEntity.h"

class GmMesh : public GmEntity
{
public:
	GmMesh(const char * sMeshFile);
	virtual ~GmMesh();

protected:
	Entity * mEntity;
};
