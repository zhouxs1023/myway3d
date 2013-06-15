#include "stdafx.h"

#include "Npc.h"
#include "EditorGameMode.h"
#include "GameAI.h"

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

		mNpc = new GameNpc(0);
		mNpc->SetId(EditorGameMode::Instance()->GetUId());
		mNpc->Init();

		EditorGameMode::Instance()->AddObject(mNpc);

		SceneNode * node = RTTI_DynamicCast(SceneNode, mNpc->GetNode());
		node->GetFlag().SetFlags(PICK_Flag);
	}

	Npc::~Npc()
	{
		if (mNpc)
		{
			EditorGameMode::Instance()->RemoveObject(mNpc->GetId());
		}
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