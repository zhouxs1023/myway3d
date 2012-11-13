#include "stdafx.h"
#include "xObj.h"
#include "xApp.h"

DF_PROPERTY_BEGIN(xObj)
DF_PROPERTY_END()


xObjManager gxObjMgr;

IMP_SLN(xObjManager);

xObjManager::xObjManager()
{
	INIT_SLN;
	xApp::OnShutdown += this;
}

xObjManager::~xObjManager()
{
	xApp::OnShutdown -= this;
	SHUT_SLN;
}

void xObjManager::OnCall(Event * sender, void * data)
{
	if (sender == &xApp::OnShutdown)
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

	xObj * xObj = sf->Create(uxObjName.c_str());

	mObjs.PushBack(xObj);

	return xObj;
}

void xObjManager::Distroy(xObj * xObj)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (xObj == mObjs[i])
		{
			delete mObjs[i];
			return ;
		}
	}

	d_assert (0);
}

xObj * xObjManager::Get(const char * name)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (Strcmp(name, mObjs[i]->GetName()) == 0)
			return mObjs[i];
	}

	return NULL;
}
