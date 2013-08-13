#pragma once

#include "GameMessage.h"
#include "GameObject.h"

class GCORE_ENTRY GmMode
{
public:
	GmMode();
	virtual ~GmMode();

	int GetUId();

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Update(float frameTime);

	void AddObject(GmObj * obj);
	GmObj * GetObject(int id);
	GmObj * GetObjectByIndex(int index);
	int GetObjectCount();
	void RemoveObject(int id);
	void RemoveAllObject();

	void AddController(GmController * ctrl);
	GmController * GetController(int index);
	int GetControllerCount();
	void RemoveController(int index);
	void RemoveController(GmController * ctrl);
	void RemoveAllController();

	void AddMessage(GmMsg * message);

protected:
	Array<GmObj *> mObjects;
	Array<GmController *> mControllers;
	List<GmMsg *> mMessages;
	int mUId;
};

