#pragma once

#include "xEditor.h"
#include "xObj.h"
#include "xEvent.h"
#include "xBaseOperator.h"
#include "xToolBar.h"
#include "xEnvironment.h"
#include "xGizmo.h"

enum eTransformOperator
{
	eTO_Unknown,
	eTO_Move,
	eTO_Rotate,
	eTO_Scale,
};

#define xSliderMin 1
#define xSliderMax 100

class X_ENTRY xApp
{
	DECLARE_SINGLETON(xApp);

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

	void SetOperator(int op) { mOperator = op; }
	int GetOperator() { return mOperator; }

	void Activate(bool b) { mActivate = b; }
	bool IsActivate() { return mActivate; }

protected:
	void _input();
	void _loadPlugins();

protected:
	Engine * mEngine;
	ShaderLib * mHelperShaderLib;
	Thread mThread;
	bool mQuit;
	bool mActivate;

	Mutex * mMutex;
	HWND mhWnd;
	bool mInited;

	bool mNeedResize;

	Array<xObj *> mSelectedObjs;

	int mOperator;

	xSelectOp mSelectOp;
	xMoveOp mMoveOp;
	xRotateOp mRotateOp;
	xScaleOp mScaleOp;

	xOpToolBar mOpToolBar;

	xGizmo mGizmo;

	xEnvironment mEnvironment;

	xObjManager mObjMgr;
};



struct _Locker
{
	_Locker();
	~_Locker();
};

#define __enter() _Locker __locker
