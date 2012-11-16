#pragma once

#include "xObj.h"

class xApp
{
	DECLARE_SINGLETON(xApp);

public:
	static Event OnInit;
	static Event OnInitUI;
	static Event OnShutdown;
	static Event OnSelectObj;
	static Event OnUnSelectObj;
	static Event OnUpdate;

public:
	xApp();
	~xApp();

	void Run();

	void Init(HWND hWnd);
	void Shutdown();

	void Resize(int w, int h);

	bool IsQuit() { return mQuit; }

	void _lock() { if (mMutex) mMutex->Lock(); }
	void _unlock() { if (mMutex) mMutex->Unlock(); }

	void _InitEngine();

	bool IsInited() { return mInited; }

	ShaderLib * GetHelperShaderLib() { return mHelperShaderLib; }

	void SetSelectedObj(xObj * obj);
	void SetSelectedObjs(xObj ** objs, int size);
	int GetSelectedObjSize();
	xObj * GetSelectedObj(int index);

protected:
	void _input();

protected:
	Engine * mEngine;
	ShaderLib * mHelperShaderLib;
	Thread mThread;
	bool mQuit;

	Mutex * mMutex;
	HWND mhWnd;
	bool mInited;

	bool mNeedResize;

	Array<xObj *> mSelectedObjs;
};


struct _Locker
{
	_Locker();
	~_Locker();
};

#define __enter() _Locker __locker
