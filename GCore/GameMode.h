#pragma once

#include "GameMessage.h"
#include "GameObject.h"

class GCORE_ENTRY IGameMode
{
public:
	IGameMode();
	virtual ~IGameMode();

	int GetUId();

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Update(float frameTime);

	void AddObject(IGameObject * obj);
	IGameObject * GetObject(int id);
	IGameObject * GetObjectByIndex(int index);
	int GetObjectCount();
	void RemoveObject(int id);
	void RemoveAllObject();

	void AddController(IGameController * ctrl);
	IGameController * GetController(int index);
	int GetControllerCount();
	void RemoveController(int index);
	void RemoveController(IGameController * ctrl);
	void RemoveAllController();

	void AddMessage(IGameMessage * message);

protected:
	Array<IGameObject *> mObjects;
	Array<IGameController *> mControllers;
	List<IGameMessage *> mMessages;
	int mUId;
};

