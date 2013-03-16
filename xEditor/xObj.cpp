#include "stdafx.h"
#include "xObj.h"
#include "xApp.h"
#include "xScene.h"

const Aabb xObj::K_DefaultBound = Aabb(-5, -5, -5, 5, 5, 5);
const Sphere xObj::K_DefaultSphere = Sphere(0, 0, 0, 5);

DF_PROPERTY_BEGIN(xObj)
	DF_PROPERTY(xObj, Name, "General", "Name", PT_TString, 128)
DF_PROPERTY_END()

xObj::xObj(const TString128 & name)
{
	Name = name;
}

xObj::~xObj()
{
}

const TString128 & xObj::GetName()
{
	return Name;
}

void xObj::Serialize(xSerializer & Serializer)
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



IMP_SLN(xObjManager);

xObjManager::xObjManager()
	: OnShutdown(xEvent::OnShutdown, this, &xObjManager::_Shutdown)
	, OnSerialize(xEvent::OnSerialize, this, &xObjManager::_Serialize)
	, OnUnloadScene(xEvent::OnUnloadScene, this, &xObjManager::_UnloadScene)
{
	INIT_SLN;
}

xObjManager::~xObjManager()
{
	SHUT_SLN;
}

void xObjManager::_UnloadScene(Event * sender)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		delete mObjs[i];
	}

	mObjs.Clear();
}

void xObjManager::_Serialize(Event * sender)
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
	}
}

void xObjManager::_Load(xSerializer & Serializer)
{
	int numOfObjs;

	Serializer >> numOfObjs;

	for (int i = 0; i < numOfObjs; ++i)
	{
		TString128 Name;
		TString128 Type;

		Serializer >> Name;
		Serializer >> Type;

		xObj * obj = _Create(Name, Type);

		obj->Serialize(Serializer);
	}
}

void xObjManager::_Save(xSerializer & Serializer)
{
	int numOfObjs = mObjs.Size();

	Serializer << numOfObjs;

	for (int i = 0; i < numOfObjs; ++i)
	{
		xObj * obj = mObjs[i];

		TString128 Name = obj->GetName();
		TString128 Type = obj->GetTypeName();

		Serializer << Name;
		Serializer << Type;

		obj->Serialize(Serializer);
	}
}

void xObjManager::_Shutdown(Event * sender)
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

void xObjManager::AddFactory(xObjFactory * sf)
{
	d_assert (GetFactory(sf->GetTypeName()) == NULL);

	mFactorys.PushBack(sf);
}

xObjFactory * xObjManager::GetFactory(const char * type)
{
	for (int i = 0; i < mFactorys.Size(); ++i)
	{
		if (Strcmp(type, mFactorys[i]->GetTypeName()) ==0)
			return mFactorys[i];
	}

	return NULL;
}

int xObjManager::GetFactoryCount()
{
	return mFactorys.Size();
}

xObjFactory * xObjManager::GetFactory(int index)
{
	d_assert (index < GetFactoryCount());
	return mFactorys[index];
}


xObj * xObjManager::Create(const char * type)
{
	xObjFactory * sf = GetFactory(type);

	if (!sf)
		return NULL;

	d_assert (sf);

	int uId = 0;
	TString128 uName = "xObj";
	TString128 uxObjName;
	bool flag = true;

	while (1)
	{
		uxObjName = uName + uId++;

		if (Get(uxObjName.c_str()) == NULL)
			break;
	}

	xObj * obj = sf->Create(uxObjName.c_str());

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

xObj * xObjManager::_Create(const TString128 & name, const TString128 & type)
{
	xObjFactory * sf = GetFactory(type.c_str());

	xObj * obj = sf->Create(name.c_str());

	d_assert (obj);

	mObjs.PushBack(obj);

	return obj;
}


void xObjManager::Distroy(xObj * obj)
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

xObj * xObjManager::Get(const char * name)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (mObjs[i]->GetName() == name)
			return mObjs[i];
	}

	return NULL;
}

xObj * xObjManager::Get(SceneNode * node)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (mObjs[i]->IsSceneNode(node))
			return mObjs[i];
	}

	return NULL;
}
