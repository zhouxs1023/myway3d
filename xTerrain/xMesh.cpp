#include "stdafx.h"
#include "xMesh.h"


DF_PROPERTY_BEGIN(xMesh)
    DF_PROPERTY(xMesh, MeshFile, "General", "File", PT_TString, 128)
	DF_PROPERTY(xMesh, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xMesh, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xMesh, Scale, "Transform", "Scale", PT_Vec3, 12)
DF_PROPERTY_END();

xMesh::xMesh(const TString128 & name)
    : xObj(name)
{
	MeshFile = "temp.mesh";
	Position = Vec3::Zero;
	Orientation = Quat::Identity;
	Scale = Vec3::Unit;

    mNode = World::Instance()->CreateSceneNode();
    mEntity = World::Instance()->CreateEntity(name, MeshFile);
	mNode->Attach(mEntity);
	
}

xMesh::~xMesh()
{
    if (mEntity)
        World::Instance()->DestroyEntity(mEntity);

    if (mNode)
        World::Instance()->DestroySceneNode(mNode);
}

Aabb xMesh::GetBound()
{
	return mNode->GetWorldAabb();
}

void xMesh::Serialize(xSerializer & Serializer)
{
	xObj::Serialize(Serializer);

	int version = 0;

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

void xMesh::SetScale(const Vec3 & scale)
{
	Scale = scale;
    mNode->SetScale(scale);
}

xMeshFactoryListener gListener;


void xMeshFactoryListener::_OnDragFile(void * param0, void * param1)
{
	Point2f pt = *(Point2f*)param0;
	TString128 meshFile = (const char *)param1;
	TString128 base, path;

	meshFile.Lower();
	meshFile.SplitFileNameR(base, path); 

	TString128 externName;

	externName = File::GetExternName(base);

	if (externName != "mesh")
		return ;

	xMesh * obj = (xMesh *)xObjManager::Instance()->Create("Mesh");

	d_assert (obj);

	Vec3 pos = xApp::Instance()->GetHitPosition(pt.x, pt.y);

	obj->SetPosition(pos);
	obj->SetScale(0.01f);
	obj->SetMeshFile(base);

	xApp::Instance()->SetSelectedObj(obj);
}