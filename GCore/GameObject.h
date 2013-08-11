#pragma once

#include "GameCoreEntry.h"
#include "GameEntity.h"

class IGameObject;

class GCORE_ENTRY IGameComponent
{
	DeclareRootRTTI(IGameComponent);

public:
	IGameComponent() { mObject = NULL; }
	virtual ~IGameComponent() {}

	virtual void SetObject(IGameObject * obj) { mObject = obj; }
	virtual IGameObject * GetObject() { return mObject; }

	virtual void Update(float time) = 0;

protected:
	IGameObject * mObject;
};


class GCORE_ENTRY IGameController
{
	DeclareRootRTTI(IGameController);

public:
	IGameController() { mObject = NULL; mEnable = true; }
	virtual ~IGameController() {}

	virtual void SetObject(IGameObject * obj) { mObject = obj; }
	virtual IGameObject * GetObject() { return mObject; }

	void SetEnable(bool enable) { mEnable = enable; } 
	bool GetEnable() const { return mEnable; }

	virtual void Update(float time) = 0;

protected:
	IGameObject * mObject;
	bool mEnable;
};



class GCORE_ENTRY IGameObject : public IGameEntity
{
	DeclareRootRTTI(IGameObject);

public:
	IGameObject();
	virtual ~IGameObject();

	void SetId(int id);
	int GetId() const;

	void SetName(const char * name);
	const char * GetName() const;

	virtual void Update(float frameTime);

	// Component
	//
	void AddComponent(IGameComponent * cp);
	void RemoveComponent(IGameComponent * cp);
	void RemoveComponent(int index);
	void RemoveAllComponent();
	int GetComponetCount();
	IGameComponent * GetComponent(int index);

	// Message

protected:
	int mId;
	TString128 mName;
	Array<IGameComponent *> mComponents;
};