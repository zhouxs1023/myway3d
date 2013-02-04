#include "stdafx.h"
#include "xPrefab.h"

DF_PROPERTY_BEGIN(xPrefab)
	DF_PROPERTY(xPrefab, PrefabFile, "General", "File", PT_TString, 128)
	DF_PROPERTY(xPrefab, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xPrefab, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xPrefab, Scale, "Transform", "Scale", PT_Float, 4)
DF_PROPERTY_END();

xPrefab::xPrefab(const TString128 & name)
	: xObj(name)
{
	Position = Vec3::Zero;
	Orientation = Quat::Identity;
	Scale = 1;

	mPrefab = new MPrefab(name);

	mPrefab->GetRootNode()->GetFlag().SetFlags(PICK_Flag);
}

xPrefab::~xPrefab()
{
	delete mPrefab;
}

Aabb xPrefab::GetBound()
{
	return mPrefab->GetRootNode()->GetWorldAabb();
}

void xPrefab::Serialize(xSerializer & Serializer)
{
	xObj::Serialize(Serializer);

	int version = 1;

	if (Serializer.IsSave())
	{
		Serializer << version;
		Serializer << PrefabFile;
		Serializer << Position;
		Serializer << Orientation;
		Serializer << Scale;
	}
	else
	{
		Serializer >> version;
		if (version == 0)
		{
			Serializer >> PrefabFile;
			Serializer >> Position;
			Serializer >> Orientation;

			Vec3 scale;
			Serializer >> scale;

			Scale = scale.x;
		}
		else if (version == 1)
		{
			Serializer >> PrefabFile;
			Serializer >> Position;
			Serializer >> Orientation;
			Serializer >> Scale;
		}

		SetPosition(Position);
		SetOrientation(Orientation);
		SetScale(Scale);
		SetPrefabFile(PrefabFile);
	}
}

bool xPrefab::OnPropertyChanged(const Property * p)
{
	if (p->name == "Name")
	{
		SetName(Name);
	}
	else if (p->name == "PrefabFile")
	{
		SetPrefabFile(PrefabFile);
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

void xPrefab::SetName(const TString128 & name)
{
	Name = name;
}

void xPrefab::SetPrefabFile(const TString128 & file)
{
	PrefabFile = file;

	if (mPrefab && mPrefab->GetSourceName() == PrefabFile)
		return ;

	mPrefab->Load(file);
}

xObj * xPrefab::Clone()
{
	xPrefab * Actor = (xPrefab *)xObjManager::Instance()->Create(GetTypeName().c_str());

	Actor->SetPosition(Position);
	Actor->SetOrientation(Orientation);
	Actor->SetScale(Scale);
	Actor->SetPrefabFile(PrefabFile);

	return Actor;
}

bool xPrefab::IsSceneNode(SceneNode * node)
{
	return node == mPrefab->GetRootNode();
}

void xPrefab::SetPosition(const Vec3 & position)
{
	Position = position;
	mPrefab->GetRootNode()->SetPosition(position);
}

void xPrefab::SetOrientation(const Quat & ort)
{
	Orientation = ort;
	mPrefab->GetRootNode()->SetOrientation(ort);
}

void xPrefab::SetScale(float scale)
{
	Scale = scale;
	mPrefab->GetRootNode()->SetScale(scale);
}













xPrefabFactoryListener gListener;


void xPrefabFactoryListener::_OnDragFile(void * param0, void * param1)
{
	Point2f pt = *(Point2f*)param0;
	TString128 file = (const char *)param1;

	file.Lower();
	file.Replace('/', '\\');

	TString128 externName;

	externName = File::GetExternName(file);

	if (externName != "prefab")
		return ;

	const char * testFile = file.c_str();
	int length = file.Length();

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

	xPrefab * obj = (xPrefab *)xObjManager::Instance()->Create("Prefab");

	d_assert (obj);

	//Vec3 pos = xApp::Instance()->GetHitPosition(pt.x, pt.y);

	//obj->SetPosition(pos);
	obj->SetPrefabFile(testFile);

	xApp::Instance()->SetSelectedObj(obj);
}