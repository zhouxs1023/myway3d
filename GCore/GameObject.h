#pragma once

#include "GameCoreEntry.h"
#include "GameEntity.h"

class GmObj;

class GCORE_ENTRY GmComponent
{
	DeclareRootRTTI(GmComponent);

public:
	GmComponent() { mObject = NULL; }
	virtual ~GmComponent() {}

	virtual void SetObject(GmObj * obj) { mObject = obj; }
	virtual GmObj * GetObject() { return mObject; }

	virtual void Update(float time) = 0;

protected:
	GmObj * mObject;
};


class GCORE_ENTRY GmController
{
	DeclareRootRTTI(GmController);

public:
	GmController() { mObject = NULL; mEnable = true; }
	virtual ~GmController() {}

	virtual void SetObject(GmObj * obj) { mObject = obj; }
	virtual GmObj * GetObject() { return mObject; }

	void SetEnable(bool enable) { mEnable = enable; } 
	bool GetEnable() const { return mEnable; }

	virtual void Update(float time) = 0;

protected:
	GmObj * mObject;
	bool mEnable;
};



class GCORE_ENTRY GmObj : public GmEntity
{
	DeclareRootRTTI(GmObj);

public:
	GmObj();
	virtual ~GmObj();

	void SetId(int id);
	int GetId() const;

	void SetName(const char * name);
	const char * GetName() const;

	virtual void Update(float frameTime);

	// Component
	//
	void AddComponent(GmComponent * cp);
	void RemoveComponent(GmComponent * cp);
	void RemoveComponent(int index);
	void RemoveAllComponent();
	int GetComponetCount();
	GmComponent * GetComponent(int index);

	// Message

protected:
	int mId;
	TString128 mName;
	Array<GmComponent *> mComponents;
};