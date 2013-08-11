#include "stdafx.h"
#include "Shape.h"
#include "xEvent.h"
#include "xScene.h"

namespace Infinite {

const Aabb Shape::K_DefaultBound = Aabb(-5, -5, -5, 5, 5, 5);
const Sphere Shape::K_DefaultSphere = Sphere(0, 0, 0, 5);

DF_PROPERTY_BEGIN(Shape)
	DF_PROPERTY(Shape, Name, "General", "Name", PT_TString, 128)
DF_PROPERTY_END()

ImplementRootRTTI(Shape);

Shape::Shape(const TString128 & name)
{
	Name = name;
}

Shape::~Shape()
{
}

const TString128 & Shape::GetName()
{
	return Name;
}

void Shape::Serialize(xSerializer & Serializer)
{
	if (Serializer.IsSave())
	{
		// do something
	}
	else
	{
		// do something
	}
}



IMP_SLN(ShapeManager);

ShapeManager::ShapeManager()
	: OnSerialize(xEvent::OnSerialize, this, &ShapeManager::_Serialize)
	, OnUnloadScene(xEvent::OnUnloadScene, this, &ShapeManager::_UnloadScene)
{
	INIT_SLN;
}

ShapeManager::~ShapeManager()
{
	SHUT_SLN;
}

void ShapeManager::_UnloadScene(Event * sender)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		delete mObjs[i];
	}

	mObjs.Clear();
}

void ShapeManager::_Serialize(Event * sender)
{
	const int CHUNK_ID = 'OBJS';

	int chunkId = *(int*)sender->GetParam(0);
	xSerializer & Serializer = *(xSerializer*)sender->GetParam(1);

	if (Serializer.IsSave())
	{
		Serializer << CHUNK_ID;
		_Save(Serializer);
	}
	else if (CHUNK_ID == chunkId)
	{
		_Load(Serializer);
		xScene::Instance()->DirtLoadChunk();
	}
}

void ShapeManager::_Load(xSerializer & Serializer)
{
	int numOfObjs;

	Serializer >> numOfObjs;

	for (int i = 0; i < numOfObjs; ++i)
	{
		TString128 Name;
		TString128 Type;

		Serializer >> Name;
		Serializer >> Type;

		Shape * obj = _Create(Name, Type);

		obj->Serialize(Serializer);
	}
}

void ShapeManager::_Save(xSerializer & Serializer)
{
	int numOfObjs = mObjs.Size();

	Serializer << numOfObjs;

	for (int i = 0; i < numOfObjs; ++i)
	{
		Shape * obj = mObjs[i];

		TString128 Name = obj->GetName();
		TString128 Type = obj->GetTypeName();

		Serializer << Name;
		Serializer << Type;

		obj->Serialize(Serializer);
	}
}

void ShapeManager::_Shutdown(Event * sender)
{
	for (int i = 0; i < mFactorys.Size(); ++i)
	{
		delete mFactorys[i];
	}

	for (int i = 0; i < mObjs.Size(); ++i)
	{
		delete mObjs[i];
	}

	mFactorys.Clear();
	mObjs.Clear();
}

void ShapeManager::AddFactory(ShapeFactory * sf)
{
	d_assert (GetFactory(sf->GetTypeName()) == NULL);

	mFactorys.PushBack(sf);
}

ShapeFactory * ShapeManager::GetFactory(const char * type)
{
	for (int i = 0; i < mFactorys.Size(); ++i)
	{
		if (Strcmp(type, mFactorys[i]->GetTypeName()) ==0)
			return mFactorys[i];
	}

	return NULL;
}

int ShapeManager::GetFactoryCount()
{
	return mFactorys.Size();
}

ShapeFactory * ShapeManager::GetFactory(int index)
{
	d_assert (index < GetFactoryCount());
	return mFactorys[index];
}


Shape * ShapeManager::Create(const char * type)
{
	ShapeFactory * sf = GetFactory(type);

	if (!sf)
		return NULL;

	d_assert (sf);

	int uId = 0;
	TString128 uName = "Shape";
	TString128 uShapeName;
	bool flag = true;

	while (1)
	{
		uShapeName = uName + uId++;

		if (Get(uShapeName.c_str()) == NULL)
			break;
	}

	Shape * obj = sf->Create(uShapeName.c_str());

	if (obj)
	{
		mObjs.PushBack(obj);
		if (!xScene::Instance()->IsLoading())
		{
			xEvent::OnObjCreated(obj, NULL);
		}
	}

	return obj;
}

Shape * ShapeManager::_Create(const TString128 & name, const TString128 & type)
{
	ShapeFactory * sf = GetFactory(type.c_str());

	Shape * obj = sf->Create(name.c_str());

	d_assert (obj);

	mObjs.PushBack(obj);

	return obj;
}


void ShapeManager::Distroy(Shape * obj)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (obj == mObjs[i])
		{
			xEvent::OnObjDistroy(obj, NULL);
			delete mObjs[i];
			mObjs.Erase(i);
			return ;
		}
	}

	d_assert (0);
}

Shape * ShapeManager::Get(const char * name)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (mObjs[i]->GetName() == name)
			return mObjs[i];
	}

	return NULL;
}

Shape * ShapeManager::Get(SceneNode * node)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (mObjs[i]->IsSceneNode(node))
			return mObjs[i];
	}

	return NULL;
}

int ShapeManager::GetShapeCount()
{
	return mObjs.Size();
}

Shape * ShapeManager::GetShape(int index)
{
	return mObjs[index];
}

}
