#pragma once

#include "xEditor.h"
#include "xObj.h"
#include "xEvent.h"
#include "xBaseOperator.h"
#include "xToolBar.h"
#include "xGizmo.h"
#include "xUndoRedo.h"
#include "xDebugRender.h"
#include "IDockPane.h"
#include "MWDeferredRenderer.h"

enum eTransformOperator
{
	eTO_Unknown,
	eTO_Move,
	eTO_Rotate,
	eTO_Scale,
};

enum ePick
{
	PICK_Flag = 0x01,
};

enum eAppMode
{
	eAM_Editor,
	eAM_Game,
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

	void AddPane(IDockPane * pane) { mDockPanes.PushBack(pane); }
	int GetPaneCount() { return mDockPanes.Size(); }
	IDockPane * GetPane(int i) { return mDockPanes[i]; }

	void SetSelectedObj(xObj * obj);
	void SetSelectedObjs(xObj ** objs, int size);
	int GetSelectedObjSize();
	xObj * GetSelectedObj(int index);

	void SetOperator(int op) { mOperator = op; }
	int GetOperator() { return mOperator; }

	void Activate(bool b) { mActivate = b; }
	bool IsActivate() { return mActivate; }

	Vec3 GetHitPosition(float fx, float fy);

	void SetAppMode(eAppMode mode);
	eAppMode GetAppMode() { return mMode; }

protected:
	void _input();
	void _loadPlugins();
	void _unloadScene(Event * sender);

protected:
	Engine * mEngine;
	DeferredRenderer * mRenderer;
	ShaderLib * mHelperShaderLib;
	Thread mThread;
	bool mQuit;
	bool mActivate;

	eAppMode mMode;

	Mutex * mMutex;
	HWND mhWnd;
	bool mInited;

	bool mNeedResize;

	Array<IDockPane *> mDockPanes;
	Array<xObj *> mSelectedObjs;

	int mOperator;

	xSelectOp mSelectOp;
	xMoveOp mMoveOp;
	xRotateOp mRotateOp;
	xScaleOp mScaleOp;

	xOpToolBar mOpToolBar;

	xGizmo mGizmo;

	xObjManager mObjMgr;

	xUndoRedoManager mUndoRedoManager;

	xDebugRender mDebugRender;

	tEventListener<xApp> OnUnloadScene;
};



struct _Locker
{
	_Locker();
	~_Locker();
};

#define __enter() _Locker __locker
