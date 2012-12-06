#pragma once

#include "xEditor.h"
#include "xUndoRedo.h"

class X_ENTRY xBaseOp
{
protected:
	static int OP_Command;
	static int OP_Id;

public:
	xBaseOp();
	virtual ~xBaseOp();

	virtual void Process() = 0;
	virtual const char * GetIcon() = 0;

protected:
	void Init(void * param0, void * param1);
	void OnCommand(void * param0, void * param1);

protected:
	tEventListener<xBaseOp> mOnCommand;
	tEventListener<xBaseOp> mOnInit;

	int mOperatorId;
};

#define xImplementOp(classname, op) const int classname::##op = xBaseOp::OP_Id++

class xSelectOp : public xBaseOp
{
public:
	static const int eOp_Select;

	virtual void Process();
	virtual const char * GetIcon() { return "select.jpg"; }
};


class xMoveOp : public xBaseOp
{
public:
	static const int eOp_Move;

	virtual void Process();
	virtual const char * GetIcon() { return "move.jpg"; }
};

class xRotateOp : public xBaseOp
{
public:
	static const int eOp_Rotate;

	virtual void Process();
	virtual const char * GetIcon() { return "rotate.jpg"; }
};

class xScaleOp : public xBaseOp
{
public:
	static const int eOp_Scale;

	virtual void Process();
	virtual const char * GetIcon() { return "scale.jpg"; }
};



class xUndoRedo_Move : public xUndoRedo
{
public: 
	xUndoRedo_Move(const TString128 & objName, const Vec3 & orgPosition, const Vec3 & newPosition);
	virtual ~xUndoRedo_Move();

	virtual void Undo();
	virtual void Redo();

protected:
	TString128 mObjName;
	Vec3 mOrgPosition;
	Vec3 mNewPosition;
};

class xUndoRedo_Rotate : public xUndoRedo
{
public: 
	xUndoRedo_Rotate(const TString128 & objName, const Quat & orgOrientation, const Quat & newOrientation);
	virtual ~xUndoRedo_Rotate();

	virtual void Undo();
	virtual void Redo();

protected:
	TString128 mObjName;
	Quat mOrgOrientation;
	Quat mNewOrientation;
};

class xUndoRedo_Scale : public xUndoRedo
{
public:
	xUndoRedo_Scale(const TString128 & objName, const Vec3 & orgScale, const Vec3 & newScale);
	virtual ~xUndoRedo_Scale();

	virtual void Undo();
	virtual void Redo();

protected:
	TString128 mObjName;
	Vec3 mOrgScale;
	Vec3 mNewScale;
};