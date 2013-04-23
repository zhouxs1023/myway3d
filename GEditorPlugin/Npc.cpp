#include "stdafx.h"

#include "Npc.h"

namespace game {

	DF_PROPERTY_BEGIN(Npc)
		DF_PROPERTY(Npc, Position, "Transform", "Position", PT_Vec3, 12)
		DF_PROPERTY(Npc, Orientation, "Transform", "Orientation", PT_Vec4, 16)
		DF_PROPERTY(Npc, Scale, "Transform", "Scale", PT_Float, 4)
	DF_PROPERTY_END();

	Npc::Npc(const TString128 & name)
		: Shape(name)
	{
		Position = Vec3::Zero;
		Orientation = Quat::Identity;
		Scale = 1;

		mNpc = GWorld::Instance()->CreateNpc(-1, 0);

		mNpc->GetNode()->GetFlag().SetFlags(PICK_Flag);
	}

	Npc::~Npc()
	{
		if (mNpc != NULL)
			GWorld::Instance()->DestroyNpc(mNpc);
	}

	void Npc::SetName(const TString128 & name)
	{
		Name = name;
	}

	Shape * Npc::Clone()
	{
		return NULL;
	}

	bool Npc::IsSceneNode(SceneNode * node)
	{
		return mNpc->GetNode() == node;
	}

	void Npc::SetPosition(const Vec3 & p)
	{
		Position = p;
		mNpc->SetPosition(p);
	}

	void Npc::SetOrientation(const Quat & q)
	{
		Orientation = q;
		mNpc->SetOrientation(q);
	}

	void Npc::SetScale(float s)
	{
		Scale = s;
		mNpc->SetScale(s);
	}

	void Npc::Serialize(xSerializer & serializer)
	{
	}

	Aabb Npc::GetBound()
	{
		return mNpc->GetNode()->GetWorldAabb();
	}

	bool Npc::OnPropertyChanged(const Property * p)
	{
		return false;
	}
}