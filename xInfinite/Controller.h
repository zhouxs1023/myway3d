#pragma once

class Controller
{
	DECLARE_SINGLETON(Controller);

public:
	static Event OnEngineInit;
	static Event OnEngineDeInit;

public:
	Controller();
	~Controller();

	void Run();

	void Init(HWND hWnd);
	void Shutdown();

	void Resize(int w, int h);

	bool IsQuit() { return mQuit; }

	void _lock() { if (mMutex) mMutex->Lock(); }
	void _unlock() { if (mMutex) mMutex->Unlock(); }

	void _InitEngine();

	bool IsInited() { return mInited; }

protected:
	void _input();

protected:
	Engine * mEngine;
	Thread mThread;
	bool mQuit;

	Mutex * mMutex;
	HWND mhWnd;
	bool mInited;

	bool mNeedResize;
};


struct _Locker
{
	_Locker();
	~_Locker();
};

#define __enter() _Locker __locker
