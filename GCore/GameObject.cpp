#include "stdafx.h"

#include "GameObject.h"

ImplementRootRTTI(IGameController);

ImplementRootRTTI(IGameComponent);

ImplementRootRTTI(IGameObject);

IGameObject::IGameObject()
{
}

IGameObject::~IGameObject()
{
	RemoveAllComponent();
}

void IGameObject::SetId(int id)
{
	mId = id;
}

int IGameObject::GetId() const
{
	return mId;
}

void IGameObject::SetName(const char * name)
{
	mName = name;
}

const char * IGameObject::GetName() const
{
	return mName.c_str();
}

void IGameObject::Update(float frameTime)
{
	for (int i = 0; i < mComponents.Size(); ++i)
	{
		mComponents[i]->Update(frameTime);
	}
}

void IGameObject::AddComponent(IGameComponent * cp)
{
	cp->SetObject(this);
	mComponents.PushBack(cp);
}

void IGameObject::RemoveComponent(IGameComponent * cp)
{
	for (int i = 0; i < mComponents.Size(); ++i)
	{
		if (mComponents[i] == cp)
		{
			RemoveComponent(i);
			return ;
		}
	}

	d_assert (0);
}

void IGameObject::RemoveComponent(int index)
{
	d_assert (index < mComponents.Size());

	delete mComponents[index];
}

void IGameObject::RemoveAllComponent()
{
	for (int i = 0; i < mComponents.Size(); ++i)
		delete mComponents[i];

	mComponents.Clear();
}

int IGameObject::GetComponetCount()
{
	return mComponents.Size();
}

IGameComponent * IGameObject::GetComponent(int index)
{
	d_assert (index < mComponents.Size());

	return mComponents[index];
}
