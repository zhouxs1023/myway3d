#include "stdafx.h"
#include "xActor.h"

DF_PROPERTY_BEGIN(xActor)
	DF_PROPERTY(xActor, ActorFile, "General", "File", PT_TString, 128)
	DF_PROPERTY(xActor, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xActor, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xActor, Scale, "Transform", "Scale", PT_Float, 4)
DF_PROPERTY_END();

xActor::xActor(const TString128 & name)
    : xObj(name)
{
	ActorFile = "Actor\\Test.lma";
	Position = Vec3::Zero;
	Orientation = Quat::Identity;
	Scale = 1;

	mNode = World::Instance()->CreateSceneNode();
	mActor = MActorManager::Instance()->CreateActor(name, ActorFile);
	mNode->Attach(mActor);

	mNode->GetFlag().SetFlags(PICK_Flag);
}

xActor::~xActor()
{
    if (mActor)
        MActorManager::Instance()->DestroyActor(mActor);

    if (mNode)
        World::Instance()->DestroySceneNode(mNode);
}

Aabb xActor::GetBound()
{
	return mNode->GetWorldAabb();
}

void xActor::Serialize(xSerializer & Serializer)
{
	xObj::Serialize(Serializer);

	int version = 1;

	if (Serializer.IsSave())
	{
		Serializer << version;
		Serializer << ActorFile;
		Serializer << Position;
		Serializer << Orientation;
		Serializer << Scale;
	}
	else
	{
		Serializer >> version;
		if (version == 0)
		{
			Serializer >> ActorFile;
			Serializer >> Position;
			Serializer >> Orientation;

			Vec3 scale;
			Serializer >> scale;

			Scale = scale.x;
		}
		else if (version == 1)
		{
			Serializer >> ActorFile;
			Serializer >> Position;
			Serializer >> Orientation;
			Serializer >> Scale;
		}

		SetPosition(Position);
		SetOrientation(Orientation);
		SetScale(Scale);
		SetActorFile(ActorFile);
	}
}

bool xActor::OnPropertyChanged(const Property * p)
{
	if (p->name == "Name")
	{
		SetName(Name);
	}
	else if (p->name == "ActorFile")
	{
		SetActorFile(ActorFile);
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

void xActor::SetName(const TString128 & name)
{
	Name = name;
}

void xActor::SetActorFile(const TString128 & file)
{
	ActorFile = file;

    if (mActor && mActor->GetPart(0) != NULL && mActor->GetPart(0)->GetSourceName() == ActorFile)
        return ;

	MActorResPtr res = MActorManager::Instance()->_LoadActorRes(file, 0);

	mActor->SetPart(0, res);
}

xObj * xActor::Clone()
{
	xActor * Actor = (xActor *)xObjManager::Instance()->Create(GetTypeName().c_str());

	Actor->SetPosition(Position);
	Actor->SetOrientation(Orientation);
	Actor->SetScale(Scale);
	Actor->SetActorFile(ActorFile);

	return Actor;
}

bool xActor::IsSceneNode(SceneNode * node)
{
	return node == mNode;
}

void xActor::SetPosition(const Vec3 & position)
{
	Position = position;
    mNode->SetPosition(position);
}

void xActor::SetOrientation(const Quat & ort)
{
	Orientation = ort;
	mNode->SetOrientation(ort);
}

void xActor::SetScale(float scale)
{
	Scale = scale;
    mNode->SetScale(scale);
}








xActorFactoryListener gListener;


void xActorFactoryListener::_OnDragFile(Event * sender)
{
	Point2f pt = *(Point2f*)sender->GetParam(0);
	TString128 ActorFile = (const char *)sender->GetParam(1);

	ActorFile.Lower();
	ActorFile.Replace('/', '\\');

	TString128 externName;

	externName = File::GetExternName(ActorFile);

	if (externName != "lma")
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

	xActor * obj = (xActor *)xObjManager::Instance()->Create("Actor");

	d_assert (obj);

	Vec3 pos = xApp::Instance()->GetHitPosition(pt.x, pt.y);

	obj->SetPosition(pos);
	obj->SetActorFile(testFile);

	xApp::Instance()->SetSelectedObj(obj);
}

