#include "stdafx.h"

#include "GMesh.h"

namespace game {

	GMesh::GMesh(const char * name, const char * meshFile)
	{
		mEntity = World::Instance()->CreateEntity(name, meshFile);
		mNode->Attach(mEntity);
	}

	GMesh::~GMesh()
	{
		World::Instance()->DestroyEntity(mEntity);
	}

}