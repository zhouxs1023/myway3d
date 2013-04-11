#include "stdafx.h"
#include "GEntity.h"

namespace game {

	GEntity::GEntity()
	{
		mNode = World::Instance()->CreateSceneNode();
	}

	GEntity::~GEntity()
	{
		World::Instance()->DestroySceneNode(mNode);
	}

	void GEntity::SetPosition(float x, float y, float z)
	{
		SetPosition(Vec3(x, y, z));
	}

	void GEntity::SetPosition(const Vec3 & v)
	{
		mNode->SetPosition(v);
	}

	Vec3 GEntity::GetPosition() const
	{
		return mNode->GetPosition();
	}

	void GEntity::SetOrientation(float x, float y, float z, float w)
	{
		SetOrientation(Quat(x, y, z, w));
	}

	void GEntity::SetOrientation(const Quat & q)
	{
		mNode->SetOrientation(q);
	}

	Quat GEntity::GetOrientation() const
	{
		return mNode->GetOrientation();
	}

	void GEntity::SetScale(float x)
	{
		mNode->SetScale(x);
	}

	float GEntity::GetScale()
	{
		return mNode->GetScale().x;
	}
}