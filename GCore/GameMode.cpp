#include "stdafx.h"

#include "GameMode.h"

IGameMode::IGameMode()
{
}

IGameMode::~IGameMode()
{
	RemoveAllController();
	RemoveAllObject();
}

int IGameMode::GetUId()
{
	return mUId;
}

void IGameMode::Update(float frameTime)
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

void IGameMode::AddObject(IGameObject * obj)
{
	if (obj->GetId() == -1)
	{
		obj->SetId(mUId++);
	}

	d_assert (GetObject(obj->GetId()) == NULL);

	mObjects.PushBack(obj);

	mUId = Math::Maximum(mUId, obj->GetId() + 1);
}


IGameObject * IGameMode::GetObject(int id)
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

void IGameMode::RemoveObject(int id)
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

void IGameMode::RemoveAllObject()
{
	for (int i = 0; i < mObjects.Size(); ++i)
	{
		delete mObjects[i];
	}

	mObjects.Clear();
}

int IGameMode::GetObjectCount()
{
	return mObjects.Size();
}

IGameObject * IGameMode::GetObjectByIndex(int index)
{
	return mObjects[index];
}

void IGameMode::AddController(IGameController * ctrl)
{
	mControllers.PushBack(ctrl);
}

IGameController * IGameMode::GetController(int index)
{
	return mControllers[index];
}

int IGameMode::GetControllerCount()
{
	return mControllers.Size();
}

void IGameMode::RemoveController(int index)
{
	delete mControllers[index];
	mControllers.Erase(index);
}

void IGameMode::RemoveController(IGameController * ctrl)
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

void IGameMode::RemoveAllController()
{
	for (int i = 0; i < mControllers.Size(); ++i)
	{
		delete mControllers[i];
	}

	mControllers.Clear();
}





void IGameMode::AddMessage(IGameMessage * msg)
{
	if (msg->GetTimeDelay() <= 0)
	{
		msg->Do();
	}
	else
	{
		List<IGameMessage *>::Iterator whr = mMessages.Begin();
		List<IGameMessage *>::Iterator end = mMessages.End();

		while (whr != end)
		{
			IGameMessage * m = *whr;

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