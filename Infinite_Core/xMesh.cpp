#include "stdafx.h"
#include "xMesh.h"

namespace Infinite {

DF_PROPERTY_BEGIN(xMesh)
	DF_PROPERTY(xMesh, MeshFile, "General", "File", PT_TString, 128)
    DF_PROPERTY(xMesh, AnimName, "General", "Animation", PT_TString, 128)
	DF_PROPERTY(xMesh, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xMesh, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xMesh, Scale, "Transform", "Scale", PT_Float, 4)
DF_PROPERTY_END();

xMesh::xMesh(const TString128 & name)
    : Shape(name)
	, OnUpdate(xEvent::OnUpdate, this, &xMesh::_Update)
{
	MeshFile = "";
	Position = Vec3::Zero;
	Orientation = Quat::Identity;
	Scale = 1;

	mAnimState = NULL;

	mNode = World::Instance()->CreateSceneNode();
	mEntity = World::Instance()->CreateEntity(name);
	mNode->Attach(mEntity);

	mNode->GetFlag().SetFlags(PICK_Flag);

	mEntity->SetBounds(Shape::K_DefaultBound, Shape::K_DefaultSphere);
}

xMesh::~xMesh()
{
	IPhyWorld::Instance()->RemoveNode(mNode);
    World::Instance()->DestroyEntity(mEntity);
    World::Instance()->DestroySceneNode(mNode);
}

Aabb xMesh::GetBound()
{
	return mNode->GetWorldAabb();
}

void xMesh::Serialize(xSerializer & Serializer)
{
	Shape::Serialize(Serializer);

	int version = 1;

	if (Serializer.IsSave())
	{
		Serializer << version;
		Serializer << MeshFile;
		Serializer << Position;
		Serializer << Orientation;
		Serializer << Scale;
	}
	else
	{
		Serializer >> version;
		if (version == 0)
		{
			Serializer >> MeshFile;
			Serializer >> Position;
			Serializer >> Orientation;

			Vec3 scale;
			Serializer >> scale;

			Scale = scale.x;
		}
		else if (version == 1)
		{
			Serializer >> MeshFile;
			Serializer >> Position;
			Serializer >> Orientation;
			Serializer >> Scale;
		}

		SetPosition(Position);
		SetOrientation(Orientation);
		SetScale(Scale);
		SetMeshFile(MeshFile);
	}
}

bool xMesh::OnPropertyChanged(const Property * p)
{
	if (p->name == "Name")
	{
		SetName(Name);
	}
	else if (p->name == "MeshFile")
	{
		SetMeshFile(MeshFile);
	}
	else if (p->name == "AnimName")
	{
		SetAnimName(AnimName);
	}
	else if (p->name == "Position")
	{
		SetPosition(Position);
	}
	else if (p->name == "Orientation")
	{
		SetOrientation(Orientation);
	}
	else if (p->name == "Scale")
	{
		SetScale(Scale);
	}

	return true;
}

void xMesh::SetName(const TString128 & name)
{
	Name = name;
    World::Instance()->RenameEntity(name, mEntity);
}

void xMesh::SetMeshFile(const TString128 & meshFile)
{
	MeshFile = meshFile;

    if (mEntity && mEntity->GetMesh() != NULL && mEntity->GetMesh()->GetSourceName() == meshFile)
        return ;

    mEntity->SetMesh(meshFile);

	IPhyWorld::Instance()->RemoveNode(mNode);

	if (mEntity->GetMesh() == NULL)
		return ;

	if (mEntity->GetMesh()->GetColMesh() == NULL)
		mEntity->GetMesh()->GenColMeshFromRenderMesh();

	IColObjPtr colObj = IPhyWorld::Instance()->GetColMesh(mEntity->GetMesh().c_ptr(), Scale);

	if (colObj == NULL)
		colObj = IPhyWorld::Instance()->AddColMesh(mEntity->GetMesh().c_ptr(), mEntity->GetMesh()->GetColMesh(), Scale);

	IPhyWorld::Instance()->AddNode(mNode, colObj);
}

void xMesh::SetAnimName(const TString128 & animName)
{
	AnimName = animName;

	if (mAnimState && mAnimState->GetName() == animName)
		return ;

	if (mAnimState)
	{
		mAnimState->SetEnable(false);
		mAnimState->SetPosition(0);
		mAnimState = NULL;
	}

	mAnimState = mEntity->GetAnimationSet()->GetState(AnimName);

	if (mAnimState)
	{
		mAnimState->SetEnable(true);
		mAnimState->SetPosition(0);
		mAnimState->SetLoop(true);
	}
}

Shape * xMesh::Clone()
{
	xMesh * mesh = (xMesh *)ShapeManager::Instance()->Create(GetTypeName().c_str());

	mesh->SetPosition(Position);
	mesh->SetOrientation(Orientation);
	mesh->SetScale(Scale);
	mesh->SetMeshFile(MeshFile);

	return mesh;
}

bool xMesh::IsSceneNode(SceneNode * node)
{
	return node == mNode;
}

void xMesh::SetPosition(const Vec3 & position)
{
	Position = position;
    mNode->SetPosition(position);
}

void xMesh::SetOrientation(const Quat & ort)
{
	Orientation = ort;
	mNode->SetOrientation(ort);
}

void xMesh::SetScale(float scale)
{
	Scale = scale;
    mNode->SetScale(scale);

	IPhyWorld::Instance()->OnNodeScaleChanged(mNode);
}

void xMesh::_Update(Event * sender)
{
	float time = Engine::Instance()->GetFrameTime();

	if (mAnimState)
		mAnimState->AddTime(time);

	mEntity->UpdateAnimation();
}

ColMesh * xMesh::GetColMesh()
{
	if (mEntity && mEntity->GetMesh() != NULL)
		return mEntity->GetMesh()->GetColMesh();

	return NULL;
}
















void xMeshFactory::_OnDragFile(Event * sender)
{
	Point2f pt = *(Point2f*)sender->GetParam(0);
	TString128 ActorFile = (const char *)sender->GetParam(1);

	ActorFile.Lower();
	ActorFile.Replace('/', '\\');

	TString128 externName;

	externName = File::GetExternName(ActorFile);

	if (externName != "mesh")
		return ;

	const char * testFile = ActorFile.c_str();
	int length = ActorFile.Length();

	bool exist = false;

	while (length > 0)
	{
		if (testFile[length - 1] == '\\')
		{
			const char * tfile = testFile + length;
			exist = ResourceManager::Instance()->Exist(tfile);

			if (exist)
				break;
		}

		--length;
	}

	if (!exist)
		return ;

	testFile += length;

	xMesh * obj = (xMesh *)ShapeManager::Instance()->Create("Mesh");

	d_assert (obj);

	Vec3 pos = Editor::Instance()->GetHitPosition(pt.x, pt.y);

	obj->SetPosition(pos);
	obj->SetMeshFile(testFile);

	Editor::Instance()->SetSelectedShape(obj);
}

}

