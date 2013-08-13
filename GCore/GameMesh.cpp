#include "stdafx.h"

#include "GameMesh.h"

GmMesh::GmMesh(const char * sMeshFile)
{
	mEntity = World::Instance()->CreateEntity("GmMesh", sMeshFile);
	mNode->Attach(mEntity);
}

GmMesh::~GmMesh()
{
	World::Instance()->DestroyEntity("GmMesh");
}