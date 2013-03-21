#include "stdafx.h"

#include "xUndoRedo.h"
#include "xEvent.h"

IMP_SLN(xUndoRedoManager);

xUndoRedoManager::xUndoRedoManager()
	: OnShutdown(xEvent::OnShutdown, this, &xUndoRedoManager::_Shutdown)
{
	INIT_SLN;
}

xUndoRedoManager::~xUndoRedoManager()
{
	SHUT_SLN;
}

void xUndoRedoManager::Push(xUndoRedo * op)
{
	mUndoStack.PushFront(op);
	
	while (mRedoStack.Size())
	{
		xUndoRedo * redo = mRedoStack.Front();
		delete redo;
		mRedoStack.PopFront();
	}

	if (mUndoStack.Size() > K_STACK_SIZE)
	{
		xUndoRedo * undo = mUndoStack.Back();
		delete undo;
		mUndoStack.PopBack();
	}
}

void xUndoRedoManager::Undo()
{
	if (mUndoStack.Size())
	{
		xUndoRedo * undo = mUndoStack.Front();
		undo->Undo();

		mUndoStack.PopFront();
		mRedoStack.PushFront(undo);
	}
}

void xUndoRedoManager::Redo()
{
	if (mRedoStack.Size())
	{
		xUndoRedo * redo = mRedoStack.Front();
		redo->Redo();

		mRedoStack.PopFront();
		mUndoStack.PushFront(redo);
	}
}

void xUndoRedoManager::_Shutdown(Event * sender)
{
	while (mRedoStack.Size())
	{
		xUndoRedo * redo = mRedoStack.Front();
		delete redo;
		mRedoStack.PopFront();
	}

	while (mUndoStack.Size())
	{
		xUndoRedo * undo = mUndoStack.Front();
		delete undo;
		mUndoStack.PopFront();
	}
}
