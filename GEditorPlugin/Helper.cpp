#include "stdafx.h"

#include "Helper.h"

ImplementRTTI(Helper, Shape);

Helper::Helper(const TString128 & name)
	: Shape(name)
{
	Position = Vec3(0, 0, 0);
	Radius = 1;
	UserString = "";

	MeshPtr mesh = MeshManager::Instance()->Find("GameHelper");

	if (mesh == NULL)
		mesh = MeshManager::Instance()->CreateSphere("GameHelper", 10, 10, 1);

	mEntity = World::Instance()->CreateEntity("GameHelper", mesh);
	mNode = World::Instance()->CreateSceneNode("GameHelper");
	mNode->Attach(mEntity);

	mNode->GetFlag().SetFlags(PICK_Flag);

	mesh->GetSubMesh(0)->GetMaterial()->SetFillMode(FILL_FRAME);
	
	if (mesh->GetColMesh() == NULL)
		mesh->GenColMeshFromRenderMesh();

	IColObjPtr colObj = IPhyWorld::Instance()->GetColMesh(mesh.c_ptr(), Radius);

	if (colObj == NULL)
		colObj = IPhyWorld::Instance()->AddColMesh(mesh.c_ptr(), mesh->GetColMesh(), Radius);

	IPhyWorld::Instance()->AddNode(mNode, colObj);
}

Helper::~Helper()
{
	IPhyWorld::Instance()->RemoveNode(mNode);
	World::Instance()->DestroyEntity(mEntity);
	World::Instance()->DestroySceneNode(mNode);
}

void Helper::SetName(const TString128 & name)
{
	Name = name;
}

Shape * Helper::Clone()
{
	return NULL;
}

bool Helper::IsSceneNode(SceneNode * node)
{
	return mNode == node;
}

void Helper::SetPosition(const Vec3 & p)
{
	Position = p;
	mNode->SetPosition(p);
}

void Helper::SetScale(float s)
{
	Radius = s;
	mNode->SetScale(s);
	IPhyWorld::Instance()->OnNodeScaleChanged(mNode);
}

void Helper::SetUserString(const TString128 & str)
{
	UserString = str;
}

void Helper::Serialize(xSerializer & serializer)
{
	const int K_Version = 1;

	if (serializer.IsSave())
	{
		serializer << K_Version;
		serializer << Name;
		serializer << Position;
		serializer << Radius;
		serializer << UserString;
	}
	else
	{
		int version = K_Version;

		serializer >> version; d_assert(version == K_Version);
		serializer >> Name;
		serializer >> Position;
		serializer >> Radius;
		serializer >> UserString;

		SetName(Name);
		SetPosition(Position);
		SetScale(Radius);
	}
}

Aabb Helper::GetBound()
{
	return mNode->GetWorldAabb();
}

ColMesh * Helper::GetColMesh()
{
	return mEntity->GetMesh()->GetColMesh();
}

bool Helper::OnPropertyChanged(const Property * p)
{
	return false;
}