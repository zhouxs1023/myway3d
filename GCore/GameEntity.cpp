#include "stdafx.h"
#include "GameEntity.h"

GmEntity::GmEntity()
{
	mNode = World::Instance()->CreateSceneNode();
}

GmEntity::~GmEntity()
{
	World::Instance()->DestroySceneNode((SceneNode *)mNode);
}

void GmEntity::SetPosition(float x, float y, float z)
{
	SetPosition(Vec3(x, y, z));
}

void GmEntity::SetPosition(const Vec3 & v)
{
	mNode->SetPosition(v);
}

Vec3 GmEntity::GetPosition() const
{
	return mNode->GetPosition();
}

void GmEntity::SetOrientation(float x, float y, float z, float w)
{
	SetOrientation(Quat(x, y, z, w));
}

void GmEntity::SetOrientation(const Quat & q)
{
	mNode->SetOrientation(q);
}

Quat GmEntity::GetOrientation() const
{
	return mNode->GetOrientation();
}

void GmEntity::SetScale(float x)
{
	mNode->SetScale(x);
}

float GmEntity::GetScale()
{
	return mNode->GetScale().x;
}

void GmEntity::SetVisible(bool b)
{
	mNode->SetVisible(b);
}

bool GmEntity::IsVisible()
{
	return mNode->IsVisible();
}