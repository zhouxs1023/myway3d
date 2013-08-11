#include "stdafx.h"
#include "GameEntity.h"

IGameEntity::IGameEntity()
{
	mNode = World::Instance()->CreateSceneNode();
}

IGameEntity::~IGameEntity()
{
	World::Instance()->DestroySceneNode((SceneNode *)mNode);
}

void IGameEntity::SetPosition(float x, float y, float z)
{
	SetPosition(Vec3(x, y, z));
}

void IGameEntity::SetPosition(const Vec3 & v)
{
	mNode->SetPosition(v);
}

Vec3 IGameEntity::GetPosition() const
{
	return mNode->GetPosition();
}

void IGameEntity::SetOrientation(float x, float y, float z, float w)
{
	SetOrientation(Quat(x, y, z, w));
}

void IGameEntity::SetOrientation(const Quat & q)
{
	mNode->SetOrientation(q);
}

Quat IGameEntity::GetOrientation() const
{
	return mNode->GetOrientation();
}

void IGameEntity::SetScale(float x)
{
	mNode->SetScale(x);
}

float IGameEntity::GetScale()
{
	return mNode->GetScale().x;
}

void IGameEntity::SetVisible(bool b)
{
	mNode->SetVisible(b);
}

bool IGameEntity::IsVisible()
{
	return mNode->IsVisible();
}