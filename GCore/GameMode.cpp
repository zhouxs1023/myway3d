#include "stdafx.h"

#include "GameMode.h"

GmMode::GmMode()
{
}

GmMode::~GmMode()
{
	RemoveAllController();
	RemoveAllObject();
}

int GmMode::GetUId()
{
	return mUId;
}

void GmMode::Update(float frameTime)
{
	for (int i = 0; i < mControllers.Size(); ++i)
	{
		if (mControllers[i]->GetEnable())
			mControllers[i]->Update(frameTime);
	}

	for (int i = 0; i < mObjects.Size(); ++i)
	{
		mObjects[i]->Update(frameTime);
	}
}

void GmMode::AddObject(GmObj * obj)
{
	if (obj->GetId() == -1)
	{
		obj->SetId(mUId++);
	}

	d_assert (GetObject(obj->GetId()) == NULL);

	mObjects.PushBack(obj);

	mUId = Math::Maximum(mUId, obj->GetId() + 1);
}


GmObj * GmMode::GetObject(int id)
{
	for (int i = 0; i < mObjects.Size(); ++i)
	{
		if (mObjects[i]->GetId() == id)
		{
			return mObjects[i];
		}
	}

	return NULL;
}

void GmMode::RemoveObject(int id)
{
	for (int i = 0; i < mObjects.Size(); ++i)
	{
		if (mObjects[i]->GetId() == id)
		{
			delete mObjects[i];
			mObjects.Erase(i);
			return ;
		}
	}

	d_assert (0);
}

void GmMode::RemoveAllObject()
{
	for (int i = 0; i < mObjects.Size(); ++i)
	{
		delete mObjects[i];
	}

	mObjects.Clear();
}

int GmMode::GetObjectCount()
{
	return mObjects.Size();
}

GmObj * GmMode::GetObjectByIndex(int index)
{
	return mObjects[index];
}

void GmMode::AddController(GmController * ctrl)
{
	mControllers.PushBack(ctrl);
}

GmController * GmMode::GetController(int index)
{
	return mControllers[index];
}

int GmMode::GetControllerCount()
{
	return mControllers.Size();
}

void GmMode::RemoveController(int index)
{
	delete mControllers[index];
	mControllers.Erase(index);
}

void GmMode::RemoveController(GmController * ctrl)
{
	for (int i = 0; i < mControllers.Size(); ++i)
	{
		if (mControllers[i] == ctrl)
		{
			delete mControllers[i];
			mControllers.Erase(i);
			return ;
		}
	}

	d_assert (0);
}

void GmMode::RemoveAllController()
{
	for (int i = 0; i < mControllers.Size(); ++i)
	{
		delete mControllers[i];
	}

	mControllers.Clear();
}





void GmMode::AddMessage(GmMsg * msg)
{
	if (msg->GetTimeDelay() <= 0)
	{
		msg->Do();
	}
	else
	{
		List<GmMsg *>::Iterator whr = mMessages.Begin();
		List<GmMsg *>::Iterator end = mMessages.End();

		while (whr != end)
		{
			GmMsg * m = *whr;

			float t0 = msg->GetTimeDelay();
			float t1 = m->GetTimeDelay();

			if (t0 <= t1)
			{
				mMessages.Insert(whr, msg);
				return ;
			}

			++whr;
		}

		mMessages.PushBack(msg);
	}
}