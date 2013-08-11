#include "stdafx.h"

#include "GameMesh.h"

namespace Game {

	GameMesh::GameMesh(const char * sMeshFile)
	{
		mEntity = World::Instance()->CreateEntity("GameMesh", sMeshFile);
	}

	GameMesh::~GameMesh()
	{
		World::Instance()->DestroyEntity("GameMesh");
	}

}