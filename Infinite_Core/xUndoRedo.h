//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

class xUndoRedo
{
public:
	xUndoRedo() {}
	virtual ~xUndoRedo() {}

	virtual void Undo() = 0;
	virtual void Redo() = 0;
};

class xUndoRedoManager
{
	DECLARE_SINGLETON(xUndoRedoManager);

	static const int K_STACK_SIZE = 10;

public:
	xUndoRedoManager();
	~xUndoRedoManager();

	void Push(xUndoRedo * op);

	void Undo();
	void Redo();

protected:
	void _Shutdown(Event * sender);

protected:
	DQueue<xUndoRedo *> mUndoStack;
	DQueue<xUndoRedo *> mRedoStack;

	tEventListener<xUndoRedoManager> OnShutdown;
};