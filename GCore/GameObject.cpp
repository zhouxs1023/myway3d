#include "stdafx.h"

#include "GameObject.h"

ImplementRootRTTI(GmController);

ImplementRootRTTI(GmComponent);

ImplementRootRTTI(GmObj);

GmObj::GmObj()
{
}

GmObj::~GmObj()
{
	RemoveAllComponent();
}

void GmObj::SetId(int id)
{
	mId = id;
}

int GmObj::GetId() const
{
	return mId;
}

void GmObj::SetName(const char * name)
{
	mName = name;
}

const char * GmObj::GetName() const
{
	return mName.c_str();
}

void GmObj::Update(float frameTime)
{
	for (int i = 0; i < mComponents.Size(); ++i)
	{
		mComponents[i]->Update(frameTime);
	}
}

void GmObj::AddComponent(GmComponent * cp)
{
	cp->SetObject(this);
	mComponents.PushBack(cp);
}

void GmObj::RemoveComponent(GmComponent * cp)
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

void GmObj::RemoveComponent(int index)
{
	d_assert (index < mComponents.Size());

	delete mComponents[index];
}

void GmObj::RemoveAllComponent()
{
	for (int i = 0; i < mComponents.Size(); ++i)
		delete mComponents[i];

	mComponents.Clear();
}

int GmObj::GetComponetCount()
{
	return mComponents.Size();
}

GmComponent * GmObj::GetComponent(int index)
{
	d_assert (index < mComponents.Size());

	return mComponents[index];
}
